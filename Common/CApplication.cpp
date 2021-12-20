/**
 * @file CApplication.cpp
 * @brief DirectX11をベースにしたアプリケーションクラス
 * @author 織戸　喜隆
 * @date 日付（2017.02.14）
 */

#include "stdafx.h"
#include "CApplication.h"

CApplication::CApplication()
{
}

CApplication::~CApplication()
{
}

/**
 * @fn HRESULT CApplication::initWindow(HINSTANCE hInstance_, WNDPROC wndProc,　INT iX_, INT iY_, INT iWidth_, INT iHeight_, LPCWSTR windowName_)
 * @brief ウインドウの生成と初期化
 * @param hInstance_	WindowsAPIのインスタンス
 * @param wndProc		コールバック
 * @param iX_			ウインドウのx位置
 * @param iY_			ウインドウのy位置
 * @param iWidth_		ウインドウの幅
 * @param iHeight_		ウインドウの高さ
 * @param windowName_	ウインドウの名前
 * @return				WindowsAPI 実行結果
 */
HRESULT CApplication::initWindow(HINSTANCE hInstance_, WNDPROC wndProc, INT iX_, INT iY_, INT iWidth_, INT iHeight_, LPCWSTR windowName_)
{
	/**
	 * ウィンドウの定義
	 */
	WNDCLASSEX  wc;
	ZeroMemory(&wc, sizeof(wc));
	wc.cbSize = sizeof(wc);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = wndProc;
	wc.hInstance = hInstance_;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
	wc.lpszClassName = windowName_	;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&wc);

	/**
	 * ウィンドウの作成
	 */
	m_hWnd = CreateWindow(windowName_, windowName_, WS_OVERLAPPEDWINDOW, 0, 0, iWidth_, iHeight_, 0, 0, hInstance_, 0);
	if (!m_hWnd)
	{
		return E_FAIL;
	}

	/**
	 * ウインドウの表示
	 */
	setClientSize(m_hWnd, WINDOW_WIDTH, WINDOW_HEIGHT);
	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);
	
	return S_OK;
}

/**
 * @fn LRESULT CApplication::msgProc(HWND hWnd_, UINT iMsg_, WPARAM wParam_, LPARAM lParam_)
 * @brief Windowsからコールバックされる
 * @param hWnd_		ウインドウハンドル
 * @param iMsg_		WindowsAPI
 * @param wParam_	WindowsAPI
 * @param lParam_	WindowsAPI
 * @return			WindowsAPI
 */
