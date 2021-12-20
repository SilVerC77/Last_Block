/**
* @file CTexture.h
* @brief テクスチャ・クラス
* @author 織戸　喜隆
* @date 日付（2018.07.27）
*/
#pragma once

#include "stdafx.h"

class CTexture
{
private:
	ID3D11SamplerState*			m_pSampleLinear = NULL;	//!< テクスチャーのサンプラー
	ID3D11ShaderResourceView*	m_pTexture		= NULL;	//!< テクスチャー

public:
	static std::unique_ptr<CTexture> create(
		ID3D11Device*			pDevice_,		//!< デバイス
		LPCWSTR					textureName_);	//!< 画像ファイル
	void init(
		ID3D11Device*			pDevice_,		//!< デバイス
		LPCWSTR					textureName_);	//!< 画像ファイル
	
	CTexture();
	~CTexture();
	void setup(ID3D11DeviceContext*	pDeviceContext_);
	void release();
};