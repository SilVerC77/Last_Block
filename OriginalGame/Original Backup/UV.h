/**
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
