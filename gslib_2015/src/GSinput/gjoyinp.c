/******************************************************************************
* File Name : gjoyinp.c                            Ver : 1.00  Date : 98-10-22
*
* Description :
*
*       ゲームシステム用 ジョイスティック入力.
*
* Author : Moriharu Ohzu.
*
******************************************************************************/
#include	"gjoyinp.h"
#include	"heap.h"
#pragma		warning(disable:4201) // 無名構造体の使用
#define		DIRECTINPUT_VERSION		0x0800
#include	<dinput.h>

/****** データ型宣言 *********************************************************/

/*----- ジョイスティック入力デバイス構造体 ----------------------------------*/

typedef	struct
{
	HINSTANCE            hinst;					/* インスタンスハンドラ     */
	HWND                 hwnd;					/* ウィンドウハンドラ       */
	LPDIRECTINPUT8       dinp;					/* DirectInputハンドラ      */
	int                  n;						/* ジョイスティック数       */
	LPDIRECTINPUTDEVICE8 idev[GJOYSTICK_MAX];	/* ジョイスティックデバイス */
	GJOYSTATE            buf[2][GJOYSTICK_MAX];	/* ジョイスティックバッファ */
	GJOYSTATE*           newbuf;				/* 現在の入力バッファ       */
	GJOYSTATE*           oldbuf;				/* 前回の入力バッファ       */

} GINPJOYSTICK;

/****** 関数プロトタイプ宣言 *************************************************/

static BOOL init_joystick_input( GINPJOYSTICK*, HINSTANCE, HWND );
static int get_joystick_count( GINPJOYSTICK* );
static void cleanup_joystick_input( GINPJOYSTICK* );
static void read_joystick_input( GINPJOYSTICK* );
static GJOYSTATE get_joystick_state( GINPJOYSTICK*, int );
static GJOYSTATE get_joystick_trigger( GINPJOYSTICK*, int );
static GJOYSTATE get_joystick_detach( GINPJOYSTICK*, int );
static BOOL reacquire_joystick_input( GINPJOYSTICK* );
static BOOL FAR PASCAL init_joystick_device( LPCDIDEVICEINSTANCE, LPVOID );
static void cleanup_joystick_device( LPDIRECTINPUTDEVICE8 );
static GJOYSTATE read_joystick_device( LPDIRECTINPUTDEVICE8 );
static HRESULT set_property( LPDIRECTINPUTDEVICE8,REFGUID,DWORD,DWORD,DWORD );
static BOOL reacquire_input( LPDIRECTINPUTDEVICE8 );
static void read_joystick_axis(LPDIRECTINPUTDEVICE8, int*, int*, int*, int*);

/*=============================================================================
*
* Purpose : ジョイスティックの生成．
*
* Return  : ジョイスティックハンドラを返す．
*
*============================================================================*/
GJOYINP
GJoyInpNew
(
	HINSTANCE			hinst,		/* インスタンスハンドラ     */
	HWND				hwnd		/* ウィンドウハンドラ       */
)
{
	GINPJOYSTICK*		joy;

	joy = new_object( sizeof( GINPJOYSTICK ) );

	if ( init_joystick_input( joy, hinst, hwnd ) == FALSE )
	{
		del_object( joy );

		return	NULL;
	}

	return	(GJOYINP)joy;
}

/*=============================================================================
*
* Purpose : ジョイスティックの消滅．
*
* Return  : なし．
*
*============================================================================*/
void
GJoyInpDelete
(
	GJOYINP				hnd			/* ジョイスティックハンドラ */
)
{
	GINPJOYSTICK*		joy = (GINPJOYSTICK*)hnd;

	cleanup_joystick_input( joy );

	del_object( joy );
}

/*=============================================================================
*
* Purpose : ジョイスティックの読み込み．
*
* Return  : なし．
*
*============================================================================*/
void
GJoyInpReadDevice
(
	GJOYINP				hnd			/* ジョイスティックハンドラ */
)
{
	GINPJOYSTICK*		joy = (GINPJOYSTICK*)hnd;

	read_joystick_input( joy );
}

