/******************************************************************************
* File Name : gwave.c                              Ver : 1.00  Date : 98-10-22
*
* Description :
*
*       ゲームシステム用 ウェーブデータ管理．
*
* Author : Moriharu Ohzu.
*
******************************************************************************/
#include	"gwave.h"
#include	"heap.h"
#include	<stdio.h>

/****** データ型宣言 *********************************************************/

/*----- ウェーブデータ構造体 ------------------------------------------------*/

typedef struct
{
	GWAVEFLAG			flag;		/* 再生タイプ指定フラグ                  */
	int					volume;		/* ボリューム                            */
	int					pan;		/* 左右のチャンネルの相対ボリューム      */
	void*				wave;		/* ウェーブファイルデータ                */
	LPWAVEFORMATEX		wvfmt;		/* ウェーブファイルフォーマット          */
	LPBYTE				wvdata;		/* ウェーブデータ                        */
	DWORD				wvsize;		/* ウェーブデータサイズ                  */
	int					bufcnt;		/* DirectSoundバッファ数                 */
	int					bufidx;		/* DirectSoundバッファインデックス       */
	LPDIRECTSOUNDBUFFER	lpBuf[1];	/* DirectSoundバッファ                   */

} WAVEINFO;

/****** 関数プロトタイプ宣言 *************************************************/

static LPDIRECTSOUNDBUFFER	create_sound_buffer( LPDIRECTSOUND, WAVEINFO* );
static BOOL	play_sound_buffer( WAVEINFO*, LPDIRECTSOUNDBUFFER );
static BOOL	is_play_sound_buffer( LPDIRECTSOUNDBUFFER );
static void	stop_sound_buffer( LPDIRECTSOUNDBUFFER );
static BOOL	wave_to_buffer( LPDIRECTSOUNDBUFFER, LPBYTE, DWORD );
static BOOL	fread_wave_file( const char*, LPBYTE* );
static BOOL	unpuck_wave_chunk( void*, LPWAVEFORMATEX*, LPBYTE*, DWORD* );

/*=============================================================================
*
* Purpose : ウェーブデータの生成．
*
* Return  : ウェーブデータハンドラを返す．
*
*============================================================================*/
GWAVE
GWaveNew
(
	LPDIRECTSOUND		lpDS,		/* DirectSoundハンドラ              */
	const char*			fname,		/* WAVEファイル名                   */
	int					cnt,		/* 同時再生数                       */
	GWAVEFLAG			flag		/* 再生タイプ指定フラグ             */
)
{
	WAVEINFO*			wave;
	LPBYTE				pwav;
	LPWAVEFORMATEX		lpwfmx;
	LPBYTE				wvdata;
	DWORD				wvsize;
	size_t				size;
	int					i;
	HRESULT				dsval;

	/* ウェーブファイルの読み込み */
	if ( fread_wave_file( fname, &pwav ) == FALSE )
	{
		return	NULL;
	}

	/* ウェーブデータのアンパック */
	if ( unpuck_wave_chunk( pwav, &lpwfmx, &wvdata, &wvsize ) == FALSE )
	{
		del_object( pwav );

		return	NULL;
	}

	size = sizeof( WAVEINFO ) + ( cnt - 1 ) * sizeof( LPDIRECTSOUNDBUFFER );

	wave = new_object( size );

	wave->flag   = flag;
	wave->volume = 0;
	wave->pan    = 0;
	wave->wave   = pwav;
	wave->wvfmt  = lpwfmx;
	wave->wvdata = wvdata;
	wave->wvsize = wvsize;
	wave->bufcnt = cnt;
	wave->bufidx = 0;

	/* サウンドバッファの初期化 */
	for ( i = 0; i < cnt; i++ )
	{
		wave->lpBuf[i] = NULL;
	}

	/* オリジナルサウンドバッファの作成 */
	if ( ( wave->lpBuf[0] = create_sound_buffer( lpDS, wave ) ) == NULL )
	{
		GWaveDelete( wave );
	}

	/* 同時再生用のサウンドバッファの作成 */
	for ( i = 1; i < cnt; i++ )
	{
		dsval = lpDS->lpVtbl->DuplicateSoundBuffer( lpDS, wave->lpBuf[0],
		                                                 &wave->lpBuf[i] );
		if ( dsval != DS_OK )
		{
			GWaveDelete( wave );

			return	NULL;
		}
	}

	return	(GWAVE)wave;
}

