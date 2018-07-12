/******************************************************************************
* File Name : gmidi.c                              Ver : 1.00  Date : 98-10-22
*
* Description :
*
*       ゲームシステム用 ＭＩＤＩデータ管理．
*
* Author : Moriharu Ohzu.
*
******************************************************************************/
#include	"gmidi.h"
#include	"heap.h"
#include	<stdio.h>
#include	<MMSystem.h>

/****** 定数宣言 *************************************************************/

#define MDS_F_NOSTREAMID 0x00000001UL	/* ストリーム圧縮フラグ */

/****** データ型宣言 *********************************************************/

/*----- ＭＩＤＩストリームコールバック関数型 --------------------------------*/

typedef	void (CALLBACK* PMIDIPROC)(HMIDIIN hmidiIn, UINT uiMsg,
						 DWORD dwInstance, DWORD dwParam1, DWORD dwParam2);

/*----- ＭＩＤＩストリームバッファヘッダ構造体 ------------------------------*/

typedef struct
{
	DWORD			dwTickStart;	/* タイマ                      */
	DWORD			dwBuffLen;		/* バッファサイズ               */

} MIDSBUFFERHDR;

/*----- ＭＤＳファイル情報構造体 --------------------------------------------*/

typedef	struct
{
	DWORD			dwTimeFormat;	/* ストリームの時間分割の属性   */
	DWORD			cbMaxBuffer;	/* バッファの最大サイズ         */
	DWORD			dwFlags;		/* フォーマットフラグ           */

} MIDSFORMAT;

/*----- MIDIストリーム構造体 ------------------------------------------------*/

typedef	struct
{
	BOOL			play_flag;		/* 再生中フラグ                 */
	BOOL			loop_flag;		/* ループ指定フラグ             */
	BOOL			pause_flag;		/* 一時停止中フラグ             */
	HMIDISTRM		hmstrm;			/* MIDIストリームハンドラ       */
	UINT			strm_id;		/* ストリームＩＤ               */
	MIDSFORMAT		mfmt;			/* ＭＳＤフォーマット情報       */
	DWORD			bufcnt;			/* MIDIストリームバッファ数     */
	BYTE*			strmbuf;		/* MIDIストリームバッファ       */
	UINT			curbuf;			/* 再生中のバッファの位置       */

} MIDIINFO;

/*----- ＭＤＳファイルヘッダ構造体 ------------------------------------------*/

#pragma pack(1)

typedef	struct
{
	BYTE			RIFF[4];		/* "RIFF"                       */
	DWORD			dwSize;			/* データサイズ                 */
	BYTE			MIDS[4];		/* "MIDS"                       */
	BYTE			fmt_[4];		/* "fmt "                       */
	DWORD			swFSize;		/* フォーマット部のサイズ       */
	DWORD			dwTimeFormat;	/* ストリームの時間分割の属性   */
	DWORD			cbMaxBuffer;	/* バッファの最大サイズ         */
	DWORD			dwFlags;		/* フォーマットフラグ           */
	BYTE			DATA[4];		/* "DATA"                       */
	DWORD			dwDSize;		/* データ部のサイズ             */
	DWORD			dwNumBuffs;		/* データバッファ数             */

} MdsHeader;

#pragma pack()

/****** 関数プロトタイプ宣言 *************************************************/

static BOOL	open_midi_stream( MIDIINFO* );
static BOOL	close_midi_stream( MIDIINFO* );
static BOOL	play_midi_stream( MIDIINFO* );
static BOOL	stop_midi_stream( MIDIINFO* );
static BOOL	pause_midi_stream( MIDIINFO* );
static BOOL	restart_midi_stream( MIDIINFO* );
static void CALLBACK cback_midi_stream( HMIDIIN,UINT,DWORD,DWORD,DWORD );
static BOOL	fread_mds_file( const char*, MdsHeader*, BYTE** );
static BYTE* create_strmbuf( MdsHeader*, BYTE* );
static BOOL	decompress_strmbuf( MIDIHDR*, MIDIHDR* );

