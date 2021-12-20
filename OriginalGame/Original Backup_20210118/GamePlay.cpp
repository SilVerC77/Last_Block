#include "COriginalGameApp.h"
#include "GameResource.h"
#include <stdlib.h>
#include <time.h>
#define _USE_MATH_DEFINES
#include <math.h>

//変数定義-----------------------------------------------------------------------
//ゲーム画面のフェーズを管理する変数
ePhase g_GamePhase = eBegin;
//背景の画像を管理する変数
CPicture* g_pBackground = NULL;
//ゲーム画面から次へ進むときのフローを管理する変数
eGameStatus g_NextStatusFromGame = eNone;

XMFLOAT2 PosFromLeftTop(float x, float y);
XMFLOAT2 PosFromRightTop(float x, float y);
XMFLOAT2 PosFromLeftBottom(float x, float y);
XMFLOAT2 PosFromRightBottom(float x, float y);

//PLAYER-------------------------------------------------------------------------
CPicture* g_pPlayer = NULL;
//Player Size
const float PLAYER_SIZE_X = 32.0f;
const float PLAYER_SIZE_Y = 48.0f;
//Player Movement Speed
float g_playerMoveSpeed_x = 0.0f;
float g_playerMoveSpeed_y = 0.0f;

//Jump
const float g_jumpPower = 10.0f;

//Collision
bool g_isRightHit = false;
bool g_isLeftHit = false;
bool g_isTopHit = false;
bool g_onGround = false;

const float gravity = 0.5f;
const float adjustY = 6.0f;

//BLOCK-------------------------------------------------------------------------
struct Block {
	CPicture* obj;
	CPicture* disObj;
	int disAnimIndex;
	int disCounter;
	XMFLOAT2 beginPos;
	bool flag;
	bool drop;
	float velocity_y;
};
const int BLOCK_OBJECT_COUNT = 15;
const float BLOCK_SIZE_X = 32.0f;
const float BLOCK_SIZE_Y = 32.0f;
Block g_blockObjs[BLOCK_OBJECT_COUNT];

//ENEMY------------------------------------------------------------------------
struct Enemy
{
	CPicture* obj;
	CPicture* dieObj;
	int dieAnimIndex;
	int dieAnimCounter;
	XMFLOAT2 beginPos;
	float velocity_X;
	float velocity_Y;
	float rad;
	bool flag;
	bool onGround;
};
typedef enum eEnemy {
	eWalk = 0,
	eSpring,
	eFly,
}eEnemy;
const int ENEMY_OBJECT_COUNT = 2;
const float ENEMY_SIZE_X = 32.0f;
const float ENEMY_SIZE_Y = 32.0f;
Enemy g_enemyObjs[3][ENEMY_OBJECT_COUNT];

//SCORE-----------------------------------------------------------------------
CFont* g_pScore = NULL;
int g_score = 0;
int digit = 1;
const float SCORE_SIZE_X = 40.0f;
const float SCORE_SIZE_Y = 64.0f;


/*
 * ゲーム本編の処理を行う部分
 */

void COriginalGameApp::procGame()
{
	//GamePhase
	switch (g_GamePhase)
	{
		//Begin
	case eBegin:
	{
		//Process Game Begin
		procPlayBegin();
		//switch to Main
		g_GamePhase = eMain;
	}
	break;

	//Main
	case eMain:
	{
		//Process Main
		procPlayMain();

		//WIN
		int Total = 0;
		for (int t = 0; t < 3; ++t) {
			for (int i = 0; i < ENEMY_OBJECT_COUNT; ++i) {
				if (g_enemyObjs[t][i].obj != NULL) {
					Total++;
				}
			}
		}
		for (int i = 0; i < BLOCK_OBJECT_COUNT; ++i) {
			if (g_blockObjs[i].obj != NULL) {
				Total++;
			}
		}
		if (Total <= 0) {
			g_NextStatusFromGame = eGameClear;
			static float fly = 0.0f;
			fly += 0.5;
			if (g_pPlayer->getPos().y < (WINDOW_HEIGHT / 2.0)) {
				g_pPlayer->setPos(g_pPlayer->getPos().x, g_pPlayer->getPos().y + fly);
			}
			else {
				g_GamePhase = eEnd;
				fly = 0.0;
			}
		}

		//LOSE
		if (g_pPlayer->getPos().y < -(WINDOW_HEIGHT + PLAYER_SIZE_Y) / 2)
		{
			g_NextStatusFromGame = eGameOver;
			g_GamePhase = eEnd;
		}
	}
	break;

	//Ending
	case eEnd:
	{
		//Game Ending
		procPlayEnd();
		//reset GamePhase
		g_GamePhase = eBegin;
		//NextStatus
		SetNextGameStatus(g_NextStatusFromGame);
	}
	break;
	}
}

