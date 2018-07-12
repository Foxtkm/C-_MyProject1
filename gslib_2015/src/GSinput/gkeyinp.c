/******************************************************************************
* File Name : gkeyinp.c                            Ver : 1.00  Date : 98-10-22
*
* Description :
*
*       ゲームシステム用 キーボード入力.
*
* Author : Moriharu Ohzu.
*
******************************************************************************/
#include	"gkeyinp.h"
#include	"heap.h"
#define		DIRECTINPUT_VERSION		0x0800
#pragma		warning(disable:4201) // 無名構造体の使用
#include	<dinput.h>

/****** データ型宣言 *********************************************************/

/*----- キーボード入力デバイス構造体 ----------------------------------------*/

typedef	struct
{
	HINSTANCE            hinst;				/* インスタンスハンドラ     */
	HWND                 hwnd;				/* ウィンドウハンドラ       */
	LPDIRECTINPUT8       dinp;				/* DirectInputハンドラ      */
	LPDIRECTINPUTDEVICE8 idev;				/* キーボードデバイス       */
	BYTE                 buffer[2][256];	/* キーボードバッファ       */
	BYTE*                newbuf;			/* 現在のキーボードバッファ */
	BYTE*                oldbuf;			/* 前回のキーボードバッファ */

} GINPKEYBOARD;

/****** 関数プロトタイプ宣言 *************************************************/

static BOOL	init_keyboard_input( GINPKEYBOARD*, HINSTANCE, HWND );
static void	cleanup_keyboard_input( GINPKEYBOARD* );
static void	read_keyboard_input( GINPKEYBOARD* );
static BOOL	get_keyboard_state( GINPKEYBOARD*, GKEYCODE );
static BOOL	get_keyboard_trigger( GINPKEYBOARD*, GKEYCODE );
static BOOL	get_keyboard_detach( GINPKEYBOARD*, GKEYCODE );
static GKEYCODE get_keyboard_key( GINPKEYBOARD* );
static BOOL	reacquire_keyboard_input( GINPKEYBOARD* );
static BOOL	reacquire_input( LPDIRECTINPUTDEVICE8 );

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
)
{
	GINPKEYBOARD*		key;

	key = new_object( sizeof( GINPKEYBOARD ) );

	if ( init_keyboard_input( key, hinst, hwnd ) == FALSE )
	{
		del_object( key );

		return	NULL;
	}

	return	(GKEYINP)key;
}

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
)
{
	GINPKEYBOARD*		key = (GINPKEYBOARD*)hnd;

	cleanup_keyboard_input( key );

	del_object( key );
}

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
)
{
	GINPKEYBOARD*		key = (GINPKEYBOARD*)hnd;

	read_keyboard_input( key );
}

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
)
{
	GINPKEYBOARD*		key = (GINPKEYBOARD*)hnd;

	return	get_keyboard_state( key, key_code );
}

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
)
{
	GINPKEYBOARD*		key = (GINPKEYBOARD*)hnd;

	return	get_keyboard_trigger( key, key_code );
}

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
)
{
	GINPKEYBOARD*		key = (GINPKEYBOARD*)hnd;

	return	get_keyboard_detach( key, key_code );
}

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
)
{
	GINPKEYBOARD*		key = (GINPKEYBOARD*)hnd;

	return	get_keyboard_key( key );
}

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
)
{
	GINPKEYBOARD*		key = (GINPKEYBOARD*)hnd;

	return	reacquire_keyboard_input( key );
}