/*=============================================================================
*
* Purpose : ＭＩＤＩの生成．
*
* Return  : ＭＩＤＩハンドラを返す．
*
*============================================================================*/
GMIDI
GMidiNew
(
	const char*		fname,			/* ＭＤＳファイル名             */
	BOOL			loop_flag		/* ループフラグ                 */
)
{
	MIDIINFO*		midi;
	MdsHeader		hmds;
	BYTE*			pmds = NULL;

	/* ＭＤＳファイルの読み込み */
	if ( fread_mds_file( fname, &hmds, &pmds ) == FALSE )
	{
		return	NULL;
	}

	midi = new_object( sizeof( MIDIINFO ) );

	midi->play_flag         = FALSE;
	midi->loop_flag         = loop_flag;
	midi->pause_flag        = FALSE;
	midi->hmstrm            = NULL;
	midi->strm_id           = MIDI_MAPPER;
	midi->mfmt.dwTimeFormat = hmds.dwTimeFormat;
	midi->mfmt.cbMaxBuffer  = hmds.cbMaxBuffer;
	midi->mfmt.dwFlags      = hmds.dwFlags;
	midi->bufcnt            = hmds.dwNumBuffs;
	midi->strmbuf           = pmds;
	midi->curbuf            = 0;

	return	(GMIDI)midi;
}

/*=============================================================================
*
* Purpose : ＭＩＤＩの消滅．
*
* Return  : なし．
*
*============================================================================*/
void
GMidiDelete
(
	GMIDI			hnd				/* ＭＩＤＩハンドラ             */
)
{
	MIDIINFO*		midi = (MIDIINFO*)hnd;

	stop_midi_stream( midi );

	if ( midi->strmbuf != NULL )
	{
		del_object( midi->strmbuf );
	}

	del_object( midi );
}

/*=============================================================================
*
* Purpose : ＭＩＤＩの再生．
*
* Return  : 再生に成功すれば真，失敗すれば偽を返す．
*
*============================================================================*/
BOOL
GMidiPlay
(
	GMIDI			hnd				/* ＭＩＤＩハンドラ             */
)
{
	MIDIINFO*		midi = (MIDIINFO*)hnd;

	return	play_midi_stream( midi );
}

/*=============================================================================
*
* Purpose : ＭＩＤＩの停止．
*
* Return  : 停止に成功すれば真，失敗すれば偽を返す．
*
*============================================================================*/
BOOL
GMidiStop
(
	GMIDI			hnd				/* ＭＩＤＩハンドラ             */
)
{
	MIDIINFO*		midi = (MIDIINFO*)hnd;

	return	stop_midi_stream( midi );
}

/*=============================================================================
*
* Purpose : ＭＩＤＩの一時停止．
*
* Return  : 一時停止に成功すれば真，失敗すれば偽を返す．
*
*============================================================================*/
BOOL
GMidiPause
(
	GMIDI			hnd				/* ＭＩＤＩハンドラ             */
)
{
	MIDIINFO*		midi = (MIDIINFO*)hnd;

	return	pause_midi_stream( midi );
}

/*=============================================================================
*
* Purpose : ＭＩＤＩの一時停止状態からの再開．
*
* Return  : 成功すれば真，失敗すれば偽を返す．
*
*============================================================================*/
BOOL
GMidiRestart
(
	GMIDI			hnd				/* ＭＩＤＩハンドラ             */
)
{
	MIDIINFO*		midi = (MIDIINFO*)hnd;

	return	restart_midi_stream( midi );
}

/*=============================================================================
*
* Purpose : ＭＩＤＩが再生中か調べる．
*
* Return  : 再生中であれば真，停止中であれば偽を返す．
*
*============================================================================*/
BOOL
GMidiIsPlay
(
	GMIDI			hnd				/* ＭＩＤＩハンドラ             */
)
{
	MIDIINFO*		midi = (MIDIINFO*)hnd;

	return	midi->play_flag;
}

