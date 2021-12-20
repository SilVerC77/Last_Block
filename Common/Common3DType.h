/**
 * @file Common3DType.h
 * @brief DirectX11をベースにした3Dの基本的な構造
 * @author 織戸　喜隆
 * @date 日付（2017.02.14）
 */
#pragma once

#include "stdafx.h"
#include "GameConst.h"
#include "GamePGUtil.h"

/**
 * テクセル
 */

/*! @struct QuadrangleTexel
@brief  四角形のテクセル
*/
typedef struct QuadrangleTexel {
	XMFLOAT2 v1 = { 0, 1 };	//!< 頂点1
	XMFLOAT2 v2 = { 0, 0 };	//!< 頂点2
	XMFLOAT2 v3 = { 1, 1 };	//!< 頂点3
	XMFLOAT2 v4 = { 1, 0 };	//!< 頂点4
	QuadrangleTexel(XMFLOAT2 v1_, XMFLOAT2 v2_, XMFLOAT2 v3_, XMFLOAT2 v4_){
		v1 = v1_;
		v2 = v2_;
		v3 = v3_;
		v4 = v4_;
	}
}QuadrangleTexel;

/**
 * 頂点の構造体 XMFLOAT3 XMFLOAT4
 */
/*! @struct VertexWithTex
@brief  頂点 <3D/4D位置 テクセル>
*/
template < typename XMFLOAT_X >
struct VertexWithTex
{
	XMFLOAT_X	pos;	//!< 位置
	XMFLOAT2	texel;	//!< テクセル
};

/*! @struct VertexWithNormal
@brief  頂点 <3D/4D位置 法線>
*/
template < typename XMFLOAT_X >
struct VertexWithNormal
{
	XMFLOAT_X	pos;	//!< 位置
	XMFLOAT_X	normal;	//!< 法線　シェーディング（陰影計算）には法線は必須
};

/*! @struct Vertex3DWithNormalTexel
@brief  頂点 <3D位置 法線 テクセル>
*/
template < typename XMFLOAT_X >
struct VertexWithNormalTex
{
	XMFLOAT_X	pos;	//!< 位置
	XMFLOAT_X	normal;	//!< 法線　シェーディング（陰影計算）には法線は必須
	XMFLOAT2	texel;	//!< テクセル
};

/*! @struct VertexBone
@brief  頂点の構造体
*/
template < typename XMFLOAT_X >
struct VertexBone {
	XMFLOAT_X	pos;			//!< 頂点位置
	XMFLOAT_X	normal;			//!< 頂点法線
	XMFLOAT2	texel;			//!< テクセル	
	UINT		boneIndex[4];	//!< ボーン　番号
	FLOAT		boneWeight[4];	//!< ボーン　重み
	VertexBone()
	{
//		ZeroMemory (this, sizeof (Vertex3DBone));
	}
};


/**
 * 姿勢の構造体 XMFLOAT3 XMFLOAT4
 */
/*! @struct Position
@brief  姿勢
*/
template < typename XMFLOAT_X >
struct Position
{
	XMFLOAT_X	pos;	//!< 位置		
	FLOAT		pitch;	//!< ピッチ	<X>
	FLOAT		yaw;	//!< ヨー	<Y>
	FLOAT		roll;	//!< ロール	<Z>
	XMFLOAT3	scale;	//!< スケール

	void setup(XMFLOAT_X pos_, FLOAT pitchDegree_, FLOAT yawDegree_, FLOAT rollDegree_, XMFLOAT3 scale_)
	{
		pos		= pos_;
		pitch	= pitchDegree_;
		yaw		= yawDegree_;
		roll	= rollDegree_;
		scale	= scale_;
	}
	void offset(XMFLOAT4 alpha_)
	{
		pos.x += alpha_.x;
		pos.y += alpha_.y;
		pos.z += alpha_.z;
		pos.w += alpha_.w;
	}
	void normalize(void)
	{
		if (pitch >= 360.0f || pitch < 0.0f) {
			pitch = fmodf((pitch), 360.0f);
		}
		if (yaw >= 360.0f || yaw < 0.0f) {
			yaw = fmodf((yaw), 360.0f);
		}
		if (roll >= 360.0f || roll < 0.0f) {
			roll = fmodf((roll), 360.0f);
		}
	}

	void setAngle(XMFLOAT3 degrees_) {
		pitch	= degrees_.x;
		yaw		= degrees_.y;
		roll	= degrees_.z;
		normalize();		//!< 正規化
	}
};

