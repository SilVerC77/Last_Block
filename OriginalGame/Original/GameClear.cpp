#include "COriginalGameApp.h"
#include "GameResource.h"
#define _USE_MATH_DEFINES
#include <math.h>

//変数定義-----------------------------------------------------------------------
//クリア画面のフェーズを管理する変数
ePhase g_ClearPhase = eBegin;

//クリア画像を管理する変数
CPicture* g_pClear = NULL;
float scale = 1.0f;
const float CLEAR_SIZE_X = 1962.0f;
const float CLEAR_SIZE_Y = 1122.0f;
CPicture* g_pClearPress = NULL;
CPicture* g_pPlatform[2] = { NULL };
CPicture* g_pClearBackground = NULL;
CPicture* g_pCredit = NULL;
//CPicture* g_pEye = NULL;
//CPicture* g_pEyelid = NULL;
//int EyelidCnt = 0;
//int EyelidAnimIndex = 0;
//CPicture* g_pRate = NULL;
const float CREDIT_SIZE_X = 1920.0f;
const float CREDIT_SIZE_Y = 1881.0f;
CFont* g_pDead = NULL;
float fontpos = 0.0f;

//COUNTER
int framecounter = 0;

const float slidespeed = 1.5f;

struct ClearChar
{
	CPicture* obj;
	CPicture* dieObj;
	int animIndex;
	int counter;
	float size_x;
	float size_y;
};
ClearChar Player;
ClearChar Player1;
ClearChar Enemy_Walk;
ClearChar Enemy_Spring;
ClearChar Enemy_Fly;


//クリア画面から次へ進むときのフローを管理する変数
eGameStatus g_NextStatusFromClear = eNone;

/*
 * ゲームクリア画面の処理を行う部分
 */
void COriginalGameApp::procClear()
{
	//フェーズの進行状態によって処理を切り替える
	switch (g_ClearPhase)
	{
		//開始時
	case eBegin:
	{
		//クリア画面の開始処理を行う
		procClearBegin();
		//進行状態のメインに切り替える
		g_ClearPhase = eMain;
	}
	break;
	//メイン
	case eMain:
	{
		//クリア画面のメイン処理
		procClearMain();
	}
	break;
	//終了時
	case eEnd:
	{
		//クリア画面の終了処理を行う
		procClearEnd();
		//再度クリア画面に来た時に開始から処理を行うためにフェーズを戻しておく
		g_ClearPhase = eBegin;
		//次のステータスを設定する
		SetNextGameStatus(g_NextStatusFromClear);
	}
	break;
	}
}


/*
 * 関数名　：procClearBegin()
 * 処理内容　：クリア画面の開始時に行う処理
 * 引数　　　：なし
 * 戻り値　　：なし
 */
void COriginalGameApp::procClearBegin()
{
	if (Player.obj == NULL) {
		Player.size_x = 192.0f;
		Player.size_y = 192.0f;
		Player.obj = createSprite(Tex_Player_Move, Player.size_x, Player.size_y);
		Player.dieObj = NULL;
		Player.counter = 0;
		Player.animIndex = 1;
	}
	if (Player1.obj == NULL) {
		Player1.size_x = 192.0f;
		Player1.size_y = 192.0f;
		Player1.obj = createSprite(Tex_Player_Move, Player.size_x, Player.size_y);
		Player1.dieObj = NULL;
		Player1.counter = 0;
		Player1.animIndex = 1;
		Player1.obj->setPos(-(WINDOW_WIDTH + Player.size_x) / 2.0f, 0.0f);
	}
	if (Enemy_Walk.obj == NULL) {
		Enemy_Walk.size_x = 192.0f;
		Enemy_Walk.size_y = 192.0f;
		Enemy_Walk.obj = createSprite(Tex_Enemy_0_Move, Enemy_Walk.size_x, Enemy_Walk.size_y);
	}
	if (Enemy_Spring.obj == NULL) {
		Enemy_Spring.size_x = 192.0f;
		Enemy_Spring.size_y = 192.0f;
		Enemy_Spring.obj = createSprite(Tex_Enemy_1_Move, Enemy_Spring.size_x, Enemy_Spring.size_y);
	}
	if (Enemy_Fly.obj == NULL) {
		Enemy_Fly.size_x = 192.0f;
		Enemy_Fly.size_y = 192.0f;
		Enemy_Fly.obj = createSprite(Tex_Enemy_2_Move, Enemy_Fly.size_x, Enemy_Fly.size_y);
	}

	//g_pClearがNULLの時のみ画像を作成する
	if (g_pClear == NULL) {
		//クリア画面の画像を作成
		g_pClear = createSprite(Tex_Clear, CLEAR_SIZE_X, CLEAR_SIZE_Y);
		g_pClear->setPos(0.0f, 0.0f);
		scale = 1.0f;
	}
	if (g_pCredit == NULL) {
		g_pCredit = createSprite(Tex_Credit, CREDIT_SIZE_X, CREDIT_SIZE_Y);
		g_pCredit->setPos(0.0f, -(WINDOW_HEIGHT + CREDIT_SIZE_Y) / 2.0f);
	}

	if (g_pPlatform[0] == NULL) {
		g_pPlatform[0] = createSprite(Tex_Platform, 562.0f, 68.0f);
	}
	if (g_pPlatform[1] == NULL) {
		g_pPlatform[1] = createSprite(Tex_Platform, 64, 16.0f);
		g_pPlatform[1]->setPos(Player1.obj->getPos());
	}

	if (g_pClearBackground == NULL) {
		//クリア画面の画像を作成
		g_pClearBackground = createSprite(Tex_GameClear);
		g_pClearBackground->setPos(0.0f, 0.0f);
	}
	if (g_pClearPress == NULL) {
		//タイトルの画像を作成
		g_pClearPress = createSprite(Tex_Press, 812.0f, 62.0f);
		g_pClearPress->setPos(0.0f, -WINDOW_HEIGHT / 2.0f + 96.0f);
	}

	int digit = 1;
	if (DeadCounter > 9)
		digit = 2;
	if (DeadCounter > 99)
		digit = 3;
	if (DeadCounter > 999)
		digit = 4;
	fontpos = -(WINDOW_HEIGHT + 62.0f) / 2.0f;
	if (g_pDead == NULL) {
		g_pDead = createFont(Tex_Number, digit, 42.0f, 62.0f);
		g_pDead->setPos(0.0f, fontpos);
	}

	framecounter = 0;
}

