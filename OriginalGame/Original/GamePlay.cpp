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

CPicture* g_pLoading = NULL;

//ゲーム画面から次へ進むときのフローを管理する変数
eGameStatus g_NextStatusFromGame = eNone;

//(FUNCTION) POSITION COUNT FROM TOP OF LEFT
XMFLOAT2 PosFromLeftTop(float x, float y);

//STAGE
typedef enum eStage {
	eStage1 = 0,
	eStage2,
	eStage3,
	eStage4,
	eStagefinal,
} eStage;

int Stage = eStage1;

//PLAYER-------------------------------------------------------------------------
CPicture* g_pPlayer = NULL;
CPicture* g_pPlayer_Move = NULL;
//Player Size
const float PLAYER_SIZE_X = 64.0f;
const float PLAYER_SIZE_Y = 64.0f;
//Player Movement Speed
float g_playerMoveSpeed_x = 0.0f;
float g_playerMoveSpeed_y = 0.0f;
//Animation counter
int AnimCounter = 0;

//Die
CPicture* g_pDie = NULL;
const float DIE_SIZE_X = 192.0f;
const float DIE_SIZE_Y = 192.0f;
bool g_isDead = false;
bool g_endDieEffect = false;
int g_DieAnimIndex = 0;
int g_DieAnimCounter = 0;

//Jump
const float g_jumpPower = 20.0f;

//Collision
bool g_isRightHit = false;
bool g_isLeftHit = false;
bool g_isTopHit = false;
bool g_onGround = false;

const float gravity = 1.0f;
const float adjustY = 10.0f;

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
const int BLOCK_OBJECT_COUNT = 30;
const float BLOCK_SIZE_X = 64.0f;
const float BLOCK_SIZE_Y = 64.0f;
Block g_blockObjs[BLOCK_OBJECT_COUNT];

//AIR BLOCK--------------------------------------------------------------------
struct Air {
	CPicture* obj;
	XMFLOAT2 beginPos;
};
const int AIR_OBJECT_COUNT = 4;
Air g_air[AIR_OBJECT_COUNT];

//ENEMY------------------------------------------------------------------------
struct Enemy
{
	CPicture* obj;
	CPicture* Move;
	int AnimCounter;
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
const int ENEMY_OBJECT_COUNT = 5;
const float ENEMY_SIZE_X = 64.0f;
const float ENEMY_SIZE_Y = 64.0f;
Enemy g_enemyObjs[3][ENEMY_OBJECT_COUNT];

//SCORE-----------------------------------------------------------------------
CFont* g_pScore = NULL;
int g_score = 0;
int digit = 1;
const float SCORE_SIZE_X = 42.0f;
const float SCORE_SIZE_Y = 62.0f;


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
			static float fly = 0.0f;
			fly += 0.5;
			if (g_pPlayer->getPos().y < (WINDOW_HEIGHT / 2.0)) {
				g_pPlayer->setPos(g_pPlayer->getPos().x, g_pPlayer->getPos().y + fly);
			}
			else {
				g_GamePhase = eEnd;
				fly = 0.0;
				++Stage;
			}
		}

		//LOSE
		if (g_pPlayer != NULL && g_pPlayer->getPos().y < -(WINDOW_HEIGHT + PLAYER_SIZE_Y) / 2.0f && g_pPlayer != NULL)
		{
			g_NextStatusFromGame = ePlaying;
			g_GamePhase = eEnd;
			++DeadCounter;
		}
		if (g_endDieEffect != false)
		{
			g_NextStatusFromGame = ePlaying;
			g_GamePhase = eEnd;
			++DeadCounter;
		}
	}
	break;

	//Ending
	case eEnd:
	{
		//Game Ending
		procPlayEnd();
		//NextStatus
		if (Stage > eStagefinal) {
			g_NextStatusFromGame = eGameClear;
			SetNextGameStatus(g_NextStatusFromGame);
			Stage = eStage1;

			//DISPOSE LOADING SCREEN
			if (g_pLoading != NULL) {
				disposeSprite(g_pLoading);
				g_pLoading = NULL;
			}
		}
		//reset GamePhase
		g_GamePhase = eBegin;
	}
	break;
	}

	////loading screen
	//if (g_pLoading != NULL) {
	//	static float counter = 0;
	//	if (g_pPlayer == NULL)
	//		g_pLoading->setAnimation(rand() % 10 + 1);
	//	counter += 1.0f / 60.0f;
	//	if (counter > 2.0f / 60.0f) {
	//		g_pLoading->stepAnimation();
	//		counter = 0.0f;
	//	}
	//	renderSprite(g_pLoading);
	//}
	//loading screen
	if (g_pLoading != NULL) {
		static int counter = 0;
		if (g_pPlayer == NULL)
			g_pLoading->setAnimation(rand() % 10 + 1);
		counter++;
		if (counter % 1 == 0) {
			g_pLoading->stepAnimation();
			counter = 0;
		}
		renderSprite(g_pLoading);
	}
}

