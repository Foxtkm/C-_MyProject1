/******************************************************************************
* File Name : gkeyinp.h                            Ver : 1.00  Date : 98-10-22
*
* Description :
*
*       ゲームシステム用 キーボード入力 ヘッダファイル．
*
* Author : Moriharu Ohzu.
*
******************************************************************************/
#ifndef		_GKEYINP_H_
#define		_GKEYINP_H_

#include	"type.h"

/****** 定数宣言 *************************************************************/

/*----- キーコード情報 ------------------------------------------------------*/

#define	GKEY_ESCAPE			0x01
#define	GKEY_1				0x02
#define	GKEY_2				0x03
#define	GKEY_3				0x04
#define	GKEY_4				0x05
#define	GKEY_5				0x06
#define	GKEY_6				0x07
#define	GKEY_7				0x08
#define	GKEY_8				0x09
#define	GKEY_9				0x0A
#define	GKEY_0				0x0B
#define	GKEY_MINUS			0x0C
#define	GKEY_EQUALS			0x0D
#define	GKEY_BACK			0x0E
#define	GKEY_TAB			0x0F
#define	GKEY_Q				0x10
#define	GKEY_W				0x11
#define	GKEY_E				0x12
#define	GKEY_R				0x13
#define	GKEY_T				0x14
#define	GKEY_Y				0x15
#define	GKEY_U				0x16
#define	GKEY_I				0x17
#define	GKEY_O				0x18
#define	GKEY_P				0x19
#define	GKEY_LBRACKET		0x1A
#define	GKEY_RBRACKET		0x1B
#define	GKEY_RETURN			0x1C
#define	GKEY_LCONTROL		0x1D
#define	GKEY_A				0x1E
#define	GKEY_S				0x1F
#define	GKEY_D				0x20
#define	GKEY_F				0x21
#define	GKEY_G				0x22
#define	GKEY_H				0x23
#define	GKEY_J				0x24
#define	GKEY_K				0x25
#define	GKEY_L				0x26
#define	GKEY_SEMICOLON		0x27
#define	GKEY_APOSTROPHE		0x28
#define	GKEY_GRAVE			0x29
#define	GKEY_LSHIFT			0x2A
#define	GKEY_BACKSLASH		0x2B
#define	GKEY_Z				0x2C
#define	GKEY_X				0x2D
#define	GKEY_C				0x2E
#define	GKEY_V				0x2F
#define	GKEY_B				0x30
#define	GKEY_N				0x31
#define	GKEY_M				0x32
#define	GKEY_COMMA			0x33
#define	GKEY_PERIOD			0x34
#define	GKEY_SLASH			0x35
#define	GKEY_RSHIFT			0x36
#define	GKEY_MULTIPLY		0x37
#define	GKEY_LMENU			0x38
#define	GKEY_SPACE			0x39
#define	GKEY_CAPITAL		0x3A
#define	GKEY_F1				0x3B
#define	GKEY_F2				0x3C
#define	GKEY_F3				0x3D
#define	GKEY_F4				0x3E
#define	GKEY_F5				0x3F
#define	GKEY_F6				0x40
#define	GKEY_F7				0x41
#define	GKEY_F8				0x42
#define	GKEY_F9				0x43
#define	GKEY_F10			0x44
#define	GKEY_NUMLOCK		0x45
#define	GKEY_SCROLL			0x46
#define	GKEY_NUMPAD7		0x47
#define	GKEY_NUMPAD8		0x48
#define	GKEY_NUMPAD9		0x49
#define	GKEY_SUBTRACT		0x4A
#define	GKEY_NUMPAD4		0x4B
#define	GKEY_NUMPAD5		0x4C
#define	GKEY_NUMPAD6		0x4D
#define	GKEY_ADD			0x4E
#define	GKEY_NUMPAD1		0x4F
#define	GKEY_NUMPAD2		0x50
#define	GKEY_NUMPAD3		0x51
#define	GKEY_NUMPAD0		0x52
#define	GKEY_DECIMAL		0x53
#define	GKEY_F11			0x57
#define	GKEY_F12			0x58
#define	GKEY_F13			0x64
#define	GKEY_F14			0x65
#define	GKEY_F15			0x66
#define	GKEY_KANA			0x70
#define	GKEY_CONVERT		0x79
#define	GKEY_NOCONVERT		0x7B
#define	GKEY_YEN			0x7D
#define	GKEY_NUMPADEQUALS	0x8D
#define	GKEY_CIRCUMFLEX		0x90
#define	GKEY_AT				0x91
#define	GKEY_COLON			0x92
#define	GKEY_UNDERLINE		0x93
#define	GKEY_KANJI			0x94
#define	GKEY_STOP			0x95
#define	GKEY_AX				0x96
#define	GKEY_UNLABELED		0x97
#define	GKEY_NUMPADENTER	0x9C
#define	GKEY_RCONTROL		0x9D
#define	GKEY_NUMPADCOMMA	0xB3
#define	GKEY_DIVIDE			0xB5
#define	GKEY_SYSRQ			0xB7
#define	GKEY_RMENU			0xB8
#define	GKEY_HOME			0xC7
#define	GKEY_UP				0xC8
#define	GKEY_PRIOR			0xC9
#define	GKEY_LEFT			0xCB
#define	GKEY_RIGHT			0xCD
#define	GKEY_END			0xCF
#define	GKEY_DOWN			0xD0
#define	GKEY_NEXT			0xD1
#define	GKEY_INSERT			0xD2
#define	GKEY_DELETE			0xD3
#define	GKEY_LWIN			0xDB
#define	GKEY_RWIN			0xDC
#define	GKEY_APPS			0xDD
#define	GKEY_BACKSPACE		GKEY_BACK
#define	GKEY_NUMPADSTAR		GKEY_MULTIPLY
#define	GKEY_LALT			GKEY_LMENU
#define	GKEY_CAPSLOCK		GKEY_CAPITAL
#define	GKEY_NUMPADMINUS	GKEY_SUBTRACT
#define	GKEY_NUMPADPLUS		GKEY_ADD
#define	GKEY_NUMPADPERIOD	GKEY_DECIMAL
#define	GKEY_NUMPADSLASH	GKEY_DIVIDE
#define	GKEY_RALT			GKEY_RMENU
#define	GKEY_UPARROW		GKEY_UP
#define	GKEY_PGUP			GKEY_PRIOR
#define	GKEY_LEFTARROW		GKEY_LEFT
#define	GKEY_RIGHTARROW		GKEY_RIGHT
#define	GKEY_DOWNARROW		GKEY_DOWN
#define	GKEY_PGDN			GKEY_NEXT