/*
 * 関数名　　：procClearMain()
 * 処理内容　：クリア画面のメイン処理
 *	引数　　　：なし
 *	戻り値　　：なし
 */
void COriginalGameApp::procClearMain()
{
	++framecounter;
	//if (framecounter >= 1800) {
	//	framecounter = 1800;
	//}
	if (framecounter >= 2640) {
		framecounter = 2640;
		if (g_pClearPress != NULL)
			renderSprite(g_pClearPress);
		//Xが入力されたときはタイトルに移行
		if (getInput()->isPressedOnce(DIK_X))
		{
			//タイトルにステータスを移行する準備をする
			g_ClearPhase = eEnd;
			g_NextStatusFromClear = eOpening;
		}
	}

	if (Player1.obj != NULL) {
		if (Player1.obj->getPos().x >= -32.0f) {
			Player1.dieObj = createSprite(Tex_Player_Die, Player1.size_x, Player1.size_y);
			Player1.dieObj->setPos(Player1.obj->getPos());
			disposeSprite(Player1.obj);
			Player1.obj = NULL;
			Player1.counter = 0;
		}
	}
	if (Player1.dieObj != NULL) {
		Player1.counter++;
		if (Player1.counter % 4 == 0 && Player1.animIndex < 5) {
			Player1.dieObj->stepAnimation();
			Player1.animIndex++;
		}
		Player1.dieObj->setPos(Player1.dieObj->getPos().x, fontpos);
		g_pPlatform[1]->setPos(Player1.dieObj->getPos().x, Player1.dieObj->getPos().y - 32.0f);
		renderSprite(Player1.dieObj);
	}
	if (framecounter >= 420 && Player1.obj != NULL) {
		Player1.counter++;
		if (Player1.counter % 8 == 0) {
			Player1.obj->stepAnimation();
		}
		Player1.obj->setPos(Player1.obj->getPos().x + 4.0f, Player1.obj->getPos().y);
		g_pPlatform[1]->setPos(Player1.obj->getPos().x, Player1.obj->getPos().y - 32.0f);
		renderSprite(Player1.obj);
	}

	if (g_pDead != NULL) {
		if (framecounter >= 300) {
			fontpos += slidespeed;
			g_pDead->setPos(0.0f, fontpos);
		}
		renderFont(g_pDead, DeadCounter);
	}

	if (g_pClear != NULL) {
		if (framecounter >= 120) {
			if (scale > 0.4f)
				scale -= 0.005f;
			else {
				g_pClear->setPos(g_pClear->getPos().x, g_pClear->getPos().y + slidespeed);
			}
			g_pClear->setScale(scale, scale);
		}
		renderSprite(g_pClear);
	}

	if (g_pCredit != NULL) {
		if (framecounter >= 800 && g_pCredit->getPos().y <= CREDIT_SIZE_Y / 2.0f) {
			g_pCredit->setPos(0.0f, g_pCredit->getPos().y + slidespeed);
		}

		renderSprite(g_pCredit);
	}

	if (Enemy_Spring.obj != NULL) {
		Enemy_Spring.obj->setPos(Player.obj->getPos().x, Player.obj->getPos().y + 370.0f);

		++Enemy_Spring.counter;
		if (Enemy_Spring.counter % 15 == 0) {
			Enemy_Spring.obj->stepAnimation();
			Enemy_Spring.counter = 0;
		}

		renderSprite(Enemy_Spring.obj);
	}

	if (Player.obj != NULL) {
		Player.obj->setPos(0.0f, g_pCredit->getPos().y - CREDIT_SIZE_Y / 2.0f - 250.0f);
		++Player.counter;
		if (Player.counter % 8 == 0) {
			Player.obj->stepAnimation();
			Player.counter = 0;
		}
		renderSprite(Player.obj);
	}

	if (Enemy_Walk.obj != NULL) {
		Enemy_Walk.obj->setPos(132.0f, Player.obj->getPos().y);

		++Enemy_Walk.counter;
		if (Enemy_Walk.counter % 15 == 0) {
			Enemy_Walk.obj->stepAnimation();
			Enemy_Walk.counter = 0;
		}
		renderSprite(Enemy_Walk.obj);
	}

	if (Enemy_Fly.obj != NULL) {
		static float rad = 0.0f;
		rad += 0.05f;
		if (rad > 2.0f * M_PI) {
			rad = 0.0f;
		}
		Enemy_Fly.obj->setPos(-132.0f, Player.obj->getPos().y + 32.0f + (10.0f * sin(rad)));

		++Enemy_Fly.counter;
		if (Enemy_Fly.counter % 10 == 0) {
			Enemy_Fly.obj->stepAnimation();
			Enemy_Fly.counter = 0;
		}

		renderSprite(Enemy_Fly.obj);
	}
	if (g_pPlatform[0] != NULL) {
		g_pPlatform[0]->setPos(0.0f, Player.obj->getPos().y - 32.0f);

		renderSprite(g_pPlatform[0]);
	}
	if (g_pPlatform[1] != NULL) {
		renderSprite(g_pPlatform[1]);
	}
	//クリア画面の画像が作成されていたら描画する
	if (g_pClearBackground != NULL)
	{
		renderSprite(g_pClearBackground);
	}
}
/*
* 関数名　　：procClearEnd()
* 処理内容　：クリア画面の終了時に行う処理
* 引数　　　：なし
* 戻り値　　：なし
*/
void COriginalGameApp::procClearEnd()
{
	//g_pCLearが作成されている時のみ破棄する
	if (g_pClear != NULL)
	{
		//クリア画面の画像を破棄する
		disposeSprite(g_pClear);
		g_pClear = NULL;
	}
	if (g_pClear != NULL) {
		disposeSprite(g_pClear);
		g_pClear = NULL;
	}
	if (g_pCredit != NULL) {
		disposeSprite(g_pCredit);
		g_pCredit = NULL;
	}
	if (g_pClearBackground != NULL) {
		disposeSprite(g_pClearBackground);
		g_pClearBackground = NULL;
	}
	if (Player.obj != NULL) {
		disposeSprite(Player.obj);
		Player.obj = NULL;
	}
	if (Enemy_Walk.obj != NULL) {
		disposeSprite(Enemy_Walk.obj);
		Enemy_Walk.obj = NULL;
	}
	if (Enemy_Spring.obj != NULL) {
		disposeSprite(Enemy_Spring.obj);
		Enemy_Spring.obj = NULL;
	}
	if (Enemy_Fly.obj != NULL) {
		disposeSprite(Enemy_Fly.obj);
		Enemy_Fly.obj = NULL;
	}
	if (g_pPlatform[0] != NULL) {
		disposeSprite(g_pPlatform[0]);
		g_pPlatform[0] = NULL;
	}
	if (g_pPlatform[1] != NULL) {
		disposeSprite(g_pPlatform[1]);
		g_pPlatform[1] = NULL;
	}
	if (g_pClearPress != NULL) {
		disposeSprite(g_pClearPress);
		g_pClearPress = NULL;
	}
	if (Player1.obj != NULL) {
		disposeSprite(Player1.obj);
		Player1.obj = NULL;
	}
	if (g_pDead != NULL) {
		disposeFont(g_pDead);
		g_pDead = NULL;
	}
	//reset
	DeadCounter = 0;
}