/*=============================================================================
*
* Purpose : ＭＩＤＩが一時停止中か調べる．
*
* Return  : 一時停止中であれば真，再生中であれば偽を返す．
*
*============================================================================*/
BOOL
GMidiIsPause
(
	GMIDI			hnd				/* ＭＩＤＩハンドラ             */
)
{
	MIDIINFO*		midi = (MIDIINFO*)hnd;

	return	midi->pause_flag;
}

/*-----------------------------------------------------------------------------
*
* Purpose : ＭＩＤＩストリームのオープン．
*
* Return  : 成功すれば真，失敗すれば偽を返す．
*
*----------------------------------------------------------------------------*/
static BOOL
open_midi_stream
(
	MIDIINFO*		midi			/* ＭＩＤＩハンドラ             */
)
{
	MIDIPROPTIMEDIV		mptdTimeDiv;
	HMIDISTRM			hmstrm;
	MMRESULT			mrl;

	/* 既にオープンされていた場合はクローズする */
	if ( midi->hmstrm != NULL )
	{
		close_midi_stream( midi );
	}

	/* MIDIストリームのオープン */
	mrl = midiStreamOpen( &hmstrm, &midi->strm_id, 1,
		 (DWORD)(PMIDIPROC)cback_midi_stream, (DWORD)midi, CALLBACK_FUNCTION );
	if ( mrl != MMSYSERR_NOERROR )
	{
		return	FALSE;
	}
	midi->hmstrm  = hmstrm;

	/* ストリームの時間分割の属性を設定する */
	ZeroMemory( &mptdTimeDiv, sizeof( mptdTimeDiv ) );
	mptdTimeDiv.cbStruct  = sizeof( mptdTimeDiv );
	mptdTimeDiv.dwTimeDiv = midi->mfmt.dwTimeFormat;
	if ( midiStreamProperty( midi->hmstrm, (BYTE*)&mptdTimeDiv,
						MIDIPROP_SET | MIDIPROP_TIMEDIV ) != MMSYSERR_NOERROR )
	{
		close_midi_stream( midi );

		return FALSE;
	}

	return TRUE;
}

/*-----------------------------------------------------------------------------
*
* Purpose : ＭＩＤＩストリームのクローズ．
*
* Return  : 成功すれば真，失敗すれば偽を返す．
*
*----------------------------------------------------------------------------*/
static BOOL
close_midi_stream
(
	MIDIINFO*		midi			/* ＭＩＤＩハンドラ             */
)
{
	MIDIHDR*		pmhdr;
	int				i;

	if ( midi->hmstrm == NULL )
	{
		return	FALSE;
	}

	/* ストリームをリセットする */
	if ( midiOutReset( (HMIDIOUT)midi->hmstrm ) != MMSYSERR_NOERROR )
	{
		return	FALSE;
	}

	pmhdr = (MIDIHDR*)midi->strmbuf;
	for ( i = midi->bufcnt; i > 0; i-- )
	{
		/* ストリームのヘッダを解放する */
		midiOutUnprepareHeader( (HMIDIOUT)midi->hmstrm, pmhdr,
														sizeof( MIDIHDR ) );

			/* 次のストリームに移る */
		pmhdr = (MIDIHDR*)( ( (BYTE*)pmhdr) + sizeof( MIDIHDR )
			  + pmhdr->dwBufferLength );
	}

	/* ストリームをクローズする */
	if ( midiStreamClose( midi->hmstrm ) == MMSYSERR_NOERROR )
	{
		midi->hmstrm    = NULL;
		midi->strm_id   = MIDI_MAPPER;
		midi->play_flag = FALSE;

		return TRUE;
	}

	return	FALSE;
}