LRESULT CApplication::msgProc(HWND hWnd_, UINT iMsg_, WPARAM wParam_, LPARAM lParam_)
{
	switch (iMsg_)
	{
	case WM_KEYDOWN:
		switch ((char)wParam_)
		{
		case VK_ESCAPE: //!< ESCキーで修了
			PostQuitMessage(0);
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd_, iMsg_, wParam_, lParam_);
}

/**
 * @fn void CApplication::setClientSize(HWND hWnd_, LONG sx_, LONG sy_)
 * @brief 枠を考慮したウインドウのサイズを求める
 * @param hWnd_	ウインドウハンドル
 * @param sx_	ウインドウのx位置
 * @param sy_	ウインドウのy位置
 * @return		WindowsAPI 実行結果
 */
void CApplication::setClientSize(HWND hWnd_, LONG sx_, LONG sy_)
{
	RECT windowSize;
	RECT clientSize;
	GetWindowRect(hWnd_, &windowSize);	 //!< ウィンドウ枠を含めたサイズを取得
	GetClientRect(hWnd_, &clientSize);	 //!< 枠内のサイズを取得
	sx_ += ((windowSize.right - windowSize.left) - (clientSize.right - clientSize.left));
	sy_ += ((windowSize.bottom - windowSize.top) - (clientSize.bottom - clientSize.top));
	//SetWindowPos(hWnd_, NULL, 0, 0, 640, 480, (SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOMOVE));
	MoveWindow( hWnd_, windowSize.left, windowSize.top, sx_, sy_, FALSE);
}

/**
 * @fn HRESULT CApplication::initD3D()
 * @brief DirectX11の初期化
 * @param	無し
 * @return	WindowsAPI 実行結果
 */
HRESULT CApplication::initD3D()
{
	/**
	 * デバイスとスワップチェーンの作成
	 */
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));	//!< 構造体の値を初期化

	sd.BufferCount = 3;				//!< バック バッファ数

	sd.BufferDesc.Width = WINDOW_WIDTH;					//!< バック バッファの幅
	sd.BufferDesc.Height = WINDOW_HEIGHT;				//!< バック バッファの高さ
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	//!< フォーマット
	sd.BufferDesc.RefreshRate.Numerator = 60;							//!< リフレッシュ レート(分子)
	sd.BufferDesc.RefreshRate.Denominator = 1;							//!< リフレッシュ レート(分母)
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_PROGRESSIVE;	//!< スキャンライン
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_CENTERED;		//!< スケーリング
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;		//!< バック バッファの使用法
	sd.OutputWindow = m_hWnd;		//!< 関連付けるウインドウ
	sd.SampleDesc.Count = 1;		//!< マルチ サンプルの数
	sd.SampleDesc.Quality = 0;		//!< マルチ サンプルのクオリティ
	sd.Windowed = !kFullScreen;		//!< ウインドウ モード
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;	//!< モード自動切り替え

	D3D_FEATURE_LEVEL pFeatureLevels = D3D_FEATURE_LEVEL_11_0;
	D3D_FEATURE_LEVEL* pFeatureLevel = NULL;

	/**
	 * ハードウェア・デバイスを作成
	 */
	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0,
		&pFeatureLevels, 1, D3D11_SDK_VERSION, &sd,
		&m_pSwapChain,
		&m_pDevice,
		pFeatureLevel,
		&m_pDeviceContext);
	if (FAILED(hr))
		return PutMessage(L"CApplication::initD3D D3D11CreateDeviceAndSwapChain");

	/**
	 * ブレンド・ステート・オブジェクトの作成
	 */
	if (FAILED(createBlendState()))
		return PutMessage(L"CApplication::initD3D g_pD3DDevice->CreateBlendState");

	/**
	 * ラスタライザ・ステート・オブジェクトの作成
	 */
	D3D11_RASTERIZER_DESC rSDesc;
	rSDesc.FillMode = D3D11_FILL_SOLID;	 //!< 普通に描画する
	rSDesc.CullMode = D3D11_CULL_NONE;	 //!< 両面を描画する
	rSDesc.FrontCounterClockwise = FALSE;//!< 時計回りが表面
	rSDesc.DepthBias = 0;
	rSDesc.DepthBiasClamp = 0;
	rSDesc.SlopeScaledDepthBias = 0;
	rSDesc.DepthClipEnable = TRUE;
	rSDesc.ScissorEnable = FALSE;
	rSDesc.MultisampleEnable = FALSE;
	rSDesc.AntialiasedLineEnable = FALSE;
	if (FAILED(m_pDevice->CreateRasterizerState(&rSDesc, &m_pRasterizerState)))
		return PutMessage(L"CApplication::initD3D m_pDevice->CreateRasterizerState");

	/**
	 * 深度/ステンシル・ステート・オブジェクトの作成
	 */
	D3D11_DEPTH_STENCIL_DESC depthStencil;
	ZeroMemory(&depthStencil, sizeof(D3D11_DEPTH_STENCIL_DESC));

	depthStencil.DepthEnable = TRUE;							//!< 深度テストあり
	depthStencil.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;	//!< 書き込む
	depthStencil.DepthFunc = D3D11_COMPARISON_LESS;				//!< 手前の物体を描画
	depthStencil.StencilEnable = FALSE;							//!< ステンシル・テストなし
	depthStencil.StencilReadMask = 0xff;						//!< ステンシル読み込みマスク。
	depthStencil.StencilWriteMask = 0;							//!< ステンシル書き込みマスク。

	hr = m_pDevice->CreateDepthStencilState(&depthStencil, &m_pDepthStencilState);
	if (FAILED(hr))
		return PutMessage(L"CApplication::initD3D 深度ステンシルステート失敗");

	if (InitBackBuffer() != S_OK)
		return E_FAIL;
	if (createSprites() == false)
		return E_FAIL;

	return S_OK;
}

/**
 * @fn HRESULT CApplication::createBlendState()
 * @brief ブレンド・ステート・オブジェクトの作成
 * @param	無し
 * @return	WindowsAPI 実行結果
 */
