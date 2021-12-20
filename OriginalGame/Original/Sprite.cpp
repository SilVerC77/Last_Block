#include "stdafx.h"
#include "COriginalGameApp.h"
#include "GameResource.h"
#include "UV.h"

/**
 * 関数名 : createSprite()
 * 処理内容 : 指定されたパスにある画像を、指定されたサイズで作成する
 * 引数 :	path_		画像のあるパス
 *			width_		作成する画像の幅
 *			height_		作成する画像の高さ
 * 戻り値 : 作成した画像を管理するオブジェクト
 */
CPicture* COriginalGameApp::createSprite(LPCWSTR path_, float width_, float height_)
{
	vector <QuadrangleTexel>  singleTexels =
	{
		QuadrangleTexel{ { 0.0, 1.0 },{ 0.0, 0.0 },{ 1.0, 1.0 },{ 1.0, 0.0 } }	//!< 指定するUV座標の配列が一つの配列
	};
	XMFLOAT2 spriteSize = { width_, height_ };
	CPicture* object = NULL;

	//Player Sprite
	if (path_ == Tex_Player)
	{
		object = CPicture::create(m_pDevice, m_pDeviceContext, m_viewPort, HLSL_PlaneHLSL, path_,
			{ 0.0f,0.0f,0.0f,0.0f },
			spriteSize,
			kTexelPlayers);
	}
	if (path_ == Tex_Player_Move)
	{
		object = CPicture::create(m_pDevice, m_pDeviceContext, m_viewPort, HLSL_PlaneHLSL, path_,
			{ 0.0f,0.0f,0.0f,0.0f },
			spriteSize,
			kTexelPlayers_Move);
	}
	//Enemy
	else if (path_ == Tex_Enemy_0)
	{
		object = CPicture::create(m_pDevice, m_pDeviceContext, m_viewPort, HLSL_PlaneHLSL, path_,
			{ 0.0f,0.0f,0.0f,0.0f },
			spriteSize,
			kTexelEnemies_1);
	}
	else if (path_ == Tex_Enemy_0_Move)
	{
		object = CPicture::create(m_pDevice, m_pDeviceContext, m_viewPort, HLSL_PlaneHLSL, path_,
			{ 0.0f,0.0f,0.0f,0.0f },
			spriteSize,
			kTexelEnemies_0_Move);
	}

	else if (path_ == Tex_Enemy_1)
	{
		object = CPicture::create(m_pDevice, m_pDeviceContext, m_viewPort, HLSL_PlaneHLSL, path_,
			{ 0.0f,0.0f,0.0f,0.0f },
			spriteSize,
			kTexelEnemies_1);
	}
	else if (path_ == Tex_Enemy_1_Move)
	{
		object = CPicture::create(m_pDevice, m_pDeviceContext, m_viewPort, HLSL_PlaneHLSL, path_,
			{ 0.0f,0.0f,0.0f,0.0f },
			spriteSize,
			kTexelEnemies_1_Move);
	}
	else if (path_ == Tex_Enemy_2)
	{
		object = CPicture::create(m_pDevice, m_pDeviceContext, m_viewPort, HLSL_PlaneHLSL, path_,
			{ 0.0f,0.0f,0.0f,0.0f },
			spriteSize,
			kTexelEnemies_2);
	}
	else if (path_ == Tex_Enemy_2_Move)
	{
		object = CPicture::create(m_pDevice, m_pDeviceContext, m_viewPort, HLSL_PlaneHLSL, path_,
			{ 0.0f,0.0f,0.0f,0.0f },
			spriteSize,
			kTexelEnemies_2_Move);
	}
	//Die
	else if (path_ == Tex_Player_Die)
	{
		object = CPicture::create(m_pDevice, m_pDeviceContext, m_viewPort, HLSL_PlaneHLSL, path_,
			{ 0.0f,0.0f,0.0f,0.0f },
			spriteSize,
			kTexelPlayers_Die);
	}

	else if (path_ == Tex_Die)
	{
		object = CPicture::create(m_pDevice, m_pDeviceContext, m_viewPort, HLSL_PlaneHLSL, path_,
			{ 0.0f,0.0f,0.0f,0.0f },
			spriteSize,
			kTexelDie);
	}
	else if (path_ == Tex_Loading)
	{
		object = CPicture::create(m_pDevice, m_pDeviceContext, m_viewPort, HLSL_PlaneHLSL, path_,
			{ 0.0f,0.0f,0.0f,0.0f },
			spriteSize,
			kTexelLoading);
	}
	else if (path_ == Tex_Difficulty)
	{
		object = CPicture::create(m_pDevice, m_pDeviceContext, m_viewPort, HLSL_PlaneHLSL, path_,
			{ 0.0f,0.0f,0.0f,0.0f },
			spriteSize,
			kTexelDifficulty);
	}
	else if (path_ == Tex_Eyelid)
	{
		object = CPicture::create(m_pDevice, m_pDeviceContext, m_viewPort, HLSL_PlaneHLSL, path_,
			{ 0.0f,0.0f,0.0f,0.0f },
			spriteSize,
			kTexelEyelid);
	}
	else if (path_ == Tex_Rate)
	{
		object = CPicture::create(m_pDevice, m_pDeviceContext, m_viewPort, HLSL_PlaneHLSL, path_,
			{ 0.0f,0.0f,0.0f,0.0f },
			spriteSize,
			kTexelRate);
	}

	else
	{
		// 基本的な作成
		object = CPicture::create(m_pDevice, m_pDeviceContext, m_viewPort, HLSL_PlaneHLSL, path_,
			{ 0.0f, 0.0f, 0.0f, 0.0f },	//!< スプライトの位置
			spriteSize,					//!< スプライトのサイズ
			singleTexels);				//!< 指定するUV座標の配列
	}
	return object;
}

CFont* COriginalGameApp::createFont(LPCWSTR path_, int digits_, float width_, float height_)
{
	XMFLOAT2 spriteSize = { width_, height_ };
	CFont* objects = NULL;
	objects = CFont::create(m_pDevice, m_pDeviceContext, m_viewPort, HLSL_PlaneHLSL, path_,
		{ 0.0f,0.0f,0.0f,0.0f },	//スプライトの位置
		spriteSize,					//スプライトのサイズ
		kTexelNumbers,				//指定するUV座標の配列
		digits_);					//桁数

	return objects;
}