/*=============================================================================
*
* Purpose : ジョイスティックの入力状態を調べる．
*
* Return  : 押されていれば真，押されていなければ偽を返す．
*
*============================================================================*/
BOOL
GJoyInpGetState
(
	GJOYINP				hnd,		/* ジョイスティックハンドラ         */
	int					joy_no,		/* ジョイスティックナンバ           */
	GJOYSTATE			state		/* 調べたいジョイステックの状態     */
)
{
	GINPJOYSTICK*		joy = (GINPJOYSTICK*)hnd;

	if ( ( get_joystick_state( joy, joy_no ) & state ) != 0 )
	{
		return	TRUE;
	}

	return	FALSE;
}


/*=============================================================================
*
* Purpose : ジョイスティックが押されたか調べる．
*
* Return  : 押されたら真，押されなければ偽を返す．
*
*============================================================================*/
BOOL
GJoyInpGetTrigger
(
	GJOYINP				hnd,		/* ジョイスティックハンドラ         */
	int					joy_no,		/* ジョイスティックナンバ           */
	GJOYSTATE			state		/* 調べたいジョイステックの状態     */
)
{
	GINPJOYSTICK*		joy = (GINPJOYSTICK*)hnd;

	if ( ( get_joystick_trigger( joy, joy_no ) & state ) != 0 )
	{
		return	TRUE;
	}

	return	FALSE;
}

/*=============================================================================
*
* Purpose : ジョイスティックが離されたか調べる．
*
* Return  : 離されたら真，離されなければ偽を返す．
*
*============================================================================*/
BOOL
GJoyInpGetDetach
(
	GJOYINP				hnd,		/* ジョイスティックハンドラ         */
	int					joy_no,		/* ジョイスティックナンバ           */
	GJOYSTATE			state		/* 調べたいジョイステックの状態     */
)
{
	GINPJOYSTICK*		joy = (GINPJOYSTICK*)hnd;

	if ( ( get_joystick_detach( joy, joy_no ) & state ) != 0 )
	{
		return	TRUE;
	}

	return	FALSE;
}

/*=============================================================================
*
* Purpose : ジョイスティックの軸の状態を読み込む．
*
* Return  : なし．
*
*============================================================================*/
void
GJoyInpGetAxis
(
	GJOYINP				hnd,		/* ジョイスティックハンドラ         */
	int					joy_no,		/* ジョイスティックナンバ           */
	int*				lx,			/*-> 左ジョイステッィクのｘ軸の状態   */
	int*				ly,			/*-> 左ジョイステッィクのｙ軸の状態   */
	int*				rx,			/*-> 右ジョイステッィクのｘ軸の状態   */
	int*				ry			/*-> 右ジョイステッィクのｙ軸の状態   */
)
{
	GINPJOYSTICK*		joy = (GINPJOYSTICK*)hnd;

	/* ジョイスティックの軸の状態を読み込む */
	read_joystick_axis( joy->idev[ joy_no ], lx, ly, rx, ry );
}

/*=============================================================================
*
* Purpose : 接続されているジョイスティック数を調べる．
*
* Return  : 接続されているジョイスティック数を返す．
*
*============================================================================*/
int
GJoyInpGetCount
(
	GJOYINP				hnd			/* ジョイスティックハンドラ */
)
{
	GINPJOYSTICK*		joy = (GINPJOYSTICK*)hnd;

	return	get_joystick_count( joy );
}

/*=============================================================================
*
* Purpose : ジョイスティックのアクセス権の再取得．
*
* Return  : 取得できれば真，取得できなければ偽を返す．
*
*============================================================================*/
BOOL
GJoyInpReacquire
(
	GJOYINP				hnd			/* ジョイスティックハンドラ */
)
{
	GINPJOYSTICK*		joy = (GINPJOYSTICK*)hnd;

	return	reacquire_joystick_input( joy );
}

