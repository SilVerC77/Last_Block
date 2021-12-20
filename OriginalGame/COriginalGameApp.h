/**
* @file COriginalGameApp.h
* @author 織戸　喜隆
* @date 日付（2017.02.14）
*/
#pragma once

#include "stdafx.h"
#include "CApplication.h"
#include "CPicture.h"
#include "MediaResource.h"
#include "UVAnimation.h"
#include "CFont.h"

class COriginalGameApp :
	public CApplication
{
	typedef CApplication super;
private:
	int m_activeEnemies										= 0;

	CPicture* createSprite(LPCWSTR path_, float width_ = WINDOW_WIDTH, float height_ = WINDOW_HEIGHT);
	void renderSprite( CPicture* object);
	void renderSprite(CPicture* object, XMFLOAT3 color);
	void renderSprite(CPicture* object, XMFLOAT4 color);
	void disposeSprite( CPicture* object);
	CFont* createFont(LPCWSTR path_, int digits, float width = WINDOW_WIDTH, float height = WINDOW_HEIGHT);
	void renderFont( CFont* objects, int num_);
	void disposeFont( CFont* objects);
	bool createSpaceship(void);
	void initCameraInfo();
	void doOpenning();
	void doPlaying();
	void doGameClear();
	void doGameOver();
	void doEnding();

	void procTitle();
	void procGame();
	void procClear();
	void procGameOver();

	void procTitleBegin();
	void procTitleMain();
	void procTitleEnd();
	void procPlayBegin();
	void procPlayMain();
	void procPlayEnd();
	void procClearBegin();
	void procClearMain();
	void procClearEnd();
	void procOverBegin();
	void procOverMain();
	void procOverEnd();

	void procPlayMain_enemy();
	void procPlayMain_movePlayer();
	void procPlayMain_score();
	void procPlayMain_block();
	void procPlayMain_airblock();
	void procPlayMain_animateDie();
	void procPlayMain_animateMove();

	void procPlayMains_camera();
	//Camera
	XMFLOAT2 amplitube = { 0.f,0.f };
	void cameramove();
	void cameramove(float _amplitubeX, float _amplitubeY);

	int DeadCounter = 0;
	int Difficulty = 1;

public:
	COriginalGameApp();
	~COriginalGameApp();
	void render(XMFLOAT4X4 matView, XMFLOAT4X4 matProj);
	void release();
};
