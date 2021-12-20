/**
* @file CPicture.h
* @brief テクセル付きのピクチャー・・クラス
* @author 織戸　喜隆
* @date 日付（2017.02.14）
*/
#pragma once

#include "stdafx.h"
#include "CQuadPolygon.h"
#include "CTexture.h"

/*! @class CPicture
@brief  ポリゴン・クラス
*/
class CPicture :
	public CQuadPolygon
{
	typedef CQuadPolygon super;
private:
	std::unique_ptr<CQuadPolygon>	m_pPolygon = NULL;
	std::unique_ptr<CTexture>				m_pPolygonTex = NULL;
//	int								m_animIndex = 0;
public:
	/*
	static std::unique_ptr<CPicture> create (
		ID3D11Device*			pDevice_,		//!< デバイス
		ID3D11DeviceContext*	pDeviceContext_,//!< デバイスコンテキスト
		D3D11_VIEWPORT			viewPort_[],	//!< ビューポート
		LPCWSTR					shaderName_,	//!< シェーダープログラム
		LPCWSTR					textureName_,	//!< 画像ファイル
		XMFLOAT4				position_,		//!< ウィンドウの位置
		XMFLOAT2				size_,			//!< スプライトの大きさ
		std::vector<QuadrangleTexel>	texels);		//!< テクセルの配列
	*/
	static CPicture* create (
		ID3D11Device*			pDevice_,		//!< デバイス
		ID3D11DeviceContext*	pDeviceContext_,//!< デバイスコンテキスト
		D3D11_VIEWPORT			viewPort_[],	//!< ビューポート
		LPCWSTR					shaderName_,	//!< シェーダープログラム
		LPCWSTR					textureName_,	//!< 画像ファイル
		XMFLOAT4				position_,		//!< ウィンドウの位置
		XMFLOAT2				size_,			//!< スプライトの大きさ
		std::vector<QuadrangleTexel>	texels);		//!< テクセルの配列

	CPicture();
	~CPicture();
	virtual void init(
		ID3D11Device*			pDevice_,		//!< デバイス
		ID3D11DeviceContext*	pDeviceContext_,//!< デバイスコンテキスト
		D3D11_VIEWPORT			viewPort_[],	//!< ビューポート
		LPCWSTR					shaderName_,	//!< シェーダープログラム
		LPCWSTR					textureName_,	//!< 画像ファイル
		XMFLOAT4				position_,		//!< ウィンドウの位置
		XMFLOAT2				size_,			//!< スプライトの大きさ
		std::vector<QuadrangleTexel>	texels);		//!< テクセルの配列
	virtual void render(XMFLOAT4X4 matView_, XMFLOAT4X4 matProj_, size_t texelIndex_ = 0, XMFLOAT4 diffuse_ = { 0, 0, 0, 0 });
	void release();
	void stepAnimation ();
	void setAnimation ( int animIndex);
	void setAnimationRange( int animBeginIndex_, int animEndIndex_);
};
