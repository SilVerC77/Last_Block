/**
 * @file CDirectInput.h
 * @brief DirectInputキーボード・マウス
 * @author 織戸　喜隆
 * @date 日付（2017.02.14）
 */
#pragma once

#include "stdafx.h"
#include "Common3DType.h"

/**
 * 定数
 */

#define KEY_PRESSED			0x80
#define IS_KEY_PRESSED(x)	(x & KEY_PRESSED)
#define BUFFER_SIZE			256

/*! @class CDirectInput
@brief  DirectInputキーボード・マウス
*/
class CDirectInput
{
	// Properties
	static std::shared_ptr<CDirectInput>	m_instance;
	LPDIRECTINPUT8			m_pDinput;		//!<  DirectInput オブジェクト
	LPDIRECTINPUTDEVICE8	m_pKeyDevice;	//!<  キーボード				
	LPDIRECTINPUTDEVICE8	m_pMouseDevice;	//!<  マウス		

	BYTE m_keyboardInput[BUFFER_SIZE];		//!<  キーボードの入力情報	
	BYTE m_keyboardInputPrev[BUFFER_SIZE];	//!<  前回のキーボードの入力情報

	BOOL m_pressed[BUFFER_SIZE];
	BOOL m_pressedOnce[BUFFER_SIZE];
	DIMOUSESTATE2	m_diMouseState2;		//!<  マウスの入力情報	
	DIMOUSESTATE2	m_mouseInputPrev;		//!<  前回のマウスの入力情報

	HWND m_hWnd;

	// Methods
	CDirectInput();							//!<  【シングルトン・パターン】
	HRESULT init(HWND inHWnd);
	HRESULT initKeyboard(HWND inHWnd);
	HRESULT initMouse(HWND inHWnd);
	VOID free();
public:
	~CDirectInput();						//!<  スマートポインタ　と　【シングルトン・パターン】
	// Methids
	static std::shared_ptr<CDirectInput> getInstance(HWND inHWnd);	//!<  インスタンスを取得
	static VOID releaseInstance();					//!<  インスタンスを解放

	HRESULT getState();

	BOOL isKeyPressed(BYTE aKey);
	BOOL isPressedOnce(BYTE aKey);
	BOOL isKeyTrigger(BYTE aKey);
	BOOL isKeyReleased(BYTE aKey);

	BOOL isLeftButtonClicked();
	BOOL isRightButtonClicked();
	BOOL isCenterButtonClicked();
	BOOL isLeftButtonReleased();
	BOOL isRightButtonReleased();
	BOOL isCenterButtonReleased();

	XMFLOAT2 getMousePos();
};