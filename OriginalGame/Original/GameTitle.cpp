#include "COriginalGameApp.h"
#include "GameResource.h"
#define _USE_MATH_DEFINES
#include <math.h>

//変数定義-----------------------------------------------------------------------
//タイトル画面のフェーズを管理する変数
ePhase g_TitlePhase = eBegin;
//タイトル画面の画像を管理する変数
CPicture* g_pTitle = NULL;
CPicture* g_pPress = NULL;
CPicture* g_pDifficulty = NULL;
CPicture* g_pArrow[2] = { NULL };
int ArrowCnt = 0;
CPicture* g_pTitleBackground = NULL;
const XMFLOAT2 beginPos = { 0.0f,175.0f };

int Counter = 0;
CPicture* g_pPlayer_Title = NULL;
int PlayerCounter = 0;
float moveY = 0.0f;
CPicture* g_pEnemy = NULL;
int EnemyCounter = 0;

const float BLOCK_SIZE_X = 64.0f;
const float BLOCK_SIZE_Y = 64.0f;
const int BLOCK_OBJECT_COUNT = 30;
CPicture* g_pBlock[BLOCK_OBJECT_COUNT];
//タイトルから次へ進むときのフローを管理する変数
eGameStatus g_NextStatusFromTitle = eNone;

/*
 * タイトルの処理を行う部分
 */
void COriginalGameApp::procTitle()
{
	//フェーズの進行状態によって処理を切り替える
	switch (g_TitlePhase)
	{
		//開始時
	case eBegin:
	{
		//タイトル画面の開始時の処理を行う
		procTitleBegin();
		//進行状態のメインに切り替える
		g_TitlePhase = eMain;
	}
	break;
	//メイン
	case eMain:
	{
		//タイトル画面のメイン処理を行う
		procTitleMain();
	}
	break;
	//終了時
	case eEnd:
	{
		//タイトル画面の終了処理を行う
		procTitleEnd();
		//再度タイトル画面に来た時に開始から処理を行うために
		//フェーズを戻しておく
		g_TitlePhase = eBegin;
		//次のステータスを設定する
		SetNextGameStatus(g_NextStatusFromTitle);
	}
	break;
	}
}

/*
 * 関数名　：procTitleBegin()
 * 処理内容　：タイトル画面の開始時に行う処理
 * 引数　　　：なし
 * 戻り値　　：なし
 */
void COriginalGameApp::procTitleBegin()
{
	//g_pTitleがNULLの時のみ画像を作成する
	if (g_pTitle == NULL)
	{
		//タイトルの画像を作成
		g_pTitle = createSprite(Tex_Title, 1098.0f, 190.0f);
		g_pTitle->setPos(beginPos);
	}
	if (g_pPress == NULL) {
		//タイトルの画像を作成
		g_pPress = createSprite(Tex_Press, 812.0f, 62.0f);
		g_pPress->setPos(XMFLOAT2(0.0f, -WINDOW_HEIGHT / 2.0f + 96.0f));
	}
	if (g_pDifficulty == NULL) {
		g_pDifficulty = createSprite(Tex_Difficulty, 353.0f / 2.0f, 59.0f);
		g_pDifficulty->setPos(XMFLOAT2(0.0f, -WINDOW_HEIGHT / 2.0f + 96.0f));
	}
	for (int i = 0; i < 2; ++i) {
		if (g_pArrow[i] == NULL) {
			g_pArrow[i] = createSprite(Tex_Arrow, 31.0f, 36.0f);
			if (i == 0) {
				g_pArrow[i]->setPos(g_pDifficulty->getPos().x - 200.0f, g_pDifficulty->getPos().y);
			}
			else {
				g_pArrow[i]->setPos(g_pDifficulty->getPos().x + 200.0f, g_pDifficulty->getPos().y);
				g_pArrow[i]->setAngle(XMFLOAT3(0.0f, 180.0f, 0.0f));
			}
		}
	}
	for (int i = 0; i < BLOCK_OBJECT_COUNT; ++i) {
		if (g_pBlock[i] == NULL) {
			g_pBlock[i] = createSprite(Tex_Block, BLOCK_SIZE_X, BLOCK_SIZE_Y);
			g_pBlock[i]->setPos(-(WINDOW_WIDTH - BLOCK_SIZE_X) / 2.0f + i * BLOCK_SIZE_X, -300.0f);
		}
	}
	if (g_pPlayer_Title == NULL) {
		g_pPlayer_Title = createSprite(Tex_Player_Move, 64.0f * 3.0f, 64.0f * 3.0f);
		g_pPlayer_Title->setPos(-WINDOW_WIDTH / 2.0f - 64.0f, g_pBlock[0]->getPos().y + 64.0f);
		moveY = 0.0f;
	}
	if (g_pEnemy == NULL) {
		g_pEnemy = createSprite(Tex_Enemy_0_Move, 64.0f * 3.0f, 64.0f * 3.0f);
		g_pEnemy->setPos(-WINDOW_WIDTH / 2.0f - 64.0f, g_pBlock[0]->getPos().y + 64.0f);
	}

	if (g_pTitleBackground == NULL) {
		g_pTitleBackground = createSprite(Tex_Back);
	}

	//RESET
	PlayerCounter = 0;
	Difficulty = 0;
}

/*
 * 関数名　　：procTitleMain()
 * 処理内容　：タイトル画面のメイン処理
 *	引数　　　：なし
 *	戻り値　　：なし
 */