/*-----------------------------------------------------------------------------
*
* Purpose : DirectInputジョイスティックデバイスの初期化．
*
* Return  : なし．
*
*----------------------------------------------------------------------------*/
static BOOL
init_joystick_input
(
	GINPJOYSTICK*		joy,		/* ジョイスティックハンドラ */
	HINSTANCE			hinst,		/* インスタンスハンドラ     */
	HWND				hwnd		/* ウィンドウハンドラ       */
)
{
	LPDIRECTINPUT8 		pdi;
	joy->hwnd = hwnd;
	joy->dinp = NULL;
	joy->n    = 0;
	ZeroMemory( joy->idev, sizeof( joy->idev ) );
	ZeroMemory( joy->buf,  sizeof( joy->buf  ) );
	joy->newbuf = joy->buf[0];
	joy->oldbuf = joy->buf[1];

	if (DirectInput8Create(hinst, DIRECTINPUT_VERSION, &IID_IDirectInput8, &pdi, NULL) != DI_OK)
	{
		OutputDebugString("DirectInputCreate 8.0 FAILED");
		return	FALSE;
	}
	joy->dinp = pdi;
	pdi->lpVtbl->EnumDevices( pdi, DI8DEVCLASS_GAMECTRL,
                              init_joystick_device, joy, DIEDFL_ATTACHEDONLY );
	return	TRUE;
}

/*-----------------------------------------------------------------------------
*
* Purpose : DirectInputジョイスティックデバイスの終了処理．
*
* Return  : なし．
*
*----------------------------------------------------------------------------*/
static void
cleanup_joystick_input
(
	GINPJOYSTICK*		joy			/* ジョイスティックハンドラ */
)
{
	int				i;

	/* 全てのジョイスティックの終了処理を行う */
	for ( i = 0; i < joy->n; i++ )
	{
		cleanup_joystick_device( joy->idev[i] );
		joy->idev[i] = NULL;
	}

	/* DirectInputハンドラの消滅 */
	if ( joy->dinp != NULL )
	{
		joy->dinp->lpVtbl->Release( joy->dinp );
		joy->dinp = NULL;
	}
}

/*-----------------------------------------------------------------------------
*
* Purpose : DirectInputジョイスティックデバイスの読み込み．
*
* Return  : なし．
*
*----------------------------------------------------------------------------*/
static void
read_joystick_input
(
	GINPJOYSTICK*		joy			/* ジョイスティックハンドラ */
)
{
	int					i;
	GJOYSTATE*			tmp;

	/* 現在のバッファと古いバッファを入れ換える */
	tmp         = joy->newbuf;
	joy->newbuf = joy->oldbuf;
	joy->oldbuf = tmp;

	/* 全てのジョイスティックの読み込みを行う */
	for ( i = 0; i < joy->n; i++ )
	{
		joy->newbuf[i] = read_joystick_device( joy->idev[i] );
	}
}

/*-----------------------------------------------------------------------------
*
* Purpose : ジョイスティックの接続数を調べる．
*
* Return  : ジョイスティックの接続数を返す．
*
*----------------------------------------------------------------------------*/
static int
get_joystick_count
(
	GINPJOYSTICK*		joy			/* ジョイスティックハンドラ */
)
{
	return	joy->n;
}

/*-----------------------------------------------------------------------------
*
* Purpose : DirectInputジョイスティックの読み込み．
*
* Return  : なし．
*
*----------------------------------------------------------------------------*/
static GJOYSTATE
get_joystick_state
(
	GINPJOYSTICK*		joy,		/* ジョイスティックハンドラ */
	int					joy_no		/* ジョイスティックナンバ   */
)
{
	if ( joy_no >= joy->n )
	{
		return	0;
	}

	return	joy->newbuf[ joy_no ];
}

