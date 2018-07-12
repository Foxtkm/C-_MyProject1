/******************************************************************************
* File Name : ginput.c                             Ver : 1.00  Date : 98-10-22
*
* Description :
*
*       ゲームシステム用入力デバイス管理．
*
* Author : Moriharu Ohzu.
*
******************************************************************************/
#include	"ginput.h"
#include	"heap.h"

/****** データ型宣言 *********************************************************/

/*----- 入力デバイス構造体 --------------------------------------------------*/

typedef	struct
{
	GKEYINP				key;		/* キーボード               */
	GJOYINP				joy;		/* ジョイスティック         */
	GMOUSE				mou;		/* マウス                   */

} GINPDEVICE;

/*=============================================================================
*
* Purpose : 入力デバイスハンドラの生成．
*
* Return  : 入力デバイスハンドラを返す．
*
*============================================================================*/
GINPUT
GInputNew
(
	HINSTANCE			hinst,		/* インスタンスハンドラ     */
	HWND				hwnd		/* ウィンドウハンドラ       */
)
{
	GINPDEVICE*			inp;

	inp = new_object( sizeof( GINPDEVICE ) );

	/* キーボードの生成 */
	inp->key = GKeyInpNew( hinst, hwnd );

	/* ジョイスティックの生成 */
	inp->joy = GJoyInpNew(hinst, hwnd);

	/* マウスの生成 */
	inp->mou = GMouseNew( hinst, hwnd );

	return	(GINPUT)inp;
}

/*=============================================================================
*
* Purpose : 入力デバイスハンドラの消滅．
*
* Return  : なし．
*
*============================================================================*/
void
GInputDelete
(
	GINPUT				hnd			/* 入力デバイスハンドラ    */
)
{
	GINPDEVICE*			inp = (GINPDEVICE*)hnd;

	/* キーボードの消滅 */
	if ( inp->key != NULL )
	{
		GKeyInpDelete( inp->key );
	}

	/* ジョイスティックの消滅 */
	if ( inp->joy != NULL )
	{
		GJoyInpDelete( inp->joy );
	}

	/* マウスの消滅 */
	if ( inp->mou != NULL )
	{
		GMouseDelete( inp->mou );
	}

	del_object( inp );
}

/*=============================================================================
*
* Purpose : 入力デバイスの状態の読み込み．
*
* Return  : なし．
*
*============================================================================*/
void
GInputReadDevice
(
	GINPUT				hnd			/* 入力デバイスハンドラ    */
)
{
	GINPDEVICE*			inp = (GINPDEVICE*)hnd;

	/* キーボードの読み込みを行う */
	if ( inp->key != NULL )
	{
		GKeyInpReadDevice( inp->key );
	}

	/* ジョイスティックの読み込みを行う */
	if ( inp->joy != NULL )
	{
		GJoyInpReadDevice( inp->joy );
	}

	/* マウスの読み込みを行う */
	if ( inp->mou != NULL )
	{
		GMouseReadDevice( inp->mou );
	}
}

/*=============================================================================
*
* Purpose : キーボードハンドラの取得．
*
* Return  : キーボードハンドラを返す．
*
*============================================================================*/
GKEYINP
GInputGetKeyborad
(
	GINPUT				hnd			/* 入力デバイスハンドラ    */
)
{
	GINPDEVICE*			inp = (GINPDEVICE*)hnd;

	return	inp->key;
}

/*=============================================================================
*
* Purpose : ジョイスティックハンドラの取得．
*
* Return  : ジョイスティックハンドラを返す．
*
*============================================================================*/
GJOYINP
GInputGetJoystick
(
	GINPUT				hnd			/* 入力デバイスハンドラ    */
)
{
	GINPDEVICE*			inp = (GINPDEVICE*)hnd;

	return	inp->joy;
}

/*=============================================================================
*
* Purpose : マウスハンドラの取得．
*
* Return  : なし．
*
*============================================================================*/
GMOUSE
GInputGetMouse
(
	GINPUT				hnd			/* 入力デバイスハンドラ    */
)
{
	GINPDEVICE*			inp = (GINPDEVICE*)hnd;

	return	inp->mou;
}

/*=============================================================================
*
* Purpose : 入力デバイスがアクティブになった時の処理．
*
* Return  : なし．
*
*============================================================================*/
void
GInputActivateApp
(
	GINPUT				hnd,		/* 入力デバイスハンドラ            */
	BOOL				active		/* アクティブフラグ                */
)
{
	GINPDEVICE*			inp = (GINPDEVICE*)hnd;

	if ( active == TRUE )
	{
		if ( inp->key != NULL )
		{
			GKeyInpReacquire( inp->key );
		}

		if ( inp->joy != NULL )
		{
			GJoyInpReacquire( inp->joy );
		}

		if ( inp->mou != NULL )
		{
			GMouseReacquire( inp->mou );
		}
	}
}

/********** End of File ******************************************************/
