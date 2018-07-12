/******************************************************************************
* File Name : gssound.c                            Ver : 1.00  Date : 98-10-22
*
* Description :
*
*       サウンドシステム．
*
* Author : Moriharu Ohzu.
*
******************************************************************************/
#include	"gssound.h"
#include	"gmidires.h"
#include	"gwaveres.h"

/****** 定数宣言 *************************************************************/

#define	DEFAULT_SE_MAX	256		/* デフォルトのＳＥリソースの最大数		*/
#define	DEFAULT_BGM_MAX	32		/* デフォルトのＢＧＭリソースの最大数	*/

/****** 変数宣言 *************************************************************/

static GSOUND        GSound = NULL;	/* サウンドハンドラ         */
static GMIDIRESOURCE MRes   = NULL;	/* ＭＩＤＩリソースハンドラ */
static GWAVERESOURCE WRes   = NULL;	/* ＷＡＶＥリソースハンドラ */
static GMIDI         Bgm    = NULL;	/* 現在再生中のＢＧＭ       */

/*=============================================================================
*
* Purpose : サウンドシステムの初期化．
*
* Return  : 初期化に成功すれば真，失敗すれば偽を返す．
*
*============================================================================*/
BOOL
gsInitSound
(
	HWND			hwnd			/* ウィンドウハンドラ       */
)
{
	if ( GSound == NULL )
	{
		/* ゲームシステム用サウンドの生成 */
		GSound = GSoundNew( hwnd );
	}

	if ( GSound != NULL )
	{
		/* ＭＩＤＩリソースの生成 */
		if ( MRes == NULL )
		{
			MRes = GMidiResNew( GSound );
		}

		/* ＷＡＶＥリソースの生成 */
		if ( WRes == NULL )
		{
			WRes = GWaveResNew( GSound );
		}

		/* ＢＧＭの初期化 */
		gsInitBGM( DEFAULT_BGM_MAX );

		/* ＳＥの初期化   */
		gsInitSE( DEFAULT_SE_MAX );

		return	TRUE;
	}

	return	FALSE;
}

/*=============================================================================
*
* Purpose : サウンドシステムの終了処理．
*
* Return  : なし．
*
*============================================================================*/
void
gsFinishSound
(
	void
)
{
	if ( GSound == NULL )
	{
		return;
	}

	/* ＭＩＤＩサウンドの停止 */
	GSoundStopMidi( GSound );

	/* ＷＡＶＥサウンドの停止 */
	GSoundStopWave( GSound );

	/* ＭＩＤＩリソース管理の消滅 */
	GMidiResDelete( MRes );

	/* ＷＡＶＥリソース管理の消滅 */
	GWaveResDelete( WRes );

	/* ゲームシステム用サウンドの消滅 */
	GSoundDelete( GSound );


	/* ハンドラを全て初期化 */
	GSound = NULL;
	MRes   = NULL;
	WRes   = NULL;
}

/*=============================================================================
*
* Purpose : ウィンドウがアクティブになった時の処理．
*
* Return  : なし．
*
*============================================================================*/
void
gsActivateSound
(
	BOOL		active				/* アクティブフラグ         */
)
{
	if ( GSound == NULL )
	{
		return;
	}

	GSoundActivateApp( GSound, active );
}

/*=============================================================================
*
* Purpose : 全てのサウンドを停止する．
*
* Return  : なし．
*
*============================================================================*/
void
gsStopSound
(
	void
)
{
	if ( GSound == NULL )
	{
		return;
	}

	/* ＭＩＤＩとＷＡＶＥの再生を停止 */
	GSoundStop( GSound );
}

/*=============================================================================
*
* Purpose : ＢＧＭの初期化．
*
* Return  : なし．
*
*============================================================================*/
void
gsInitBGM
(
	int				n				/* ＢＧＭリソースデータ数   */
)
{
	if ( GSound == NULL )
	{
		return;
	}

	/* ＢＧＭの停止 */
	gsStopBGM();

	/* ＢＧＭリソースの初期化 */
	GMidiResInit( MRes, n );
}