/**
 * シェーダーに渡す値
 */

/*! @struct ShaderGlobal_0
@brief  シェーダーに渡す値
*/
typedef struct ShaderGlobal_0 {
	XMFLOAT4X4	matWVP;		//!< ワールドから射影までの変換行列
	XMFLOAT4	diffuse;	//!< ディフューズ色				
}ShaderGlobal_0;

/*! @struct ShaderGlobal_1
@brief  シェーダー・グローバル
*/
typedef struct ShaderGlobal_1 {
	XMFLOAT4X4	matWorld;	//!< ワールド行列					
	XMFLOAT4X4	matWVP;		//!< ワールドから射影までの変換行列
	XMFLOAT4	lightDir;	//!< ライト方向					
	XMFLOAT4	diffuse;	//!< ディフューズ色				
}ShaderGlobal_1;

/*! @struct ShaderGlobal_2
@brief  シェーダーに渡す値
*/
typedef struct ShaderGlobal_2 {
	XMFLOAT4X4	matWorld;		//!< ワールド行列					
	XMFLOAT4X4	matWVP;			//!< ワールドから射影までの変換行列
	XMFLOAT4	lightDir;		//!< ライト方向					
	XMFLOAT4	diffuse;		//!< ディフューズ色				
	XMVECTORF32	eye;			//!< カメラ位置（視点位置）		
}ShaderGlobal_2;

/*! @struct ShaderGlobal_3
@brief  シェーダー・グローバル
*/
typedef struct ShaderGlobal_3 {
	XMFLOAT4X4	matWorld;	//!< ワールド行列					
	XMFLOAT4X4	matWVP;		//!< ワールドから射影までの変換行列
	XMFLOAT4	ambient;	//!< アンビエント光				
	XMFLOAT4	diffuse;	//!< ディフューズ色				
	XMFLOAT4	specular;	//!< 鏡面反射					
} ShaderGlobal_3;

/*! @struct ShaderGlobal_Bones
@brief  シェーダーに渡すボーン行列配列
*/
typedef struct ShaderGlobal_Bones {
	XMFLOAT4X4 matBone[256];
	ShaderGlobal_Bones ()
	{
		for (int i = 0; i < 256; i++)
		{
			XMStoreFloat4x4 (&matBone[i], XMMatrixIdentity ());
		}
	}
}ShaderGlobalBones;

/**
 * シェーダーに渡す値　定数バッファ
 */

/*! @struct ShaderConstantBufferLE
@brief  定数バッファ(ライト方向とカメラ位置)
*/
typedef struct ShaderConstantBufferLE  {
	XMFLOAT4 lightDir;	//!< ライト方向
	XMFLOAT4 eye;		//!< カメラ位置
} ShaderConstantBufferLE;

/**
 * FBX
 */

/*! @struct Material
@brief  マテリアル
*/
typedef struct Material {
	XMFLOAT4 ambient;				//!< アンビエント	
	XMFLOAT4 diffuse;				//!< ディフューズ	
	XMFLOAT4 specular;				//!< スペキュラー	
	CHAR textureName[100];			//!< テクスチャーファイル名		
	ID3D11ShaderResourceView* pTexture;	//!< テクスチャーファイル名		
	DWORD numOfFace;					//!< そのマテリアルであるポリゴン数
	Material ()
	{
		ZeroMemory(this, sizeof(Material));
	}
	~Material ()
	{
		SAFE_RELEASE(pTexture);
	}
} Material;

/**
 * ボーン
 */

/*! @struct Bone
@brief  ボーン構造体
*/
typedef struct Bone {
	XMFLOAT4X4 matBindPose;	//!< 初期ポーズ（ずっと変わらない）
	XMFLOAT4X4 matNewPose;	//!< 現在のポーズ（その都度変わる）
	Bone ()
	{
		ZeroMemory (this, sizeof (Bone));
	}
}Bone;

/*! @struct PolyTable
@brief  １頂点の共有　最大20ポリゴンまで
*/
typedef struct PolyTable {
	int polyIndex[20];	//!< ポリゴン番号			
	int index123[20];	//!< 3つの頂点のうち、何番目か
	int numOfRef;		//!< 属しているポリゴン数	
	PolyTable ()
	{
		ZeroMemory (this, sizeof (PolyTable));
	}
}PolyTable;