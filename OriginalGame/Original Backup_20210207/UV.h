﻿/**
 * ここでそれぞれの画像のUV座標を指定する
 */
#pragma once

#include "stdafx.h"
#include "Common3DType.h"

 // サンプル
const std::vector <QuadrangleTexel>   kTexelSamples = {
	//	左下					左上					右下					右上
	{ { 0.0 / 4.0, 1.0 }, { 0.0 / 4.0, 0.0 }, { 1.0 / 4.0, 1.0 }, { 1.0 / 4.0, 0.0 } },
};

//Player
const std::vector<QuadrangleTexel> kTexelPlayers = {
	//左下				左上	   		  右下				右上
	{{0.0 / 1.0,1.0},{0.0 / 1.0,0.0},{1.0 / 1.0,1.0},{1.0 / 1.0,0.0}}
};
const std::vector<QuadrangleTexel> kTexelPlayers_Move = {
	//左下				左上	   		  右下				右上
	{{0.0 / 4.0,1.0},{0.0 / 4.0,0.0},{1.0 / 4.0,1.0},{1.0 / 4.0,0.0}},		//0
	{ {1.0 / 4.0,1.0},{1.0 / 4.0,0.0},{2.0 / 4.0,1.0},{2.0 / 4.0,0.0}},		//1
	{ {2.0 / 4.0,1.0},{2.0 / 4.0,0.0},{3.0 / 4.0,1.0},{3.0 / 4.0,0.0}},		//2
	{ {3.0 / 4.0,1.0},{3.0 / 4.0,0.0},{4.0 / 4.0,1.0},{4.0 / 4.0,0.0}},		//3
};
const std::vector<QuadrangleTexel> kTexelPlayers_Die = {
	//左下				左上	   		  右下				右上
	{{0.0 / 5.0,1.0},{0.0 / 5.0,0.0},{1.0 / 5.0,1.0},{1.0 / 5.0,0.0}},		//0
	{ {1.0 / 5.0,1.0},{1.0 / 5.0,0.0},{2.0 / 5.0,1.0},{2.0 / 5.0,0.0}},		//1
	{ {2.0 / 5.0,1.0},{2.0 / 5.0,0.0},{3.0 / 5.0,1.0},{3.0 / 5.0,0.0}},		//2
	{ {3.0 / 5.0,1.0},{3.0 / 5.0,0.0},{4.0 / 5.0,1.0},{4.0 / 5.0,0.0}},		//3
	{ {4.0 / 5.0,1.0},{4.0 / 5.0,0.0},{5.0 / 5.0,1.0},{5.0 / 5.0,0.0}},		//4
};

const std::vector<QuadrangleTexel> kTexelEnemies_0 = {
	//左下				左上	   		  右下				右上
	{{0.0 / 3.0,1.0},{0.0 / 3.0,0.0},{3.0 / 3.0,1.0},{3.0 / 3.0,0.0}},
};
const std::vector<QuadrangleTexel> kTexelEnemies_0_Move = {
	//左下				左上	   		  右下				右上
	{{0.0 / 3.0,1.0},{0.0 / 3.0,0.0},{1.0 / 3.0,1.0},{1.0 / 3.0,0.0}},		//0
	{ {1.0 / 3.0,1.0},{1.0 / 3.0,0.0},{2.0 / 3.0,1.0},{2.0 / 3.0,0.0}},		//1
	{ {2.0 / 3.0,1.0},{2.0 / 3.0,0.0},{3.0 / 3.0,1.0},{3.0 / 3.0,0.0}},		//2
};
const std::vector<QuadrangleTexel> kTexelEnemies_1 = {
	//左下				左上	   		  右下				右上
	{{0.0 / 3.0,1.0},{0.0 / 3.0,0.0},{3.0 / 3.0,1.0},{3.0 / 3.0,0.0}},
};
const std::vector<QuadrangleTexel> kTexelEnemies_1_Move = {
	//左下				左上	   		  右下				右上
	{{0.0 / 6.0,1.0},{0.0 / 6.0,0.0},{1.0 / 6.0,1.0},{1.0 / 6.0,0.0}},		//0
	{ {1.0 / 6.0,1.0},{1.0 / 6.0,0.0},{2.0 / 6.0,1.0},{2.0 / 6.0,0.0}},		//1
	{ {2.0 / 6.0,1.0},{2.0 / 6.0,0.0},{3.0 / 6.0,1.0},{3.0 / 6.0,0.0}},		//2
	{ {3.0 / 6.0,1.0},{3.0 / 6.0,0.0},{4.0 / 6.0,1.0},{4.0 / 6.0,0.0}},		//3
	{ {4.0 / 6.0,1.0},{4.0 / 6.0,0.0},{5.0 / 6.0,1.0},{5.0 / 6.0,0.0}},		//4
	{ {5.0 / 6.0,1.0},{5.0 / 6.0,0.0},{6.0 / 6.0,1.0},{6.0 / 6.0,0.0}},		//5
};