/****** 関数プロトタイプ宣言 *************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/****** データ型宣言 *********************************************************/

typedef	void*	GKEYINP;			/* キーボードハンドラ       */

typedef	DWORD	GKEYCODE;			/* キーコード               */

/*=============================================================================
*
* Purpose : キーボードの生成．
*
* Return  : キーボードハンドラを返す．
*
*============================================================================*/
GKEYINP
GKeyInpNew
(
	HINSTANCE			hinst,		/* インスタンスハンドラ     */
	HWND				hwnd		/* ウィンドウハンドラ       */
);

/*=============================================================================
*
* Purpose : キーボードの消滅．
*
* Return  : なし．
*
*============================================================================*/
void
GKeyInpDelete
(
	GKEYINP				hnd			/* キーボードハンドラ       */
);

/*=============================================================================
*
* Purpose : キーボードの読み込み．
*
* Return  : なし．
*
*============================================================================*/
void
GKeyInpReadDevice
(
	GKEYINP				hnd			/* キーボードハンドラ       */
);

/*=============================================================================
*
* Purpose : キーの入力状態を調べる．
*
* Return  : キーが押されていれば真，押されていなければ偽を返す．
*
*============================================================================*/
BOOL
GKeyInpGetState
(
	GKEYINP				hnd,		/* キーボードハンドラ       */
	GKEYCODE			key_code	/* キーコード               */
);

/*=============================================================================
*
* Purpose : キーが押されたか調べる．
*
* Return  : キーが押されていれば真，押されていなければ偽を返す．．
*
*============================================================================*/
BOOL
GKeyInpGetTrigger
(
	GKEYINP				hnd,		/* キーボードハンドラ       */
	GKEYCODE			key_code	/* キーコード               */
);

/*=============================================================================
*
* Purpose : キーが離されたか調べる．
*
* Return  : キーが離されれば真，離されていなければ偽を返す．
*
*============================================================================*/
BOOL
GKeyInpGetDetach
(
	GKEYINP				hnd,		/* キーボードハンドラ       */
	GKEYCODE			key_code	/* キーコード               */
);

/*=============================================================================
*
* Purpose : キーボードから押されているキーを調べる．
*
* Return  : 押されているキーのキーコードを返す．
*           何も押されていなければ０を返す．
*
*============================================================================*/
GKEYCODE
GKeyInpGetKey
(
	GKEYINP				hnd			/* キーボードハンドラ       */
);

/*=============================================================================
*
* Purpose : キーボードのアクセス権の再取得．
*
* Return  : 取得できれば真，取得できなければ偽を返す．
*
*============================================================================*/
BOOL
GKeyInpReacquire
(
	GKEYINP				hnd			/* キーボードハンドラ       */
);

#ifdef __cplusplus
}
#endif

#endif

/********** End of File ******************************************************/
