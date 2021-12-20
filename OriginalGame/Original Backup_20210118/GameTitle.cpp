#include "COriginalGameApp.h"
#include "GameResource.h"

//変数定義-----------------------------------------------------------------------
//タイトル画面のフェーズを管理する変数
ePhase g_TitlePhase = eBegin;
//タイトル画面の画像を管理する変数
CPicture* g_pTitle = NULL;
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
		g_pTitle = createSprite(Tex_Title);
	}
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
	//画像が作成されていたら描画する
	if (g_pTitle != NULL)
	{
		renderSprite(g_pTitle);
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
}