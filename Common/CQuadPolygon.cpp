/**
* @file CQuadPolygon.cpp
* @brief ポリゴン・クラス
* @author 織戸　喜隆
* @date 日付（2018.07.27）
*/

#include "stdafx.h"
#include "CQuadPolygon.h"

/**
* @fn std::unique_ptr<CQuadPolygon> CQuadPolygon::create(ID3D11Device* pDevice_, ID3D11DeviceContext* pDeviceContext_, D3D11_VIEWPORT viewPort_[], LPCWSTR shaderName_, XMFLOAT4 position_, XMFLOAT2 size_, std::vector<QuadrangleTexel> texels)
* @brief インスタンスを生成し初期化 【クラスメソッド】
* @param pDevice_			デバイス
* @param pDeviceContext_	デバイスコンテキスト
* @param viewPort_[]		ビューポート
* @param shaderName_		シェーダープログラム
* @param position_			ウィンドウの位置
* @param size_				スプライトの大きさ
* @param texels			テクセルの配列
* @return					CQuadPolygonのユニークポインタ
*/
std::unique_ptr<CQuadPolygon> CQuadPolygon::create(ID3D11Device* pDevice_, ID3D11DeviceContext* pDeviceContext_, D3D11_VIEWPORT viewPort_[], LPCWSTR shaderName_, XMFLOAT4 position_, XMFLOAT2 size_, std::vector<QuadrangleTexel>	texels)
{
	std::unique_ptr<CQuadPolygon> object = unique_ptr<CQuadPolygon>(new CQuadPolygon());
	object->init(pDevice_, pDeviceContext_, viewPort_, shaderName_, position_, size_, texels);
	return object;
}

/**
* @fn void CQuadPolygon::init(ID3D11Device* pDevice_, ID3D11DeviceContext* pDeviceContext_, D3D11_VIEWPORT viewPort_[], LPCWSTR shaderName_, XMFLOAT4 position_, XMFLOAT2 size_, std::vector<QuadrangleTexel> texels)
* @brief インスタンスを初期化
* @param pDevice_			デバイス
* @param pDeviceContext_	デバイスコンテキスト
* @param viewPort_[]		ビューポート
* @param shaderName_		シェーダープログラム
* @param position_			ウィンドウの位置
* @param size_				スプライトの大きさ
* @param texels			テクセルの配列
* @return					無し
*/
void CQuadPolygon::init(ID3D11Device* pDevice_, ID3D11DeviceContext* pDeviceContext_, D3D11_VIEWPORT viewPort_[], LPCWSTR shaderName_, XMFLOAT4 position_, XMFLOAT2 size_, std::vector<QuadrangleTexel>	texels)
{
	m_damage = FALSE;
	m_pDeviceContext = pDeviceContext_;	//!< レンダリングに必要　＜実行中に変化しない＞
	m_viewPort[0] = viewPort_[0];		//!< レンダリングに必要　＜実行中に変化しない＞
	m_originalSize = size_;
	m_VBSize = size_;
	m_active = TRUE;
	m_state = eWaiting;
	m_position.setup({ position_.x, position_.y, position_.z, 0.0f }, 0.0f, 0.0f, 0.0f, XMFLOAT3(1.0f, 1.0f, 1.0f));	//!< 位置

	// デフォルトブレンドステート
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(D3D11_BLEND_DESC));

	/*
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
	*/

	// ｾﾋﾆﾄｺ昞ｻｵ・ｰｪﾀｻ ｼｳﾁ､ﾇﾕｴﾏｴﾙ.
	blendDesc.RenderTarget[0].BlendEnable = FALSE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;// INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;


	pDevice_->CreateBlendState(&blendDesc, &m_pDefaultBlendState);

	// アルファブレンドステート
	ZeroMemory(&blendDesc, sizeof(D3D11_BLEND_DESC));

	// ｾﾋﾆﾄｺ昞ｻｵ・ｰｪﾀｻ ｼｳﾁ､ﾇﾕｴﾏｴﾙ.
	blendDesc.RenderTarget[0].BlendEnable = TRUE;

	pDevice_->CreateBlendState(&blendDesc, &m_pAlphaBlendState);


	createVertexShader(pDevice_, shaderName_);		//!<頂点・シェーダの作成
	createVertexInputLayout(pDevice_);					//!<頂点インプットレイアウトを作成
	createPixelShader(pDevice_, shaderName_);		//!<ピクセル・シェーダの作成
	createVertexBuffer(pDevice_, size_, texels);	//!<頂点バッファー作成
	createConstBuffer(pDevice_);								//!<定数バッファ作成
}