HRESULT CApplication::createBlendState()
{
	D3D11_BLEND_DESC blendDesc;
	//!< ブレンドステート 無効		getDefaultBlendDesc
	//!< ブレンドステート 線形合成		getAlignmenttBlendDesc
	//!< ブレンドステート 加算合成		getAddBlendDesc
	//!< ブレンドステート 減算合成		getSubtractBlendDesc
	//!< ブレンドステート 乗算合成		getMultipleBlendDesc

	blendDesc = getDefaultBlendDesc();//getAlignmenttBlendDesc();	//!< ブレンドステート 線形合成

	if (FAILED(m_pDevice->CreateBlendState(&blendDesc, &m_pBlendState)))
		return PutMessage(L"CApplication::createBlendState g_pD3DDevice->CreateBlendState");
	return S_OK;
}

/**
 * @fn D3D11_BLEND_DESC CApplication::getDefaultBlendDesc()
 * @brief ブレンドステート 無効	【　SrcBlend * 1 + DestBlend * 0　】
 * @param	無し
 * @return  ブレンドステート
 */
D3D11_BLEND_DESC CApplication::getDefaultBlendDesc()
{
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(D3D11_BLEND_DESC));
	blendDesc.AlphaToCoverageEnable = TRUE;						//!< マルチサンプリングで描画ターゲットにα値を使用する
	blendDesc.IndependentBlendEnable = FALSE;					//!< RenderTarget[0]のみ使用する
	blendDesc.RenderTarget[0].BlendEnable = FALSE;				//!< ブレンディングは有効
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;		//!< ソースのRGB値を選択
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;		//!< ディスティネーションのRGB値を選択
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;		//!< RGB値のブレンド設定 src + dest
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;	//!< ソースのα値を選択　「_COLOR」で終わる値は選択不可
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;//!< ディスティネーションのα値を選択　「_COLOR」で終わる値は選択不可
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;//!< α値のブレンド設定
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;	//!< RGB値のうち、書き込む値、「D3D11_COLOR_WRITE_ENABLE列挙型」の組み合わせ
	return blendDesc;
}

/**
 * @fn D3D11_BLEND_DESC CApplication::getAlignmenttBlendDesc()
 * @brief ブレンドステート 線形合成		【　SrcBlend * SrcAlpha + DestBlend * ( 1 - SrcAlpha )　】
 * @param	無し
 * @return   ブレンドステート
 */
D3D11_BLEND_DESC CApplication::getAlignmenttBlendDesc()
{
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(D3D11_BLEND_DESC));
	blendDesc.AlphaToCoverageEnable = TRUE;							//!< マルチサンプリングで描画ターゲットにα値を使用する
	blendDesc.IndependentBlendEnable = FALSE;						//!< RenderTarget[0]のみ使用する
	blendDesc.RenderTarget[0].BlendEnable = TRUE;					//!< ブレンディングは有効
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;		//!< ソースのRGB値を選択
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;//!< ディスティネーションのRGB値を選択
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;			//!< RGB値のブレンド設定 src + dest
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;		//!< ソースのα値を選択　「_COLOR」で終わる値は選択不可
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;	//!< ディスティネーションのα値を選択　「_COLOR」で終わる値は選択不可
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;	//!< α値のブレンド設定
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;	//!< RGB値のうち、書き込む値、「D3D11_COLOR_WRITE_ENABLE列挙型」の組み合わせ
	return blendDesc;
}

/**
 * @fn D3D11_BLEND_DESC CApplication::getAddBlendDesc()
 * @brief ブレンドステート 加算合成		【　SrcBlend * SrcAlpha + DestBlend * 1　】
 * @param	無し
 * @return   ブレンドステート
 */
D3D11_BLEND_DESC CApplication::getAddBlendDesc()
{
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(D3D11_BLEND_DESC));
	blendDesc.AlphaToCoverageEnable = TRUE;						//!< マルチサンプリングで描画ターゲットにα値を使用する
	blendDesc.IndependentBlendEnable = FALSE;					//!< RenderTarget[0]のみ使用する
	blendDesc.RenderTarget[0].BlendEnable = TRUE;				//!< ブレンディングは有効
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;	//!< ソースのRGB値を選択
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;		//!< ディスティネーションのRGB値を選択
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;		//!< RGB値のブレンド設定 src + dest
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;	//!< ソースのα値を選択　「_COLOR」で終わる値は選択不可
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;//!< ディスティネーションのα値を選択　「_COLOR」で終わる値は選択不可
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;//!< α値のブレンド設定
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;	//!< RGB値のうち、書き込む値、「D3D11_COLOR_WRITE_ENABLE列挙型」の組み合わせ
	return blendDesc;
}