/*=============================================================================
*
* Purpose : ウェーブデータの消滅．
*
* Return  : なし．
*
*============================================================================*/
void
GWaveDelete
(
	GWAVE				hnd			/* ウェーブデータハンドラ           */
)
{
	WAVEINFO*			wave = (WAVEINFO*)hnd;
	LPDIRECTSOUNDBUFFER	lpBuf;
	int					i;

	for ( i = 0; i < wave->bufcnt; i++ )
	{
		if ( ( lpBuf = wave->lpBuf[i] ) != NULL )
		{
			lpBuf->lpVtbl->Release( lpBuf );
		}
	}

	if ( wave->wave != NULL )
	{
		del_object( wave->wave );
	}

	del_object( wave );
}

/*=============================================================================
*
* Purpose : ウェーブデータの再生初期化．
*
* Return  : なし．
*
*============================================================================*/
void
GWavePlayInit
(
	GWAVE				hnd			/* ウェーブデータハンドラ           */
)
{
	GWaveStop( hnd );

	GWaveSetVolume( hnd, 0 );

	GWaveSetPan( hnd, 0 );
}

/*=============================================================================
*
* Purpose : ウェーブデータの再生．
*
* Return  : なし．
*
*============================================================================*/
void
GWavePlay
(
	GWAVE				hnd			/* ウェーブデータハンドラ           */
)
{
	WAVEINFO*			wave = (WAVEINFO*)hnd;

	/* ウェーブデータの再生 */
	if ( play_sound_buffer( wave, wave->lpBuf[ wave->bufidx ] ) == TRUE )
	{
		/* 次のバッファに移動 */
		wave->bufidx = ( wave->bufidx + 1 ) % wave->bufcnt;
	}
}

/*=============================================================================
*
* Purpose : ウェーブデータが再生中か調べる．
*
* Return  : 再生中であれば真，再生中でなければ偽を返す．
*
*============================================================================*/
BOOL
GWaveIsPlay
(
	GWAVE				hnd			/* ウェーブデータハンドラ           */
)
{
	WAVEINFO*			wave = (WAVEINFO*)hnd;
	int					i;

	for ( i = 0; i < wave->bufcnt; i++ )
	{
		if ( is_play_sound_buffer( wave->lpBuf[i] ) == TRUE )
		{
			return	TRUE;
		}
	}

	return	FALSE;
}

/*=============================================================================
*
* Purpose : ウェーブデータの停止．
*
* Return  : なし．
*
*============================================================================*/
void
GWaveStop
(
	GWAVE				hnd			/* ウェーブデータハンドラ           */
)
{
	WAVEINFO*			wave = (WAVEINFO*)hnd;
	int					i;

	for ( i = 0; i < wave->bufcnt; i++ )
	{
		stop_sound_buffer( wave->lpBuf[i] );
	}
}

/*=============================================================================
*
* Purpose : ウェーブデータのボリューム設定．
*
* Return  : なし．
*
*============================================================================*/
void
GWaveSetVolume
(
	GWAVE				hnd,		/* ウェーブデータハンドラ           */
	int					volume		/* 左右のチャンネルの相対ボリューム */
)
{
	WAVEINFO*			wave = (WAVEINFO*)hnd;
	LPDIRECTSOUNDBUFFER	lpBuf;
	int					i;

	wave->volume = volume;

	for ( i = 0; i < wave->bufcnt; i++ )
	{
		lpBuf = wave->lpBuf[i];

		lpBuf->lpVtbl->SetVolume( lpBuf, (LONG)volume );
	}
}

/*=============================================================================
*
* Purpose : ウェーブデータのボリューム取得．
*
* Return  : ウェーブデータのボリューム取得を返す．
*
*============================================================================*/
int
GWaveGetVolume
(
	GWAVE				hnd			/* ウェーブデータハンドラ           */
)
{
	WAVEINFO*			wave = (WAVEINFO*)hnd;

	return	wave->volume;
}