/*=============================================================================
*
* Purpose : ＢＧＭデータファイルの読み込み．
*
* Return  : 読み込みに成功すれば真，失敗すれば偽を返す．
*
*============================================================================*/
BOOL
gsLoadBGM
(
	GRESOURCEID		id,				/* ＢＧＭリソースＩＤ       */
	const char*		fname,			/* ＢＧＭファイル名         */
	BOOL			flag			/* ループ指定フラグ         */
)
{
	GMIDI			midi;

	if ( GSound == NULL )
	{
		return	FALSE;
	}

	/* ＭＩＤＩリソースの取得 */
	if ( ( midi = GMidiResGetResource( MRes, id ) ) != NULL )
	{
		if ( midi == Bgm )
		{
			/* 再生中のＢＧＭであれば停止する */
			gsStopBGM();
		}
		/* ＭＩＤＩリソースの解放 */
		GMidiResDelResource( MRes, id );
	}

	/* ＭＩＤＩリソースの読み込み */
	return	GMidiResLoadResource( MRes, id, fname, flag );
}

/*=============================================================================
*
* Purpose : ＢＧＭの解放．
*
* Return  : なし．
*
*============================================================================*/
void
gsDeleteBGM
(
	GRESOURCEID		id				/* ＢＧＭリソースＩＤ       */
)
{
	GMIDI			midi;

	if ( GSound == NULL )
	{
		return;
	}

	/* ＭＩＤＩリソースの取得 */
	if ( ( midi = GMidiResGetResource( MRes, id ) ) != NULL )
	{
		if ( midi == Bgm )
		{
			/* 再生中のＢＧＭであれば停止する */
			gsStopBGM();
		}
		/* ＭＩＤＩリソースの解放 */
		GMidiResDelResource( MRes, id );
	}
}

/*=============================================================================
*
* Purpose : ＢＧＭの再生．
*
* Return  : なし．
*
*============================================================================*/
void
gsPlayBGM
(
	GRESOURCEID		id				/* ＢＧＭリソースＩＤ   */
)
{
	GMIDI			midi;

	if ( GSound == NULL )
	{
		return;
	}

	/* ＭＩＤＩリソースの取得 */
	if ( ( midi = GMidiResGetResource( MRes, id ) ) == NULL )
	{
		return;
	}

	/* 既に再生中であるか調べる */
	if ( Bgm == midi )
	{
		return;
	}

	/* ＭＩＤＩサウンドの停止 */
	if ( Bgm != NULL )
	{
		GMidiStop( Bgm );
	}

	/* ＭＩＤＩサウンドの再生 */
	GMidiPlay( midi );

	/* 現在再生中のＢＧＭに設定 */
	Bgm = midi;
}

/*=============================================================================
*
* Purpose : ＢＧＭの停止．
*
* Return  : なし．
*
*============================================================================*/
void
gsStopBGM
(
	void
)
{
	/* ＭＩＤＩサウンドの停止 */
	if ( Bgm != NULL )
	{
		GMidiStop( Bgm );

		Bgm = NULL;
	}
}

/*=============================================================================
*
* Purpose : ＢＧＭの一時停止．
*
* Return  : なし．
*
*============================================================================*/
void
gsPauseBGM
(
	void
)
{
	if ( Bgm != NULL )
	{
		GMidiPause( Bgm );
	}
}

/*=============================================================================
*
* Purpose : ＢＧＭの一時停止からの再開．
*
* Return  : なし．
*
*============================================================================*/
void
gsRestartBGM
(
	void
)
{
	if ( Bgm != NULL )
	{
		GMidiRestart( Bgm );
	}
}

/*=============================================================================
*
* Purpose : ＢＧＭが再生中か調べる．
*
* Return  : 再生中であれば真，再生中でなければ偽を返す．
*
*============================================================================*/
BOOL
gsIsPlayBGM
(
	void
)
{
	if ( Bgm == NULL )
	{
		return	FALSE;
	}

	return	GMidiIsPlay( Bgm );
}