/*-----------------------------------------------------------------------------
*
* Purpose : DirectInputキーボードデバイスの初期化．
*
* Return  : 初期化に成功すれば真，失敗すれば偽を返す．
*
*----------------------------------------------------------------------------*/
static BOOL
init_keyboard_input
(
	GINPKEYBOARD*		key,		/* キーボードハンドラ       */
	HINSTANCE			hinst,		/* インスタンスハンドラ     */
	HWND				hwnd		/* ウィンドウハンドラ       */
)
{
	LPDIRECTINPUT8 			pdi;
	LPDIRECTINPUTDEVICE8	pdev;
	DIDEVICEINSTANCE		di;

	/* キーボードデバイスの初期化 */
	key->hwnd = hwnd;
	key->dinp = NULL;
	key->idev = NULL;
	ZeroMemory( key->buffer, sizeof( key->buffer ) );
	key->newbuf = key->buffer[0];
	key->oldbuf = key->buffer[1];

	/* DirectInputの生成 */
	if( DirectInput8Create( hinst, DIRECTINPUT_VERSION, &IID_IDirectInput8, &pdi, NULL) != DI_OK )
	{
		OutputDebugString( "DirectInputCreate 8.0 FAILED\n" );

		return FALSE;
	}

	/* DirectInputキーボードデバイスの生成 */
	if( pdi->lpVtbl->CreateDevice( pdi, &GUID_SysKeyboard, &pdev, NULL) != DI_OK)
	{
		OutputDebugString( "CreateDevice FAILED\n" );

		return	FALSE;
	}

	/* キーボードデータフォーマットの設定 */
	if( pdev->lpVtbl->SetDataFormat( pdev, &c_dfDIKeyboard ) != DI_OK )
	{
		OutputDebugString( "SetDataFormat FAILED\n" );

		pdev->lpVtbl->Release( pdev );

		return	FALSE;
	}

	/* 協調レベルの設定 */
	if( pdev->lpVtbl->SetCooperativeLevel( pdev, key->hwnd,
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
	key->dinp = pdi;
	key->idev = pdev;

	return	TRUE;
}

/*-----------------------------------------------------------------------------
*
* Purpose : DirectInputキーボードデバイスの終了処理．
*
* Return  : なし．
*
*----------------------------------------------------------------------------*/
static void
cleanup_keyboard_input
(
	GINPKEYBOARD*		key			/* キーボードハンドラ       */
)
{
	/* DirectInputキーボードデバイスの消滅 */
	if ( key->idev != NULL )
	{
		key->idev->lpVtbl->Unacquire( key->idev );
		key->idev->lpVtbl->Release( key->idev );
		key->idev = NULL;
	}

	/* DirectInputハンドラの消滅 */
	if ( key->dinp != NULL )
	{
		key->dinp->lpVtbl->Release( key->dinp );
		key->dinp = NULL;
	}
}

/*-----------------------------------------------------------------------------
*
* Purpose : DirectInputキーボードデータの読み込み．
*
* Return  : なし．
*
*----------------------------------------------------------------------------*/
static void
read_keyboard_input
(
	GINPKEYBOARD*		key			/* キーボードハンドラ       */
)
{
	LPDIRECTINPUTDEVICE8	idev;
	HRESULT					hres;
	BYTE*					tmp;

	if ( ( idev = key->idev ) == NULL )
	{
		return;
	}

	/* 新しいバッファと古いバッファを入れ替える */
	tmp         = key->newbuf;
	key->newbuf = key->oldbuf;
	key->oldbuf = tmp;

	/* キーボードの状態を読み込む */
	hres = idev->lpVtbl->GetDeviceState( idev, sizeof( BYTE ) * 256,
														 (LPVOID)key->newbuf );
	if( hres != DI_OK )
	{
		/* キーボードバッファの初期化 */
		ZeroMemory( key->newbuf, sizeof( BYTE ) * 256 );

		if ( ( hres == DIERR_INPUTLOST   )
		  || ( hres == DIERR_NOTACQUIRED ) )
		{
			reacquire_input( idev );
		}
	}
}

/*-----------------------------------------------------------------------------
*
* Purpose : DirectInputキーボードの状態を取得．
*
* Return  : 押されていれば真，押されていなければ偽を返す．
*
*----------------------------------------------------------------------------*/
static BOOL
get_keyboard_state
(
	GINPKEYBOARD*		key,		/* キーボードハンドラ       */
	GKEYCODE			key_code	/* キーコード               */
)
{
	if ( ( key->newbuf[ key_code ] & 0x80 ) != 0 )
	{
		return	TRUE;
	}

	return	FALSE;
}

/*-----------------------------------------------------------------------------
*
* Purpose : DirectInputキーボードが押されたか調べる．
*
* Return  : 押されていれば真，押されていなければ偽を返す．
*
*----------------------------------------------------------------------------*/
static BOOL
get_keyboard_trigger
(
	GINPKEYBOARD*		key,		/* キーボードハンドラ       */
	GKEYCODE			key_code	/* キーコード               */
)
{
	if ( ( ~key->oldbuf[ key_code ] & key->newbuf[ key_code ] & 0x80 ) != 0 )
	{
		return	TRUE;
	}

	return	FALSE;
}

/*-----------------------------------------------------------------------------
*
* Purpose : DirectInputキーボードが離されたか調べる．
*
* Return  : 離されていれば真，離されていなければ偽を返す．
*
*----------------------------------------------------------------------------*/
static BOOL
get_keyboard_detach
(
	GINPKEYBOARD*		key,		/* キーボードハンドラ       */
	GKEYCODE			key_code	/* キーコード               */
)
{
	if ( ( key->oldbuf[ key_code ] & ~key->newbuf[ key_code ] & 0x80 ) != 0 )
	{
		return	TRUE;
	}

	return	FALSE;
}

/*-----------------------------------------------------------------------------
*
* Purpose : DirectInputキーボードの押されているキーの取得．
*
* Return  : 押されているキーのキーコードを返す．
*
*----------------------------------------------------------------------------*/
static GKEYCODE
get_keyboard_key
(
	GINPKEYBOARD*		key		/* キーボードハンドラ       */
)
{
	int					i;

	for ( i = 0; i < 256; i++ )
	{
		if ( ( key->newbuf[ i ] & 0x80 ) != 0 )
		{
			return	(GKEYCODE)i;
		}
	}

	/* 何も押されていなければ０を返す */
	return	0;
}

/*-----------------------------------------------------------------------------
*
* Purpose : DirectInputキーボードデバイスのアクセス権の再取得．
*
* Return  : なし．
*
*----------------------------------------------------------------------------*/
static BOOL
reacquire_keyboard_input
(
	GINPKEYBOARD*		key			/* キーボードハンドラ       */
)
{
	return	reacquire_input( key->idev );
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