void CQuadPolygon::createVertexShader(ID3D11Device* pDevice_, LPCWSTR shaderName_)
{
	HRESULT hr;
	ID3DBlob* pErrors = NULL;
	/**
	* hlslファイル読み込み ブロブ作成　ブロブとはシェーダーの塊みたいなもの。XXシェーダーとして特徴を持たない。後で各種シェーダーに成り得る
	*/
	hr = D3DCompileFromFile(
		shaderName_,	 //!< ファイル名
		NULL,			 //!< マクロ定義(なし)
		NULL,			 //!< インクルード・ファイル定義(なし)
		"VS",			 //!< 「VS関数」がシェーダから実行される
		"vs_5_0",		 //!< 頂点シェーダ
		0,				 //!< コンパイル・オプション
		0,				 //!< エフェクトのコンパイル・オプション(なし)
		&m_pBlobVS,		 //!< コンパイルされたバイト・コード
		&pErrors);		 //!< エラーメッセージ

	SAFE_RELEASE(pErrors);
	if (FAILED(hr))
	{
		TCHAR   str[256];
		wsprintf(str, L"CQuadPolygon::init HLSL (VS)読み込み失敗 %s", shaderName_);
		throw str;
	}
	/**
	* ブロブから頂点シェーダー作成
	*/
	hr = pDevice_->CreateVertexShader(
		m_pBlobVS->GetBufferPointer(),	//!< バイト・コードへのポインタ
		m_pBlobVS->GetBufferSize(),		//!< バイト・コードの長さ
		NULL,
		&m_pVertexShader);				//!< 頂点シェーダを受け取る変数
	if (FAILED(hr))
	{
		SAFE_RELEASE(m_pBlobVS);			//!< バイト・コードを解放
		throw L"CQuadPolygon::init 頂点シェーダー作成失敗";
	}
}

void CQuadPolygon::createVertexInputLayout(ID3D11Device* pDevice_)
{
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{
			"POSITION",						//!< シグネチャ
			0,								//!< 成分のセマンティック・インデックス番号
			DXGI_FORMAT_R32G32B32_FLOAT,	//!< エレメントのフォーマット
			0,								//!< 入力スロット番号
			0,								//!< 先頭までのオフセット値（バイト数）
			D3D11_INPUT_PER_VERTEX_DATA,	//!< 入力データは各頂点のデータ
			0								//!< インスタンス毎の繰り返し回数
		},
		{
			"TEXCOORD",						//!< シグネチャ
			0,								//!< 成分のセマンティック・インデックス番号
			DXGI_FORMAT_R32G32_FLOAT,		//!< エレメントのフォーマット
			0,								//!< 入力スロット番号
			sizeof(XMFLOAT3),				//!< 先頭までのオフセット値（バイト数）
			D3D11_INPUT_PER_VERTEX_DATA,	//!< 入力データは各頂点のデータ
			0								//!< インスタンス毎の繰り返し回数
		}
	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);
	/**
	* 頂点インプットレイアウトを作成
	*/
	if (FAILED(pDevice_->CreateInputLayout(layout, numElements, m_pBlobVS->GetBufferPointer(), m_pBlobVS->GetBufferSize(), &m_pInputLayout)))
		throw L"CQuadPolygon::init 頂点インプットレイアウト作成失敗";
}