/*-----------------------------------------------------------------------------
*
* Purpose : DirectInputジョイスティックの読み込み． （ 立ち上がり ）
*
* Return  : なし．
*
*----------------------------------------------------------------------------*/
static GJOYSTATE
get_joystick_trigger
(
	GINPJOYSTICK*		joy,		/* ジョイスティックハンドラ */
	int					joy_no		/* ジョイスティックナンバ   */
)
{
	if ( joy_no >= joy->n )
	{
		return	0;
	}

	return	~joy->oldbuf[ joy_no ] & joy->newbuf[ joy_no ];
}

/*-----------------------------------------------------------------------------
*
* Purpose : DirectInputジョイスティックの読み込み． （ 立ち下がり ）
*
* Return  : なし．
*
*----------------------------------------------------------------------------*/
static GJOYSTATE
get_joystick_detach
(
	GINPJOYSTICK*		joy,		/* ジョイスティックハンドラ */
	int					joy_no		/* ジョイスティックナンバ   */
)
{
	if ( joy_no >= joy->n )
	{
		return	0;
	}

	return	joy->oldbuf[ joy_no ] & ~joy->newbuf[ joy_no ];
}

/*-----------------------------------------------------------------------------
*
* Purpose : DirectInputジョイスティックのアクセス権の再取得．
*
* Return  : なし．
*
*----------------------------------------------------------------------------*/
static BOOL
reacquire_joystick_input
(
	GINPJOYSTICK*		joy			/* ジョイスティックハンドラ */
)
{
	int					i;

	for ( i = 0; i < joy->n; i++ )
	{
		if ( reacquire_input( joy->idev[i] ) == FALSE )
		{
			return	FALSE;
		}
	}

	return	TRUE;
}

