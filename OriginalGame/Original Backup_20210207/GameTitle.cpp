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
CPicture* g_pTitleBackground = NULL;
const XMFLOAT2 beginPos = { 0.0f,175.0f };

int Counter = 0;
CPicture* g_pPlayer_Title = NULL;
int PlayerCounter = 0;
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
	for (int i = 0; i < BLOCK_OBJECT_COUNT; ++i) {
		if (g_pBlock[i] == NULL) {
			g_pBlock[i] = createSprite(Tex_Block, BLOCK_SIZE_X, BLOCK_SIZE_Y);
			g_pBlock[i]->setPos(-(WINDOW_WIDTH - BLOCK_SIZE_X) / 2.0f + i * BLOCK_SIZE_X, -300.0f);
		}
	}
	if (g_pPlayer_Title == NULL) {
		g_pPlayer_Title = createSprite(Tex_Player_Move, 64.0f * 3.0f, 64.0f * 3.0f);
		g_pPlayer_Title->setPos(-WINDOW_WIDTH / 2.0f - 64.0f, g_pBlock[0]->getPos().y + 64.0f);
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
}

/*
 * 関数名　　：procTitleMain()
 * 処理内容　：タイトル画面のメイン処理
 *	引数　　　：なし
 *	戻り値　　：なし
 */
void COriginalGameApp::procTitleMain()
{
	//Xキーを押されたらフェーズを終了に変更し
	//ゲーム本編にステータスを移行する準備をする
	if (getInput()->isPressedOnce(DIK_X))
	{
		g_TitlePhase = eEnd;
		g_NextStatusFromTitle = ePlaying;
	}

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
	//Render [Press]
	if (g_pPress != NULL) {
		renderSprite(g_pPress);
	}
	//画像が作成されていたら描画する
	if (g_pTitle != NULL)
	{
		static float rad = 0.0f;
		rad += 0.05;
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