void COriginalGameApp::procPlayBegin()
{
	//Prepare Player
	if (g_pPlayer == NULL)
	{
		g_pPlayer = createSprite(Tex_Player, PLAYER_SIZE_X, PLAYER_SIZE_Y);
		g_pPlayer->setPos(PosFromLeftTop(5.0f, 10.0f));
		g_playerMoveSpeed_y = 0.0f;
	}

	//Prepare Enemy
	for (int t = 0; t < 3; ++t) {
		for (int i = 0; i < ENEMY_OBJECT_COUNT; ++i)
		{
			g_enemyObjs[t][i].obj = NULL;
			g_enemyObjs[t][i].dieObj = NULL;
			g_enemyObjs[t][i].dieAnimIndex = 0;
			g_enemyObjs[t][i].dieAnimCounter = 0;
			g_enemyObjs[t][i].beginPos = XMFLOAT2(100.0f + i * 32.0f, 50.0f);
			if (t == eWalk)
				g_enemyObjs[t][i].velocity_X = 2.0f;
			if (t == eSpring)
				g_enemyObjs[t][i].velocity_X = 0.0f;
			if (t == eFly)
				g_enemyObjs[t][i].velocity_X = 0.0f;
			g_enemyObjs[t][i].velocity_Y = 0.0f;
			g_enemyObjs[t][i].rad = 0.0f;
			g_enemyObjs[t][i].flag = true;
			g_enemyObjs[t][i].onGround = true;

			if (t == eWalk)
				g_enemyObjs[t][i].obj = createSprite(Tex_Enemy_0, ENEMY_SIZE_X, ENEMY_SIZE_Y);
			if (t == eSpring)
				g_enemyObjs[t][i].obj = createSprite(Tex_Enemy_1, ENEMY_SIZE_X, ENEMY_SIZE_Y);
			if (t == eFly)
				g_enemyObjs[t][i].obj = createSprite(Tex_Enemy_2, ENEMY_SIZE_X, ENEMY_SIZE_Y);
			g_enemyObjs[t][i].obj->setPos(g_enemyObjs[t][i].beginPos);
		}
	}
	g_enemyObjs[eWalk][0].beginPos = PosFromLeftTop(10.0f, 10.0f);
	g_enemyObjs[eSpring][0].beginPos = PosFromLeftTop(17.0f, 12.0f);
	g_enemyObjs[eFly][0].beginPos = PosFromLeftTop(10.0f, 5.0f);
	//g_enemyObjs[0][3].beginPos = XMFLOAT2(32.0f, 50.0f);
	//g_enemyObjs[0][4].beginPos = XMFLOAT2(32.0f, 50.0f);
	//g_enemyObjs[0][5].beginPos = XMFLOAT2(0.0f, 0.0f);
	//g_enemyObjs[0][6].beginPos = XMFLOAT2(0.0f, 0.0f);
	//g_enemyObjs[0][7].beginPos = XMFLOAT2(0.0f, 0.0f);
	//g_enemyObjs[0][8].beginPos = XMFLOAT2(0.0f, 0.0f);
	//g_enemyObjs[0][9].beginPos = XMFLOAT2(0.0f, 0.0f);
	//g_enemyObjs[0][10].beginPos = XMFLOAT2(0.0f, 0.0f);
	//g_enemyObjs[0][11].beginPos = XMFLOAT2(0.0f, 0.0f);
	//g_enemyObjs[0][12].beginPos = XMFLOAT2(0.0f, 0.0f);
	//g_enemyObjs[0][13].beginPos = XMFLOAT2(0.0f, 0.0f);
	//g_enemyObjs[0][14].beginPos = XMFLOAT2(0.0f, 0.0f);
	for (int t = 0; t < 3; ++t) {
		for (int i = 0; i < ENEMY_OBJECT_COUNT; ++i) {
			g_enemyObjs[t][i].obj->setPos(g_enemyObjs[t][i].beginPos);
		}
	}

	//Prepare Block
	for (int i = 0; i < BLOCK_OBJECT_COUNT; ++i)
	{
		g_blockObjs[i].obj = NULL;
		g_blockObjs[i].disObj = NULL;
		g_blockObjs[i].disAnimIndex = 0;
		g_blockObjs[i].disCounter = 0;
		if (/*i == 0 || */i == BLOCK_OBJECT_COUNT - 1) {
			g_blockObjs[i].beginPos = XMFLOAT2(-200.0f + i * BLOCK_SIZE_X, -175.0f + BLOCK_SIZE_Y);
		}
		else {
			g_blockObjs[i].beginPos = XMFLOAT2(-200.0f + i * BLOCK_SIZE_X, -175.0f);
		}
		g_blockObjs[i].flag = true;
		g_blockObjs[i].drop = false;
		g_blockObjs[i].velocity_y = 0.0;
	}
	g_blockObjs[0].beginPos = PosFromLeftTop(4.0f, 12.0f);
	g_blockObjs[1].beginPos = PosFromLeftTop(4.0f, 13.0f);
	g_blockObjs[2].beginPos = PosFromLeftTop(5.0f, 13.0f);
	g_blockObjs[3].beginPos = PosFromLeftTop(6.0f, 13.0f);
	g_blockObjs[4].beginPos = PosFromLeftTop(7.0f, 13.0f);
	g_blockObjs[5].beginPos = PosFromLeftTop(8.0f, 13.0f);
	g_blockObjs[6].beginPos = PosFromLeftTop(9.0f, 13.0f);
	g_blockObjs[7].beginPos = PosFromLeftTop(10.0f, 13.0f);
	g_blockObjs[8].beginPos = PosFromLeftTop(11.0f, 13.0f);
	g_blockObjs[9].beginPos = PosFromLeftTop(12.0f, 13.0f);
	g_blockObjs[10].beginPos = PosFromLeftTop(13.0f, 13.0f);
	g_blockObjs[11].beginPos = PosFromLeftTop(14.0f, 13.0f);
	g_blockObjs[12].beginPos = PosFromLeftTop(15.0f, 13.0f);
	g_blockObjs[13].beginPos = PosFromLeftTop(16.0f, 13.0f);
	g_blockObjs[14].beginPos = PosFromLeftTop(16.0f, 12.0f);

	//背景の画像を作成
	if (g_pBackground == NULL)
	{
		g_pBackground = createSprite(Tex_Back);
		g_pBackground->setPos(0.0f, 0.0f);
	}
}

