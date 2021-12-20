/**
 * @file D3DEnv.h
 * @brief 標準のシステム インクルード ファイルのインクルード ファイル、または
 *        参照回数が多く、かつあまり変更されない、プロジェクト専用のインクルード ファイルを記述します。
 * @author 織戸　喜隆
 * @date 日付（2017.02.14）
 */
#pragma once

#include "stdafx.h"

/**
 *デバッグ用のコンパイラディレクティブ
 */
#ifndef DEBUG_BLENDING
#define DEBUG_BLENDING
#endif // !DEBUG_BLENDING
#undef DEBUG_BLENDING


#define WIN32_LEAN_AND_MEAN     //!< Windows ヘッダーから使用されていない部分を除外します。
 //! Windows ヘッダー ファイル:
#include <windows.h>
#define DIRECTINPUT_VERSION 0x0800

#include <array>
#include <vector>
#include <time.h>
#pragma comment(lib,"dxguid")
#pragma comment(lib,"dInput8")
#include <dInput.h>

#pragma warning(disable:4005)
#pragma warning(disable:4316)
#pragma warning(disable:4838)
#pragma warning(disable:4566)

/**
 * DXGI関連の定義の停止	<XNAMath>
 */
#undef DXGI_STATUS_OCCLUDED
#undef DXGI_STATUS_CLIPPED
#undef DXGI_STATUS_NO_REDIRECTION
#undef DXGI_STATUS_NO_DESKTOP_ACCESS
#undef DXGI_STATUS_GRAPHICS_VIDPN_SOURCE_IN_USE
#undef DXGI_STATUS_MODE_CHANGED
#undef DXGI_STATUS_MODE_CHANGE_IN_PROGRESS
#undef DXGI_ERROR_INVALID_CALL
#undef DXGI_ERROR_NOT_FOUND
#undef DXGI_ERROR_MORE_DATA
#undef DXGI_ERROR_UNSUPPORTED
#undef DXGI_ERROR_DEVICE_REMOVED
#undef DXGI_ERROR_DEVICE_HUNG
#undef DXGI_ERROR_DEVICE_RESET
#undef DXGI_ERROR_WAS_STILL_DRAWING
#undef DXGI_ERROR_FRAME_STATISTICS_DISJOINT
#undef DXGI_ERROR_GRAPHICS_VIDPN_SOURCE_IN_USE
#undef DXGI_ERROR_DRIVER_INTERNAL_ERROR
#undef DXGI_ERROR_NONEXCLUSIVE
#undef DXGI_ERROR_NOT_CURRENTLY_AVAILABLE
#undef DXGI_ERROR_REMOTE_CLIENT_DISCONNECTED
#undef DXGI_ERROR_REMOTE_OUTOFMEMORY
#undef D3D11_ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS
#undef D3D11_ERROR_FILE_NOT_FOUND
#undef D3D11_ERROR_TOO_MANY_UNIQUE_VIEW_OBJECTS
#undef D3D11_ERROR_DEFERRED_CONTEXT_MAP_WITHOUT_INITIAL_DISCARD
#undef D3D10_ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS
#undef D3D10_ERROR_FILE_NOT_FOUND

#include <mmsystem.h>
#include <stdio.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <DirectXTex.h>			// VC2017 WindowsSDK 10
#include <WICTextureLoader.h>	// VC2017 WindowsSDK 10
#include <d3dCompiler.h>
//#include <fbxsdk.h>
#include <iostream>
#include <string.h>
#include <functional>
#include <memory>

/**
 * 必要なライブラリファイルのロード
 */
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dCompiler.lib")//#pragma comment(lib,"d3dx11.lib")	// VC_2017

#if defined(DEBUG) || defined(_DEBUG)
#else
#pragma comment( lib, "d3dx11.lib" )
#endif

//#pragma comment(lib,"libfbxsdk-mt.lib")
#pragma comment(lib,"wininet.lib")

#pragma comment( lib, "dxgi.lib" )

/**
 * 名前空間
 */
using namespace std;
using namespace DirectX;