void COriginalGameApp::procTitleMain()
{
	////Xキーを押されたらフェーズを終了に変更し
	////ゲーム本編にステータスを移行する準備をする
	//if (getInput()->isPressedOnce(DIK_X))
	//{
	//	g_TitlePhase = eEnd;
	//	g_NextStatusFromTitle = eGameClear;
	//}

	++Counter;
	//Render Block
	for (int i = 0; i < BLOCK_OBJECT_COUNT; ++i) {
		if (g_pBlock[i] != NULL) {
			renderSprite(g_pBlock[i]);
		}
	}
	//Render Player
	if (g_pPlayer_Title != NULL) {
		++PlayerCounter;
		if (PlayerCounter % 8 == 0) {
			g_pPlayer_Title->stepAnimation();
			PlayerCounter = 0;
		}
		if (Counter >= 100) {
			g_pPlayer_Title->setPos(g_pPlayer_Title->getPos().x + 4.0f, g_pPlayer_Title->getPos().y);

		}

		renderSprite(g_pPlayer_Title);
	}
	if (g_pEnemy != NULL) {
		//Move
		g_pEnemy->setPos(g_pEnemy->getPos().x + 3.0f, g_pEnemy->getPos().y);
		++EnemyCounter;
		if (EnemyCounter % 12 == 0) {
			g_pEnemy->stepAnimation();
			EnemyCounter = 0;
		}
		renderSprite(g_pEnemy);
	}
	if (g_pDifficulty != NULL && g_pPress == NULL) {
		//ARROW
		for (int i = 0; i < 2; ++i) {
			if (g_pArrow[i] != NULL) {
				renderSprite(g_pArrow[i]);
			}
		}
		//DIFFICULTY SETTING
		if (getInput()->isPressedOnce(DIK_LEFTARROW)) {
			--Difficulty;
			if (g_pArrow[0] != NULL) {
				g_pArrow[0]->setPos(g_pDifficulty->getPos().x - 203.0f, g_pDifficulty->getPos().y);
			}
		}
		else if (getInput()->isPressedOnce(DIK_RIGHTARROW)) {
			++Difficulty;
			if (g_pArrow[1] != NULL) {
				g_pArrow[1]->setPos(g_pDifficulty->getPos().x + 203.0f, g_pDifficulty->getPos().y);
			}
		}
		else {
			static int counter = 0;
			counter++;
			if (counter % 10 == 0) {
				g_pArrow[0]->setPos(g_pDifficulty->getPos().x - 200.0f, g_pDifficulty->getPos().y);
				g_pArrow[1]->setPos(g_pDifficulty->getPos().x + 200.0f, g_pDifficulty->getPos().y);
				counter = 0;
			}
		}
		//ADJUST
		if (Difficulty < 0) {
			Difficulty = 1;
		}
		else if (Difficulty > 1) {
			Difficulty = 0;
		}
		if (Difficulty == 0) {
			g_pDifficulty->setAnimation(0);
		}
		if (Difficulty == 1) {
			g_pDifficulty->setAnimation(1);
		}

		//Xキーを押されたらフェーズを終了に変更し
		//ゲーム本編にステータスを移行する準備をする
		if (getInput()->isPressedOnce(DIK_X))
		{
			g_TitlePhase = eEnd;
			g_NextStatusFromTitle = ePlaying;
		}

		renderSprite(g_pDifficulty);
	}
	if (g_pPress != NULL) {
		if (getInput()->isPressedOnce(DIK_X)) {
			disposeSprite(g_pPress);
			g_pPress = NULL;
		}
	}
	//Render [Press]
	if (g_pPress != NULL) {
		renderSprite(g_pPress);
	}
	//画像が作成されていたら描画する
	if (g_pTitle != NULL)
	{
		static float rad = 0.0f;
		rad += 0.02f;
		g_pTitle->setPos(beginPos.x, beginPos.y + 10.f * sin(rad));
		if (rad > 2.0 * M_PI) {
			rad = 0.0f;
		}
		renderSprite(g_pTitle);
	}

	if (g_pTitleBackground != NULL) {
		renderSprite(g_pTitleBackground);
	}

}
/*
* 関数名　　：procTitleEnd()
* 処理内容　：タイトル画面の終了時に行う処理
* 引数　　　：なし
* 戻り値　　：なし
*/
void COriginalGameApp::procTitleEnd()
{
	//g_pTitleが作成されている時のみ破棄する
	if (g_pTitle != NULL)
	{
		//タイトル画面の画像を破棄する
		disposeSprite(g_pTitle);
		g_pTitle = NULL;
	}
	if (g_pPress != NULL) {
		disposeSprite(g_pPress);
		g_pPress = NULL;
	}
	if (g_pDifficulty != NULL) {
		disposeSprite(g_pDifficulty);
		g_pDifficulty = NULL;
	}
	for (int i = 0; i < 2; ++i) {
		if (g_pArrow[i] != NULL) {
			disposeSprite(g_pArrow[i]);
			g_pArrow[i] = NULL;
		}
	}
	for (int i = 0; i < BLOCK_OBJECT_COUNT; ++i) {
		if (g_pBlock[i] != NULL) {
			disposeSprite(g_pBlock[i]);
			g_pBlock[i] = NULL;
		}
	}
	if (g_pPlayer_Title != NULL) {
		disposeSprite(g_pPlayer_Title);
		g_pPlayer_Title = NULL;
	}
	if (g_pEnemy != NULL) {
		disposeSprite(g_pEnemy);
		g_pEnemy = NULL;
	}
	if (g_pTitleBackground != NULL) {
		disposeSprite(g_pTitleBackground);
		g_pTitleBackground = NULL;
	}
}