void CQuadPolygon::createPixelShader(ID3D11Device* pDevice_, LPCWSTR shaderName_)
{
	HRESULT hr;
	ID3DBlob* pErrors = NULL;
	/**
	* ブロブからピクセルシェーダー作成
	*/
	hr = D3DCompileFromFile(
		shaderName_,	//!< ファイル名
		NULL,			//!< マクロ定義(なし)
		NULL,			//!< インクルード・ファイル定義(なし)
		"PS",			//!< 「PS関数」がシェーダから実行される
		"ps_5_0",		//!< ピクセル・シェーダ
		0,				//!< コンパイル・オプション
		0,				//!< エフェクトのコンパイル・オプション(なし)
		&m_pBlobVS,		//!< コンパイルされたバイト・コード
		&pErrors);		//!< エラーメッセージ

	SAFE_RELEASE(pErrors);
	if (FAILED(hr))
	{
		TCHAR   str[256];
		wsprintf(str, L"CQuadPolygon::init HLSL (PS)読み込み失敗 %s", shaderName_);
		throw str;
	}
	hr = pDevice_->CreatePixelShader(
		m_pBlobVS->GetBufferPointer(),	//!< バイト・コードへのポインタ
		m_pBlobVS->GetBufferSize(),		//!< バイト・コードの長さ
		NULL,
		&m_pPixelShader);				//!< ピクセル・シェーダを受け取る変数
	SAFE_RELEASE(m_pBlobVS);				//!< バイト・コードを解放
	if (FAILED(hr))
		throw L"CQuadPolygon::init ピクセルシェーダー作成失敗";
}

void CQuadPolygon::createVertexBuffer(ID3D11Device* pDevice_, XMFLOAT2 size_, std::vector<QuadrangleTexel>	texels)
{
	m_VBLeftTop = { 0.0f - (size_.x / 2.0f), 0.0f - (size_.y / 2.0f) };
	m_VBRightBottom = { 0.0f + (size_.x / 2.0f), 0.0f + (size_.y / 2.0f) };
	m_texels = texels;
	for (size_t i = 0; i < texels.size(); i++)
	{
		VertexWithTex<XMFLOAT3> vertices[] =
		{
		{ XMFLOAT3(m_VBLeftTop.x,		m_VBLeftTop.y,		0),texels[i].v1 },	//!<頂点1
		{ XMFLOAT3(m_VBLeftTop.x,		m_VBRightBottom.y,	0),texels[i].v2 },	//!<頂点2
		{ XMFLOAT3(m_VBRightBottom.x,	m_VBLeftTop.y,		0),texels[i].v3 },	//!<頂点3
		{ XMFLOAT3(m_VBRightBottom.x,	m_VBRightBottom.y,	0),texels[i].v4 }		//!<頂点4	
		};

		D3D11_BUFFER_DESC bd;
		bd.Usage = D3D11_USAGE_DYNAMIC; //!< D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(VertexWithTex<XMFLOAT3>) * 4;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bd.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA initData;
		ID3D11Buffer* pVB;
		initData.pSysMem = vertices;
		if (FAILED(pDevice_->CreateBuffer(&bd, &initData, &pVB)))
		{
			throw L"CQuadPolygon::init 頂点バッファ作成失敗";
		}
		m_pVertexBuffer.push_back(pVB);
	}

}
void CQuadPolygon::createConstBuffer(ID3D11Device* pDevice_)
{
	D3D11_BUFFER_DESC cb;
	cb.Usage = D3D11_USAGE_DYNAMIC;				 //!< 動的(ダイナミック)使用法
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	 //!< 定数バッファ
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	 //!< CPUから書き込む
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;

	cb.ByteWidth = sizeof(ShaderGlobal_0);						//!< バッファ・サイズ
	if (FAILED(pDevice_->CreateBuffer(&cb, NULL, &m_pConstantBuffer))) //!< 定数バッファの作成　ここでは変換行列渡し用
	{
		throw L"CQuadPolygon::init 定数バッファ作成失敗";
	}
}

CQuadPolygon::CQuadPolygon()
{
}

CQuadPolygon::~CQuadPolygon()
{
}

/**
* @fn void CQuadPolygon::computeMatrixFromPos (void)
* @brief ワールド行列を求める
* @param					無し
* @param					無し
* @return					無し
*/
void CQuadPolygon::computeMatrixFromPos(void)
{
	XMMATRIX scale = XMMatrixScaling(m_position.scale.x, m_position.scale.y, m_position.scale.z);
	XMMATRIX pitch = XMMatrixRotationX(XMConvertToRadians(m_position.pitch));
	XMMATRIX yaw = XMMatrixRotationY(XMConvertToRadians(m_position.yaw));
	XMMATRIX roll = XMMatrixRotationZ(XMConvertToRadians(m_position.roll));
	XMMATRIX trans = XMMatrixTranslation(getPos().x, getPos().y, getPos().z);
	XMMATRIX finalWorld = scale * pitch * yaw * roll * trans;

	XMStoreFloat4x4(&m_matScale, scale);
	XMStoreFloat4x4(&m_matPitch, pitch);
	XMStoreFloat4x4(&m_matYaw, yaw);
	XMStoreFloat4x4(&m_matRoll, roll);
	XMStoreFloat4x4(&m_matTran, trans);
	XMStoreFloat4x4(&m_mFinalWorld, finalWorld);
}