void COriginalGameApp::procPlayMain()
{
	//Player Control
	if (g_pPlayer != NULL)
	{
		procPlayMain_movePlayer();
	}

	//Process of SCORE
	procPlayMain_score();

	//Process of BLOCK
	procPlayMain_block();

	//Process of ENEMY
	procPlayMain_enemy();

	//RENDER--------------------------------------------
	if (g_pPlayer != NULL)
	{
		renderSprite(g_pPlayer);
	}

	if (g_pScore != NULL) {
		//renderFont()
		//第一引数：スコアの画像を管理する変数
		//第二引数：スコア値
		renderFont(g_pScore, g_score);
	}

	if (g_pBackground != NULL)
	{
		renderSprite(g_pBackground);
	}
}

void COriginalGameApp::procPlayMain_score()
{
	if (g_pScore != NULL)
	{

		/*if (m_pDirectInput->isPressedOnce(DIK_RIGHTARROW)) {
			g_score += 1;
		}
		if (m_pDirectInput->isPressedOnce(DIK_LEFTARROW)) {
			g_score -= 1;
		}*/
		g_score = 0;
		for (int t = 0; t < 3; ++t) {
			for (int i = 0; i < ENEMY_OBJECT_COUNT; ++i) {
				if (g_enemyObjs[t][i].obj != NULL) {
					g_score++;
				}
			}
		}
		for (int i = 0; i < BLOCK_OBJECT_COUNT; ++i) {
			if (g_blockObjs[i].obj != NULL) {
				g_score++;
			}
		}
		if (g_score < 0) {
			g_score = 0;
		}
		if (g_score > 99) {
			g_score = 99;
		}
		if (g_score > 9 && digit == 1) {
			disposeFont(g_pScore);
			g_pScore = NULL;
			digit = 2;
		}
		if (g_score < 10 && digit == 2) {
			disposeFont(g_pScore);
			g_pScore = NULL;
			digit = 1;
		}
	}
	if (g_pScore == NULL)
	{
		if (g_score < 10) {
			g_pScore = createFont(Tex_Number, 1, SCORE_SIZE_X, SCORE_SIZE_Y);
		}
		if (g_score > 9) {
			g_pScore = createFont(Tex_Number, 2, SCORE_SIZE_X, SCORE_SIZE_Y);
		}
		g_pScore->setPos(-SCORE_SIZE_X / 2 * digit, -(WINDOW_HEIGHT - SCORE_SIZE_Y) / 2);
	}

}

