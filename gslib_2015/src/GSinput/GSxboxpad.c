/******************************************************************************
* File Name : GSxboxpad.c                        Ver : 1.00  Date : 2016-04-16
*
* Description :
*
*       XBOXパッド入力.
*
* Author : Moriharu Ohzu.
*
******************************************************************************/
#include	"GSxboxpad.h"
#include	"heap.h"
#include	<GStype.h>
#include	<Xinput.h>

/*----- XInputのlibファイル -------------------------------------------------*/

#pragma comment(lib, "Xinput9_1_0.lib")

/****** データ型宣言 *********************************************************/

extern const GSushort GS_XBOX_PAD_UP = XINPUT_GAMEPAD_DPAD_UP;
extern const GSushort GS_XBOX_PAD_DOWN = XINPUT_GAMEPAD_DPAD_DOWN;
extern const GSushort GS_XBOX_PAD_LEFT = XINPUT_GAMEPAD_DPAD_LEFT;
extern const GSushort GS_XBOX_PAD_RIGHT = XINPUT_GAMEPAD_DPAD_RIGHT;
extern const GSushort GS_XBOX_PAD_START = XINPUT_GAMEPAD_START;
extern const GSushort GS_XBOX_PAD_BACK = XINPUT_GAMEPAD_BACK;
extern const GSushort GS_XBOX_PAD_LEFT_THUMB = XINPUT_GAMEPAD_LEFT_THUMB;
extern const GSushort GS_XBOX_PAD_RIGHT_THUMB = XINPUT_GAMEPAD_RIGHT_THUMB;
extern const GSushort GS_XBOX_PAD_LEFT_SHOULDER = XINPUT_GAMEPAD_LEFT_SHOULDER;
extern const GSushort GS_XBOX_PAD_RIGHT_SHOULDER = XINPUT_GAMEPAD_RIGHT_SHOULDER;
extern const GSushort GS_XBOX_PAD_A = XINPUT_GAMEPAD_A;
extern const GSushort GS_XBOX_PAD_B = XINPUT_GAMEPAD_B;
extern const GSushort GS_XBOX_PAD_X = XINPUT_GAMEPAD_X;
extern const GSushort GS_XBOX_PAD_Y = XINPUT_GAMEPAD_Y;

/*=============================================================================
*
* Purpose : XBOXパッドの生成．
*
* Return  : XBOXを返す．
*
*============================================================================*/
extern GSxboxpad*
XBoxPadNew
(
	void
)
{
	GSxboxpad*	pad;
	pad = new_object(sizeof(GSxboxpad));
	memset(pad, 0, sizeof(GSxboxpad));
	pad->newbuf = pad->state[0];
	pad->oldbuf = pad->state[1];
	return pad;
}

/*=============================================================================
*
* Purpose : XBOXパッドの消滅．
*
* Return  : なし．
*
*============================================================================*/
extern void
XBoxPadDelete
(
	GSxboxpad*			pad		/* XBOXパッド         　　　　      */
)
{
	del_object(pad);
}

/*=============================================================================
*
* Purpose : XBOXパッドの読み込み．
*
* Return  : なし．
*
*============================================================================*/
extern void
XBoxPadRead
(
	GSxboxpad*			pad		/* XBOXパッド         　　　　      */
)
{
	XINPUT_STATE*	tmp;
	int				i;

	/* 現在のバッファと古いバッファを入れ換える */
	tmp = pad->newbuf;
	pad->newbuf = pad->oldbuf;
	pad->oldbuf = tmp;

	/* 全てのパッドの状態を読み込む */
	pad->num_pad = 0;
	for (i = 0; i < GS_XBOX_PAD_MAX; ++i)
	{
		DWORD result;
		ZeroMemory(&pad->newbuf[i], sizeof(XINPUT_STATE)); // 初期化
		result = XInputGetState(i, &pad->newbuf[i]);
		if (result == ERROR_SUCCESS)
		{
			pad->num_pad++;
		}
	}
}

/*=============================================================================
*
* Purpose : ボタンが押されていか調べる．
*
* Return  : 押されていれば真，押されていなければ偽を返す．
*
*============================================================================*/
extern GSboolean
XBoxPadButtonState
(
	GSxboxpad*			pad,		/* XBOXパッド         　　　　      */
	GSuint				pad_no,		/* パッドナンバ                     */
	GSushort			button		/* 調べたいボタン名			　      */
)
{
	if (pad_no >= GS_XBOX_PAD_MAX)
	{
		return GS_FALSE;
	}
	if ((pad->newbuf[pad_no].Gamepad.wButtons & button) != 0) {
		return GS_TRUE;
	}
	return	GS_FALSE;
}

