/******************************************************************************
* File Name : gsinput.c                            Ver : 1.00  Date : 98-10-22
*
* Description :
*
*       入力デバイスシステム．
*
* Author : Moriharu Ohzu.
*
******************************************************************************/
#include	"gsinput.h"
#include	"GSxboxpad.h"

/****** 変数宣言 *************************************************************/

static GINPUT	GInput = NULL;			/* 入力デバイスハンドラ        */

static GSxboxpad* s_xbox_pad = NULL;	/* XBOXコントローラー			*/

/*=============================================================================
*
* Purpose : 入力デバイスシステムの初期化．
*
* Return  : 初期化に成功すれば真，失敗すれば偽を返す．
*
*============================================================================*/
BOOL
gsInitInput
(
	HWND			hwnd			/* ウィンドウハンドラ           */
)
{

	/* 入力デバイスハンドラの生成 */
	if ( GInput == NULL )
	{
		HINSTANCE		hinst;
			
		hinst = GetModuleHandle( NULL );
		if ( ( GInput = GInputNew( hinst, hwnd ) ) == NULL )
		{
			return	FALSE;
		}
	}
	/* Xボックスコントローラの生成 */
	if (s_xbox_pad == NULL) {
		s_xbox_pad = XBoxPadNew();
	}
	return	TRUE;
}

/*=============================================================================
*
* Purpose : 入力デバイスシステムの終了処理．
*
* Return  : なし．
*
*============================================================================*/
void
gsFinishInput
(
	void
)
{
	/* 入力デバイスハンドラの消滅 */
	if ( GInput != NULL )
	{
		GInputDelete( GInput );
	}
	GInput = NULL;

	/* Xボックスコントローラの消滅 */
	if (s_xbox_pad != NULL) {
		XBoxPadDelete(s_xbox_pad);
	}
	s_xbox_pad = NULL;
}

/*=============================================================================
*
* Purpose : ウィンドウがアクティブになった時の処理．
*
* Return  : なし．
*
*============================================================================*/
void
gsActivateInput
(
	BOOL		active				/* アクティブフラグ             */
)
{
	if ( GInput == NULL )
	{
		return;
	}

	GInputActivateApp( GInput, active );
}

/*=============================================================================
*
* Purpose : 入力デバイスの状態を読み込む．
*
* Return  : なし．
*
*============================================================================*/
void
gsReadInput
(
	void
)
{
	GInputReadDevice( GInput );

	XBoxPadRead(s_xbox_pad);
}

/*=============================================================================
*
* Purpose : キーの入力状態を調べる．
*
* Return  : キーが押されていれば真，押されていなければ偽を返す．
*
*============================================================================*/
BOOL
gsGetKeyState
(
	GKEYCODE		key_code		/* キーコード                   */
)
{
	GKEYINP			key;

	if ( ( key = GInputGetKeyborad( GInput ) ) == NULL )
	{
		return	FALSE;
	}

	return	GKeyInpGetState( key, key_code );
}

/*=============================================================================
*
* Purpose : キーが押されたか調べる．
*
* Return  : キーが押されていれば真，押されていなければ偽を返す．
*
*============================================================================*/
BOOL
gsGetKeyTrigger
(
	GKEYCODE		key_code		/* キーコード                   */
)
{
	GKEYINP			key;

	if ( ( key = GInputGetKeyborad( GInput ) ) == NULL )
	{
		return	FALSE;
	}

	return	GKeyInpGetTrigger( key, key_code );
}

/*=============================================================================
*
* Purpose : キーが離されたか調べる．
*
* Return  : キーが離されれば真，離されていなければ偽を返す．
*
*============================================================================*/
BOOL
gsGetKeyDetach
(
	GKEYCODE		key_code		/* キーコード                   */
)
{
	GKEYINP			key;

	if ( ( key = GInputGetKeyborad( GInput ) ) == NULL )
	{
		return	FALSE;
	}

	return	GKeyInpGetDetach( key, key_code );
}

/*=============================================================================
*
* Purpose : キーボードの押されているキーを調べる．
*
* Return  : 押されているキーのキーコードを返す．
*           何も押されていなければ０を返す．
*
*============================================================================*/
GKEYCODE
gsGetKey
(
	void
)
{
	GKEYINP			key;

	if ( ( key = GInputGetKeyborad( GInput ) ) == NULL )
	{
		return	0;
	}

	return	GKeyInpGetKey( key );
}