/*=============================================================================
*
* Purpose : ウェーブデータの左右のチャンネルの相対ボリュームを設定する．
*
* Return  : なし．
*
*============================================================================*/
void
GWaveSetPan
(
	GWAVE				hnd,		/* ウェーブデータハンドラ           */
	int					pan			/* 左右のチャンネルの相対ボリューム */
)
{
	WAVEINFO*			wave = (WAVEINFO*)hnd;
	LPDIRECTSOUNDBUFFER	lpBuf;
	int					i;

	wave->volume = pan;

	for ( i = 0; i < wave->bufcnt; i++ )
	{
		lpBuf = wave->lpBuf[i];

		lpBuf->lpVtbl->SetPan( lpBuf, (LONG)pan );
	}
}

/*=============================================================================
*
* Purpose : ウェーブデータの左右のチャンネルの相対ボリュームを取得する．
*
* Return  : 左右のチャンネルの相対ボリュームを返す．
*
*============================================================================*/
int
GWaveGetPan
(
	GWAVE				hnd			/* ウェーブデータハンドラ           */
)
{
	WAVEINFO*			wave = (WAVEINFO*)hnd;

	return	wave->pan;
}

/*-----------------------------------------------------------------------------
*
* Purpose : DirectSoundバッファの作成．
*
* Return  : DirectSoundバッファを返す．
*
*----------------------------------------------------------------------------*/
static LPDIRECTSOUNDBUFFER
create_sound_buffer
(
	LPDIRECTSOUND		lpDS,		/* DirectSoundハンドラ              */
	WAVEINFO*			wave		/* ウェーブデータ情報               */
)
{
	LPDIRECTSOUNDBUFFER	lpBuf;
	DSBUFFERDESC		dsbd;
	HRESULT				hrs;

	/* サウンドバッファの情報を設定 */
	ZeroMemory( &dsbd, sizeof( dsbd ) );
	dsbd.dwSize        = sizeof( dsbd );
	dsbd.dwFlags       = DSBCAPS_STATIC
		               | DSBCAPS_CTRLPAN
					   | DSBCAPS_CTRLVOLUME
					   | DSBCAPS_CTRLFREQUENCY;
	dsbd.dwBufferBytes = wave->wvsize;
	dsbd.lpwfxFormat   = wave->wvfmt;

	/* サウンドバッファの作成 */
	hrs = lpDS->lpVtbl->CreateSoundBuffer( lpDS, &dsbd, &lpBuf, NULL );

	if ( hrs != DS_OK )
	{
		return	NULL;
	}

	/* サウンドバッファにウェーブデータを転送 */
	if ( wave_to_buffer( lpBuf, wave->wvdata, wave->wvsize ) == FALSE )
	{
		lpBuf->lpVtbl->Release( lpBuf );

		return	NULL;
	}

	return	lpBuf;
}

/*-----------------------------------------------------------------------------
*
* Purpose : DirectSoundバッファの再生．
*
* Return  : 再生が成功すれば真，失敗したら偽を返す．
*
*----------------------------------------------------------------------------*/
static BOOL
play_sound_buffer
(
	WAVEINFO*			wave,		/* ウェーブデータ情報               */
	LPDIRECTSOUNDBUFFER	lpBuf		/* 再生するサウンドバッファ         */
)
{
	DWORD		dwStatus;
	DWORD		dwFlags;

	/* サウンドバッファの状態の取得 */
	if ( lpBuf->lpVtbl->GetStatus( lpBuf, &dwStatus ) != DS_OK )
	{
		return	FALSE;
	}

	/* サウンドバッファが失われているかチェック */
	if ( ( dwStatus & DSBSTATUS_BUFFERLOST) == DSBSTATUS_BUFFERLOST )
	{
		/* サウンドバッファの復元 */
		if ( lpBuf->lpVtbl->Restore( lpBuf ) != DS_OK )
		{
			return	FALSE;
		}

		/* ウェーブデータの再転送 */
		if ( wave_to_buffer( lpBuf, wave->wvdata, wave->wvsize ) == FALSE )
		{
			return	FALSE;
		}
	}

	/* 再生中のチェック */
	if ( ( dwStatus & DSBSTATUS_PLAYING ) == DSBSTATUS_PLAYING )
	{
		if ( ( wave->flag & GWAVE_WAIT ) == GWAVE_WAIT )
		{
			/* サウンド終了待ち */
			return	FALSE;
		}

		/* サウンド停止 */
		stop_sound_buffer( lpBuf );
	}

	/* ループ指定の設定 */
	dwFlags = 0;
	if ( ( wave->flag & GWAVE_LOOP ) == GWAVE_LOOP )
	{
		/* サウンド終了待ち */
		dwFlags = DSBPLAY_LOOPING;
	}

	/* サウンドの再生 */
	if ( lpBuf->lpVtbl->Play( lpBuf, 0,0, dwFlags ) != DS_OK )
	{
		return	FALSE;
	}

	return	TRUE;
}