void COriginalGameApp::procPlayMain_block()
{
	for (int i = 0; i < BLOCK_OBJECT_COUNT; ++i) {
		if (g_blockObjs[i].obj == NULL && g_blockObjs[i].flag != false) {
			g_blockObjs[i].obj = createSprite(Tex_Block, BLOCK_SIZE_X, BLOCK_SIZE_Y);
			g_blockObjs[i].obj->setPos(g_blockObjs[i].beginPos);
		}
	}

	if (m_pDirectInput->isPressedOnce(DIK_X)) {
		for (int i = 0; i < BLOCK_OBJECT_COUNT; ++i) {
			if (g_blockObjs[i].obj != NULL) {
				if (g_blockObjs[i].obj->collisionTop(g_pPlayer) != false) {
					g_blockObjs[i].flag = false;
					break;
				}
			}
		}
	}
	//Collision with enemy die
	for (int t = 0; t < 3; ++t) {
		for (int i = 0; i < ENEMY_OBJECT_COUNT; ++i) {
			for (int b = 0; b < BLOCK_OBJECT_COUNT; ++b) {
				if (g_blockObjs[b].obj != NULL) {
					if (g_enemyObjs[t][i].obj != NULL) {
						if (g_blockObjs[b].obj->collision(g_enemyObjs[t][i].obj) != false && g_enemyObjs[t][i].flag == false && g_blockObjs[b].flag != false) {
							g_enemyObjs[t][i].dieObj = createSprite(Tex_Die, ENEMY_SIZE_X, ENEMY_SIZE_Y);
							g_enemyObjs[t][i].dieObj->setPos(g_enemyObjs[t][i].obj->getPos());
							disposeSprite(g_enemyObjs[t][i].obj);
							g_enemyObjs[t][i].obj = NULL;
							g_blockObjs[b].flag = false;
							break;
						}
					}
				}
			}
		}
	}
	//Collision with block die
	for (int i = 0; i < BLOCK_OBJECT_COUNT; ++i) {
		for (int b = 0; b < BLOCK_OBJECT_COUNT; ++b) {
			if (i == b)
				continue;
			if (g_blockObjs[i].obj != NULL) {
				if (g_blockObjs[b].obj != NULL) {
					if (g_blockObjs[i].obj->collision(g_blockObjs[b].obj->getPos(), BLOCK_SIZE_X + 1.0f) != false && g_blockObjs[b].drop != false && g_blockObjs[i].flag != false) {
						g_blockObjs[i].flag = false;
						break;
					}
				}
			}
		}
	}


	//FALL
	for (int i = 0; i < BLOCK_OBJECT_COUNT; ++i) {
		if (g_blockObjs[i].flag == false && g_blockObjs[i].obj != NULL) {
			++g_blockObjs[i].disCounter;
			////DELAY////
			if (g_blockObjs[i].disCounter > 15) {
				g_blockObjs[i].drop = true;
				g_blockObjs[i].disCounter = 15;
			}
			if (g_blockObjs[i].drop != false) {
				g_blockObjs[i].velocity_y -= gravity;
				g_blockObjs[i].obj->setPos(g_blockObjs[i].obj->getPos().x, g_blockObjs[i].obj->getPos().y + g_blockObjs[i].velocity_y);
			}
			////MAXIMUM FALL SPEED////
			if (g_blockObjs[i].velocity_y > adjustY) {
				g_blockObjs[i].velocity_y = adjustY;
			}
		}
	}
	//WHEN OUT FROM SCREEN
	for (int i = 0; i < BLOCK_OBJECT_COUNT; ++i) {
		if (g_blockObjs[i].obj != NULL) {
			if (g_blockObjs[i].obj->getPos().y < -(WINDOW_WIDTH + BLOCK_SIZE_Y) / 2.0f) {
				disposeSprite(g_blockObjs[i].obj);
				g_blockObjs[i].obj = NULL;
				g_blockObjs[i].disCounter = 0;
			}
		}
	}

	for (int i = 0; i < BLOCK_OBJECT_COUNT; ++i) {
		if (g_blockObjs[i].obj != NULL) {
			renderSprite(g_blockObjs[i].obj);
		}
	}
}

