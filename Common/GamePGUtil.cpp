/**
* @file GamePGUtil.cpp
* @brief DirectXMath.h -- SIMD C++ Math library, etc
* @author 織戸　喜隆
* @date 日付（2017.02.14）
*/
#include "stdafx.h"
#include "GamePGUtil.h"

/**
* @fn XMFLOAT4 ComputePosByDirectionZ(XMFLOAT4 old_, FLOAT direction_)
* @brief Zの向きから位置を求める
* @param old_				移動前の位置
* @param direction_	方向
* @return					移動後の位置
*/
XMFLOAT4 ComputePosByDirectionZ(XMFLOAT4 old_, FLOAT direction_)
{
	XMFLOAT4 resultVector;
	XMMATRIX tempMat = XMMatrixRotationZ(XMConvertToRadians(direction_));

	XMStoreFloat4(
		&resultVector,
		XMVector3TransformCoord(XMLoadFloat4(&old_), tempMat));

	return resultVector;
}

/**
 * @fn void debugString(HWND hWnd_, int nXStart_, int nYStart_, LPCTSTR lpString_)
 * @brief Zの向きから位置を求める
 * @param hWnd_		WindowsAPIウインドウハンドル
 * @param nXStart_	x位置
 * @param nYStart_	y位置
 * @param lpString_	表示する文字列
 * @return			無し
 */
void debugString(HWND hWnd_, int nXStart_, int nYStart_, LPCTSTR lpString_)
{
	HDC hdc = GetDC(hWnd_);
	TextOut(hdc, nXStart_, nYStart_, lpString_, sizeof(lpString_));
	ReleaseDC(hWnd_, hdc);
}

/**
 * @fn HRESULT PutMessage(LPCTSTR lpString_)
 * @brief Zの向きから位置を求める
 * @param lpString_	表示する文字列
 * @return			無し
 */
HRESULT PutMessage(LPCTSTR lpString_)
{
	std::cout << lpString_ << std::endl;
	MessageBox(0, lpString_, NULL, MB_OK);
	return GetLastError();
}