/*-----------------------------------------------------------------------------
*
* Purpose : DirectSoundバッファの停止．
*
* Return  : なし．
*
*----------------------------------------------------------------------------*/
static BOOL
is_play_sound_buffer
(
	LPDIRECTSOUNDBUFFER	lpBuf		/* サウンドバッファ                  */
)
{
	DWORD		dwStatus;

	/* サウンドバッファの状態の取得 */
	if ( lpBuf->lpVtbl->GetStatus( lpBuf, &dwStatus ) != DS_OK )
	{
		return	FALSE;
	}

	/* 再生中のチェック */
	if ( ( dwStatus & DSBSTATUS_PLAYING ) == DSBSTATUS_PLAYING )
	{
		return	TRUE;
	}

	return	FALSE;
}
/*-----------------------------------------------------------------------------
*
* Purpose : DirectSoundバッファの停止．
*
* Return  : なし．
*
*----------------------------------------------------------------------------*/
static void
stop_sound_buffer
(
	LPDIRECTSOUNDBUFFER	lpBuf		/* 停止するサウンドバッファ         */
)
{
	lpBuf->lpVtbl->Stop( lpBuf );

	lpBuf->lpVtbl->SetCurrentPosition( lpBuf, 0 );
}

/*-----------------------------------------------------------------------------
*
* Purpose : サウンドバッファにウェーブデータを転送．
*
* Return  : 転送に成功すれば真，失敗したら偽を返す．
*
*----------------------------------------------------------------------------*/
static BOOL
wave_to_buffer
(
	LPDIRECTSOUNDBUFFER	lpBuf,		/* 転送先のサウンドバッファ         */
	LPBYTE				wave,		/* ウェーブデータ                   */
	DWORD				size		/* ウェーブデータのサイズ           */
)
{
	HRESULT				hrs;
	LPVOID				lpAudioPtr1;
	LPVOID				lpAudioPtr2;
	DWORD				dwAudioBytes1;
	DWORD				dwAudioBytes2;

	/* サウンドバッファのロック */
	hrs = lpBuf->lpVtbl->Lock( lpBuf, 0, size,
	           &lpAudioPtr1, &dwAudioBytes1, &lpAudioPtr2, &dwAudioBytes2, 0 );
	if ( hrs != DS_OK )
	{
		return	FALSE;
	}

	/* サウンドバッファにデータを転送 */
	CopyMemory( lpAudioPtr1, wave, dwAudioBytes1 );

	if( dwAudioBytes2 != 0 )
	{
		CopyMemory( lpAudioPtr2, wave + dwAudioBytes1, dwAudioBytes2 );
	}

	/* サウンドバッファのアンロック */
	lpBuf->lpVtbl->Unlock( lpBuf, lpAudioPtr1, dwAudioBytes1,
	                              lpAudioPtr2, dwAudioBytes2 );

	return TRUE;
}

/*-----------------------------------------------------------------------------
*
* Purpose : ウェーブファイルの読み込み．
*
* Return  : 読み込みに成功すれば真，失敗したら偽を返す．
*
*----------------------------------------------------------------------------*/
static BOOL
fread_wave_file
(
	const char*		fname,			/* ウェーブファイル名               */
	LPBYTE* 		pwave			/* ウェーブデータ                   */
)
{
	FILE*			fhnd;
	long			size;

	/* ウェーブファイルのオープン */
	if ( ( fhnd = fopen( fname, "rb" ) ) == NULL )
	{
		return	FALSE;
	}

	/* ファイルサイズを求める */
	if ( fseek( fhnd, 0, SEEK_END ) != 0 )
	{
		fclose( fhnd );

		return	FALSE;
	}
	size = ftell( fhnd );

	/* ファイルの位置を先頭に戻す */
	if ( fseek( fhnd, 0, SEEK_SET ) != 0 )
	{
		fclose( fhnd );

		return	FALSE;
	}

	/* ウェーブデータの確保 */
	*pwave = new_object( size );

	/* ウェーブデータの読み込み */
	if ( fread( *pwave, sizeof(BYTE), size, fhnd ) != (size_t)size )
	{
		fclose( fhnd );

		del_object( *pwave );

		return	FALSE;
	}

	/* ウェーブファイルのクローズ */
	fclose( fhnd );

	return	TRUE;
}