const std::vector<QuadrangleTexel> kTexelEnemies_2 = {
	//左下				左上	   		  右下				右上
	{{0.0 / 3.0,1.0},{0.0 / 3.0,0.0},{3.0 / 3.0,1.0},{3.0 / 3.0,0.0}},
};
const std::vector<QuadrangleTexel> kTexelEnemies_2_Move = {
	//左下				左上	   		  右下				右上
	{{0.0 / 4.0,1.0},{0.0 / 4.0,0.0},{1.0 / 4.0,1.0},{1.0 / 4.0,0.0}},		//0
	{ {1.0 / 4.0,1.0},{1.0 / 4.0,0.0},{2.0 / 4.0,1.0},{2.0 / 4.0,0.0}},		//1
	{ {2.0 / 4.0,1.0},{2.0 / 4.0,0.0},{3.0 / 4.0,1.0},{3.0 / 4.0,0.0}},		//2
	{ {3.0 / 4.0,1.0},{3.0 / 4.0,0.0},{4.0 / 4.0,1.0},{4.0 / 4.0,0.0}},		//3
};


const std::vector<QuadrangleTexel> kTexelDie = {
	//左下				左上	   		  右下				右上
	{{0.0 / 5.0,1.0},{0.0 / 5.0,0.0},{1.0 / 5.0,1.0},{1.0 / 5.0,0.0}},// 1/5
	{{1.0 / 5.0,1.0},{1.0 / 5.0,0.0},{2.0 / 5.0,1.0},{2.0 / 5.0,0.0}},// 2/5
	{{2.0 / 5.0,1.0},{2.0 / 5.0,0.0},{3.0 / 5.0,1.0},{3.0 / 5.0,0.0}},// 3/5
	{{3.0 / 5.0,1.0},{3.0 / 5.0,0.0},{4.0 / 5.0,1.0},{4.0 / 5.0,0.0}},// 4/5
	{{4.0 / 5.0,1.0},{4.0 / 5.0,0.0},{5.0 / 5.0,1.0},{5.0 / 5.0,0.0}},// 5/5
};

//数字のUV座標
const std::vector<QuadrangleTexel> kTexelNumbers = {
	//左下		　左上		　右下		　右上
	{{0.8f,1.0f},{0.8f,0.5f},{1.0f,1.0f},{1.0f,0.5f}},//!<Number 0

	{{0.0f,0.5f},{0.0f,0.0f},{0.2f,0.5f},{0.2f,0.0f}},//!<Number 1
	{{0.2f,0.5f},{0.2f,0.0f},{0.4f,0.5f},{0.4f,0.0f}},//!<Number 2
	{{0.4f,0.5f},{0.4f,0.0f},{0.6f,0.5f},{0.6f,0.0f}},//!<Number 3
	{{0.6f,0.5f},{0.6f,0.0f},{0.8f,0.5f},{0.8f,0.0f}},//!<Number 4
	{{0.8f,0.5f},{0.8f,0.0f},{1.0f,0.5f},{1.0f,0.0f}},//!<Number 5

	{{0.0f,1.0f},{0.0f,0.5f},{0.2f,1.0f},{0.2f,0.5f}},//!<Number 6
	{{0.2f,1.0f},{0.2f,0.5f},{0.4f,1.0f},{0.4f,0.5f}},//!<Number 7
	{{0.4f,1.0f},{0.4f,0.5f},{0.6f,1.0f},{0.6f,0.5f}},//!<Number 8
	{{0.6f,1.0f},{0.6f,0.5f},{0.8f,1.0f},{0.8f,0.5f}},//!<Number 9
};