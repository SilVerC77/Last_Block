/**
* @file CTexture.cpp
* @brief テクセル・クラス
* @author 織戸　喜隆
* @date 日付（2018.07.27）
*/
#include "stdafx.h"
#include "CTexture.h"
#include "GamePGUtil.h"

/**
* @fn std::unique_ptr<CTexture> CTexture::create(ID3D11Device* pDevice_, LPCWSTR textureName_)
* @brief インスタンスを生成し初期化 【クラスメソッド】
* @param pDevice_			デバイス
* @param textureName_		PNGファイル
* @return					CTextureのユニークポインタ
*/
std::unique_ptr<CTexture> CTexture::create(ID3D11Device* pDevice_, LPCWSTR textureName_)
{
	std::unique_ptr<CTexture> object = unique_ptr<CTexture>(new CTexture());
	object->init(pDevice_, textureName_);
	return object;
}


/**
* @fn void CTexture::init(ID3D11Device* pDevice_, LPCWSTR textureName_)
* @brief インスタンスを初期化
* @param pDevice_			デバイス
* @param textureName_		PNGファイル
* @return					無し
*/
void CTexture::init(ID3D11Device* pDevice_, LPCWSTR textureName_)
{
	/**
	* テクスチャー作成(サンプラーを含む) DirectXTex <WindowsSDK 10>
	*/
	if (textureName_ == NULL)
		return;

	/**
	* テクスチャー用サンプラー作成
	*/
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));

	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC; //!< D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 2;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.BorderColor[0] = 0.0f;
	samplerDesc.BorderColor[1] = 0.0f;
	samplerDesc.BorderColor[2] = 0.0f;
	samplerDesc.BorderColor[3] = 0.0f;
	samplerDesc.MinLOD = -FLT_MAX;
	samplerDesc.MaxLOD = FLT_MAX;

	pDevice_->CreateSamplerState(&samplerDesc, &m_pSampleLinear);
	/**
	* テクスチャー作成 DirectXTex <WindowsSDK 10>
	*/
	if (FAILED(CreateWICTextureFromFile(pDevice_, textureName_, NULL, &m_pTexture)))
	{
		TCHAR   str[256];
		wsprintf(str, L"CTexture::init <CreateWICTextureFromFile>テクスチャー作成失敗 %s", textureName_);	//!< テクスチャファイル名を付加する
		throw str;
	}
}

CTexture::CTexture()
{
}

CTexture::~CTexture()
{
}

/**
* @fn BOOL CTexture::setup()
* @brief テクスチャー
* @param		無し
* @return		無し
*/
void CTexture::setup(ID3D11DeviceContext*	pDeviceContext_)
{
	pDeviceContext_->PSSetSamplers(0, 1, &m_pSampleLinear);	//!< PSにサンプラーを設定
	pDeviceContext_->PSSetShaderResources(					//!< テクスチャーをシェーダーに渡す
		0, //!< 設定する最初のスロット番号
		1, //!< 設定するシェーダ・リソース・ビューの数
		&m_pTexture);	//!< 設定するシェーダ・リソース・ビューの配列
}

/**
* @fn void CTexture::release()
* @brief リリース
* @param	無し
* @return	無し
*/
void CTexture::release()
{
	/**
	*  必ず生成したのと逆の順番でリリース
	*/
	SAFE_RELEASE(m_pTexture);
	SAFE_RELEASE(m_pSampleLinear);
}