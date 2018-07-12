/******************************************************************************
* File Name : gmouse.c                             Ver : 1.00  Date : 98-10-22
*
* Description :
*
*       ゲームシステム用 マウス入力.
*
* Author : Moriharu Ohzu.
*
******************************************************************************/
#include	"gmouse.h"
#include	"heap.h"
#pragma		warning(disable:4201) // 無名構造体の使用
#define		DIRECTINPUT_VERSION		0x0800
#include	<dinput.h>

/****** データ型宣言 *********************************************************/

/*----- マウス入力構造体 ----------------------------------------------------*/

typedef	struct
{
	HINSTANCE            hinst;		/* インスタンスハンドラ         */
	HWND                 hwnd;		/* ウィンドウハンドラ           */
	LPDIRECTINPUT8       dinp;		/* DirectInputハンドラ          */
	LPDIRECTINPUTDEVICE8 idev;		/* マウスデバイス 　　          */
	int                  vx;		/* マウスのｘ方向移動量       　*/
	int                  vy;		/* マウスのｙ方向移動量       　*/
	int                  vz;		/* マウスのｚ方向移動量       　*/
	int                  px;		/* マウスカーソルｘ座標         */
	int                  py;		/* マウスカーソルｙ座標         */
	int		             pz;		/* マウスカーソルｚ座標         */
	GMOUSTATE            newbtn;	/* 現在のマウスのボタンの状態   */
	GMOUSTATE            oldbtn;	/* 前回のマウスのボタンの状態   */

} GINPMOUSE;

/****** 関数プロトタイプ宣言 *************************************************/

static BOOL	init_mouse_input( GINPMOUSE*, HINSTANCE, HWND );
static void	cleanup_mouse_input( GINPMOUSE* );
static void	read_mouse_input( GINPMOUSE* );
static BOOL	reacquire_mouse_input( GINPMOUSE* );
static BOOL	reacquire_input( LPDIRECTINPUTDEVICE8 );

/*=============================================================================
*
* Purpose : マウスの生成．
*
* Return  : マウスハンドラを返す．
*
*============================================================================*/
GMOUSE
GMouseNew
(
	HINSTANCE		hinst,			/* インスタンスハンドラ     */
	HWND			hwnd			/* ウィンドウハンドラ       */
)
{
	GINPMOUSE*		mouse;

	mouse = new_object( sizeof( GINPMOUSE ) );

	if ( init_mouse_input( mouse, hinst, hwnd ) == FALSE )
	{
		del_object( mouse );

		return	NULL;
	}

	return	(GMOUSE)mouse;
}

/*=============================================================================
*
* Purpose : マウスの消滅．
*
* Return  : なし．
*
*============================================================================*/
void
GMouseDelete
(
	GMOUSE			hnd				/* マウスハンドラ           */
)
{
	GINPMOUSE*		mouse = (GINPMOUSE*)hnd;

	cleanup_mouse_input( mouse );

	del_object( mouse );
}

/*=============================================================================
*
* Purpose : マウスの読み込み．
*
* Return  : なし．
*
*============================================================================*/
void
GMouseReadDevice
(
	GMOUSE			hnd				/* マウスハンドラ           */
)
{
	GINPMOUSE*		mouse = (GINPMOUSE*)hnd;
	POINT			position;

	/* マウスの状態の読み込み */
	read_mouse_input( mouse );

	/* ウィンドウ上のマウスカーソルの位置を取得 */
	GetCursorPos(&position);
	ScreenToClient(mouse->hwnd, &position);
	mouse->px = position.x;
	mouse->py = position.y;
	mouse->pz += mouse->vz;
}

/*=============================================================================
*
* Purpose : マウスの移動量を取得．
*
* Return  : なし．
*
*============================================================================*/
void
GMouseGetVelocity
(
	GMOUSE			hnd,			/* マウスハンドラ           */
	int*			x,				/*-> ｘ方向移動量           */
	int*			y,				/*-> ｙ方向移動量           */
	int*			z				/*-> ｚ方向移動量           */
)
{
	GINPMOUSE*		mouse = (GINPMOUSE*)hnd;

	if ( x != NULL )
	{
		*x = mouse->vx;
	}
	if ( y != NULL )
	{
		*y = mouse->vy;
	}
	if ( z != NULL )
	{
		*z = mouse->vz;
	}
}

/*=============================================================================
*
* Purpose : マウスカーソルの位置を取得．
*
* Return  : なし．
*
*============================================================================*/
void
GMouseGetCursorPosition
(
	GMOUSE			hnd,			/* マウスハンドラ           */
	int*			x,				/*-> ｘ座標                 */
	int*			y				/*-> ｙ座標                 */
)
{
	GINPMOUSE*		mouse = (GINPMOUSE*)hnd;

	if ( x != NULL )
	{
		*x = (int)mouse->px;
	}
	if ( y != NULL )
	{
		*y = (int)mouse->py;
	}
}

