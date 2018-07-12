/******************************************************************************
* File Name : gsound.c                             Ver : 1.00  Date : 98-10-22
*
* Description :
*
*       ゲームシステム用 サウンドデータ管理．
*
* Author : Moriharu Ohzu.
*
******************************************************************************/
#include	"gsound.h"
#include	"heap.h"
#include	"lklist.h"

/****** データ型宣言 *********************************************************/

/*----- サウンドデータ管理構造体 --------------------------------------------*/

typedef	struct
{
	HWND			hwnd;			/* ウィンドウハンドラ           */
	LPDIRECTSOUND 	lpDS;			/* DirectSoundハンドラ          */
	LKLIST			mlst;			/* ＭＩＤＩデータリスト         */
	LKLIST			wlst;			/* ＷＡＶＥデータリスト         */
	GMIDI			mstop;			/* 停止中のＭＩＤＩデータ       */

} SOUNDENV;

/****** 関数プロトタイプ宣言 *************************************************/

static BOOL	is_midi_play( const void*, const void* );
static BOOL	is_midi_pause( const void*, const void* );

/*=============================================================================
*
* Purpose : サウンドハンドラの生成．
*
* Return  : サウンドハンドラを返す．
*
*============================================================================*/
GSOUND
GSoundNew
(
	HWND			hwnd			/* ウィンドウハンドラ           */
)
{
	SOUNDENV*		snd;
	LPDIRECTSOUND 	lpDS;
	HRESULT			dsval;

	CoInitialize(NULL);

	/* DirectSoundの作成 */
	dsval = DirectSoundCreate( NULL, &lpDS, NULL );
	if ( dsval != DS_OK )
	{
		OutputDebugString( "DirectSoundCreate FAILED" );

		return	NULL;
	}

	/* 強調レベルの設定 */
	dsval = lpDS->lpVtbl->SetCooperativeLevel( lpDS, hwnd, DSSCL_NORMAL );
	if ( dsval != DS_OK )
	{
		lpDS->lpVtbl->Release( lpDS );

		OutputDebugString( "SetCooperativeLevel FAILED" );

		return	NULL;
	}

	snd = new_object( sizeof( SOUNDENV ) );

	snd->hwnd  = hwnd;
	snd->lpDS  = lpDS;
	snd->mlst  = LkListNew();
	snd->wlst  = LkListNew();
	snd->mstop = NULL;

	return	(GSOUND)snd;
}

/*=============================================================================
*
* Purpose : サウンドハンドラの消滅．
*
* Return  : なし．
*
*============================================================================*/
void
GSoundDelete
(
	GSOUND			hnd				/* サウンドハンドラ             */
)
{
	SOUNDENV*		snd = (SOUNDENV*)hnd;
	LPDIRECTSOUND 	lpDS;

	/* 全てのサウンドデータを消滅 */
	GSoundCleanUp( hnd );

	/* ウェーブデータリストの消滅 */
	LkListDelete( snd->wlst );

	/* ＭＩＤＩデータリストの消滅 */
	LkListDelete( snd->mlst );

	/* DirectSoundの消滅 */
	if ( ( lpDS = snd->lpDS ) != NULL )
	{
		lpDS->lpVtbl->Release( lpDS );
	}

	del_object( snd );

	CoUninitialize();
}

/*=============================================================================
*
* Purpose : 全てのサウンドの停止．
*
* Return  : なし．
*
*============================================================================*/
void
GSoundStop
(
	GSOUND			hnd				/* サウンドハンドラ             */
)
{
	/* ＷＡＶＥサウンドの停止 */
	GSoundStopWave( hnd );

	/* ＭＩＤＩサウンドの停止 */
	GSoundStopMidi( hnd );
}

/*=============================================================================
*
* Purpose : 全てのＷＡＶＥサウンドの停止．
*
* Return  : なし．
*
*============================================================================*/
void
GSoundStopWave
(
	GSOUND			hnd				/* サウンドハンドラ             */
)
{
	SOUNDENV*		snd = (SOUNDENV*)hnd;
	GWAVE			wave;
	LKITER			iter;

	iter = LkListIterNew( snd->wlst );
	while ( ( wave = LkListIterGetNext( iter ) ) != NULL )
	{
		GWaveStop( wave );
	}
	LkListIterDelete( iter );
}