/*=============================================================================
*
* Purpose : 接続されているジョイスティック数を調べる．
*
* Return  : 接続されているジョイスティック数を返す．
*
*============================================================================*/
int
gsGetJoyCount
(
	void
)
{
	GJOYINP			joy;

	if ( ( joy = GInputGetJoystick( GInput ) ) == NULL )
	{
		return	0;
	}

	return	GJoyInpGetCount( joy );
}

/*=============================================================================
*
* Purpose : ジョイスティックの入力状態を調べる．
*
* Return  : ジョイスティックの入力状態を返す．
*
*============================================================================*/
BOOL
gsGetJoyState
(
	int				joy_no,			/* ジョイスティックナンバ       */
	GJOYSTATE		state			/* 調べたいジョイステックの状態 */
)
{
	GJOYINP			joy;

	if ( ( joy = GInputGetJoystick( GInput ) ) == NULL )
	{
		return	0;
	}

	return	GJoyInpGetState( joy, joy_no, state );
}

/*=============================================================================
*
* Purpose : ジョイスティックが押されたか調べる．
*
* Return  : ジョイスティックの押された状態を返す．
*
*============================================================================*/
BOOL
gsGetJoyTrigger
(
	int				joy_no,			/* ジョイスティックナンバ       */
	GJOYSTATE		state			/* 調べたいジョイステックの状態 */
)
{
	GJOYINP			joy;

	if ( ( joy = GInputGetJoystick( GInput ) ) == NULL )
	{
		return	0;
	}

	return	GJoyInpGetTrigger( joy, joy_no, state );
}

/*=============================================================================
*
* Purpose : ジョイスティックが離されたか調べる．
*
* Return  : ジョイスティックの離された状態を返す．
*
*============================================================================*/
BOOL
gsGetJoyDetach
(
	int				joy_no,			/* ジョイスティックナンバ       */
	GJOYSTATE		state			/* 調べたいジョイステックの状態 */
)
{
	GJOYINP			joy;

	if ( ( joy = GInputGetJoystick( GInput ) ) == NULL )
	{
		return	0;
	}

	return	GJoyInpGetDetach( joy, joy_no, state );
}

/*=============================================================================
*
* Purpose : ジョイスティックの軸の状態を調べる．
*
* Return  : なし．
*
*============================================================================*/
void
gsGetJoyAxis
(
	int				joy_no,			/* ジョイスティックナンバ			  */
	int*			x,				/*-> 左ジョイステッィクのｘ軸の状態   */
	int*			y,				/*-> 左ジョイステッィクのｙ軸の状態   */
	int*			z				/*-> 左ジョイステッィクのｚ軸の状態   */
)
{
	GJOYINP			joy;
	int				tlx = 0;
	int				tly = 0;
	int				trx = 0;
	int				try = 0;

	if ( ( joy = GInputGetJoystick( GInput ) ) != NULL )
	{
		/* ジョイステックの軸の状態を読み込む */
		GJoyInpGetAxis(joy, joy_no, &tlx, &tly, &trx, &try);
	}

	if (x != NULL)
	{
		*x = tlx;
	}
	if (y != NULL)
	{
		*y = tly;
	}
	if (z != NULL)
	{
		*z = trx;
	}
}

/*=============================================================================
*
* Purpose : ジョイスティックの左軸の状態を調べる．
*
* Return  : なし．
*
*============================================================================*/
void
gsGetJoyLeftAxis
(
	int				joy_no,			/* ジョイスティックナンバ			  */
	int*			x,				/*-> ジョイステッィクの左ｘ軸の状態   */
	int*			y				/*-> ジョイステッィクの左ｙ軸の状態   */
)
{
	GJOYINP			joy;
	int				tlx = 0;
	int				tly = 0;
	int				trx = 0;
	int				try = 0;

	if ((joy = GInputGetJoystick(GInput)) != NULL)
	{
		/* ジョイステックの軸の状態を読み込む */
		GJoyInpGetAxis(joy, joy_no, &tlx, &tly, &trx, &try);
	}

	if (x != NULL)
	{
		*x = tlx;
	}
	if (y != NULL)
	{
		*y = tly;
	}
}