/*-----------------------------------------------------------------------------
*
* Purpose : DirectInputジョイスティックデバイスの生成．（ コールバック関数 ）
*
* Return  : DIENUM_CONTINUE または FALSE を返す．
*
*----------------------------------------------------------------------------*/
static BOOL FAR PASCAL
init_joystick_device
(
	LPCDIDEVICEINSTANCE		pdinst,	/* DirectInputデバイスインスタンス */
	LPVOID					hnd		/* ジョイスティックハンドラ        */
)
{
	GINPJOYSTICK*			joy = hnd;
	LPDIRECTINPUT8			pdi;
	LPDIRECTINPUTDEVICE8	pdev;
	DIPROPRANGE				diprg;

	/* DirectInputジョイステックデバイスの生成 */
	pdi = joy->dinp;
	if( pdi->lpVtbl->CreateDevice( pdi, &pdinst->guidInstance, &pdev, NULL) != DI_OK)
	{
		OutputDebugString( "CreateDevice FAILED\n" );

		return	DIENUM_CONTINUE;
	}

	/* ジョイスティックデータフォーマットの設定 */
	if( pdev->lpVtbl->SetDataFormat( pdev, &c_dfDIJoystick ) != DI_OK )
	{
		OutputDebugString( "SetDataFormat FAILED\n" );

		pdev->lpVtbl->Release( pdev );

		return	DIENUM_CONTINUE;
	}

	/* 協調レベルの設定 */
	if( pdev->lpVtbl->SetCooperativeLevel( pdev, joy->hwnd,
							 DISCL_NONEXCLUSIVE | DISCL_FOREGROUND ) != DI_OK )
	{
		OutputDebugString( "SetCooperativeLevel FAILED\n" );

		pdev->lpVtbl->Release( pdev );

		return	DIENUM_CONTINUE;
	}

	/* 左ジョイスティックのx軸の設定 */
	diprg.diph.dwSize       = sizeof( diprg );
	diprg.diph.dwHeaderSize = sizeof( diprg.diph );
	diprg.diph.dwObj        = DIJOFS_X;
	diprg.diph.dwHow        = DIPH_BYOFFSET;
	diprg.lMin              = -1000;
	diprg.lMax              = +1000;
	if( pdev->lpVtbl->SetProperty( pdev, DIPROP_RANGE, &diprg.diph ) != DI_OK )
	{
		OutputDebugString( "SetProperty(DIPH_RANGE) FAILED\n" );

		pdev->lpVtbl->Release( pdev );

		return	FALSE;
	}

	/* 左ジョイスティックのｙ軸の設定 */
	diprg.diph.dwObj        = DIJOFS_Y;
	if( pdev->lpVtbl->SetProperty( pdev, DIPROP_RANGE, &diprg.diph ) != DI_OK )
	{
		OutputDebugString( "SetProperty(DIPH_RANGE) FAILED\n" );

		pdev->lpVtbl->Release( pdev );

		return	FALSE;
	}
	/* 左ｘ軸の無効ゾーンの設定 */
	if (set_property(pdev, DIPROP_DEADZONE, DIJOFS_X, DIPH_BYOFFSET, 5000) != DI_OK)
	{
		OutputDebugString("SetProperty(DIPH_DEADZONE) FAILED\n");

		pdev->lpVtbl->Release(pdev);

		return	FALSE;
	}

	/* 左ｙ軸の無効ゾーンの設定 */
	if (set_property(pdev, DIPROP_DEADZONE, DIJOFS_Y, DIPH_BYOFFSET, 5000) != DI_OK)
	{
		OutputDebugString("SetProperty(DIPH_DEADZONE) FAILED\n");

		pdev->lpVtbl->Release(pdev);

		return	FALSE;
	}

	/* 右ジョイスティックのx軸の設定 */
	diprg.diph.dwObj = DIJOFS_Z;
	pdev->lpVtbl->SetProperty(pdev, DIPROP_RANGE, &diprg.diph);

	/* 右ジョイスティックのｙ軸の設定 */
	diprg.diph.dwObj = DIJOFS_RZ;
	pdev->lpVtbl->SetProperty(pdev, DIPROP_RANGE, &diprg.diph);

	/* 右ｘ軸の無効ゾーンの設定 */
	set_property(pdev, DIPROP_DEADZONE, DIJOFS_Z, DIPH_BYOFFSET, 5000);

	/* 右ｙ軸の無効ゾーンの設定 */
	set_property(pdev, DIPROP_DEADZONE, DIJOFS_RZ, DIPH_BYOFFSET, 5000);

	/* アクセス権の取得を行う */
	reacquire_input( pdev );

	/* ジョイスティックの追加登録 */
	if ( joy->n < GJOYSTICK_MAX )
	{
		joy->idev[ joy->n ] = pdev;
		joy->n++;
	}
	else
	{
		/* 必要以上のジョイスティックは使えない */
		pdev->lpVtbl->Release( pdev );
	}

	return	DIENUM_CONTINUE;
}

/*-----------------------------------------------------------------------------
*
* Purpose : DirectInputジョイスティックデバイスの終了処理．
*
* Return  : なし．
*
*----------------------------------------------------------------------------*/
static void
cleanup_joystick_device
(
	LPDIRECTINPUTDEVICE8	joy		/* DirectInputジョイスティックデバイス  */
)
{
	if ( joy != NULL )
	{
		joy->lpVtbl->Unacquire( joy );

		joy->lpVtbl->Release( joy );
	}
}