/**
 * @fn D3D11_BLEND_DESC CApplication::getSubtractBlendDesc()
 * @briefブレンドステート 減算合成		【　DestBlend * 1 - SrcBlend * SrcAlpha　】
 * @param	無し
 * @return   ブレンドステート
 */
D3D11_BLEND_DESC CApplication::getSubtractBlendDesc()
{
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(D3D11_BLEND_DESC));
	blendDesc.AlphaToCoverageEnable = TRUE;							//!< マルチサンプリングで描画ターゲットにα値を使用する
	blendDesc.IndependentBlendEnable = FALSE;						//!< RenderTarget[0]のみ使用する
	blendDesc.RenderTarget[0].BlendEnable = TRUE;					//!< ブレンディングは有効
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;		//!< ソースのRGB値を選択
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;			//!< ディスティネーションのRGB値を選択
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_REV_SUBTRACT;//!< RGB値のブレンド設定 src + dest
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;		//!< ソースのα値を選択　「_COLOR」で終わる値は選択不可
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;	//!< ディスティネーションのα値を選択　「_COLOR」で終わる値は選択不可
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;	//!< α値のブレンド設定
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;	//!< RGB値のうち、書き込む値、「D3D11_COLOR_WRITE_ENABLE列挙型」の組み合わせ
	return blendDesc;
}

/**
 * @fn D3D11_BLEND_DESC CApplication::getMultipleBlendDesc()
 * @brief ブレンドステート 乗算合成			【　SrcBlend * 0 + DestBlend * SrcColor　】
 * @param	無し
 * @return   ブレンドステート
 */
D3D11_BLEND_DESC CApplication::getMultipleBlendDesc()
{
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(D3D11_BLEND_DESC));
	blendDesc.AlphaToCoverageEnable = TRUE;						//!< マルチサンプリングで描画ターゲットにα値を使用する
	blendDesc.IndependentBlendEnable = FALSE;					//!< RenderTarget[0]のみ使用する
	blendDesc.RenderTarget[0].BlendEnable = TRUE;				//!< ブレンディングは有効
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ZERO;		//!< ソースのRGB値を選択
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_SRC_COLOR;	//!< ディスティネーションのRGB値を選択
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;			//!< RGB値のブレンド設定 src + dest
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;		//!< ソースのα値を選択　「_COLOR」で終わる値は選択不可
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;	//!< ディスティネーションのα値を選択　「_COLOR」で終わる値は選択不可
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;	//!< α値のブレンド設定
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;	//!< RGB値のうち、書き込む値、「D3D11_COLOR_WRITE_ENABLE列挙型」の組み合わせ
	return blendDesc;
}

/**
 * @fn HRESULT CApplication::InitBackBuffer(void)
 * @brief Zの向きから位置を求める
 * @param	無し
 * @return   WindowsAPI 実行結果
 */