/*=============================================================================
*
* Purpose : ＢＧＭが一時停止中か調べる．
*
* Return  : 一時停止中であれば真，再生中であれば偽を返す．
*
*============================================================================*/
BOOL
gsIsPauseBGM
(
	void
)
{
	if ( Bgm == NULL )
	{
		return	FALSE;
	}

	return	GMidiIsPause( Bgm );
}

/*=============================================================================
*
* Purpose : ＳＥの初期化．
*
* Return  : なし．
*
*============================================================================*/
void
gsInitSE
(
	int				n				/* ＳＥリソースデータ数   */
)
{
	if ( GSound == NULL )
	{
		return;
	}

	/* ＷＡＶＥサウンドの停止 */
	GSoundStopWave( GSound );

	/* ＷＡＶＥリソースの初期化 */
	GWaveResInit( WRes, n );
}

/*=============================================================================
*
* Purpose : ＳＥデータファイルの読み込み．
*
* Return  : 読み込みに成功すれば真，失敗すれば偽を返す．
*
*============================================================================*/
BOOL
gsLoadSE
(
	GRESOURCEID		id,				/* ＳＥリソースＩＤ         */
	const char*		fname,			/* ＳＥデータファイル名     */
	int				cnt,			/* 同時再生数               */
	GWAVEFLAG		flag			/* 再生タイプフラグ         */
)
{
	GWAVE			wave;

	if ( GSound == NULL )
	{
		return	FALSE;
	}

	/* ＷＡＶＥリソースの取得 */
	if ( ( wave = GWaveResGetResource( WRes, id ) ) != NULL )
	{
		/* ＷＡＶＥリソースの解放 */
		GWaveResDelResource( WRes, id );
	}

	/* ＷＡＶＥリソースの読み込み */
	return	GWaveResLoadResource( WRes, id, fname, cnt, flag );
}

/*=============================================================================
*
* Purpose : ＳＥの解放．
*
* Return  : なし．
*
*============================================================================*/
void
gsDeleteSE
(
	GRESOURCEID		id				/* ＢＧＭリソースＩＤ       */
)
{
	GWAVE			wave;

	if ( WRes == NULL )
	{
		return;
	}

	/* ＷＡＶＥリソースの取得 */
	if ( ( wave = GWaveResGetResource( WRes, id ) ) != NULL )
	{
		/* ＷＡＶＥリソースの解放 */
		GWaveResDelResource( WRes, id );
	}
}

/*=============================================================================
*
* Purpose : ＳＥの再生．
*
* Return  : なし．
*
*============================================================================*/
void
gsPlaySE
(
	GRESOURCEID		id					/* ＳＥリソースＩＤ     */
)
{
	GWAVE			wave;

	if ( GSound == NULL )
	{
		return;
	}

	/* ＷＡＶＥリソースの取得 */
	if ( ( wave = GWaveResGetResource( WRes, id ) ) == NULL )
	{
		return;
	}

	/* ＷＡＶＥデータを再生 */
	GWavePlay( wave );
}

/*=============================================================================
*
* Purpose : ＳＥの停止．
*
* Return  : なし．
*
*============================================================================*/
void
gsStopSE
(
	GRESOURCEID		id					/* ＳＥリソースＩＤ     */
)
{
	GWAVE			wave;

	if ( GSound == NULL )
	{
		return;
	}

	/* ＷＡＶＥリソースの取得 */
	if ( ( wave = GWaveResGetResource( WRes, id ) ) == NULL )
	{
		return;
	}

	/* ＷＡＶＥデータを停止 */
	GWaveStop( wave );
}

/*=============================================================================
*
* Purpose : ＳＥが再生中か調べる．
*
* Return  : 再生中であれば真，再生中でなければ偽を返す．
*
*============================================================================*/
BOOL
gsIsPlaySE
(
	GRESOURCEID		id					/* ＳＥリソースＩＤ     */
)
{
	GWAVE			wave;

	if ( GSound == NULL )
	{
		return	FALSE;
	}

	/* ＷＡＶＥリソースの取得 */
	if ( ( wave = GWaveResGetResource( WRes, id ) ) == NULL )
	{
		return	FALSE;
	}

	return	GWaveIsPlay( wave );
}

/********** End of File ******************************************************/