/*=============================================================================
*
* Purpose : マウスのボタンの押下状態を取得．
*
* Return  : 押されていれば真，押されていければ偽を返す．
*
*============================================================================*/
BOOL
GMouseGetButtonState
(
	GMOUSE			hnd,			/* マウスハンドラ           */
	GMOUSTATE		state			/* 調べたいマウスのボタン   */
)
{
	GINPMOUSE*		mouse = (GINPMOUSE*)hnd;

	if ( ( mouse->newbtn & state ) != 0 )
	{
		return	TRUE;
	}

	return	FALSE;
}

/*=============================================================================
*
* Purpose : マウスのボタンが押されたか調べる．
*
* Return  : 押されたら真，押されなければ偽を返す．
*
*============================================================================*/
BOOL
GMouseGetButtonTrigger
(
	GMOUSE			hnd,			/* マウスハンドラ           */
	GMOUSTATE		state			/* 調べたいマウスのボタン   */
)
{
	GINPMOUSE*		mouse = (GINPMOUSE*)hnd;

	if ( ( ( ~mouse->oldbtn & mouse->newbtn ) & state ) != 0 )
	{
		return	TRUE;
	}

	return	FALSE;
}

/*=============================================================================
*
* Purpose : マウスのボタンが離されたか調べる．
*
* Return  : 離されたら真，離されなければ偽を返す．
*
*============================================================================*/
BOOL
GMouseGetButtonDetach
(
	GMOUSE			hnd,			/* マウスハンドラ           */
	GMOUSTATE		state			/* 調べたいマウスのボタン   */
)
{
	GINPMOUSE*		mouse = (GINPMOUSE*)hnd;

	if ( ( ( mouse->oldbtn & ~mouse->newbtn ) & state ) != 0 )
	{
		return	TRUE;
	}

	return	FALSE;
}

/*=============================================================================
*
* Purpose : マウスカーソルの位置を設定．
*
* Return  : なし．
*
*============================================================================*/
void
GMouseSetCursorPosition
(
	GMOUSE			hnd,			/* マウスハンドラ           */
	int				x,				/* ｘ座標                   */
	int				y				/* ｙ座標                   */
)
{
	GINPMOUSE*		mouse = (GINPMOUSE*)hnd;
	POINT			point;
	/* マウスカーソルの位置設定 */
	point.x = x;
	point.y = y;
	ClientToScreen(mouse->hwnd, &point);
	SetCursorPos(point.x, point.y);
	mouse->px = x;
	mouse->py = y;
}

/*=============================================================================
*
* Purpose : マウスのアクセス権の再取得．
*
* Return  : 取得できれば真，取得できなければ偽を返す．
*
*============================================================================*/
BOOL
GMouseReacquire
(
	GMOUSE				hnd			/* マウスハンドラ           */
)
{
	GINPMOUSE*		mouse = (GINPMOUSE*)hnd;

	return	reacquire_mouse_input( mouse );
}

/*-----------------------------------------------------------------------------
*
* Purpose : DirectInputマウスデバイスの初期化．
*
* Return  : 初期化に成功すれば真，失敗すれば偽を返す．
*
*----------------------------------------------------------------------------*/
static BOOL
init_mouse_input
(
	GINPMOUSE*			mouse,		/* マウスハンドラ           */
	HINSTANCE			hinst,		/* インスタンスハンドラ     */
	HWND				hwnd		/* ウィンドウハンドラ       */
)
{
	LPDIRECTINPUT8 			pdi;
	LPDIRECTINPUTDEVICE8	pdev;
	DIDEVICEINSTANCE		di;

	/* マウスデバイスの初期化 */
	mouse->hwnd = hwnd;
	mouse->dinp = NULL;
	mouse->idev = NULL;

	/* DirectInputの生成 */
	if( DirectInput8Create( hinst, DIRECTINPUT_VERSION, &IID_IDirectInput8, &pdi, NULL) != DI_OK )
	{
		OutputDebugString( "DirectInputCreate FAILED\n" );

		return FALSE;
	}

	/* DirectInputマウスデバイスの生成 */
	if( pdi->lpVtbl->CreateDevice( pdi, &GUID_SysMouse, &pdev, NULL) != DI_OK)
	{
		OutputDebugString( "CreateDevice FAILED\n" );

		return	FALSE;
	}

	/* マウスデータフォーマットの設定 */
	if( pdev->lpVtbl->SetDataFormat( pdev, &c_dfDIMouse ) != DI_OK )
	{
		OutputDebugString( "SetDataFormat FAILED\n" );

		pdev->lpVtbl->Release( pdev );

		return	FALSE;
	}

	/* 協調レベルの設定 */
	if( pdev->lpVtbl->SetCooperativeLevel( pdev, mouse->hwnd,
							 DISCL_NONEXCLUSIVE | DISCL_FOREGROUND ) != DI_OK )
	{
		OutputDebugString( "SetCooperativeLevel FAILED\n" );

		pdev->lpVtbl->Release( pdev );

		return	FALSE;
	}

	/* 識別情報の取得 */
	di.dwSize = sizeof( di );
	if ( pdev->lpVtbl->GetDeviceInfo( pdev, &di ) != DI_OK )
	{
		OutputDebugString( "GetDeviceInfo FAILED\n" );

		pdev->lpVtbl->Release( pdev );

		return	FALSE;
	}

	/* アクセス権の取得を行う */
	reacquire_input( pdev );

	/* DirectInputハンドラの保存 */
	mouse->dinp = pdi;
	mouse->idev = pdev;

	/* マウスの状態の初期化 */
	mouse->vx          = 0;
	mouse->vy          = 0;
	mouse->vz          = 0;
	mouse->px          = 0;
	mouse->py          = 0;
	mouse->pz          = 0;
	mouse->newbtn      = 0;
	mouse->oldbtn      = 0;

	return	TRUE;
}