HRESULT CApplication::InitBackBuffer(void)
{
	HRESULT hr = S_OK;
	/**
	 * レンダーターゲットビューの作成
	 * スワップ・チェインから最初のバック・バッファを取得する
	 */
	ID3D11Texture2D *pBackBuffer;   //!< バッファのアクセスに使うインターフェイス
	hr = m_pSwapChain->GetBuffer(
		0,							 //!< バック・バッファの番号
		__uuidof(ID3D11Texture2D),	 //!< バッファにアクセスするインターフェイス
		(LPVOID*)&pBackBuffer);		 //!< バッファを受け取る変数
	if (FAILED(hr))
		return PutMessage(L"CApplication::InitBackBuffer m_pSwapChain->GetBuffer");   //!< 失敗

	/**
	 * バック・バッファの情報
	 */
	//D3D11_TEXTURE2D_DESC descBackBuffer;
	//pBackBuffer->GetDesc(&descBackBuffer);

	/**
	 * バック・バッファの描画ターゲット・ビューを作る
	 */
	m_pDevice->CreateRenderTargetView(
		pBackBuffer,			 //!< ビューでアクセスするリソース
		NULL,					 //!< 描画ターゲット・ビューの定義
		&m_pRenderTargetView);	 //!< 描画ターゲット・ビューを受け取る変数
	SAFE_RELEASE(pBackBuffer);	 //!< 以降、バック・バッファは直接使わないので解放
	if (FAILED(hr))
		return PutMessage(L"InitBackBuffer m_pDevice->CreateRenderTargetView");   //!< 失敗

	/**
	 *深度ステンシルバッファーと深度ステンシルビューの作成
	 */
	D3D11_TEXTURE2D_DESC depthDesc;
	depthDesc.Width = WINDOW_WIDTH;		 //!< 幅
	depthDesc.Height = WINDOW_HEIGHT;	 //!< 高さ
	depthDesc.MipLevels = 1;			 //!< ミップマップ・レベル数
	depthDesc.ArraySize = 1;			 //!< 配列サイズ
	depthDesc.Format = DXGI_FORMAT_D32_FLOAT;		//!< フォーマット(深度のみ)
	depthDesc.SampleDesc.Count = 1;					//!< マルチサンプリングの設定
	depthDesc.SampleDesc.Quality = 0;				//!< マルチサンプリングの品質
	depthDesc.Usage = D3D11_USAGE_DEFAULT;			//!< デフォルト使用法
	depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;	//!< 深度/ステンシルとして使用
	depthDesc.CPUAccessFlags = 0;					//!< CPUからはアクセスしない
	depthDesc.MiscFlags = 0;						//!< その他の設定なし

	if (FAILED(m_pDevice->CreateTexture2D(
		&depthDesc, //!< 作成する2Dテクスチャの設定
		NULL,
		&m_pDepthStencil))) //!< 作成したテクスチャを受け取る変数
	{
		MessageBox(0, L"深度ステンシルバッファ作成失敗", NULL, MB_OK);
		return E_FAIL;
	}
	/**
	 * 深度/ステンシル ビューの作成
	 */
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	depthStencilViewDesc.Format = depthDesc.Format;             //!< ビューのフォーマット
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Flags = 0;
	depthStencilViewDesc.Texture2D.MipSlice = 0;
	m_pDevice->CreateDepthStencilView(
		m_pDepthStencil,		 //!< 深度/ステンシル・ビューを作るテクセル
		&depthStencilViewDesc,	 //!< 深度/ステンシル・ビューの設定
		&m_pDepthStencilView);	 //!< 作成したビューを受け取る変数
	if (FAILED(hr))
		return PutMessage(L"InitBackBuffer m_pDevice->CreateDepthStencilView");   //!< 失敗

	/**
	 *　ビューポートの設定
	 */
	m_viewPort[0].TopLeftX = 0;				 //!< ビューポート領域の左上X座標
	m_viewPort[0].TopLeftY = 0;				 //!< ビューポート領域の左上Y座標
	m_viewPort[0].Width = WINDOW_WIDTH;		 //!< ビューポート領域の幅
	m_viewPort[0].Height = WINDOW_HEIGHT;	 //!< ビューポート領域の高さ
	m_viewPort[0].MinDepth = 0.0f;			 //!< ビューポート領域の深度値の最小値
	m_viewPort[0].MaxDepth = 1.0f;			 //!< ビューポート領域の深度値の最大値
	return S_OK;
}

/**
 * @fn bool CApplication::createSprites(void)
 * @brief スプライト作成
 * @return     スプライトの生成の成功
 */
bool CApplication::createSprites(void)
{
	/**
	 * カメラの生成と初期化
	 */
	initCameraInfo();
	m_pCamera = unique_ptr<CCamera>(new CCamera());
	m_pCamera->init(
		m_vEyePt,	//!< カメラ（視点）位置
		m_vLookatPt,//!< 注視位置
		m_vUpVec,	//!< 上方位置
		m_angle,	//!< 視野角
		m_aspect,	//!< アスペクト比
		m_near,		//!< 前方クリップ
		m_far);		//!< 後方クリップ
	
	m_pDirectInput = CDirectInput::getInstance(m_hWnd); //!< キーボードデバイスの生成
	return true;
}

/**
 * @fn void CApplication::loop()
 * @brief メッセージループとアプリケーション処理の入り口
 * @param	無し
 * @return    無し
 */