/*=============================================================================
*
* Purpose : 全てのＭＩＤＩサウンドの停止．
*
* Return  : なし．
*
*============================================================================*/
void
GSoundStopMidi
(
	GSOUND			hnd				/* サウンドハンドラ             */
)
{
	SOUNDENV*		snd = (SOUNDENV*)hnd;
	GMIDI			midi;
	LKITER			iter;

	iter = LkListIterNew( snd->mlst );
	while ( ( midi = LkListIterGetNext( iter ) ) != NULL )
	{
		GMidiStop( midi );
	}
	LkListIterDelete( iter );

	snd->mstop = NULL;
}

/*=============================================================================
*
* Purpose : 全てのサウンドデータの消滅．
*
* Return  : なし．
*
*============================================================================*/
void
GSoundCleanUp
(
	GSOUND			hnd				/* サウンドハンドラ             */
)
{
	/* 全てのウェーブデータを消滅 */
	GSoundCleanUpWave( hnd );

	/* 全てのＭＩＤＩデータを消滅 */
	GSoundCleanUpMidi( hnd );
}

/*=============================================================================
*
* Purpose : 全てのウェーブデータを消滅．
*
* Return  : なし．
*
*============================================================================*/
void
GSoundCleanUpWave
(
	GSOUND			hnd				/* サウンドハンドラ             */
)
{
	SOUNDENV*		snd = (SOUNDENV*)hnd;
	GWAVE			wave;

	while ( ( wave = LkListRemoveHead( snd->wlst ) ) != NULL )
	{
		GWaveDelete( wave );
	}
}

/*=============================================================================
*
* Purpose : 全てのＭＩＤＩデータを消滅．
*
* Return  : なし．
*
*============================================================================*/
void
GSoundCleanUpMidi
(
	GSOUND			hnd				/* サウンドハンドラ             */
)
{
	SOUNDENV*		snd = (SOUNDENV*)hnd;
	GMIDI			midi;

	while ( ( midi = LkListRemoveHead( snd->mlst ) ) != NULL )
	{
		GMidiDelete( midi );
	}
	snd->mstop = NULL;
}

/*=============================================================================
*
* Purpose : ＷＡＶＥファイルの読み込み．
*
* Return  : ＷＡＶＥデータハンドラを返す．
*
*============================================================================*/
GWAVE
GSoundLoadWave
(
	GSOUND			hnd,			/* サウンドハンドラ             */
	const char*		fname,			/* ＷＡＶＥファイル名           */
	int				cnt,			/* 同時再生数                   */
	GWAVEFLAG		flag			/* 再生タイプフラグ             */
)
{
	SOUNDENV*		snd = (SOUNDENV*)hnd;
	GWAVE			wave;

	/* ＷＡＶＥデータを新規読み込み */
	if ( ( wave = GWaveNew( snd->lpDS, fname, cnt, flag ) ) == NULL )
	{
		return	NULL;
	}

	/* ＷＡＶＥデータの登録 */
	LkListAddTail( snd->wlst, wave );

	return	wave;
}

/*=============================================================================
*
* Purpose : ＷＡＶＥデータの消滅．
*
* Return  : 消滅できれば真，できなければ偽を返す．
*
*============================================================================*/
BOOL
GSoundDeleteWave
(
	GSOUND			hnd,			/* サウンドハンドラ             */
	GWAVE			wave			/* ＷＡＶＥデータハンドラ       */
)
{
	SOUNDENV*		snd = (SOUNDENV*)hnd;

	/* ＷＡＶＥデータリストから削除 */
	if ( LkListRemove( snd->wlst, wave ) != NULL )
	{
		/* ＷＡＶＥデータの消滅 */
		GWaveDelete( wave );

		return	TRUE;
	}

	return	FALSE;
}