void COriginalGameApp::procPlayMain_enemy()
{
	/*for (int i = 0; i < ENEMY_OBJECT_COUNT; ++i) {
		if (g_enemyObjs[i].obj == NULL && g_enemyObjs[i].flag != false) {
			g_enemyObjs[i].obj = createSprite(Tex_Enemy, ENEMY_SIZE_X, ENEMY_SIZE_Y);
			g_enemyObjs[i].obj->setPos(g_enemyObjs[i].beginPos);
		}
	}*/

	//Movement
	XMFLOAT4 pos[3][ENEMY_OBJECT_COUNT];
	for (int t = 0; t < 3; ++t) {
		for (int i = 0; i < ENEMY_OBJECT_COUNT; ++i) {
			if (g_enemyObjs[t][i].obj != NULL) {
				pos[t][i] = g_enemyObjs[t][i].obj->getPos();
				//Ground
				//if (pos[t][i].y < -(WINDOW_HEIGHT - ENEMY_SIZE_Y) / 2.0f && g_enemyObjs[t][i].flag != false) {
				//	pos[t][i].y = -(WINDOW_HEIGHT - ENEMY_SIZE_Y) / 2.0f + adjustY;
				//}

				//collision with BLOCK_Y
				for (int b = 0; b < BLOCK_OBJECT_COUNT; ++b) {
					if (g_blockObjs[b].obj != NULL) {
						if (g_enemyObjs[t][i].obj->collisionBottom(g_blockObjs[b].obj) != false) {
							g_enemyObjs[t][i].velocity_Y = 0;
							pos[t][i].y = g_blockObjs[b].obj->getPos().y + (ENEMY_SIZE_Y + BLOCK_SIZE_Y) / 2.0f;
							//g_enemyObjs[0][i].onGround = true;
							break;
							//}
							//else {
								//g_enemyObjs[0][i].onGround = false;
						}
					}
				}
				//collision with BLOCK_X
				for (int b = 0; b < BLOCK_OBJECT_COUNT; ++b) {
					if (g_blockObjs[b].obj != NULL) {
						if (g_enemyObjs[t][i].obj->collisionLeft(g_blockObjs[b].obj) != false && g_enemyObjs[t][i].velocity_X < 0.0f) {
							pos[t][i].x = g_blockObjs[b].obj->getPos().x + (ENEMY_SIZE_X + BLOCK_SIZE_X) / 2.0f - g_enemyObjs[t][i].velocity_X;
							g_enemyObjs[t][i].velocity_X *= -1.0f;
							break;
						}
						if (g_enemyObjs[t][i].obj->collisionRight(g_blockObjs[b].obj) != false && g_enemyObjs[t][i].velocity_X > 0.0f) {
							pos[t][i].x = g_blockObjs[b].obj->getPos().x - (ENEMY_SIZE_X + BLOCK_SIZE_X) / 2.0f - g_enemyObjs[t][i].velocity_X;
							g_enemyObjs[t][i].velocity_X *= -1.0f;
							break;
						}
					}
				}

				if (g_enemyObjs[t][i].velocity_Y < -adjustY) {
					g_enemyObjs[t][i].velocity_Y = -adjustY;
				}

				//XY
				if (pos[t][i].x > (WINDOW_WIDTH - ENEMY_SIZE_X) / 2.0f || pos[t][i].x < -(WINDOW_WIDTH - ENEMY_SIZE_X) / 2.0f) {
					g_enemyObjs[t][i].velocity_X *= -1.0f;
				}

				if (t == eFly && g_enemyObjs[eFly][i].flag != false) {
					g_enemyObjs[t][i].velocity_X = 32.f * cos(g_enemyObjs[t][i].rad);
					g_enemyObjs[t][i].velocity_Y = 32.f * sin(g_enemyObjs[t][i].rad);
					pos[t][i].x = g_enemyObjs[t][i].beginPos.x + g_enemyObjs[t][i].velocity_X;
					pos[t][i].y = g_enemyObjs[t][i].beginPos.y + g_enemyObjs[t][i].velocity_Y;
					g_enemyObjs[t][i].rad += 0.05;
					if (g_enemyObjs[t][i].rad > 2.0 * M_PI) {
						g_enemyObjs[t][i].rad = 0.0f;
					}
				}
				else {
					g_enemyObjs[t][i].velocity_Y -= gravity;
					pos[t][i].y += g_enemyObjs[t][i].velocity_Y;
					pos[t][i].x += g_enemyObjs[t][i].velocity_X;
				}

			}
			if (g_enemyObjs[t][i].obj != NULL) {
				if (t == eFly && g_enemyObjs[t][i].flag != false) {
					if (g_enemyObjs[t][i].obj->getPos().y > g_enemyObjs[t][i].beginPos.y) {
						g_enemyObjs[t][i].obj->setAngle(XMFLOAT3{ 0.0f,180.0f,0.0f });
					}
					else {
						g_enemyObjs[t][i].obj->setAngle(XMFLOAT3{ 0.0f,0.0f,0.0f });
					}
				}
				else {
					if (g_enemyObjs[t][i].velocity_X > 0.0f) {
						g_enemyObjs[t][i].obj->setAngle(XMFLOAT3{ 0.0f,0.0f,0.0f });
					}
					else {
						g_enemyObjs[t][i].obj->setAngle(XMFLOAT3{ 0.0f,180.0f,0.0f });
					}
				}

				g_enemyObjs[t][i].obj->setPos(pos[t][i]);
			}
		}
	}

	//Die
	if (g_pPlayer != NULL) {
		for (int t = 0; t < 3; ++t) {
			for (int i = 0; i < ENEMY_OBJECT_COUNT; ++i) {
				if (g_enemyObjs[t][i].obj != NULL) {
					if (g_enemyObjs[t][i].obj->collisionTop(g_pPlayer) != false) {
						if (t == eFly) {
							if (g_playerMoveSpeed_x > 0.0f) {
								g_enemyObjs[t][i].velocity_X = 3.0f;
							}
							else {
								g_enemyObjs[t][i].velocity_X = -3.0f;
							}
						}
						g_enemyObjs[t][i].flag = false;
						g_enemyObjs[t][i].velocity_Y = 3.0f;

						////// Jump//////
						if (t == eSpring) {
							g_playerMoveSpeed_y = g_jumpPower * 1.5;
						}
						else {
							g_playerMoveSpeed_y = g_jumpPower;
						}
						break;
					}
					//Kill PLAYER
					if (g_enemyObjs[t][i].obj->collision(g_pPlayer) != false) {
						g_NextStatusFromGame = eGameOver;
						g_GamePhase = eEnd;
						///
						///
						/// 
					}
				}
			}
		}
	}
	//Fall out from SCREEN
	for (int t = 0; t < 3; ++t) {
		for (int i = 0; i < ENEMY_OBJECT_COUNT; ++i) {
			if (g_enemyObjs[t][i].obj != NULL) {
				if (pos[t][i].y < -(WINDOW_HEIGHT + ENEMY_SIZE_Y) / 2.0f) {
					disposeSprite(g_enemyObjs[t][i].obj);
					g_enemyObjs[t][i].obj = NULL;
					g_enemyObjs[t][i].flag = false;
				}
			}
		}
	}
	//Die Animation
	for (int t = 0; t < 3; ++t) {
		for (int i = 0; i < ENEMY_OBJECT_COUNT; ++i) {
			if (g_enemyObjs[t][i].dieObj != NULL) {
				renderSprite(g_enemyObjs[t][i].dieObj);
				g_enemyObjs[t][i].dieAnimCounter++;
				if (g_enemyObjs[t][i].dieAnimCounter % 4 == 0)
				{
					g_enemyObjs[t][i].dieObj->stepAnimation();
					g_enemyObjs[t][i].dieAnimCounter = 0;
					g_enemyObjs[t][i].dieAnimIndex++;
				}
				//指定枚数アニメーションしたら破棄する
				if (g_enemyObjs[t][i].dieAnimIndex == 5)
				{
					//アニメーションのインデックスをリセットしておく
					g_enemyObjs[t][i].dieAnimIndex = 0;
					disposeSprite(g_enemyObjs[t][i].dieObj);
					g_enemyObjs[t][i].dieObj = NULL;
					g_enemyObjs[t][i].flag = false;
				}
			}
		}
	}
	for (int t = 0; t < 3; ++t) {
		for (int i = 0; i < ENEMY_OBJECT_COUNT; ++i) {
			if (g_enemyObjs[t][i].obj != NULL) {
				renderSprite(g_enemyObjs[t][i].obj);
			}
		}
	}
}