/*-----------------------------------------------------------------------------
*
* Purpose : ウェーブデータのアンパック．
*
* Return  : 読み込みに成功すれば真，失敗したら偽を返す．
*
*----------------------------------------------------------------------------*/
static BOOL
unpuck_wave_chunk
(
	void*			pRIFFBytes,		/* ウェーブファイルデータ  */
	LPWAVEFORMATEX*	lpwfmx,			/*-> ウェーブフォーマット  */
	LPBYTE*			lpChunkData,	/*-> ウェーブデータ        */
	DWORD*			lpCkSize		/*-> ウェーブデータサイズ  */
)
{
	DWORD*			dwChunkBitsPtr;
	DWORD*			dwChunkTailPtr;
	DWORD			dwChunkID;
	DWORD			dwType;
	DWORD			dwLength;

	if ( lpwfmx != NULL )
	{
		*lpwfmx = NULL;
	}

	if ( lpChunkData != NULL )
	{
		*lpChunkData = NULL;
	}

	if ( lpCkSize != NULL )
	{
		*lpCkSize = 0;
	}

	dwChunkBitsPtr = (DWORD*)pRIFFBytes;

	/* チャンクＩＤの取り出し */
	dwChunkID = *dwChunkBitsPtr++;	

	/* チャンクサイズの取り出し */
	dwLength = *dwChunkBitsPtr++;

	/* フォームタイプＩＤの取り出し */
	dwType = *dwChunkBitsPtr++;

	/* チャンクＩＤのチェック */
	if ( dwChunkID != mmioFOURCC( 'R','I','F','F' ) )
	{
		return	FALSE;
	}

	/* フォームタイプＩＤのチェック */
	if ( dwType != mmioFOURCC( 'W','A','V','E' ) )
	{
		return	FALSE;
	}

	dwChunkTailPtr = (DWORD*)( (BYTE*)dwChunkBitsPtr + dwLength - 4 );

	while( TRUE )
	{
		/* チャンクＩＤの取り出し */
		dwType = *dwChunkBitsPtr++;

		/* チャンクサイズの取り出し */
		dwLength = *dwChunkBitsPtr++;

		switch( dwType )
		{
		case mmioFOURCC( 'f','m','t',' ' ):
			/* ＷＡＶＥデータフォーマットの取得 */
			if ( lpwfmx != NULL && *lpwfmx == NULL )
			{
				if ( dwLength < sizeof( WAVEFORMAT ) )
				{
					return	FALSE;
				}
				*lpwfmx = (LPWAVEFORMATEX)dwChunkBitsPtr;

				if ( ( lpChunkData == 0 || *lpChunkData != NULL ) 
				  && ( lpCkSize    == 0 || *lpCkSize    != 0    ) )
				{
					return	TRUE;
				}
			}
			break;

		case mmioFOURCC( 'd','a','t','a' ):
			/* ＷＡＶＥデータの取得 */
			if ( ( lpChunkData != NULL  && *lpChunkData == NULL )
			  || ( lpCkSize    != NULL  && *lpCkSize    == 0    ) )
			{
				if ( lpChunkData != NULL )
				{
					*lpChunkData = (LPBYTE)dwChunkBitsPtr;
				}

				if ( lpCkSize != NULL)
				{
					*lpCkSize = dwLength;
				}

				if ( ( lpwfmx == NULL ) || ( *lpwfmx != NULL ) )
				{
					return	TRUE;
				}
			}
			break;
		}
		/* 次のチャンクデータに移動 */
		dwChunkBitsPtr = (DWORD*)((BYTE*)dwChunkBitsPtr + ((dwLength+1)&~1) );

		if ( dwChunkBitsPtr >= dwChunkTailPtr )
		{
			break;
		}
	}

	return	FALSE;
}

/********** End of File ******************************************************/