/**
* @fn void CQuadPolygon::render(XMFLOAT4X4	matView_, XMFLOAT4X4 matProj_, size_t texelIndex_, XMFLOAT4 diffuse_)
* @brief レンダリング
* @param matView_	ビュー行列
* @param matProj_	プロジェクション行列
* @param texelIndex_	テクセル・インデックス
* @param diffuse_	ディフューズ
* @return			無し
*/
void CQuadPolygon::render(XMFLOAT4X4 matView_, XMFLOAT4X4 matProj_, std::function<void(ID3D11DeviceContext* pDeviceContext_)> callbackTextureProc_, size_t texelIndex_, XMFLOAT4 diffuse_) {
	if (!m_active)
	{
		return;
	}

	computeMatrixFromPos();

	//! シェーダー　定数バッファ
	D3D11_MAPPED_SUBRESOURCE pData;
	if (SUCCEEDED(m_pDeviceContext->Map(
		m_pConstantBuffer,			//!< マップするリソース
		0, 							//!< サブリソースのインデックス番号
		D3D11_MAP_WRITE_DISCARD,	//!< 書き込みアクセス
		0,
		&pData)))					//!< データの書き込み先ポインタ
	{
		ShaderGlobal_0 sg;
		XMStoreFloat4x4(&sg.matWVP, XMLoadFloat4x4(&m_mFinalWorld) * XMLoadFloat4x4(&matView_) * XMLoadFloat4x4(&matProj_));
		XMStoreFloat4x4(&sg.matWVP, XMMatrixTranspose(XMLoadFloat4x4(&sg.matWVP)));
		sg.diffuse = diffuse_;
		memcpy_s(pData.pData, pData.RowPitch, (void*)&sg, sizeof(ShaderGlobal_0));

		m_pDeviceContext->Unmap(m_pConstantBuffer, 0);
	}
	m_pDeviceContext->IASetInputLayout(m_pInputLayout);									//!< 頂点インプットレイアウトをセット
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);	//!< プリミティブ・トポロジーをセット
	UINT stride = sizeof(VertexWithTex<XMFLOAT3>);
	UINT offset = 0;
	m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer[texelIndex_], &stride, &offset);//!< 頂点バッファーをセット
	m_pDeviceContext->VSSetShader(m_pVertexShader, NULL, 0);							//!< 使用するシェーダーの登録　（effectの”テクニック”に相当）
	m_pDeviceContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);					//!< VSに定数バッファを設定
	m_pDeviceContext->RSSetViewports(1, m_viewPort);									//!< RSにビューポートを設定
	m_pDeviceContext->PSSetShader(m_pPixelShader, NULL, 0);								//!< PSにピクセル・シェーダを設定
	m_pDeviceContext->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);					//!< PSに定数バッファを設定
																						//! サンプラーとテクスチャーの設定
	callbackTextureProc_(m_pDeviceContext);												//! サンプラーとテクスチャーは利用者で用意する
	m_pDeviceContext->Draw(4, 0);														//!<  プリミティブをレンダリング
}

/**
* @fn BOOL CQuadPolygon::collision(XMFLOAT4 pos_, FLOAT range_)
* @brief 衝突
* @param pos_	位置
* @param range_	衝突の範囲
* @return		衝突
*/
BOOL CQuadPolygon::collision(XMFLOAT4 pos_, FLOAT range_)
{
	return collision(pos_, getPos(), range_);
}

BOOL CQuadPolygon::collision(XMFLOAT2 pos_, FLOAT range_)
{
	XMFLOAT4 pos = getPos();
	FLOAT dist2 =
		(pos_.x - pos.x) * (pos_.x - pos.x) +
		(pos_.y - pos.y) * (pos_.y - pos.y);
	return dist2 <= (range_ * range_);
}

