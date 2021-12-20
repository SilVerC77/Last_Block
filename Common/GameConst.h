/**
* @file GameConst.h
* @brief 定数
*
* @author 織戸　喜隆
* @date 日付（2017.02.14）
*/
#pragma once

#include "stdafx.h"

/**
 * 定数
 */
#define APP_NAME L"GameProgramming1 2D Shooting"

 /*! @name 定義グループ ウインドウ
 ウインドウに関する定数
 */
 /* @{ */
#define WINDOW_WIDTH	1920	//! ウィンドウ幅
#define WINDOW_HEIGHT	1080	//! ウィンドウ高さ
const bool kFullScreen = FALSE;	//! フルスクリーンモード

#define	ASPECT			((FLOAT)WINDOW_WIDTH / (FLOAT)WINDOW_HEIGHT)
#define WINDOW_LEFT		(-WINDOW_WIDTH / 2)
#define WINDOW_RIGHT	( WINDOW_WIDTH / 2)
#define WINDOW_TOP		( WINDOW_HEIGHT / 2)
#define WINDOW_BOTTOM	(-WINDOW_HEIGHT / 2)
const XMFLOAT2 kSpriteScreenkSize	= { WINDOW_WIDTH, WINDOW_HEIGHT - 4 };
const FLOAT k1_1	= 1.0f;
const FLOAT k1_2	= 1.0f /  2;
const FLOAT k1_3	= 1.0f /  3;
const FLOAT k1_4	= 1.0f /  4;
const FLOAT k1_5	= 1.0f /  5;
const FLOAT k1_8	= 1.0f /  8;
const FLOAT k1_26	= 1.0f / 26;
/* @} */

/*! @name 定義グループ Shooting
シューティングで使用する定数
*/
/* @{ */
const int kMaxHP					= 7;
const XMFLOAT2 kCharSpriteSize	= { 48.0f , 48.0f };
const XMFLOAT2 kBulletSize		= { 16.0f , 16.0f };
const XMFLOAT2 kExplosionSize	= { 80.0f , 80.0f };
const FLOAT kBullet_Left	= -7.0f;
const FLOAT kBullet_Right	= -7.0f + 12.0f;
const FLOAT kEnemyQuantity			= 0.25f * 1.5;
const FLOAT kPlayerQuantity			= 15.25f;
const int kTexelIntervalFrames			= 15;
const int kIntervalShot				= 64*4;
const int kExplosionTexelPauseFrames	= 8;
const int kTexelPlayerBulletIndex	= 0;
const int kTexelEnemyBulletIndex	= 1;
/* @} */

/*! @name 定義グループ DotEat
ドットイートで使用する定数
*/
/* @{ */
const int kMapVertical = 9;		//! マップの行数
const int kMapHorizontal = 12;	//! マップの列数
const int kChipWidth = 32;		//! マップチップ
const int kChipHeight = 32;
const XMFLOAT2 kBlockSize	= { 32.0f , 32.0f };
const XMFLOAT2 kDotSize		= { 16.0f , 16.0f };
const XMFLOAT2 kPlayerSize	= { 32.0f , 32.0f };
const XMFLOAT2 kEnemySize	= { 32.0f , 32.0f };
/* @} */

/**
 * 型
 */

 /**
 * @enum eGameStatus
 * ゲームの進行状況
 */
typedef enum eGameStatus
{
	eNone,
	eOpening,
	ePlaying,
	eGameClear,
	eGameOver,
	eEnding
}eGameStatus;

/**
* @enum ePhase
* フローの進行状態
*/
typedef enum ePhase
{
	eBegin = 0,
	eMain,
	eEnd,
}ePhase;

/**
* @enum eScrollMode
* スクロール方向
*/
typedef enum eScrollMode
{
	eHorizontal,
	eVirtical
}eScrollMode;

/**
* @enum eObjectState
* キャラクタの状態
*/
typedef enum eObjectState
{
	eWaiting,
	eMoving
}eObjectStatus;