/*=============================================================================
*
* Purpose : ジョイスティックの右軸の状態を調べる．
*
* Return  : なし．
*
*============================================================================*/
void
gsGetJoyRightAxis
(
	int				joy_no,			/* ジョイスティックナンバ			  */
	int*			x,				/*-> 右ジョイステッィクのｘ軸の状態   */
	int*			y				/*-> 右ジョイステッィクのｙ軸の状態   */
)
{
	GJOYINP			joy;
	int				tlx = 0;
	int				tly = 0;
	int				trx = 0;
	int				try = 0;

	if ((joy = GInputGetJoystick(GInput)) != NULL)
	{
		/* ジョイステックの軸の状態を読み込む */
		GJoyInpGetAxis(joy, joy_no, &tlx, &tly, &trx, &try);
	}

	if (x != NULL)
	{
		*x = trx;
	}
	if (y != NULL)
	{
		*y = try;
	}
}

/*=============================================================================
*
* Purpose : マウスの移動量を取得．
*
* Return  : なし．
*
*============================================================================*/
void
gsGetMouseVelocity
(
	int*			x,				/*-> ｘ方向移動量               */
	int*			y,				/*-> ｙ方向移動量               */
	int*			z				/*-> ｚ方向移動量               */
)
{
	GMOUSE			mou;

	if ( ( mou = GInputGetMouse( GInput ) ) == NULL )
	{
		*x = 0;
		*y = 0;
		*z = 0;

		return;
	}

	GMouseGetVelocity(mou, x, y, z);
}

/*=============================================================================
*
* Purpose : マウスカーソルの位置を取得．
*
* Return  : なし．
*
*============================================================================*/
void
gsGetMouseCursorPosition
(
	int*			x,				/*-> ｘ座標                     */
	int*			y				/*-> ｙ座標                     */
)
{
	GMOUSE			mou;

	if ( ( mou = GInputGetMouse( GInput ) ) == NULL )
	{
		*x = 0;
		*y = 0;

		return;
	}

	GMouseGetCursorPosition( mou, x, y );
}

/*=============================================================================
*
* Purpose : マウスのボタンの押下状態を取得．
*
* Return  : マウスのボタンの押下状態を返す．
*
*============================================================================*/
BOOL
gsGetMouseButtonState
(
	GMOUSTATE		state			/* 調べたいマウスのボタン       */
)
{
	GMOUSE			mou;

	if ( ( mou = GInputGetMouse( GInput ) ) == NULL )
	{
		return	0;
	}

	return	GMouseGetButtonState( mou, state );
}

/*=============================================================================
*
* Purpose : マウスのボタンが押されたか調べる．
*
* Return  : マウスのボタンの押された状態を返す．
*
*============================================================================*/
BOOL
gsGetMouseButtonTrigger
(
	GMOUSTATE		state			/* 調べたいマウスのボタン       */
)
{
	GMOUSE			mou;

	if ( ( mou = GInputGetMouse( GInput ) ) == NULL )
	{
		return	0;
	}

	return	GMouseGetButtonTrigger( mou, state );
}

/*=============================================================================
*
* Purpose : マウスのボタンが離されたか調べる．
*
* Return  : マウスのボタンの離された状態を返す．
*
*============================================================================*/
BOOL
gsGetMouseButtonDetach
(
	GMOUSTATE		state			/* 調べたいマウスのボタン       */
)
{
	GMOUSE			mou;

	if ( ( mou = GInputGetMouse( GInput ) ) == NULL )
	{
		return	0;
	}

	return	GMouseGetButtonDetach( mou, state );
}

/*=============================================================================
*
* Purpose : マウスカーソルの位置を設定．
*
* Return  : なし．
*
*============================================================================*/
void
gsSetMouseCursorPosition
(
	int				x,				/* ｘ座標                       */
	int				y				/* ｙ座標                       */
)
{
	GMOUSE			mou;

	if ( ( mou = GInputGetMouse( GInput ) ) == NULL )
	{
		return;
	}

	GMouseSetCursorPosition( mou, x, y );
}