void CApplication::loop()
{
	MSG msg = { 0 };
	ZeroMemory(&msg, sizeof(msg));
	long time = timeGetTime();	 //!< ### FPS制御 ###
	/**
	 * メッセージループ
	 */
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			app();			//!< アプリケーションの処理はここから
			/**
			 * FPS制御
			 */
			while (timeGetTime() - time < 1000 / 60)
			{
				Sleep(1);	//!< 1ミリ秒Sleepする
			}
			time = timeGetTime();
		}
	}
	/**
	 * アプリケーションの終了
	 */
	//release();
}

/**
 * @fn void CApplication::app()
 * @brief アプリケーション処理。アプリのメイン関数
 * @param	無し
 * @return	無し
 */
void CApplication::app()
{
	m_pDirectInput->getState();
	renderBegin();

	switch (m_gameStatus)
	{
	case eOpening:		doOpenning();		break;
	case ePlaying:		doPlaying();		break;
	case eGameClear:	doGameClear();		break;
	case eGameOver:		doGameOver();		break;
	default:								break;
	}

//	render();
	renderEnd();
}

void CApplication::renderBegin()
{
	FLOAT clearColor[4] = { 0, 0, 1, 1 }; //!< クリア色作成　RGBAの順

	m_pDeviceContext->ClearRenderTargetView(	//!< 画面クリア（実際は単色で画面を塗りつぶす処理）
		m_pRenderTargetView,					//!< クリアする描画ターゲット
		clearColor);							//!< クリアする値

	m_pDeviceContext->ClearDepthStencilView(
		m_pDepthStencilView,	 //!< クリアする深度/ステンシル・ビュー
		D3D11_CLEAR_DEPTH,		 //!< 深度値だけをクリアする
		1.0f,					 //!< 深度バッファをクリアする値
		0);						 //!< ステンシル・バッファをクリアする値(この場合、無関係)

	m_pCamera->update();

	m_pDeviceContext->RSSetViewports(1, m_viewPort);	 //!< RSにビューポートを設定
	m_pDeviceContext->RSSetState(m_pRasterizerState);	 //!< RSにラスタライザ・ステート・オブジェクトを設定
	m_pDeviceContext->OMSetRenderTargets(				 //!< OMに描画ターゲット ビューと深度/ステンシル・ビューを設定
		1,
		&m_pRenderTargetView,
		m_isValidDepthMode ? m_pDepthStencilView : NULL
	);

	FLOAT blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };	 //!< （RGBA）
	m_pDeviceContext->OMSetBlendState(m_pBlendState, blendFactor, 0xffffffff);	 //!< OMにブレンド・ステート・オブジェクトを設定	(サンプル用マスク)
}

/**
 * @fn void CApplication::render()
 * @brief シーンを画面にレンダリング
 * @param	無し
 * @return	無し
 */
void CApplication::render()
{
	XMFLOAT4X4	matView = m_pCamera->getViewMatrix();		 //!< ビュートランスフォーム（視点座標変換）
	XMFLOAT4X4	matProj = m_pCamera->getProjectionMatrix();	 //!< プロジェクショントランスフォーム（射影変換）
	render(matView, matProj);												//スプライトのレンダリング
	/**
	 * デバッグ情報
	 */
	//debugString(m_hWnd, 10, 10, L"ドットイートⅡ");
}
void CApplication::renderEnd()
{
	m_pSwapChain->Present(0, 0);	//!<画面更新（バックバッファをフロントバッファに）
}

void CApplication::SetNextGameStatus(eGameStatus next)
{
	m_gameStatus = next;
}

/**
 * @fn void CApplication::release()
 * @brief DirectXのリソースを解放
 * @param	無し
 * @return	無し
 */
void CApplication::release()
{
	// Direct3D
	SAFE_RELEASE(m_pDepthStencilView);
	SAFE_RELEASE(m_pDepthStencil);
	SAFE_RELEASE(m_pRenderTargetView);
	SAFE_RELEASE(m_pDepthStencilState);
	SAFE_RELEASE(m_pRasterizerState);
	SAFE_RELEASE(m_pBlendState);
	SAFE_RELEASE(m_pSwapChain);
	SAFE_RELEASE(m_pDeviceContext);
	SAFE_RELEASE(m_pDevice);

	// Direct3D 以外
	m_pDirectInput->releaseInstance();
}

/**
 * @brief 入力情報を別クラスで管理するための取得関数
 */
std::shared_ptr<CDirectInput> CApplication::getInput()
{
	return m_pDirectInput;
}