/*-----------------------------------------------------------------------------
*
* Purpose : DirectInputジョイスティックデバイスの読み込み．
*
* Return  : ジョイスティックの入力状態を返す．
*
*----------------------------------------------------------------------------*/
static GJOYSTATE
read_joystick_device
(
	LPDIRECTINPUTDEVICE8	joy		/* DirectInputジョイスティックデバイス  */
)
{
	GJOYSTATE				joy_state;
	GJOYSTATE				bmask;
	HRESULT					hres;
	DIJOYSTATE				js;
	int						i;

	if ( joy == NULL )
	{
		return	0;
	}

	/* ジョイスティックにポーリングの指示を出す */
	joy->lpVtbl->Poll( joy );

	/* ジョイスティックデータの読み込み */
	hres = joy->lpVtbl->GetDeviceState( joy, sizeof( DIJOYSTATE ), &js );

	if ( hres != DI_OK )
	{
		if ( ( hres == DIERR_INPUTLOST   )
		  || ( hres == DIERR_NOTACQUIRED ) )
		{
			reacquire_input( joy );
		}

		return	0;
	}

	/* 方向キーのチェック */
	joy_state = 0;
	if ( js.lX < 0 ) 
	{
		/* 左が押された */
		joy_state |= GJOY_LEFT;
	}
	else if ( js.lX > 0 )
	{
		/* 右が押された */
		joy_state |= GJOY_RIGHT;
	}
	if ( js.lY < 0 )
	{
		/* 上が押された */
		joy_state |= GJOY_UP;
	}
	else if ( js.lY > 0 )
	{
		/* 下が押された */
		joy_state |= GJOY_DOWN;
	}

	/* ボタンのチェック */
	bmask = GJOY_BUTTON_1;
	for ( i = 0; i < GJOY_BUTTON_MAX; i++ )
	{
		if ( ( js.rgbButtons[i] & 0x80 ) != 0 )
		{
			joy_state |= bmask;
		}
		bmask <<= 1;
	}

	return	joy_state;
}

/*-----------------------------------------------------------------------------
*
* Purpose : ジョイスティックの軸の状態を読み込む．
*
* Return  : なし．
*
*----------------------------------------------------------------------------*/
static void
read_joystick_axis
(
	LPDIRECTINPUTDEVICE8	joy,	/* DirectInputジョイスティックデバイス  */
	int*					lx,		/*-> ジョイステッィクのｘ軸の状態       */
	int*					ly,		/*-> ジョイステッィクのｙ軸の状態       */
	int*					rx,		/*-> ジョイステッィクのｚ軸の状態       */
	int*					ry		/*-> ジョイステッィクのｚ軸の状態       */
	)
{
	HRESULT					hres;
	DIJOYSTATE				js;

	*lx = 0;
	*ly = 0;
	*rx = 0;
	*ry = 0;
	if (joy == NULL)
	{
		return;
	}

	/* ジョイスティックにポーリングの指示を出す */
	joy->lpVtbl->Poll( joy );

	/* ジョイスティックデータの読み込み */
	hres = joy->lpVtbl->GetDeviceState( joy, sizeof( DIJOYSTATE ), &js );

	if ( hres != DI_OK )
	{
		if ( ( hres == DIERR_INPUTLOST   )
		  || ( hres == DIERR_NOTACQUIRED ) )
		{
			reacquire_input( joy );
		}

		return;
	}

	/* 軸の状態を取得 */
	*lx = (int)js.lX;
	*ly = (int)js.lY;
	*rx = (int)js.lZ;
	*ry = (int)js.lRz;
}

/*-----------------------------------------------------------------------------
*
* Purpose : DirectInputデバイスのプロバティの設定．
*
* Return  : 設定後のHRESULT値を返す．
*
*----------------------------------------------------------------------------*/
static HRESULT
set_property
(
	LPDIRECTINPUTDEVICE8	pdev,	/* DirectInputデバイス          */
	REFGUID					guid,	/* 設定対象の識別子             */
	DWORD					obj,	/* 設定する項目                 */
	DWORD					how,	/* 設定値の方法                 */
	DWORD					val		/* 設定値                       */
)
{
	DIPROPDWORD				dipdw;

	dipdw.diph.dwSize       = sizeof( dipdw );
	dipdw.diph.dwHeaderSize = sizeof( dipdw.diph );
	dipdw.diph.dwObj        = obj;
	dipdw.diph.dwHow        = how;
	dipdw.dwData            = val;

	return pdev->lpVtbl->SetProperty( pdev, guid, &dipdw.diph );
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