/*=============================================================================
*
* Purpose : マウスカーソルを表示．
*
* Return  : なし．
*
*============================================================================*/
void
gsShowMouseCursor
(
	void
)
{
	// マウスカーソルの非表示
	ShowCursor(FALSE);
}

/*=============================================================================
*
* Purpose : マウスカーソルを非表示．
*
* Return  : なし．
*
*============================================================================*/
void
gsHideMouseCursor
(
	void
)
{
	// マウスカーソルの非表示
	ShowCursor(FALSE);
}

/*=============================================================================
*
* Purpose : XBOXコントローラのボタンが押されているか調べる．
*
* Return  : 押されていれば真，押されていなければ偽を返す．
*
*============================================================================*/
extern GSboolean
gsXBoxPadButtonState
(
	GSuint				pad_no,		/* パッドナンバ                     */
	GSushort			button		/* 調べたいボタン名			　      */
)
{
	return XBoxPadButtonState(s_xbox_pad, pad_no, button);
}

/*=============================================================================
*
* Purpose : XBOXコントローラのボタンが押されたか調べる．
*
* Return  : 押されたら真，押されなければ偽を返す．
*
*============================================================================*/
extern GSboolean
gsXBoxPadButtonTrigger
(
	GSuint				pad_no,		/* パッドナンバ                     */
	GSushort			button		/* 調べたいボタン名			　      */
)
{
	return XBoxPadButtonTrigger(s_xbox_pad, pad_no, button);
}

/*=============================================================================
*
* Purpose : XBOXコントローラのボタンがが離されたか調べる．
*
* Return  : 離されたら真，離されなければ偽を返す．
*
*============================================================================*/
extern GSboolean
gsXBoxPadButtonDetach
(
	GSuint				pad_no,		/* パッドナンバ                     */
	GSushort			button		/* 調べたいボタン名			　      */
)
{
	return XBoxPadButtonDetach(s_xbox_pad, pad_no, button);
}

/*=============================================================================
*
* Purpose : XBOXコントローラの左トリガの値を取得．
*
* Return  : 左トリガの値を返す．(0.0f ～ 1.0f)
*
*============================================================================*/
extern GSfloat
gsXBoxPadGetLeftTrigger
(
	GSuint				pad_no		/* パッドナンバ                     */
)
{
	return XBoxPadGetLeftTrigger(s_xbox_pad, pad_no);
}

/*=============================================================================
*
* Purpose : XBOXコントローラの右トリガの値を取得．
*
* Return  : 右トリガの値を返す．(0.0f ～ 1.0f正規化した値)
*
*============================================================================*/
extern GSfloat
gsXBoxPadGetRightTrigger
(
	GSuint				pad_no		/* パッドナンバ                     */
)
{
	return XBoxPadGetRightTrigger(s_xbox_pad, pad_no);
}

/*=============================================================================
*
* Purpose : XBOXコントローラの左アナログスティックの状態を取得．
*
* Return  : 左アナログスティックの状態を返す．(-1.0f ～ 1.0fに正規化した値)
*
*============================================================================*/
extern void
gsXBoxPadGetLeftAxis
(
	GSuint				pad_no,		/* パッドナンバ                     */
	GSvector2*			result		/* アナログスティックの状態			*/
)
{
	XBoxPadGetLeftAxis(s_xbox_pad, pad_no, result);
}

/*=============================================================================
*
* Purpose : XBOXコントローラの右アナログスティックの状態を取得．
*
* Return  : 右アナログスティックの状態を返す．(-1.0f ～ 1.0fに正規化した値)
*
*============================================================================*/
extern void
gsXBoxPadGetRightAxis
(
	GSuint				pad_no,		/* パッドナンバ                     */
	GSvector2*			result		/* アナログスティックの状態			*/
)
{
	XBoxPadGetRightAxis(s_xbox_pad, pad_no, result);
}

/*=============================================================================
*
* Purpose : XBOXコントローラの接続数を調べる．
*
* Return  : 接続されているXBOXコントローラ数を返す．
*
*============================================================================*/
extern GSuint
gsXBoxGetPadCount
(
	void
)
{
	return XBoxGetPadCount(s_xbox_pad);
}

/********** End of File ******************************************************/