/*-----------------------------------------------------------------------------
*
* Purpose : ＭＩＤＩストリームの再生．
*
* Return  : 成功すれば真，失敗すれば偽を返す．
*
*----------------------------------------------------------------------------*/
static BOOL
play_midi_stream
(
	MIDIINFO*		midi			/* ＭＩＤＩハンドラ             */
)
{
	MIDIHDR*		pmhdr;
	int				i;

	/* 既に再生されているか？ */
	if ( midi->play_flag == TRUE )
	{
		return	FALSE;
	}

	/* バッファのカウンタの初期化 */
	midi->curbuf = 0;

	/* MIDIストリームのオープン */
	if( open_midi_stream( midi ) == FALSE )
	{
		return	FALSE;
	}

	pmhdr = (MIDIHDR*)midi->strmbuf;
	for ( i = midi->bufcnt; i > 0; i-- )
	{
		/* ストリームのヘッダを用意する */
		if ( midiOutPrepareHeader( (HMIDIOUT)midi->hmstrm,
							   pmhdr, sizeof( MIDIHDR ) ) != MMSYSERR_NOERROR )
		{
			stop_midi_stream( midi );

			return	FALSE;
		}

		/* 再生するストリームをキューに入れる */
		if ( midiStreamOut( midi->hmstrm,
							   pmhdr, sizeof( MIDIHDR ) ) != MMSYSERR_NOERROR )
		{
			stop_midi_stream( midi );

			return	FALSE;
		}

		/* 次のストリームに移る */
		pmhdr = (MIDIHDR*)(((BYTE*)pmhdr) + sizeof( MIDIHDR )
		      + pmhdr->dwBufferLength );
	}

	/* 実際に再生を開始するため再生を再開する */
	if ( midiStreamRestart( midi->hmstrm ) != MMSYSERR_NOERROR )
	{
		stop_midi_stream( midi );

		return	FALSE;
	}

	/* 再生中フラグの設定 */
	midi->play_flag = TRUE;

	return	TRUE;
}

/*-----------------------------------------------------------------------------
*
* Purpose : ＭＩＤＩストリームの停止．
*
* Return  : 成功すれば真，失敗すれば偽を返す．
*
*----------------------------------------------------------------------------*/
static BOOL
stop_midi_stream
(
	MIDIINFO*		midi			/* ＭＩＤＩハンドラ             */
)
{
	if ( midi->hmstrm == NULL )
	{
		return	TRUE;
	}

	midi->play_flag = FALSE;
	if ( midiStreamStop( midi->hmstrm ) == MMSYSERR_NOERROR )
	{
		close_midi_stream( midi );

		return	TRUE;
    }

	return	FALSE;
}

/*-----------------------------------------------------------------------------
*
* Purpose : ＭＩＤＩストリームの一時停止．
*
* Return  : 成功すれば真，失敗すれば偽を返す．
*
*----------------------------------------------------------------------------*/
static BOOL
pause_midi_stream
(
	MIDIINFO*		midi			/* ＭＩＤＩハンドラ             */
)
{
	if ( midi->pause_flag == TRUE )
	{
		return	FALSE;
	}

  	if ( midiStreamPause( midi->hmstrm ) == MMSYSERR_NOERROR )
	{
		midi->pause_flag = TRUE;

		return	TRUE;
	}

	return	FALSE;
}

/*-----------------------------------------------------------------------------
*
* Purpose : ＭＩＤＩストリームの再開．
*
* Return  : 成功すれば真，失敗すれば偽を返す．
*
*----------------------------------------------------------------------------*/
static BOOL
restart_midi_stream
(
	MIDIINFO*		midi			/* ＭＩＤＩハンドラ             */
)
{
	if ( midi->pause_flag == FALSE )
	{
		return	FALSE;
	}

	if ( midiStreamRestart( midi->hmstrm ) == MMSYSERR_NOERROR )
	{
		midi->pause_flag = FALSE;

		return	TRUE;
	}

	return	FALSE;
}

/*-----------------------------------------------------------------------------
*
* Purpose : ＭＩＤＩストリームのコールバック関数．
*
* Return  : なし．
*
*----------------------------------------------------------------------------*/
static void CALLBACK
cback_midi_stream
(
	HMIDIIN		hmidiIn,
	UINT		uiMsg,
	DWORD		dwInstance,
	DWORD		dwParam1,
	DWORD		dwParam2
)
{
	MIDIINFO* 	midi = (MIDIINFO*)dwInstance;
	(void)dwParam1; (void)dwParam2; (void)hmidiIn;

	switch( uiMsg )
	{
	case MOM_DONE:
		/* 再生位置のチェック */
		midi->curbuf++;
		if ( ( midi->curbuf    >= midi->bufcnt )
		  && ( midi->play_flag == TRUE         ) )
		{
			/* 再生終了 */
			midi->play_flag = FALSE;

			/* ループする必要があればループする */
			if ( midi->loop_flag == TRUE )
			{
				play_midi_stream( midi );
			}
		}
		break;
	}
}