/*=============================================================================
*
* Purpose : ＭＩＤＩファイルの読み込み．
*
* Return  : ＭＩＤＩデータハンドラを返す．
*
*============================================================================*/
GMIDI
GSoundLoadMidi
(
	GSOUND			hnd,			/* サウンドハンドラ             */
	const char*		fname,			/* ＭＤＳファイル名             */
	BOOL			flag			/* ループ指定フラグ             */
)
{
	SOUNDENV*		snd = (SOUNDENV*)hnd;
	GMIDI			midi;

	/* ＭＩＤＩデータを新規読み込み */
	if ( ( midi = GMidiNew( fname, flag ) ) == NULL )
	{
		return	NULL;
	}

	/* ＭＩＤＩデータの登録 */
	LkListAddTail( snd->mlst, midi );

	return	midi;
}

/*=============================================================================
*
* Purpose : ＭＩＤＩデータの消滅．
*
* Return  : 消滅できれば真，できなければ偽を返す．
*
*============================================================================*/
BOOL
GSoundDeleteMidi
(
	GSOUND			hnd,			/* サウンドハンドラ             */
	GMIDI			midi			/* ＭＩＤＩデータハンドラ       */
)
{
	SOUNDENV*		snd = (SOUNDENV*)hnd;

	/* ＭＩＤＩデータリストから削除 */
	if ( LkListRemove( snd->mlst, midi ) != NULL )
	{
		if ( snd->mstop == midi )
		{
			snd->mstop = NULL;
		}

		/* ＭＩＤＩデータの消滅 */
		GMidiDelete( midi );

		return	TRUE;
	}

	return	FALSE;
}

/*=============================================================================
*
* Purpose : サウンドがアクティブになった時の処理．
*
* Return  : なし．
*
*============================================================================*/
void
GSoundActivateApp
(
	GSOUND			hnd,			/* サウンドハンドラ             */
	BOOL			active			/* アクティブフラグ             */
)
{
	SOUNDENV*		snd = (SOUNDENV*)hnd;
	GMIDI			midi;

	if ( active == TRUE )
	{
		if ( snd->mstop == NULL )
		{
			return;
		}
		/* 再生中のＭＩＤＩサウンドを検索 */
		midi = LkListSearch( snd->mlst, NULL, is_midi_pause );
		if ( midi == snd->mstop )
		{
			/* 停止したサウンドの再開 */
			GMidiRestart( midi );
		}
		snd->mstop = NULL;
	}
	else
	{
		/* 再生中のＷＡＶＥサウンドの停止 */
		GSoundStopWave( hnd );

		/* 再生中のＭＩＤＩサウンドを検索 */
		midi = LkListSearch( snd->mlst, NULL, is_midi_play );
		if ( midi != NULL )
		{
			/* 再生中のサウンドを停止 */
			GMidiPause( midi );

			snd->mstop = midi;
		}
	}
}

/*-----------------------------------------------------------------------------
*
* Purpose : 再生中のＭＩＤＩデータの検索関数．
*
* Return  : 見つかれば真，見つからなければ偽を返す．
*
*----------------------------------------------------------------------------*/
static BOOL
is_midi_play
(
	const void*			key,		/* 検索キー                             */
	const void*			item		/* 比較するデータ                       */
)
{
	GMIDI				midi   = (GMIDI)item;
	(void)key;

	if ( GMidiIsPlay( midi ) == TRUE )
	{
		return	TRUE;
	}

	return	FALSE;
}

/*-----------------------------------------------------------------------------
*
* Purpose : 再生中のＭＩＤＩデータの検索関数．
*
* Return  : 見つかれば真，見つからなければ偽を返す．
*
*----------------------------------------------------------------------------*/
static BOOL
is_midi_pause
(
	const void*			key,		/* 検索キー                             */
	const void*			item		/* 比較するデータ                       */
)
{
	GMIDI				midi = (GMIDI)item;
	(void)key;

	if ( GMidiIsPause( midi ) == TRUE )
	{
		return	TRUE;
	}

	return	FALSE;
}

/********** End of File ******************************************************/
