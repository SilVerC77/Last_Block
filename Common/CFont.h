
#pragma once 

#include "stdafx.h"
#include "CPicture.h"

class CFont
{
private:
	CPicture**	objects;
	int			digits;

public:
	static CFont* create(
		ID3D11Device* pDevice_,		//!< デバイス
		ID3D11DeviceContext* pDeviceContext_,//!< デバイスコンテキスト
		D3D11_VIEWPORT			viewPort_[],	//!< ビューポート
		LPCWSTR					shaderName_,	//!< シェーダープログラム
		LPCWSTR					textureName_,	//!< 画像ファイル
		XMFLOAT4				position_,		//!< ウィンドウの位置
		XMFLOAT2				size_,			//!< スプライトの大きさ
		std::vector<QuadrangleTexel>	texels,	//!< テクセルの配列
		int						digits_);		//!< 桁数

	void init(
		ID3D11Device* pDevice_,		//!< デバイス
		ID3D11DeviceContext* pDeviceContext_,//!< デバイスコンテキスト
		D3D11_VIEWPORT			viewPort_[],	//!< ビューポート
		LPCWSTR					shaderName_,	//!< シェーダープログラム
		LPCWSTR					textureName_,	//!< 画像ファイル
		XMFLOAT4				position_,		//!< ウィンドウの位置
		XMFLOAT2				size_,			//!< スプライトの大きさ
		std::vector<QuadrangleTexel>	texels,	//!< テクセルの配列
		int						digits_
	);

	CFont();
	~CFont();

	void render(XMFLOAT4X4 matView_, XMFLOAT4X4 matProj_, int num_, size_t texelIndex_ = 0, XMFLOAT4 diffuse_ = { 0, 0, 0, 0 });
	void release();

	void setPos(float x, float y);
};
