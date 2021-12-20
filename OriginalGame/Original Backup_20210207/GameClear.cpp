#include "COriginalGameApp.h"
#include "GameResource.h"

//変数定義-----------------------------------------------------------------------
//クリア画面のフェーズを管理する変数
ePhase g_ClearPhase = eBegin;
//クリア画像を管理する変数
CPicture* g_pClear = NULL;
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
	//g_pClearがNULLの時のみ画像を作成する
	if (g_pClear == NULL)
	{
		//クリア画面の画像を作成
		g_pClear = createSprite(Tex_Clear);
	}
}

/*
 * 関数名　　：procClearMain()
 * 処理内容　：クリア画面のメイン処理
 *	引数　　　：なし
 *	戻り値　　：なし
 */
void COriginalGameApp::procClearMain()
{
	//Xが入力されたときはタイトルに移行
	if (getInput()->isPressedOnce(DIK_X))
	{
		//タイトルにステータスを移行する準備をする
		g_ClearPhase = eEnd;
		g_NextStatusFromClear = eOpening;
	}
	//クリア画面の画像が作成されていたら描画する
	if (g_pClear != NULL)
	{
		renderSprite(g_pClear);
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
}
