/**
 * @file CCamera.h
 * @brief カメラ
 * @author 織戸　喜隆
 * @date 日付（2017.02.14）
 */
#pragma once

#include "stdafx.h"
#include "Common3DType.h"

class CCamera
{
private:
	XMFLOAT4X4	m_matView;		//!< ビュー行列
	XMFLOAT3	m_vEyePt;		//!< カメラ（視点）位置
	XMFLOAT3	m_vLookatPt;	//!< 注視位置
	XMFLOAT3	m_vUpVec;		//!< 上方位置

	XMFLOAT4X4	m_matProj;		//!< プロジェクション行列
	FLOAT		m_angle;		//!< 視野角
	FLOAT		m_aspect;		//!< アスペクト比
	FLOAT		m_near;			//!< 前方クリップ
	FLOAT		m_far;			//!< 後方クリップ

	float		m_defaultEyePtZ;	//!< カメラ座標のデフォルトZ値(リセット用)

public:
	CCamera ();
	~CCamera ();
	XMFLOAT4X4 getViewMatrix()			{ return m_matView; };//!< ビュー行列
	XMFLOAT4X4 getProjectionMatrix()	{ return m_matProj; };//!< プロジェクション行列
	void init (
		XMFLOAT3 vEyePt_,		//!< カメラ（視点）位置
		XMFLOAT3 vLookatPt_,	//!< 注視位置
		XMFLOAT3 vUpVec_,		//!< 上方位置
		FLOAT	angle_,			//!< 視野角
		FLOAT	aspect_,		//!< アスペクト比
		FLOAT	near_,			//!< 前方クリップ
		FLOAT	far_			//!< 後方クリップ
		);
	void update();				//!< ビュー行列、プロジェクション行列を更新する　＜カメラを移動させたい時＞
	XMFLOAT3 getPos();			//!< カメラ座標の取得
	void setPos( XMFLOAT3 pos);	//!< カメラ座標の設定
	void setPos( float x, float y, float z);

	void resetPosZ();			//!< カメラ座標のZ値をデフォルトに戻す
};