void COriginalGameApp::procPlayMain_movePlayer()
{
	XMFLOAT4 pos = g_pPlayer->getPos();
	XMFLOAT3 angle = { 0.0f,0.0f,0.0f };
	//g_playerMoveSpeed_x = 0.0f;
	if (m_pDirectInput->isKeyPressed(DIK_LEFTARROW) || m_pDirectInput->isKeyPressed(DIK_RIGHTARROW)) {
		if (m_pDirectInput->isKeyPressed(DIK_RIGHTARROW)) {
			g_playerMoveSpeed_x = 3.0f;
			angle.y = 0.0f;
			g_pPlayer->setAngle(angle);
		}
		if (m_pDirectInput->isKeyPressed(DIK_LEFTARROW)) {
			g_playerMoveSpeed_x = -3.0f;
			angle.y = 180.0f;
			g_pPlayer->setAngle(angle);
		}
	}
	else {
		if (g_playerMoveSpeed_x < -0.0f) {
			g_playerMoveSpeed_x += 0.2f;
		}
		if (g_playerMoveSpeed_x > 0.0f) {
			g_playerMoveSpeed_x -= 0.2f;
		}
		if ((int)g_playerMoveSpeed_x == 0) {
			g_playerMoveSpeed_x = (int)g_playerMoveSpeed_x;
		}
	}


	//Process when over SCREEN WIDTH
	if ((pos.x > (WINDOW_WIDTH + PLAYER_SIZE_X) / 2 && g_playerMoveSpeed_x > 0.0f) || (pos.x < -(WINDOW_WIDTH + PLAYER_SIZE_X) / 2 && g_playerMoveSpeed_x < 0.0f)) {
		pos.x *= -1;
	}

	//Collision with BLOCK
	for (int i = 0; i < BLOCK_OBJECT_COUNT; ++i) {
		if (g_blockObjs[i].obj != NULL) {
			/// collision bottom///
			if (g_blockObjs[i].obj->collisionBottom(g_pPlayer) != false && g_playerMoveSpeed_y > 0.0f) {
				pos.y = g_blockObjs[i].obj->getPos().y - (PLAYER_SIZE_Y + BLOCK_SIZE_Y) / 2.0f - g_playerMoveSpeed_y;
				g_playerMoveSpeed_y = 0.0f;
				break;
			}
			/// collision left and right///
			if (g_pPlayer->collisionLeft(g_blockObjs[i].obj) != false && g_playerMoveSpeed_x < 0.0f) {
				pos.x = g_blockObjs[i].obj->getPos().x + (PLAYER_SIZE_X + BLOCK_SIZE_X) / 2.0f - g_playerMoveSpeed_x;
				break;
			}
			if (g_pPlayer->collisionRight(g_blockObjs[i].obj) != false && g_playerMoveSpeed_x > 0.0f) {
				pos.x = g_blockObjs[i].obj->getPos().x - (PLAYER_SIZE_X + BLOCK_SIZE_X) / 2.0f - g_playerMoveSpeed_x;
				break;
			}
		}
	}
	//for (int i = 0; i < BLOCK_OBJECT_COUNT; ++i) {
	//	if (g_blockObjs[i].obj != NULL) {
	//		
	//	}
	//}

	//Jump
	if (m_pDirectInput->isPressedOnce(DIK_SPACE) && g_onGround != false) {
		g_playerMoveSpeed_y = g_jumpPower;
	}
	g_onGround = false;
	/*for (int i = 0; i < BLOCK_OBJECT_COUNT; ++i) {
		if (g_blockObjs[i].obj != NULL) {
			if (m_pDirectInput->isPressedOnce(DIK_SPACE) && g_onGround == false && g_pPlayer->collisionLeft(g_blockObjs[i].obj) != false && g_playerMoveSpeed_x < 0.0f) {
				g_playerMoveSpeed_x = -10.0f;
				g_playerMoveSpeed_y = 5.0f;
			}
		}
	}*/

	//GROUND
	//if (pos.y <= -(WINDOW_HEIGHT - PLAYER_SIZE_Y) / 2) {
	//	pos.y = -(WINDOW_HEIGHT - PLAYER_SIZE_Y) / 2 + adjustY;
	//	g_onGround = true;
	//}
	//else {
	//	g_onGround = false;
	//}

	if (g_onGround == false) {
		for (int i = 0; i < BLOCK_OBJECT_COUNT; ++i) {
			if (g_blockObjs[i].obj != NULL) {
				if (g_blockObjs[i].obj->collisionTop(g_pPlayer) != false && g_playerMoveSpeed_y < 0.0f) {
					pos.y = g_blockObjs[i].obj->getPos().y + ((BLOCK_SIZE_Y + PLAYER_SIZE_Y) / 2.0f + adjustY);
					g_onGround = true;
					break;
				}
				else {
					g_onGround = false;
				}
			}
		}
	}

	g_playerMoveSpeed_y -= gravity;
	//Maximum fall velocity
	if (g_playerMoveSpeed_y < -adjustY) {
		g_playerMoveSpeed_y = -adjustY;
	}

	pos.y += g_playerMoveSpeed_y;
	pos.x += g_playerMoveSpeed_x;
	g_pPlayer->setPos(pos);
}

