/**
* @file GamePGUtil.h
* @brief defineマクロ、関数
* @author 織戸　喜隆
* @date 日付（2018.08.09）
*/
#pragma once

#include "stdafx.h"

/**
 * defineマクロ
 */
 /** @def
 * DirectXリソースの解放
 */
#define SAFE_RELEASE(x)			{ if(x)	{ x->Release();	x = NULL;	}	}

 /** @def
 * インスタンスの解放
 */
#define SAFE_release(x)			{ if(x)	{ x->release();	x = NULL;	}	}

 /** @def
 * インスタンスの削除
 */
#define SAFE_DELETE(x)			{ if(x)	{ delete x;		x = NULL;	}	}

 /** @def
 * DirectXリソース配列の解放
 */
#define SAFE_DELETE_ARRAY(x)	{ if(x)	{ delete[] x;	x = NULL;	}	}

 /** @def
 * 削除と解放
 */
#define SAFE_release_DELETE(x)	{ if(x)	{ x->release(); delete x;	x = NULL;	}	}

 /**
 * 関数
 */
XMFLOAT4 ComputePosByDirectionZ(XMFLOAT4 old_, FLOAT direction_);
HRESULT PutMessage(LPCTSTR lpString_);
void debugString(HWND hWnd_, int nXStart_, int nYStart_, LPCTSTR lpString_);