BOOL CQuadPolygon::collision(XMFLOAT2 pos_)
{
	XMFLOAT4 myPos = getPos();
	XMFLOAT2 mySize = getSize();
	float myLeft = myPos.x - (mySize.x / 2.0f);
	float myTop = myPos.y + (mySize.y / 2.0f);
	float myRight = myPos.x + (mySize.x / 2.0f);
	float myBottom = myPos.y - (mySize.y / 2.0f);

	bool ret = (myLeft <= pos_.x && pos_.x <= myRight) && (myBottom <= pos_.y && pos_.y <= myTop);
	return ret;
}

/**
* @fn BOOL CQuadPolygon::collision(XMFLOAT4 pos1_, XMFLOAT4 pos2_, FLOAT range_)
* @brief 衝突
* @param pos1_	位置
* @param pos2_	位置
* @param range_	衝突の範囲
* @return		衝突
*/
BOOL CQuadPolygon::collision(XMFLOAT4 pos1_, XMFLOAT4 pos2_, FLOAT range_)
{
	FLOAT dist2 =
		(pos1_.x - pos2_.x) * (pos1_.x - pos2_.x) +
		(pos1_.y - pos2_.y) * (pos1_.y - pos2_.y) +
		(pos1_.z - pos2_.z) * (pos1_.z - pos2_.z);

	return dist2 <= (range_ * range_);
}
BOOL CQuadPolygon::collision(CQuadPolygon* target_)
{
	XMFLOAT4 myPos = getPos();
	XMFLOAT2 mySize = getSize();
	float myLeft = myPos.x - (mySize.x / 2.0f);
	float myTop = myPos.y + (mySize.y / 2.0f);
	float myRight = myPos.x + (mySize.x / 2.0f);
	float myBottom = myPos.y - (mySize.y / 2.0f);
	XMFLOAT4 targetPos = target_->getPos();
	XMFLOAT2 targetSize = target_->getSize();
	float targetLeft = targetPos.x - (targetSize.x / 2.0f);
	float targetTop = targetPos.y + (targetSize.y / 2.0f);
	float targetRight = targetPos.x + (targetSize.x / 2.0f);
	float targetBottom = targetPos.y - (targetSize.y / 2.0f);

	return (myLeft < targetRight&& myTop > targetBottom && myRight > targetLeft && myBottom < targetTop);
}
BOOL CQuadPolygon::collisionBottom(CQuadPolygon* target_)
{
	XMFLOAT4 myPos = getPos();
	XMFLOAT2 mySize = getSize();
	float colLeft = myPos.x - (mySize.x / 2.0f) + 4.0f;
	float colTop = myPos.y - (mySize.y / 2.0f) + 0.01f;
	float colRight = myPos.x + (mySize.x / 2.0f) - 4.0f;
	float colBottom = myPos.y - (mySize.y / 2.0f) - 0.01f;
	XMFLOAT4 targetPos = target_->getPos();
	XMFLOAT2 targetSize = target_->getSize();
	float targetLeft = targetPos.x - (targetSize.x / 2.0f);
	float targetTop = targetPos.y + (targetSize.y / 2.0f);
	float targetRight = targetPos.x + (targetSize.x / 2.0f);
	float targetBottom = targetPos.y - (targetSize.y / 2.0f);

	return (colLeft < targetRight&& colTop > targetBottom && colRight > targetLeft && colBottom < targetTop);
}
BOOL CQuadPolygon::collisionTop(CQuadPolygon* target_)
{
	XMFLOAT4 myPos = getPos();
	XMFLOAT2 mySize = getSize();
	float colLeft = myPos.x - (mySize.x / 2.0f) + 10.0f;
	float colTop = myPos.y + (mySize.y / 2.0f) + 0.01f;
	float colRight = myPos.x + (mySize.x / 2.0f) - 10.0f;
	float colBottom = myPos.y + (mySize.y / 2.0f) - 0.01f;
	XMFLOAT4 targetPos = target_->getPos();
	XMFLOAT2 targetSize = target_->getSize();
	float targetLeft = targetPos.x - (targetSize.x / 2.0f);
	float targetTop = targetPos.y + (targetSize.y / 2.0f);
	float targetRight = targetPos.x + (targetSize.x / 2.0f);
	float targetBottom = targetPos.y - (targetSize.y / 2.0f);

	return (colLeft < targetRight&& colTop > targetBottom && colRight > targetLeft && colBottom < targetTop);
}
BOOL CQuadPolygon::collisionLeft(CQuadPolygon* target_)
{
	XMFLOAT4 myPos = getPos();
	XMFLOAT2 mySize = getSize();
	float colLeft = myPos.x - (mySize.x / 2.0f) - 0.01f;
	float colTop = myPos.y + (mySize.y / 2.0f) - 12.0f;
	float colRight = myPos.x - (mySize.x / 2.0f) + 0.01f;
	float colBottom = myPos.y - (mySize.y / 2.0f) + 12.0f;
	XMFLOAT4 targetPos = target_->getPos();
	XMFLOAT2 targetSize = target_->getSize();
	float targetLeft = targetPos.x - (targetSize.x / 2.0f);
	float targetTop = targetPos.y + (targetSize.y / 2.0f);
	float targetRight = targetPos.x + (targetSize.x / 2.0f);
	float targetBottom = targetPos.y - (targetSize.y / 2.0f);

	return (colLeft < targetRight&& colTop > targetBottom && colRight > targetLeft && colBottom < targetTop);
}
BOOL CQuadPolygon::collisionRight(CQuadPolygon* target_)
{
	XMFLOAT4 myPos = getPos();
	XMFLOAT2 mySize = getSize();
	float colLeft = myPos.x + (mySize.x / 2.0f) - 0.01f;
	float colTop = myPos.y + (mySize.y / 2.0f) - 12.0f;
	float colRight = myPos.x + (mySize.x / 2.0f) + 0.01f;
	float colBottom = myPos.y - (mySize.y / 2.0f) + 12.0f;
	XMFLOAT4 targetPos = target_->getPos();
	XMFLOAT2 targetSize = target_->getSize();
	float targetLeft = targetPos.x - (targetSize.x / 2.0f);
	float targetTop = targetPos.y + (targetSize.y / 2.0f);
	float targetRight = targetPos.x + (targetSize.x / 2.0f);
	float targetBottom = targetPos.y - (targetSize.y / 2.0f);

	return (colLeft < targetRight&& colTop > targetBottom && colRight > targetLeft && colBottom < targetTop);
}