/*-----------------------------------------------------------------------------
*
* Purpose : ＭＤＳファイルの読み込み．
*
* Return  : 読み込みに成功すれば真，失敗したら偽を返す．
*
*----------------------------------------------------------------------------*/
static BOOL
fread_mds_file
(
	const char*		fname,			/* ウェーブファイル名               */
	MdsHeader*		mhdr,			/*-> ＭＤＳファイルヘッダ           */
	BYTE** 			pmds			/*-> ＭＤＳデータ                   */
)
{
	FILE*			fhnd;
	BYTE*			buf;
	size_t			size;

	/* ＭＤＳファイルのオープン */
	if ( ( fhnd = fopen( fname, "rb" ) ) == NULL )
	{
		return	FALSE;
	}

	/* ＭＤＳファイルヘッダの読み込み */
	if ( fread( mhdr, sizeof( MdsHeader ), 1, fhnd ) != 1 )
	{
		fclose( fhnd );

		return	FALSE;
	}

	/* ＭＤＳ読み込みバッファの確保 */
	buf = new_object( mhdr->dwDSize );

	/* ＭＤＳデータの読み込み */
	size = mhdr->dwDSize - sizeof( DWORD );
	if ( fread( buf, sizeof( BYTE ), size, fhnd ) != size )
	{
		fclose( fhnd );

		del_object( *pmds );

		return	FALSE;
	}

	/* ＭＤＳファイルのクローズ */
	fclose( fhnd );

	/* ＭＩＤＩストリーミングバッファの作成 */
	*pmds = create_strmbuf( mhdr, buf );

	/* 読み込み用バッファの消滅 */
	del_object( buf );

	if ( *pmds == NULL )
	{
		return	FALSE;
	}

	return	TRUE;
}

/*-----------------------------------------------------------------------------
*
* Purpose : ＭＩＤＩストリーミングバッファの作成．
*
* Return  : ＭＩＤＩストリーミングバッファを返す．
*
*----------------------------------------------------------------------------*/
static BYTE*
create_strmbuf
(
	MdsHeader*		mhdr,			/* ＭＤＳファイルヘッダ             */
	BYTE*			pBuff			/* ファイルから読み込んだデータ     */
)
{
	MIDIHDR*		pmhdr;
	BYTE*			pmds;
	MIDSBUFFERHDR*	pmhdrBuff;
	MIDIHDR			mhdrCmp;
	size_t			size;
	int				i;

	/* ＭＩＤＩストリーミングデータのサイズを求める */
	size = ( mhdr->cbMaxBuffer + sizeof( MIDIHDR ) ) * mhdr->dwNumBuffs;

	pmds = new_object( size );

	pmhdr = (MIDIHDR*)pmds;

	for ( i = mhdr->dwNumBuffs; i > 0; i-- )
	{
		pmhdr->lpData         = (LPSTR)( pmhdr + 1 );
		pmhdr->dwBufferLength = mhdr->cbMaxBuffer;
		pmhdr->dwFlags        = 0;
		pmhdr->dwUser         = 0;
		pmhdr->lpNext         = NULL;

		pmhdrBuff  = (MIDSBUFFERHDR*)pBuff;
		pBuff     += sizeof(MIDSBUFFERHDR);
		if ( ( mhdr->dwFlags & MDS_F_NOSTREAMID ) == MDS_F_NOSTREAMID )
		{
			/* バッファは圧縮されている */
			mhdrCmp.lpData          = (LPSTR)pBuff;
			mhdrCmp.dwBufferLength  = pmhdrBuff->dwBuffLen;
			mhdrCmp.dwBytesRecorded = pmhdrBuff->dwBuffLen;

			/* バッファのデータを展開する */
			if ( decompress_strmbuf( pmhdr, &mhdrCmp ) == FALSE )
			{
				del_object( pmds );

				return	NULL;
			}
		}
		else
		{
			/* バッファにデータをコピー */
			pmhdr->dwBytesRecorded = pmhdrBuff->dwBuffLen;

			CopyMemory( pmhdr->lpData, pBuff, pmhdrBuff->dwBuffLen );
		}

		/* 次のバッファに移動 */
		pBuff += pmhdrBuff->dwBuffLen;
		pmhdr  = (MIDIHDR*)( ( (BYTE*)pmhdr ) + sizeof( MIDIHDR ) 
		                                      + mhdr->cbMaxBuffer );
	}

	return	pmds;
}