void COriginalGameApp::procPlayEnd()
{
	if (g_pPlayer != NULL)
	{
		disposeSprite(g_pPlayer);
		g_pPlayer = NULL;
	}

	if (g_pBackground != NULL)
	{
		disposeSprite(g_pBackground);
		g_pBackground = NULL;
	}

	if (g_pScore != NULL)
	{
		disposeFont(g_pScore);
		g_pScore = NULL;
	}

	for (int i = 0; i < BLOCK_OBJECT_COUNT; ++i) {
		if (g_blockObjs[i].obj != NULL) {
			disposeSprite(g_blockObjs[i].obj);
			g_blockObjs[i].obj = NULL;
		}
	}

	for (int t = 0; t < 3; ++t) {
		for (int i = 0; i < ENEMY_OBJECT_COUNT; ++i) {
			if (g_enemyObjs[t][i].obj != NULL) {
				disposeSprite(g_enemyObjs[t][i].obj);
				g_enemyObjs[t][i].obj = NULL;
			}
		}
	}
}

XMFLOAT2 PosFromLeftTop(float x, float y)
{
	float PosFromLeftTop_x = -(WINDOW_WIDTH + BLOCK_SIZE_X) / 2.0f + (BLOCK_SIZE_X * x);
	float PosFromLeftTop_y = (WINDOW_HEIGHT + BLOCK_SIZE_Y) / 2.0f - (BLOCK_SIZE_Y * y);

	return { PosFromLeftTop_x, PosFromLeftTop_y };
}
XMFLOAT2 PosFromRightTop(float x, float y)
{
	float PosFromRightTop_x = (WINDOW_WIDTH + BLOCK_SIZE_X) / 2.0f - (BLOCK_SIZE_X * x);
	float PosFromRightTop_y = (WINDOW_HEIGHT + BLOCK_SIZE_Y) / 2.0f - (BLOCK_SIZE_Y * y);

	return { PosFromRightTop_x, PosFromRightTop_y };
}
XMFLOAT2 PosFromLeftBottom(float x, float y)
{
	float PosFromLeftBottom_x = -(WINDOW_WIDTH + BLOCK_SIZE_X) / 2.0f + (BLOCK_SIZE_X * x);
	float PosFromLeftBottom_y = -(WINDOW_HEIGHT + BLOCK_SIZE_Y) / 2.0f + (BLOCK_SIZE_Y * y);

	return { PosFromLeftBottom_x, PosFromLeftBottom_y };
}
XMFLOAT2 PosFromRightBottom(float x, float y)
{
	float PosFromRightBottom_x = -(WINDOW_WIDTH + BLOCK_SIZE_X) / 2.0f + (BLOCK_SIZE_X * x);
	float PosFromRightBottom_y = -(WINDOW_HEIGHT + BLOCK_SIZE_Y) / 2.0f + (BLOCK_SIZE_Y * y);

	return { PosFromRightBottom_x, PosFromRightBottom_y };
}