BOOL CQuadPolygon::collisionOffset(CQuadPolygon* target_, XMFLOAT4 offset_, XMFLOAT2 scale_)
{

	XMFLOAT4 myPos = getPos();
	XMFLOAT2 mySize = getSize();
	myPos.x += offset_.x;
	myPos.y += offset_.y;
	myPos.z += offset_.z;
	myPos.w += offset_.w;
	float myLeft = myPos.x - (mySize.x * scale_.x);
	float myTop = myPos.y + (mySize.y * scale_.y);
	float myRight = myPos.x + (mySize.x * scale_.x);
	float myBottom = myPos.y - (mySize.y * scale_.y);
	XMFLOAT4 targetPos = target_->getPos();
	XMFLOAT2 targetSize = target_->getSize();
	float targetLeft = targetPos.x - (targetSize.x / 2.0f);
	float targetTop = targetPos.y + (targetSize.y / 2.0f);
	float targetRight = targetPos.x + (targetSize.x / 2.0f);
	float targetBottom = targetPos.y - (targetSize.y / 2.0f);

	return (myLeft < targetRight&& myTop > targetBottom && myRight > targetLeft && myBottom < targetTop);
}


/**
* @fn void CQuadPolygon::release()
* @brief リリース
* @param	無し
* @return	無し
*/
void CQuadPolygon::release()
{
	/**
	*  必ず生成したのと逆の順番でリリース
	*/
	SAFE_RELEASE(m_pConstantBuffer);
	for (size_t i = 0; i < m_pVertexBuffer.size(); i++)
	{
		SAFE_RELEASE(m_pVertexBuffer[i]);
	}
	SAFE_RELEASE(m_pPixelShader);
	SAFE_RELEASE(m_pInputLayout);
	SAFE_RELEASE(m_pVertexShader);
}