/*=============================================================================
*
* Purpose : ボタンが押されたか調べる．
*
* Return  : 押されたら真，押されなければ偽を返す．
*
*============================================================================*/
extern GSboolean
XBoxPadButtonTrigger
(
	GSxboxpad*			pad,		/* XBOXパッド         　　　　      */
	GSuint				pad_no,		/* パッドナンバ                     */
	GSushort			button		/* 調べたいボタン名			　      */
)
{
	if (pad_no >= GS_XBOX_PAD_MAX)
	{
		return GS_FALSE;
	}
	if (((~pad->oldbuf[pad_no].Gamepad.wButtons & pad->newbuf[pad_no].Gamepad.wButtons) & button) != 0)
	{
		return GS_TRUE;
	}
	return	GS_FALSE;
}

/*=============================================================================
*
* Purpose : ボタンがが離されたか調べる．
*
* Return  : 離されたら真，離されなければ偽を返す．
*
*============================================================================*/
extern GSboolean
XBoxPadButtonDetach
(
	GSxboxpad*			pad,		/* XBOXパッド         　　　　      */
	GSuint				pad_no,		/* パッドナンバ                     */
	GSushort			button		/* 調べたいボタン名			　      */
)
{
	if (pad_no >= GS_XBOX_PAD_MAX)
	{
		return GS_FALSE;
	}
	if (((pad->oldbuf[pad_no].Gamepad.wButtons & ~pad->newbuf[pad_no].Gamepad.wButtons) & button) != 0)
	{
		return GS_TRUE;
	}
	return	GS_FALSE;
}

/*=============================================================================
*
* Purpose : 左トリガの値を取得．
*
* Return  : 左トリガの値を返す．(0.0f ～ 1.0f)
*
*============================================================================*/
extern GSfloat
XBoxPadGetLeftTrigger
(
	GSxboxpad*			pad,		/* XBOXパッド         　　　　      */
	GSuint				pad_no		/* パッドナンバ                     */
)
{
	if (pad_no >= GS_XBOX_PAD_MAX)
	{
		return 0.0f;
	}
	return (float)pad->newbuf[pad_no].Gamepad.bLeftTrigger / 255.0f;
}

/*=============================================================================
*
* Purpose : 右トリガの値を取得．
*
* Return  : 右トリガの値を返す．(0.0f ～ 1.0f正規化した値)
*
*============================================================================*/
extern GSfloat
XBoxPadGetRightTrigger
(
	GSxboxpad*			pad,		/* XBOXパッド         　　　　      */
	GSuint				pad_no		/* パッドナンバ                     */
)
{
	if (pad_no >= GS_XBOX_PAD_MAX)
	{
		return 0.0f;
	}
	return (GSfloat)pad->newbuf[pad_no].Gamepad.bRightTrigger / 255.0f;
}

/*=============================================================================
*
* Purpose : 左アナログスティックの状態を取得．
*
* Return  : 左アナログスティックの状態を返す．(-1.0f ～ 1.0fに正規化した値)
*
*============================================================================*/
extern void
XBoxPadGetLeftAxis
(
	GSxboxpad*			pad,		/* XBOXパッド         　　　　      */
	GSuint				pad_no,		/* パッドナンバ                     */
	GSvector2*			result		/* アナログスティックの状態         */
)
{
	result->x = 0.0f;
	result->y = 0.0f;
	if (pad_no >= GS_XBOX_PAD_MAX)
	{
		return;
	}
	SHORT lx = pad->newbuf[pad_no].Gamepad.sThumbLX;
	SHORT ly = pad->newbuf[pad_no].Gamepad.sThumbLY;
	lx = (abs(lx) <= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) ? 0 : lx;
	ly = (abs(ly) <= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) ? 0 : ly;
	result->x = (GSfloat)lx / 32767.0f;
	result->y = (GSfloat)ly / 32767.0f;
}

/*=============================================================================
*
* Purpose : 右アナログスティックの状態を取得．
*
* Return  : 右アナログスティックの状態を返す．(-1.0f ～ 1.0fに正規化した値)
*
*============================================================================*/
extern void
XBoxPadGetRightAxis
(
	GSxboxpad*			pad,		/* XBOXパッド         　　　　      */
	GSuint				pad_no,		/* パッドナンバ                     */
	GSvector2*			result		/* アナログスティックの状態         */
)
{
	result->x = 0.0f;
	result->y = 0.0f;
	if (pad_no >= GS_XBOX_PAD_MAX)
	{
		return;
	}
	SHORT rx = pad->newbuf[pad_no].Gamepad.sThumbRX;
	SHORT ry = pad->newbuf[pad_no].Gamepad.sThumbRY;
	rx = (abs(rx) <= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) ? 0 : rx;
	ry = (abs(ry) <= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) ? 0 : ry;
	result->x = (GSfloat)rx / 32767.0f;
	result->y = (GSfloat)ry / 32767.0f;
}

/*=============================================================================
*
* Purpose : 接続されているジョイスティック数を調べる．
*
* Return  : 接続されているジョイスティック数を返す．
*
*============================================================================*/
extern GSuint
XBoxGetPadCount
(
	GSxboxpad*			pad			/* XBOXパッド         　　　　      */
)
{
	return pad->num_pad;
}

/********** End of File ******************************************************/