/*-----------------------------------------------------------------------------
*
* Purpose : ＭＤＳファイルの読み込み．
*
* Return  : 読み込みに成功すれば真，失敗したら偽を返す．
*
*----------------------------------------------------------------------------*/
static BOOL
decompress_strmbuf
(
	MIDIHDR*	pmhdrDst,			/*-> 展開されたデータ           */
	MIDIHDR*	pmhdrSrc			/* 圧縮されたデータ             */
)
{
	DWORD*  	pSrcData;
	DWORD*		pDstData;
	DWORD		dwSrc;
	DWORD		dwDst;
	DWORD		dwExtra;

	pSrcData = (DWORD*)pmhdrSrc->lpData;
	pDstData = (DWORD*)pmhdrDst->lpData;
	dwSrc    = pmhdrSrc->dwBytesRecorded;
	dwDst    = pmhdrDst->dwBufferLength;

	/* バッファ全体のサイズは DWORD の倍数でなければいけない */
	if ( ( dwSrc & 3 ) != 0 )
	{
		return	FALSE;
	}

	while ( dwSrc != 0 )
	{
		/* 少なくとも，時間差，ストリームＩＤ，イベントのDWORDの領域が必要 */
		if ( dwDst < ( 3 * sizeof( DWORD ) ) )
		{
			return	FALSE;
		}

		/* 時間差をコピーする */
		*pDstData++ = *pSrcData++;
		dwSrc -= sizeof( DWORD );
		dwDst -= sizeof( DWORD );

		/* ストリームＩＤを０に設定する */
		*pDstData++ = 0;
		dwDst -= sizeof( DWORD );

		/* 処理するデータがあるか確認する */
		if ( dwSrc == 0 )
		{
			return	FALSE;
		}

		/* イベントデータをコピーする */
		dwExtra = 0;
		if ( ( (*pSrcData) & 0x80000000L ) != 0 )
		{
			dwExtra = (*pSrcData) & 0x00FFFFFFL;
		}

		/* イベントの長さを境界にあわせてならべる ( 次のDWORDまで ) */
		dwExtra = ( dwExtra + 3 ) & ~3;

		/* イベントのDWORDをコピーする */
		*pDstData++ = *pSrcData++;
		dwSrc -= sizeof( DWORD );
		dwDst -= sizeof( DWORD );

		/* イベントパラメータをコピーする */
		if ( dwExtra != 0 )
		{
			if ( ( dwExtra > dwSrc ) || ( dwExtra > dwDst ) )
			{
				return	FALSE;
			}
			CopyMemory( pDstData, pSrcData, dwExtra );
		}

		/* ポインタをインクリメントして長さを更新する */
		pDstData += ( dwExtra / sizeof( DWORD ) );
		pSrcData += ( dwExtra / sizeof( DWORD ) );
		dwSrc    -= dwExtra;
		dwDst    -= dwExtra;
	}

	/* 結果のバイト数を計算する */
	pmhdrDst->dwBytesRecorded = ( ((BYTE*)pDstData)
							  - (BYTE*)(pmhdrDst->lpData) );

	return	TRUE;
}

/********** End of File ******************************************************/