void COriginalGameApp::procPlayBegin()
{
	//Loading Screen
	if (g_pLoading == NULL) {
		g_pLoading = createSprite(Tex_Loading);
	}

	//Prepare Player
	if (g_pPlayer == NULL)
	{
		g_pPlayer = createSprite(Tex_Player, PLAYER_SIZE_X, PLAYER_SIZE_Y);
		g_pPlayer->setPos(PosFromLeftTop(5.0f, 9.0f));
		g_playerMoveSpeed_x = 0.0f;
		g_playerMoveSpeed_y = 0.0f;
	}

	//Prepare Enemy------------------------------------------------------------------------------------------------------------------------------------------------
	for (int t = 0; t < 3; ++t) {
		for (int i = 0; i < ENEMY_OBJECT_COUNT; ++i)
		{
			g_enemyObjs[t][i].obj = NULL;
			g_enemyObjs[t][i].Move = NULL;
			g_enemyObjs[t][i].AnimCounter = rand() % 15;
			g_enemyObjs[t][i].dieObj = NULL;
			g_enemyObjs[t][i].dieAnimIndex = 0;
			g_enemyObjs[t][i].dieAnimCounter = 0;
			g_enemyObjs[t][i].beginPos = PosFromLeftTop(-1.0f, 25.0f);
			if (t == eWalk)
				g_enemyObjs[t][i].velocity_X = 2.0f;
			if (t == eSpring)
				g_enemyObjs[t][i].velocity_X = 0.0f;
			if (t == eFly)
				g_enemyObjs[t][i].velocity_X = 0.0f;
			g_enemyObjs[t][i].velocity_Y = 0.0f;
			g_enemyObjs[t][i].rad = i * 2.0f;
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
	if (Difficulty == 0) {																		//----------------------------EASY------------------------------------//
		if (Stage == eStage1) {
			g_enemyObjs[eWalk][0].beginPos = PosFromLeftTop(15.0f, 12.0f);
		}
		if (Stage == eStage2) {
			g_enemyObjs[eSpring][0].beginPos = PosFromLeftTop(7.0f, 14.0f);
			g_enemyObjs[eSpring][1].beginPos = PosFromLeftTop(10.5f, 10.0f);
			g_enemyObjs[eSpring][2].beginPos = PosFromLeftTop(18.0f, 11.0f);
			g_enemyObjs[eFly][0].beginPos = PosFromLeftTop(14.0f, 9.0f);
			g_enemyObjs[eWalk][0].beginPos = PosFromLeftTop(26.0f, 13.0f);
			//g_enemyObjs[eFly][1].beginPos = PosFromLeftTop(18.0f, 7.0f);
		}
		if (Stage == eStage3) {
			g_enemyObjs[eSpring][0].beginPos = PosFromLeftTop(8.0f, 14.0f);
			g_enemyObjs[eWalk][0].beginPos = PosFromLeftTop(13.0f, 8.0f);
			g_enemyObjs[eFly][0].beginPos = PosFromLeftTop(20.0f, 12.0f);
		}
		if (Stage == eStage4) {
			g_enemyObjs[eWalk][0].beginPos = PosFromLeftTop(7.0f, 6.0f);
			g_enemyObjs[eWalk][1].beginPos = PosFromLeftTop(24.0f, 6.0f);
			g_enemyObjs[eFly][0].beginPos = PosFromLeftTop(10.5f, 6.0f);
			g_enemyObjs[eFly][1].beginPos = PosFromLeftTop(19.5f, 5.0f);
			g_enemyObjs[eFly][2].beginPos = PosFromLeftTop(15.0f, 10.0f);
			g_enemyObjs[eSpring][0].beginPos = PosFromLeftTop(11.0f, 11.0f);
			g_enemyObjs[eSpring][1].beginPos = PosFromLeftTop(19.0f, 11.0f);
		}
		if (Stage == eStagefinal) {
			g_enemyObjs[eWalk][0].beginPos = PosFromLeftTop(8.0f, 4.0f);
			g_enemyObjs[eSpring][0].beginPos = PosFromLeftTop(11.0f, 11.0f);
			g_enemyObjs[eSpring][1].beginPos = PosFromLeftTop(16.0f, 10.0f);
			g_enemyObjs[eFly][0].beginPos = PosFromLeftTop(13.5f, 8.0f);
			g_enemyObjs[eFly][1].beginPos = PosFromLeftTop(20.5f, 5.0f);
		}
	}
	else if (Difficulty == 1) {																	//----------------------------HARD------------------------------------//
		if (Stage == eStage1) {
			g_enemyObjs[eWalk][0].beginPos = PosFromLeftTop(15.0f, 12.0f);
			g_enemyObjs[eWalk][1].beginPos = PosFromLeftTop(19.0f, 12.0f);
		}
		if (Stage == eStage2) {
			g_enemyObjs[eSpring][0].beginPos = PosFromLeftTop(7.0f, 14.0f);
			g_enemyObjs[eSpring][1].beginPos = PosFromLeftTop(14.5f, 10.0f);
			g_enemyObjs[eFly][0].beginPos = PosFromLeftTop(10.0f, 9.0f);
			g_enemyObjs[eFly][1].beginPos = PosFromLeftTop(18.0f, 7.0f);
		}
		if (Stage == eStage3) {
			g_enemyObjs[eSpring][0].beginPos = PosFromLeftTop(10.0f, 15.0f);
			g_enemyObjs[eFly][0].beginPos = PosFromLeftTop(13.5f, 8.0f);
			g_enemyObjs[eFly][1].beginPos = PosFromLeftTop(17.5f, 11.0f);
		}
		if (Stage == eStage4) {
			g_enemyObjs[eWalk][0].beginPos = PosFromLeftTop(5.0f, 6.0f);
			g_enemyObjs[eWalk][1].beginPos = PosFromLeftTop(24.0f, 6.0f);
			g_enemyObjs[eFly][0].beginPos = PosFromLeftTop(10.0f, 5.0f);
			g_enemyObjs[eFly][1].beginPos = PosFromLeftTop(19.5f, 5.0f);
			g_enemyObjs[eFly][2].beginPos = PosFromLeftTop(24.5f, 11.0f);
			g_enemyObjs[eFly][3].beginPos = PosFromLeftTop(14.5f, 11.0f);
			g_enemyObjs[eFly][4].beginPos = PosFromLeftTop(7.0f, 11.0f);
			g_enemyObjs[eSpring][0].beginPos = PosFromLeftTop(11.0f, 12.0f);
			g_enemyObjs[eSpring][1].beginPos = PosFromLeftTop(20.0f, 13.0f);
		}
		if (Stage == eStagefinal) {
			g_enemyObjs[eWalk][0].beginPos = PosFromLeftTop(5.0f, 4.0f);
			g_enemyObjs[eSpring][0].beginPos = PosFromLeftTop(8.0f, 11.0f);
			g_enemyObjs[eFly][0].beginPos = PosFromLeftTop(9.0f, 8.0f);
			g_enemyObjs[eFly][1].beginPos = PosFromLeftTop(17.5f, 5.0f);
		}
	}

	for (int t = 0; t < 3; ++t) {
		for (int i = 0; i < ENEMY_OBJECT_COUNT; ++i) {
			g_enemyObjs[t][i].obj->setPos(g_enemyObjs[t][i].beginPos);
		}
	}

	//Prepare Block------------------------------------------------------------------------------------------------------------------------------------------------
	for (int i = 0; i < BLOCK_OBJECT_COUNT; ++i)
	{
		g_blockObjs[i].obj = createSprite(Tex_Block, BLOCK_SIZE_X, BLOCK_SIZE_Y);
		g_blockObjs[i].disObj = NULL;
		g_blockObjs[i].disAnimIndex = 0;
		g_blockObjs[i].disCounter = 0;
		g_blockObjs[i].beginPos = PosFromLeftTop(1.0f, 25.0f);//XMFLOAT2(0.0f, 0.0f);
		g_blockObjs[i].flag = true;
		g_blockObjs[i].drop = false;
		g_blockObjs[i].velocity_y = 0.0;
	}
	if (Difficulty == 0) {																				//----------------------------EASY------------------------------------//
		if (Stage == eStage1) {
			g_blockObjs[0].beginPos = PosFromLeftTop(5.0f, 11.0f);
			g_blockObjs[1].beginPos = PosFromLeftTop(6.0f, 11.0f);
			g_blockObjs[2].beginPos = PosFromLeftTop(7.0f, 11.0f);
			g_blockObjs[3].beginPos = PosFromLeftTop(8.0f, 11.0f);
			g_blockObjs[4].beginPos = PosFromLeftTop(9.0f, 11.0f);
			g_blockObjs[5].beginPos = PosFromLeftTop(10.0f, 11.0f);
			g_blockObjs[6].beginPos = PosFromLeftTop(11.0f, 11.0f);
			g_blockObjs[7].beginPos = PosFromLeftTop(12.0f, 11.0f);
			g_blockObjs[8].beginPos = PosFromLeftTop(13.0f, 11.0f);
			g_blockObjs[9].beginPos = PosFromLeftTop(13.0f, 12.0f);
			g_blockObjs[10].beginPos = PosFromLeftTop(13.0f, 13.0f);
			g_blockObjs[11].beginPos = PosFromLeftTop(14.0f, 13.0f);
			g_blockObjs[12].beginPos = PosFromLeftTop(15.0f, 13.0f);
			g_blockObjs[13].beginPos = PosFromLeftTop(16.0f, 13.0f);
			g_blockObjs[14].beginPos = PosFromLeftTop(17.0f, 13.0f);
			g_blockObjs[15].beginPos = PosFromLeftTop(18.0f, 13.0f);
			g_blockObjs[16].beginPos = PosFromLeftTop(19.0f, 13.0f);
			g_blockObjs[17].beginPos = PosFromLeftTop(20.0f, 13.0f);
			g_blockObjs[18].beginPos = PosFromLeftTop(21.0f, 13.0f);
			g_blockObjs[19].beginPos = PosFromLeftTop(22.0f, 13.0f);
		}
		if (Stage == eStage2) {
			g_blockObjs[0].beginPos = PosFromLeftTop(8.0f, 15.0f);
			g_blockObjs[1].beginPos = PosFromLeftTop(9.0f, 15.0f);
			g_blockObjs[2].beginPos = PosFromLeftTop(7.0f, 15.0f);
			g_blockObjs[3].beginPos = PosFromLeftTop(6.0f, 15.0f);
			g_blockObjs[4].beginPos = PosFromLeftTop(10.5f, 11.0f);
			g_blockObjs[5].beginPos = PosFromLeftTop(18.0f, 12.0f);
			g_blockObjs[6].beginPos = PosFromLeftTop(22.0f, 14.0f);
			g_blockObjs[7].beginPos = PosFromLeftTop(23.0f, 14.0f);
			g_blockObjs[8].beginPos = PosFromLeftTop(24.0f, 14.0f);
			g_blockObjs[9].beginPos = PosFromLeftTop(25.0f, 14.0f);
			g_blockObjs[10].beginPos = PosFromLeftTop(26.0f, 14.0f);
		}
		if (Stage == eStage3) {
			g_blockObjs[0].beginPos = PosFromLeftTop(3.0f, 15.0f);
			g_blockObjs[1].beginPos = PosFromLeftTop(4.0f, 15.0f);
			g_blockObjs[2].beginPos = PosFromLeftTop(5.0f, 15.0f);
			g_blockObjs[3].beginPos = PosFromLeftTop(6.0f, 15.0f);
			g_blockObjs[4].beginPos = PosFromLeftTop(7.0f, 15.0f);
			g_blockObjs[5].beginPos = PosFromLeftTop(8.0f, 15.0f);
			g_blockObjs[6].beginPos = PosFromLeftTop(12.0f, 9.0f);
			g_blockObjs[7].beginPos = PosFromLeftTop(13.0f, 9.0f);
			g_blockObjs[8].beginPos = PosFromLeftTop(14.0f, 9.0f);
			g_blockObjs[9].beginPos = PosFromLeftTop(15.0f, 9.0f);
			g_blockObjs[10].beginPos = PosFromLeftTop(16.0f, 9.0f);
			g_blockObjs[11].beginPos = PosFromLeftTop(17.0f, 9.0f);
			g_blockObjs[12].beginPos = PosFromLeftTop(22.0f, 15.0f);
			g_blockObjs[13].beginPos = PosFromLeftTop(23.0f, 15.0f);
			g_blockObjs[14].beginPos = PosFromLeftTop(24.0f, 15.0f);
			g_blockObjs[15].beginPos = PosFromLeftTop(25.0f, 15.0f);
			g_blockObjs[16].beginPos = PosFromLeftTop(26.0f, 15.0f);
			g_blockObjs[17].beginPos = PosFromLeftTop(27.0f, 15.0f);
		}
		if (Stage == eStage4) {
			g_blockObjs[0].beginPos = PosFromLeftTop(3.0f, 6.0f);
			g_blockObjs[1].beginPos = PosFromLeftTop(3.0f, 5.0f);
			g_blockObjs[2].beginPos = PosFromLeftTop(4.0f, 6.0f);
			g_blockObjs[3].beginPos = PosFromLeftTop(5.0f, 6.0f);
			g_blockObjs[4].beginPos = PosFromLeftTop(6.0f, 6.0f);
			g_blockObjs[5].beginPos = PosFromLeftTop(7.0f, 6.0f);
			g_blockObjs[17].beginPos = PosFromLeftTop(8.0f, 6.0f);
			g_blockObjs[18].beginPos = PosFromLeftTop(13.0f, 6.0f);
			g_blockObjs[6].beginPos = PosFromLeftTop(14.0f, 6.0f);
			g_blockObjs[7].beginPos = PosFromLeftTop(15.0f, 6.0f);
			g_blockObjs[8].beginPos = PosFromLeftTop(16.0f, 6.0f);
			g_blockObjs[9].beginPos = PosFromLeftTop(17.0f, 6.0f);
			g_blockObjs[10].beginPos = PosFromLeftTop(22.0f, 6.0f);
			g_blockObjs[11].beginPos = PosFromLeftTop(23.0f, 6.0f);
			g_blockObjs[12].beginPos = PosFromLeftTop(24.0f, 6.0f);
			g_blockObjs[13].beginPos = PosFromLeftTop(25.0f, 6.0f);
			g_blockObjs[14].beginPos = PosFromLeftTop(26.0f, 6.0f);
			g_blockObjs[19].beginPos = PosFromLeftTop(27.0f, 6.0f);
			g_blockObjs[20].beginPos = PosFromLeftTop(27.0f, 5.0f);
			g_blockObjs[15].beginPos = PosFromLeftTop(11.0f, 12.0f);
			g_blockObjs[16].beginPos = PosFromLeftTop(19.0f, 12.0f);
		}
		if (Stage == eStagefinal) {
			g_blockObjs[0].beginPos = PosFromLeftTop(7.0f, 6.0f);
			g_blockObjs[1].beginPos = PosFromLeftTop(6.0f, 6.0f);
			g_blockObjs[2].beginPos = PosFromLeftTop(5.0f, 6.0f);
			g_blockObjs[3].beginPos = PosFromLeftTop(8.0f, 6.0f);
			g_blockObjs[23].beginPos = PosFromLeftTop(9.0f, 6.0f);
			g_blockObjs[4].beginPos = PosFromLeftTop(10.0f, 12.0f);
			g_blockObjs[5].beginPos = PosFromLeftTop(11.0f, 12.0f);
			g_blockObjs[6].beginPos = PosFromLeftTop(12.0f, 12.0f);
			g_blockObjs[7].beginPos = PosFromLeftTop(13.0f, 12.0f);
			g_blockObjs[8].beginPos = PosFromLeftTop(17.0f, 12.0f);
			g_blockObjs[9].beginPos = PosFromLeftTop(17.0f, 11.0f);
			g_blockObjs[10].beginPos = PosFromLeftTop(17.0f, 10.0f);
			g_blockObjs[11].beginPos = PosFromLeftTop(17.0f, 9.0f);
			g_blockObjs[12].beginPos = PosFromLeftTop(17.0f, 8.0f);
			g_blockObjs[13].beginPos = PosFromLeftTop(17.0f, 7.0f);
			g_blockObjs[14].beginPos = PosFromLeftTop(17.0f, 6.0f);
			g_blockObjs[15].beginPos = PosFromLeftTop(17.0f, 5.0f);
			g_blockObjs[22].beginPos = PosFromLeftTop(16.0f, 12.0f);
			g_blockObjs[16].beginPos = PosFromLeftTop(26.0f, 9.0f);
			g_blockObjs[17].beginPos = PosFromLeftTop(25.0f, 9.0f);
			g_blockObjs[18].beginPos = PosFromLeftTop(24.0f, 9.0f);
			g_blockObjs[19].beginPos = PosFromLeftTop(24.0f, 8.0f);
			g_blockObjs[20].beginPos = PosFromLeftTop(24.0f, 7.0f);
			g_blockObjs[21].beginPos = PosFromLeftTop(24.0f, 6.0f);
		}
	}
	else if (Difficulty == 1) {																				//----------------------------HARD------------------------------------//
		if (Stage == eStage1) {
			g_blockObjs[0].beginPos = PosFromLeftTop(5.0f, 11.0f);
			g_blockObjs[1].beginPos = PosFromLeftTop(6.0f, 11.0f);
			g_blockObjs[2].beginPos = PosFromLeftTop(7.0f, 11.0f);
			g_blockObjs[3].beginPos = PosFromLeftTop(8.0f, 11.0f);
			g_blockObjs[4].beginPos = PosFromLeftTop(9.0f, 11.0f);
			g_blockObjs[5].beginPos = PosFromLeftTop(10.0f, 11.0f);
			g_blockObjs[6].beginPos = PosFromLeftTop(11.0f, 11.0f);
			g_blockObjs[7].beginPos = PosFromLeftTop(12.0f, 11.0f);
			g_blockObjs[8].beginPos = PosFromLeftTop(13.0f, 11.0f);
			g_blockObjs[9].beginPos = PosFromLeftTop(13.0f, 12.0f);
			g_blockObjs[10].beginPos = PosFromLeftTop(13.0f, 13.0f);
			g_blockObjs[11].beginPos = PosFromLeftTop(14.0f, 13.0f);
			g_blockObjs[12].beginPos = PosFromLeftTop(15.0f, 13.0f);
			g_blockObjs[13].beginPos = PosFromLeftTop(16.0f, 13.0f);
			g_blockObjs[14].beginPos = PosFromLeftTop(17.0f, 13.0f);
			g_blockObjs[15].beginPos = PosFromLeftTop(18.0f, 13.0f);
			g_blockObjs[16].beginPos = PosFromLeftTop(19.0f, 13.0f);
			g_blockObjs[17].beginPos = PosFromLeftTop(20.0f, 13.0f);
			g_blockObjs[18].beginPos = PosFromLeftTop(21.0f, 13.0f);
			g_blockObjs[19].beginPos = PosFromLeftTop(22.0f, 13.0f);
		}
		if (Stage == eStage2) {
			g_blockObjs[0].beginPos = PosFromLeftTop(8.0f, 15.0f);
			g_blockObjs[1].beginPos = PosFromLeftTop(9.0f, 15.0f);
			g_blockObjs[2].beginPos = PosFromLeftTop(7.0f, 15.0f);
			g_blockObjs[3].beginPos = PosFromLeftTop(6.0f, 15.0f);
			g_blockObjs[4].beginPos = PosFromLeftTop(14.5f, 11.0f);
			g_blockObjs[5].beginPos = PosFromLeftTop(18.0f, 7.0f);
		}
		if (Stage == eStage3) {
			g_blockObjs[0].beginPos = PosFromLeftTop(3.0f, 15.0f);
			g_blockObjs[1].beginPos = PosFromLeftTop(4.0f, 15.0f);
			g_blockObjs[2].beginPos = PosFromLeftTop(5.0f, 15.0f);
			g_blockObjs[3].beginPos = PosFromLeftTop(6.0f, 15.0f);
			g_blockObjs[4].beginPos = PosFromLeftTop(7.0f, 15.0f);
			g_blockObjs[5].beginPos = PosFromLeftTop(8.0f, 15.0f);
			g_blockObjs[6].beginPos = PosFromLeftTop(9.0f, 15.0f);
			g_blockObjs[7].beginPos = PosFromLeftTop(10.0f, 15.0f);
			g_blockObjs[8].beginPos = PosFromLeftTop(21.0f, 15.0f);
			g_blockObjs[9].beginPos = PosFromLeftTop(22.0f, 15.0f);
			g_blockObjs[10].beginPos = PosFromLeftTop(23.0f, 15.0f);
			g_blockObjs[11].beginPos = PosFromLeftTop(24.0f, 15.0f);
			g_blockObjs[12].beginPos = PosFromLeftTop(25.0f, 15.0f);
			g_blockObjs[13].beginPos = PosFromLeftTop(26.0f, 15.0f);
			g_blockObjs[14].beginPos = PosFromLeftTop(27.0f, 15.0f);
			g_blockObjs[15].beginPos = PosFromLeftTop(28.0f, 15.0f);
		}
		if (Stage == eStage4) {
			g_blockObjs[0].beginPos = PosFromLeftTop(3.0f, 6.0f);
			g_blockObjs[1].beginPos = PosFromLeftTop(3.0f, 5.0f);
			g_blockObjs[2].beginPos = PosFromLeftTop(4.0f, 6.0f);
			g_blockObjs[3].beginPos = PosFromLeftTop(5.0f, 6.0f);
			g_blockObjs[4].beginPos = PosFromLeftTop(6.0f, 6.0f);
			g_blockObjs[5].beginPos = PosFromLeftTop(7.0f, 6.0f);
			g_blockObjs[6].beginPos = PosFromLeftTop(14.0f, 6.0f);
			g_blockObjs[7].beginPos = PosFromLeftTop(15.0f, 6.0f);
			g_blockObjs[8].beginPos = PosFromLeftTop(16.0f, 6.0f);
			g_blockObjs[9].beginPos = PosFromLeftTop(17.0f, 6.0f);
			g_blockObjs[10].beginPos = PosFromLeftTop(22.0f, 6.0f);
			g_blockObjs[11].beginPos = PosFromLeftTop(23.0f, 6.0f);
			g_blockObjs[12].beginPos = PosFromLeftTop(24.0f, 6.0f);
			g_blockObjs[13].beginPos = PosFromLeftTop(25.0f, 6.0f);
			g_blockObjs[14].beginPos = PosFromLeftTop(26.0f, 6.0f);
			g_blockObjs[15].beginPos = PosFromLeftTop(11.0f, 13.0f);
			g_blockObjs[16].beginPos = PosFromLeftTop(20.0f, 14.5f);
		}
		if (Stage == eStagefinal) {
			g_blockObjs[0].beginPos = PosFromLeftTop(4.0f, 6.0f);
			g_blockObjs[1].beginPos = PosFromLeftTop(3.0f, 6.0f);
			g_blockObjs[2].beginPos = PosFromLeftTop(2.0f, 6.0f);
			g_blockObjs[3].beginPos = PosFromLeftTop(5.0f, 6.0f);
			g_blockObjs[4].beginPos = PosFromLeftTop(7.0f, 12.0f);
			g_blockObjs[5].beginPos = PosFromLeftTop(8.0f, 12.0f);
			g_blockObjs[6].beginPos = PosFromLeftTop(9.0f, 12.0f);
			g_blockObjs[7].beginPos = PosFromLeftTop(10.0f, 12.0f);
			g_blockObjs[8].beginPos = PosFromLeftTop(12.0f, 12.0f);
			g_blockObjs[9].beginPos = PosFromLeftTop(12.0f, 11.0f);
			g_blockObjs[10].beginPos = PosFromLeftTop(12.0f, 10.0f);
			g_blockObjs[11].beginPos = PosFromLeftTop(12.0f, 9.0f);
			g_blockObjs[12].beginPos = PosFromLeftTop(12.0f, 8.0f);
			g_blockObjs[13].beginPos = PosFromLeftTop(12.0f, 7.0f);
			g_blockObjs[14].beginPos = PosFromLeftTop(12.0f, 6.0f);
			g_blockObjs[15].beginPos = PosFromLeftTop(12.0f, 5.0f);
			g_blockObjs[16].beginPos = PosFromLeftTop(23.0f, 10.0f);
			g_blockObjs[17].beginPos = PosFromLeftTop(22.0f, 10.0f);
			g_blockObjs[18].beginPos = PosFromLeftTop(21.0f, 10.0f);
			g_blockObjs[19].beginPos = PosFromLeftTop(21.0f, 9.0f);
			g_blockObjs[20].beginPos = PosFromLeftTop(21.0f, 8.0f);
			g_blockObjs[21].beginPos = PosFromLeftTop(21.0f, 7.0f);
		}
	}
	for (int i = 0; i < BLOCK_OBJECT_COUNT; ++i) {
		g_blockObjs[i].obj->setPos(g_blockObjs[i].beginPos);
	}

	//SET SPAWNPOINT
	if (Stage == eStagefinal) {
		g_pPlayer->setPos(g_blockObjs[15].beginPos.x, g_blockObjs[15].beginPos.y + PLAYER_SIZE_Y);
	}
	else {
		g_pPlayer->setPos(g_blockObjs[1].beginPos.x, g_blockObjs[1].beginPos.y + PLAYER_SIZE_Y);
	}

	//Prepare AIR BLOCK------------------------------------------------------------------------------------------------------------------------------------------------
	for (int i = 0; i < AIR_OBJECT_COUNT; ++i) {
		g_air[i].obj = createSprite(Tex_Air, BLOCK_SIZE_X, BLOCK_SIZE_Y);
		g_air[i].beginPos = PosFromLeftTop(-1.0f, -1.0f);
	}
	if (Difficulty == 0) {																		//----------------------------EASY------------------------------------//
		if (Stage == eStage1) {
			g_air[0].beginPos = PosFromLeftTop(23.0f, 12.0f);
		}
		if (Stage == eStage2) {
			g_air[0].beginPos = PosFromLeftTop(21.0f, 13.0f);
			g_air[1].beginPos = PosFromLeftTop(27.0f, 13.0f);
		}
		if (Stage == eStage3) {
			g_air[0].beginPos = PosFromLeftTop(11.0f, 8.0f);
			g_air[1].beginPos = PosFromLeftTop(18.0f, 8.0f);
		}
		if (Stage == eStage4) {
			g_air[0].beginPos = PosFromLeftTop(9.0f, 5.0f);
			g_air[1].beginPos = PosFromLeftTop(21.0f, 5.0f);
		}
		if (Stage == eStagefinal) {
			g_air[0].beginPos = PosFromLeftTop(10.0f, 5.0f);
			g_air[1].beginPos = PosFromLeftTop(4.0f, 5.0f);
		}
	}
	if (Difficulty == 1) {																		//----------------------------HARD------------------------------------//
		if (Stage == eStage1) {
			g_air[0].beginPos = PosFromLeftTop(23.0f, 12.0f);
		}
		if (Stage == eStage4) {
			g_air[0].beginPos = PosFromLeftTop(8.0f, 5.0f);
			g_air[1].beginPos = PosFromLeftTop(21.0f, 5.0f);
			g_air[2].beginPos = PosFromLeftTop(27.0f, 5.0f);
		}
		if (Stage == eStagefinal) {
			g_air[0].beginPos = PosFromLeftTop(6.0f, 5.0f);
			g_air[1].beginPos = PosFromLeftTop(1.0f, 5.0f);
		}
	}

	//SETPOS
	for (int i = 0; i < AIR_OBJECT_COUNT; ++i) {
		g_air[i].obj->setPos(g_air[i].beginPos);
	}

	//背景の画像を作成
	if (g_pBackground == NULL)
	{
		g_pBackground = createSprite(Tex_Back, WINDOW_WIDTH + 10.0f, WINDOW_HEIGHT + 10.0f);
		g_pBackground->setPos(0.0f, 0.0f);
	}

	///Reset///
	g_isDead = false;
	g_endDieEffect = false;
}


void COriginalGameApp::procPlayMain()
{
	//Player Control
	if (g_pPlayer != NULL)
	{
		procPlayMain_movePlayer();

	}
	else {
		procPlayMain_animateDie();
	}
	procPlayMain_animateMove();


	//VIBRATE
	cameramove();

	//Process of SCORE
	procPlayMain_score();

	//Process of ENEMY
	procPlayMain_enemy();


	//Process of BLOCK
	procPlayMain_block();

	//Process of AIRBLOCK
	procPlayMain_airblock();

	//RENDER--------------------------------------------
	if (g_pPlayer != NULL) {
		renderSprite(g_pPlayer);
	}

	if (g_pDie != NULL) {
		renderSprite(g_pDie);
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
		g_pScore->setPos(-SCORE_SIZE_X * (float)digit, -WINDOW_HEIGHT / 2.0f + 96.0f);
	}

}

void COriginalGameApp::procPlayMain_block()
{
	//if (m_pDirectInput->isPressedOnce(DIK_X)) {
	//	for (int i = 0; i < BLOCK_OBJECT_COUNT; ++i) {
	//		if (g_blockObjs[i].obj != NULL) {
	//			if (g_blockObjs[i].obj->collisionTop(g_pPlayer) != false) {
	//				g_blockObjs[i].flag = false;
	//				break;
	//			}
	//		}
	//	}
	//}
	//Collision with enemy die
	for (int t = 0; t < 3; ++t) {
		for (int i = 0; i < ENEMY_OBJECT_COUNT; ++i) {
			for (int b = 0; b < BLOCK_OBJECT_COUNT; ++b) {
				if (g_blockObjs[b].obj != NULL) {
					if (g_enemyObjs[t][i].obj != NULL) {
						if (g_blockObjs[b].obj->collision(g_enemyObjs[t][i].obj) != false && g_enemyObjs[t][i].flag == false && g_blockObjs[b].flag != false) {
							g_enemyObjs[t][i].dieObj = createSprite(Tex_Die, ENEMY_SIZE_X * 3.0f, ENEMY_SIZE_Y * 3.0f);
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
			if (g_blockObjs[i].obj->getPos().y < -(WINDOW_WIDTH + BLOCK_SIZE_Y) / 2.0f || g_blockObjs[i].obj->getPos().y < g_blockObjs[i].beginPos.y - 3.0f * BLOCK_SIZE_Y) {
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

void COriginalGameApp::procPlayMain_airblock() {
	//collision with Air
	XMFLOAT4 pos[ENEMY_OBJECT_COUNT];
	for (int i = 0; i < ENEMY_OBJECT_COUNT; ++i) {
		if (g_enemyObjs[eWalk][i].obj != NULL) {
			pos[i] = g_enemyObjs[eWalk][i].obj->getPos();
			for (int a = 0; a < AIR_OBJECT_COUNT; ++a) {
				if (g_enemyObjs[eWalk][i].obj->collision(g_air[a].obj) != false && g_enemyObjs[eWalk][i].velocity_X > 0.0f) {
					pos[i].x = g_air[a].obj->getPos().x - (ENEMY_SIZE_X + BLOCK_SIZE_X) / 2.0f - g_enemyObjs[eWalk][i].velocity_X;
					g_enemyObjs[eWalk][i].velocity_X *= -1.0f;
					break;
				}
				if (g_enemyObjs[eWalk][i].obj->collision(g_air[a].obj) != false && g_enemyObjs[eWalk][i].velocity_X < 0.0f) {
					pos[i].x = g_air[a].obj->getPos().x - (ENEMY_SIZE_X + BLOCK_SIZE_X) / 2.0f - g_enemyObjs[eWalk][i].velocity_X;
					g_enemyObjs[eWalk][i].velocity_X *= -1.0f;
					break;
				}
				//renderSprite(g_air[a].obj);
			}
		}
	}
}

void COriginalGameApp::procPlayMain_enemy()
{
	//Movement
	XMFLOAT4 pos[3][ENEMY_OBJECT_COUNT];
	for (int t = 0; t < 3; ++t) {
		for (int i = 0; i < ENEMY_OBJECT_COUNT; ++i) {
			if (g_enemyObjs[t][i].obj != NULL) {
				pos[t][i] = g_enemyObjs[t][i].obj->getPos();

				//collision with BLOCK_Y
				for (int b = 0; b < BLOCK_OBJECT_COUNT; ++b) {
					if (g_blockObjs[b].obj != NULL) {
						if (g_enemyObjs[t][i].obj->collisionBottom(g_blockObjs[b].obj) != false) {
							g_enemyObjs[t][i].velocity_Y = 0;
							pos[t][i].y = g_blockObjs[b].obj->getPos().y + (ENEMY_SIZE_Y + BLOCK_SIZE_Y) / 2.0f;
							break;
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
					g_enemyObjs[t][i].velocity_X = 80.f * cos(g_enemyObjs[t][i].rad);
					g_enemyObjs[t][i].velocity_Y = 80.f * sin(g_enemyObjs[t][i].rad);
					pos[t][i].x = g_enemyObjs[t][i].beginPos.x + g_enemyObjs[t][i].velocity_X;
					pos[t][i].y = g_enemyObjs[t][i].beginPos.y + g_enemyObjs[t][i].velocity_Y;
					g_enemyObjs[t][i].rad += 0.05f;
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
				g_enemyObjs[t][i].obj->setPos(pos[t][i]);
			}
		}
	}

	//Die
	for (int t = 0; t < 3; ++t) {
		for (int i = 0; i < ENEMY_OBJECT_COUNT; ++i) {
			if (g_enemyObjs[t][i].obj != NULL && g_pPlayer != NULL) {
				if (g_enemyObjs[t][i].obj->collisionTop(g_pPlayer) != false) {

					//VIBRATE
					cameramove(0.f, 10.f);

					//////Fly Enemy//////
					if (t == eFly) {
						if (g_playerMoveSpeed_x > 0.0f) {
							g_enemyObjs[t][i].velocity_X = 6.0f;
						}
						else {
							g_enemyObjs[t][i].velocity_X = -6.0f;
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
				if (g_pPlayer != NULL) {
					if (g_enemyObjs[t][i].obj->collision(g_pPlayer) != false && g_pPlayer != NULL) {
						cameramove(5.f, 0.f);
						g_pDie = createSprite(Tex_Player_Die, DIE_SIZE_X, DIE_SIZE_Y);
						g_pDie->setPos(g_pPlayer->getPos());
						g_isDead = true;
						disposeSprite(g_pPlayer);
						g_pPlayer = NULL;
						break;
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

	////Move Animation
	for (int t = 0; t < 3; ++t) {
		for (int i = 0; i < ENEMY_OBJECT_COUNT; ++i) {
			if (g_enemyObjs[t][i].obj != NULL) {
				if (g_enemyObjs[t][i].Move == NULL) {
					if (t == eWalk) {
						g_enemyObjs[t][i].Move = createSprite(Tex_Enemy_0_Move, ENEMY_SIZE_X * 3.0, ENEMY_SIZE_Y * 3.0);
					}
					else if (t == eSpring) {
						g_enemyObjs[t][i].Move = createSprite(Tex_Enemy_1_Move, ENEMY_SIZE_X * 3.0, ENEMY_SIZE_Y * 3.0);
					}
					else if (t == eFly) {
						g_enemyObjs[t][i].Move = createSprite(Tex_Enemy_2_Move, ENEMY_SIZE_X * 3.0, ENEMY_SIZE_Y * 3.0);
					}
				}
				else {
					g_enemyObjs[t][i].Move->setPos(g_enemyObjs[t][i].obj->getPos());
					++g_enemyObjs[t][i].AnimCounter;
					if (g_enemyObjs[t][i].AnimCounter % 15 == 0) {
						if (g_enemyObjs[t][i].flag != false)
							g_enemyObjs[t][i].Move->stepAnimation();
						g_enemyObjs[t][i].AnimCounter = 0;

					}
					if (t == eFly && g_enemyObjs[t][i].flag != false) {
						if (g_enemyObjs[t][i].obj->getPos().y > g_enemyObjs[t][i].beginPos.y) {
							g_enemyObjs[t][i].Move->setAngle(XMFLOAT3{ 0.0f,180.0f,0.0f });
						}
						else {
							g_enemyObjs[t][i].Move->setAngle(XMFLOAT3{ 0.0f,0.0f,0.0f });
						}
					}
					else {
						if (g_enemyObjs[t][i].velocity_X > 0.0f) {
							g_enemyObjs[t][i].Move->setAngle(XMFLOAT3{ 0.0f,0.0f,0.0f });
						}
						else {
							g_enemyObjs[t][i].Move->setAngle(XMFLOAT3{ 0.0f,180.0f,0.0f });
						}
					}
					g_enemyObjs[t][i].obj->setActive(false);
					renderSprite(g_enemyObjs[t][i].Move);
				}
			}
			else {
				disposeSprite(g_enemyObjs[t][i].Move);
				g_enemyObjs[t][i].Move = NULL;
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
			g_playerMoveSpeed_x = 5.0f;
			angle.y = 0.0f;
			g_pPlayer->setAngle(angle);
			//g_pPlayer_Move->setAngle(angle);
		}
		if (m_pDirectInput->isKeyPressed(DIK_LEFTARROW)) {
			g_playerMoveSpeed_x = -5.0f;
			angle.y = 180.0f;
			g_pPlayer->setAngle(angle);
			//g_pPlayer_Move->setAngle(angle);
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
			g_playerMoveSpeed_x = (float)(int)g_playerMoveSpeed_x;
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
				if (g_pPlayer->collisionBottom(g_blockObjs[i].obj) != false && g_playerMoveSpeed_y < 0.0f) {
					pos.y = g_blockObjs[i].obj->getPos().y + ((BLOCK_SIZE_Y + PLAYER_SIZE_Y) / 2.0f);
					g_playerMoveSpeed_y = 0.0f;
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

void COriginalGameApp::procPlayMain_animateDie()
{
	g_DieAnimCounter++;
	if (g_DieAnimCounter % 4 == 0)
	{
		g_pDie->stepAnimation();
		g_DieAnimCounter = 0;
		g_DieAnimIndex++;
	}
	if (g_DieAnimIndex == 5)
	{
		g_DieAnimIndex = 0;
		g_endDieEffect = true;
	}
}

void COriginalGameApp::procPlayMain_animateMove()
{
	if (g_pPlayer_Move == NULL) {
		g_pPlayer_Move = createSprite(Tex_Player_Move, PLAYER_SIZE_X * 3.0f, PLAYER_SIZE_X * 3.0f);
	}
	if (g_pPlayer != NULL) {
		if (g_pPlayer_Move != NULL) {
			g_pPlayer_Move->setPos(g_pPlayer->getPos());
			g_pPlayer->setActive(false);
			if (g_playerMoveSpeed_x > 0.0) {
				g_pPlayer_Move->setAngle(XMFLOAT3(0.0f, 0.0f, 0.0f));
			}
			if (g_playerMoveSpeed_x < 0.0) {
				g_pPlayer_Move->setAngle(XMFLOAT3(0.0f, 180.0f, 0.0f));
			}
			if ((int)g_playerMoveSpeed_x == 0) {
				g_pPlayer_Move->setAnimation(0);
				AnimCounter = 5;
			}
			else {
				++AnimCounter;
				if (AnimCounter % 8 == 0) {
					g_pPlayer_Move->stepAnimation();
					AnimCounter = 0;
				}
			}
			renderSprite(g_pPlayer_Move);
		}
	}
	else {
		disposeSprite(g_pPlayer_Move);
		g_pPlayer_Move = NULL;
	}
}

void COriginalGameApp::procPlayEnd()
{
	if (g_pPlayer != NULL)
	{
		disposeSprite(g_pPlayer);
		g_pPlayer = NULL;
	}
	if (g_pPlayer_Move != NULL) {
		disposeSprite(g_pPlayer_Move);
		g_pPlayer_Move = NULL;
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
	if (g_pDie != NULL) {
		disposeSprite(g_pDie);
		g_pDie = NULL;
	}

	for (int i = 0; i < BLOCK_OBJECT_COUNT; ++i) {
		if (g_blockObjs[i].obj != NULL) {
			disposeSprite(g_blockObjs[i].obj);
			g_blockObjs[i].obj = NULL;
		}
	}

	for (int i = 0; i < AIR_OBJECT_COUNT; ++i) {
		if (g_air[i].obj != NULL) {
			disposeSprite(g_air[i].obj);
			g_air[i].obj = NULL;
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

//void COriginalGameApp::procPlayMains_camera()
//{
//	static int counter = 0;
//	if (m_pCamera->getPos().x != 0.0f || m_pCamera->getPos().y != 0.0f) {
//		++counter;
//		if (counter % 8 == 0) {
//			m_pCamera->setPos(0.0f, 0.0f, m_pCamera->getPos().z);
//			counter = 0;
//		}
//	}
//}

XMFLOAT2 PosFromLeftTop(float x, float y)
{
	float PosFromLeftTop_x = -(WINDOW_WIDTH + BLOCK_SIZE_X) / 2.0f + (BLOCK_SIZE_X * x);
	float PosFromLeftTop_y = (WINDOW_HEIGHT + BLOCK_SIZE_Y) / 2.0f - (BLOCK_SIZE_Y * y);

	return { PosFromLeftTop_x, PosFromLeftTop_y };
}

void COriginalGameApp::cameramove()
{
	if (amplitube.x == 0.f && amplitube.y == 0.f)return;
	XMFLOAT3 cpos = m_pCamera->getPos();
	if (amplitube.x != 0.f) {
		static double radX = 0.0;
		radX += 1.5;
		if (radX > 2.0 * M_PI) {
			radX = 0.0;
			amplitube.x *= 0.5;
		}
		if ((int)amplitube.x == 0)
			amplitube.x = 0.f;
		cpos.x = amplitube.x * sin(radX);
	}
	if (amplitube.y != 0.f) {
		static double radY = 0.0;
		radY += 1.5;
		if (radY > 2.0 * M_PI) {
			radY = 0.0;
			amplitube.y *= 0.5;
		}
		if ((int)amplitube.y == 0)
			amplitube.y = 0.f;
		cpos.y = amplitube.y * sin(radY);
	}
	m_pCamera->setPos(cpos);
}

void COriginalGameApp::cameramove(float _amplitubeX, float _amplitubeY)
{
	if (_amplitubeX != 0.f)
		amplitube.x = _amplitubeX;
	if (_amplitubeY != 0.f)
		amplitube.y = _amplitubeY;
}
//=========================================================================