/*-----------------------------------------------------------------------------
*
* Purpose : DirectInputマウスデバイスの終了処理．
*
* Return  : なし．
*
*----------------------------------------------------------------------------*/
static void
cleanup_mouse_input
(
	GINPMOUSE*		mouse			/* マウスハンドラ           */
)
{
	/* DirectInputマウスデバイスの消滅 */
	if ( mouse->idev != NULL )
	{
		mouse->idev->lpVtbl->Unacquire( mouse->idev );
		mouse->idev->lpVtbl->Release( mouse->idev );
		mouse->idev = NULL;
	}

	/* DirectInputハンドラの消滅 */
	if ( mouse->dinp != NULL )
	{
		mouse->dinp->lpVtbl->Release( mouse->dinp );
		mouse->dinp = NULL;
	}
}

/*-----------------------------------------------------------------------------
*
* Purpose : DirectInputマウスデータの読み込み．
*
* Return  : なし．
*
*----------------------------------------------------------------------------*/
static void
read_mouse_input
(
	GINPMOUSE*		mouse			/* マウスハンドラ           */
)
{
	LPDIRECTINPUTDEVICE8	idev;
	DIMOUSESTATE			im;
	HRESULT					hres;

	if ( ( idev = mouse->idev ) == NULL )
	{
		return;
	}

	/* 前回の状態を保存   */
	mouse->oldbtn = mouse->newbtn;

	hres = idev->lpVtbl->GetDeviceState( idev, sizeof( im ), &im );
	if( hres == DI_OK )
	{
		/* 移動量の取得 */
		mouse->vx = im.lX;
		mouse->vy = im.lY;
		mouse->vz = im.lZ;

		/* ボタンの状態の取得 */
		mouse->newbtn = 0;
		if ( ( im.rgbButtons[0] & 0x80 ) != 0 )
		{
			mouse->newbtn |= GMOUSE_BUTTON_1;
		}
		if ( ( im.rgbButtons[1] & 0x80 ) != 0 )
		{
			mouse->newbtn |= GMOUSE_BUTTON_2;
		}
		if ( ( im.rgbButtons[2] & 0x80 ) != 0 )
		{
			mouse->newbtn |= GMOUSE_BUTTON_3;
		}
		if ( ( im.rgbButtons[3] & 0x80 ) != 0 )
		{
			mouse->newbtn |= GMOUSE_BUTTON_4;
		}
	}
	else
	{
		/* 読み取りに失敗した場合 */
		mouse->vx     = 0;
		mouse->vy     = 0;
		mouse->vz     = 0;
		mouse->newbtn = 0;

		if ( ( hres == DIERR_INPUTLOST   )
		  || ( hres == DIERR_NOTACQUIRED ) )
		{
			reacquire_input( idev );
		}
	}
}

/*-----------------------------------------------------------------------------
*
* Purpose : DirectInputマウスデバイスのアクセス権の再取得．
*
* Return  : なし．
*
*----------------------------------------------------------------------------*/
static BOOL
reacquire_mouse_input
(
	GINPMOUSE*		mouse			/* マウスハンドラ           */
)
{
	return	reacquire_input( mouse->idev );
}

/*-----------------------------------------------------------------------------
*
* Purpose : DirectInputデバイスのアクセス権の再取得．
*
* Return  : 取得できれば真，取得できなければ偽を返す．
*
*----------------------------------------------------------------------------*/
static BOOL
reacquire_input
(
	LPDIRECTINPUTDEVICE8	idev	/* DirectInputデバイス      */
)
{
	HRESULT					hres;

	if( idev != NULL )
	{
		/* DirectInputデバイスのアクセス権の取得 */
		hres = idev->lpVtbl->Acquire( idev );

		if( SUCCEEDED( hres ) )
		{
			/* 取得に成功した */
			return	TRUE;
		}
		else
		{
			/* 取得に失敗した */
			return	FALSE;
		}
	}

	/* デバイスが存在しない */
	return	FALSE;
}

/********** End of File ******************************************************/
