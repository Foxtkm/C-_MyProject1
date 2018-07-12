/******************************************************************************
* File Name : gfile.c                            Ver : 1.00  Date : 2002-07-31
*
* Description :
*
*       ゲームシステム用ファイル入出力．
*
* Author : Moriharu Ohzu.
*
******************************************************************************/
#include	"gfile.h"
#include	"heap.h"
#include	<stdio.h>
#include	<string.h>

/****** 定数宣言 *************************************************************/

#define	MAX_BUFFER_SIZE		256			/* 最大読み込みバッファサイズ */
#define	DELIMITER			" \n\r\t,"	/* デフォルトデリミタ		  */

/****** データ型宣言 *********************************************************/

/*----- ファイル構造体 ------------------------------------------------------*/

typedef	struct	GFile
{
	char*		pszFileName;			/* ファイル名				*/
	FILE*		pFile;					/* ファイルポインタ			*/
	GSboolean	bError;					/* エラーフラグ				*/
	GSboolean	bEOF;					/* ＥＯＦフラグ				*/
	char*		pszDelimiter;			/* デリミタ文字				*/

} GFile;

/****** 関数プロトタイプ宣言 *************************************************/

static GSboolean	GFileReadTxtFile( GFILE, const char*, void* );
static GSboolean	GFileReadBuffer( GFILE, char* );
static GSboolean	GFileReadSkipDelimiter( GFILE, char* );
static GSboolean	GFileSkipLine( GFILE );

/****** マクロ関数宣言 *******************************************************/

/*----- デリミタ文字のチェックマクロ ------------------------------------------*/

#define	isdelimiter( fp, c )									\
(																\
	( strchr( (fp)->pszDelimiter, (c) ) != NULL ) ? GS_TRUE: GS_FALSE	\
)

/*=============================================================================
*
* Purpose : ファイルのオープン．
*
* Return  : ファイルハンドラを返す．
*           ファイルのオープンに失敗した場合ＮＵＬＬを返す．
*
*============================================================================*/
extern GFILE
GFileOpen
(
	const char*		pszFileName,		/* ファイル名			*/
	const char*		pszMode				/* オープンモード		*/
)
{
	GFile*			pGFile;
	FILE*			fp;

	/* ファイルをオープンする */
	if ( ( fp = fopen( pszFileName, pszMode ) ) == NULL )
	{
		return	NULL;
	}

	/* ファイルを生成生成する */
	pGFile = new_object( sizeof(GFile) );

	pGFile->pszFileName = new_object( strlen( pszFileName ) + 1 );
	strcpy( pGFile->pszFileName, pszFileName );
	pGFile->pFile       = fp;
	pGFile->bError      = GS_FALSE;
	pGFile->bEOF        = GS_FALSE;

	/* デフォルトのデリミタ文字を設定する */
	pGFile->pszDelimiter = new_object( strlen( DELIMITER ) + 1 );
	strcpy( pGFile->pszDelimiter, DELIMITER );

	return	(GFILE)pGFile;
}

/*=============================================================================
*
* Purpose : ファイルのクローズ．
*
* Return  : クローズに成功すれば真，失敗した場合は偽を返す．
*
*============================================================================*/
extern GSboolean
GFileClose
(
	GFILE			hnd					/* ファイルハンドラ		*/
)
{
	GFile*		pGFile = (GFile*)hnd;
	int			Error;

	/* ファイルをクローズする */
	Error = fclose( pGFile->pFile );

	/* デリミタ文字を消滅させる */
	del_object( pGFile->pszDelimiter );

	/* ファイル名を消滅させる */
	del_object( pGFile->pszFileName );

	/* ファイルを消滅させる */
	del_object( pGFile );

	if ( Error != 0 )
	{
		return	GS_FALSE;
	}

	return	GS_TRUE;
}

/*=============================================================================
*
* Purpose : ファイルアクセスでエラーがおきたか調べる．
*
* Return  : エラーであれば真，エラーでなければ偽を返す．
*
*============================================================================*/
extern GSboolean
GFileIsError
(
	GFILE			hnd					/* ファイルハンドラ		*/
)
{
	GFile*		pGFile = (GFile*)hnd;

	return	pGFile->bError;
}

/*=============================================================================
*
* Purpose : ファイルアクセスが成功したか調べる．
*
* Return  : 成功であれば真，失敗であれば偽を返す．
*
*============================================================================*/
extern GSboolean
GFileIsSuccess
(
	GFILE			hnd					/* ファイルハンドラ		*/
)
{
	GFile*		pGFile = (GFile*)hnd;

	if ( pGFile->bError == GS_FALSE )
	{
		return	GS_TRUE;
	}

	return	GS_FALSE;
}

/*=============================================================================
*
* Purpose : ファイルがＥＯＦまで達したか調べる．
*
* Return  : ＥＯＦであれば真，そうでなければ偽を返す．
*
*============================================================================*/
extern GSboolean
GFileIsEOF
(
	GFILE			hnd					/* ファイルハンドラ		*/
)
{
	GFile*		pGFile = (GFile*)hnd;

	return	pGFile->bEOF;
}

/*=============================================================================
*
* Purpose : ファイル名の取得．
*
* Return  : ファイル名の先頭アドレスを返す．
*
*============================================================================*/
extern char*
GFileGetFileName
(
	GFILE			hnd					/* ファイルハンドラ		*/
)
{
	GFile*		pGFile = (GFile*)hnd;

	return	pGFile->pszFileName;
}

/*=============================================================================
*
* Purpose : デリミタ文字の設定．
*
* Return  : なし．
*
*============================================================================*/
extern void
GFileSetDelimiter
(
	GFILE			hnd,				/* ファイルハンドラ		*/
	const char*		pszDelimiter		/* デリミタ文字			*/
)
{
	GFile*		pGFile = (GFile*)hnd;

	/* 現在のデリミタ文字を消滅させる */
	del_object( pGFile->pszDelimiter );

	/* 新しいデリミタ文字を設定する */
	pGFile->pszDelimiter = new_object( strlen( pszDelimiter ) + 1 );
	strcpy( pGFile->pszDelimiter, pszDelimiter );
}

/*=============================================================================
*
* Purpose : １バイトサイズのデータを読み込む．
*
* Return  : 読み込みに成功すれば真，失敗すると偽を返す．
*
*============================================================================*/
extern GSboolean
GFileReadByte
(
	GFILE			hnd,				/* ファイルハンドラ		*/
	GSubyte*		pByte				/*-> 読み込んだデータ	*/
)
{
	GSuint			input;
	GSboolean		Success;

	if ( ( Success = GFileReadTxtFile( hnd, "%d", &input ) ) == GS_TRUE )
	{
		*pByte = (GSubyte)input;
	}

	return	Success;
}

/*=============================================================================
*
* Purpose : ２バイトサイズのデータを読み込む．
*
* Return  : 読み込みに成功すれば真，失敗すると偽を返す．
*
*============================================================================*/
extern GSboolean
GFileReadWord
(
	GFILE			hnd,				/* ファイルハンドラ		*/
	GSushort*		pWord				/*-> 読み込んだデータ	*/
)
{
	GSuint			input;
	GSboolean		Success;

	if ( ( Success = GFileReadTxtFile( hnd, "%d", &input ) ) == GS_TRUE )
	{
		*pWord = (GSushort)input;

		return	GS_TRUE;
	}

	return	Success;
}

/*=============================================================================
*
* Purpose : ４バイトサイズのデータを読み込む．
*
* Return  : 読み込みに成功すれば真，失敗すると偽を返す．
*
*============================================================================*/
extern GSboolean
GFileReadDword
(
	GFILE			hnd,				/* ファイルハンドラ		*/
	GSuint*			pDword				/*-> 読み込んだデータ	*/
)
{
	return	GFileReadTxtFile( hnd, "%d", pDword );
}

/*=============================================================================
*
* Purpose : １バイトサイズのデータを読み込む．（１６進数）
*
* Return  : 読み込みに成功すれば真，失敗すると偽を返す．
*
*============================================================================*/
extern GSboolean
GFileReadByteHex
(
	GFILE			hnd,				/* ファイルハンドラ		*/
	GSubyte*		pByte				/*-> 読み込んだデータ	*/
)
{
	GSuint			input;
	GSboolean		Success;

	if ( ( Success = GFileReadTxtFile( hnd, "%x", &input ) ) == GS_TRUE )
	{
		*pByte = (GSubyte)input;
	}

	return	Success;
}

/*=============================================================================
*
* Purpose : ２バイトサイズのデータを読み込む．（１６進数）
*
* Return  : 読み込みに成功すれば真，失敗すると偽を返す．
*
*============================================================================*/
extern GSboolean
GFileReadWordHex
(
	GFILE			hnd,				/* ファイルハンドラ		*/
	GSushort*			pWord				/*-> 読み込んだデータ	*/
)
{
	GSuint			input;
	GSboolean		Success;

	if ( ( Success = GFileReadTxtFile( hnd, "%x", &input ) ) == GS_TRUE )
	{
		*pWord = (GSushort)input;
	}

	return	Success;
}

/*=============================================================================
*
* Purpose : ４バイトサイズのデータを読み込む．（１６進数）
*
* Return  : 読み込みに成功すれば真，失敗すると偽を返す．
*
*============================================================================*/
extern GSboolean
GFileReadDwordHex
(
	GFILE			hnd,				/* ファイルハンドラ		*/
	GSuint*			pDword				/*-> 読み込んだデータ	*/
)
{
	return	GFileReadTxtFile( hnd, "%x", pDword );
}

/*=============================================================================
*
* Purpose : 浮動小数点のデータを読み込む．
*
* Return  : 読み込みに成功すれば真，失敗すると偽を返す．
*
*============================================================================*/
extern GSboolean
GFileReadFloat
(
	GFILE			hnd,				/* ファイルハンドラ		*/
	GSfloat*		pFloat				/*-> 読み込んだデータ	*/
)
{
	return	GFileReadTxtFile( hnd, "%f", pFloat );
}

/*=============================================================================
*
* Purpose : 文字列のデータを読み込む．
*
* Return  : 読み込みに成功すれば真，失敗すると偽を返す．
*
*============================================================================*/
extern GSboolean
GFileReadString
(
	GFILE			hnd,				/* ファイルハンドラ		*/
	char*			pszString			/*-> 読み込んだデータ	*/
)
{
	return	GFileReadTxtFile( hnd, "%s", pszString );
}

/*=============================================================================
*
* Purpose : ３Ｄベクトル型のデータを読み込む．
*
* Return  : 読み込みに成功すれば真，失敗すると偽を返す．
*
*============================================================================*/
extern GSboolean
GFileReadVector3
(
	GFILE			hnd,				/* ファイルハンドラ		*/
	GSvector3*		pVector3d			/*-> 読み込んだデータ	*/
)
{
	GFileReadFloat( hnd, &pVector3d->x );
	GFileReadFloat( hnd, &pVector3d->y );
	GFileReadFloat( hnd, &pVector3d->z );

	return	GFileIsSuccess( hnd );
}

/*=============================================================================
*
* Purpose : クォータニオン型のデータを読み込む．
*
* Return  : 読み込みに成功すれば真，失敗すると偽を返す．
*
*============================================================================*/
extern GSboolean
GFileReadQuaternion
(
	GFILE			hnd,				/* ファイルハンドラ		*/
	GSquaternion*	pQuaternion			/*-> 読み込んだデータ	*/
)
{
	GFileReadFloat( hnd, &pQuaternion->x );
	GFileReadFloat( hnd, &pQuaternion->y );
	GFileReadFloat( hnd, &pQuaternion->z );
	GFileReadFloat( hnd, &pQuaternion->w );

	return	GFileIsSuccess( hnd );
}

/*=============================================================================
*
* Purpose : テクスチャ座標型のデータを読み込む．
*
* Return  : 読み込みに成功すれば真，失敗すると偽を返す．
*
*============================================================================*/
extern GSboolean
GFileReadTexCoord
(
	GFILE			hnd,				/* ファイルハンドラ		*/
	GSvector2*		pTexcood			/*-> 読み込んだデータ	*/
)
{
	GFileReadFloat( hnd, &pTexcood->s );
	GFileReadFloat( hnd, &pTexcood->t );

	return	GFileIsSuccess( hnd );
}

/*=============================================================================
*
* Purpose : カラー型のデータを読み込む．
*
* Return  : 読み込みに成功すれば真，失敗すると偽を返す．
*
*============================================================================*/
extern GSboolean
GFileReadColor
(
	GFILE			hnd,				/* ファイルハンドラ		*/
	GScolor*		pColor				/*-> 読み込んだデータ	*/
)
{
	GFileReadFloat( hnd, &pColor->r );
	GFileReadFloat( hnd, &pColor->g );
	GFileReadFloat( hnd, &pColor->b );
	GFileReadFloat( hnd, &pColor->a );

	return	GFileIsSuccess( hnd );
}

/*=============================================================================
*
* Purpose : マトリクス型のデータを読み込む．
*
* Return  : 読み込みに成功すれば真，失敗すると偽を返す．
*
*============================================================================*/
extern GSboolean
GFileReadMatrix4
(
	GFILE			hnd,				/* ファイルハンドラ		*/
	GSmatrix4*		pMatrix				/*-> 読み込んだデータ	*/
)
{
	int			i;
	int			j;

	for ( i = 0; i < 4; i++ )
	{
		for ( j = 0; j < 4; j++ )
		{
			GFileReadFloat( hnd, &pMatrix->m[i][j] );
		}
	}

	return	GFileIsSuccess( hnd );
}

/*=============================================================================
*
* Purpose : １バイトサイズのデータ配列を読み込む．
*
* Return  : 読み込みに成功すれば真，失敗すると偽を返す．
*
*============================================================================*/
extern GSboolean
GFileReadByteArray
(
	GFILE			hnd,				/* ファイルハンドラ		*/
	GSuint			numByte,			/* 配列の要素数			*/
	GSubyte*		pByteArray			/* 配列の先頭アドレス	*/
)
{
	GSuint		i;

	if ( pByteArray != NULL )
	{
		for ( i = 0; i < numByte; i++ )
		{
			GFileReadByte( hnd, &pByteArray[i] );
		}
	}

	return	GFileIsSuccess( hnd );
}

/*=============================================================================
*
* Purpose : ２バイトサイズのデータ配列を読み込む．
*
* Return  : 読み込みに成功すれば真，失敗すると偽を返す．
*
*============================================================================*/
extern GSboolean
GFileReadWordArray
(
	GFILE			hnd,				/* ファイルハンドラ		*/
	GSuint			numWord,			/* 配列の要素数			*/
	GSushort*			pWordArray			/* 配列の先頭アドレス	*/
)
{
	GSuint		i;

	if ( pWordArray != NULL )
	{
		for ( i = 0; i < numWord; i++ )
		{
			GFileReadWord( hnd, &pWordArray[i] );
		}
	}

	return	GFileIsSuccess( hnd );
}

/*=============================================================================
*
* Purpose : ４バイトサイズのデータ配列を読み込む．
*
* Return  : 読み込みに成功すれば真，失敗すると偽を返す．
*
*============================================================================*/
extern GSboolean
GFileReadDwordArray
(
	GFILE			hnd,				/* ファイルハンドラ		*/
	GSuint			numDword,			/* 配列の要素数			*/
	GSuint*			pDwordArray			/* 配列の先頭アドレス	*/
)
{
	GSuint			i;

	if ( pDwordArray != NULL )
	{
		for ( i = 0; i < numDword; i++ )
		{
			GFileReadDword( hnd, &pDwordArray[i] );
		}
	}

	return	GFileIsSuccess( hnd );
}

/*=============================================================================
*
* Purpose : １バイトサイズのデータ配列を読み込む．（１６進数）
*
* Return  : 読み込みに成功すれば真，失敗すると偽を返す．
*
*============================================================================*/
extern GSboolean
GFileReadByteHexArray
(
	GFILE			hnd,				/* ファイルハンドラ		*/
	GSuint			numByte,			/* 配列の要素数			*/
	GSubyte*		pByteArray			/* 配列の先頭アドレス	*/
)
{
	GSuint			i;

	if ( pByteArray != NULL )
	{
		for ( i = 0; i < numByte; i++ )
		{
			GFileReadByteHex( hnd, &pByteArray[i] );
		}
	}

	return	GFileIsSuccess( hnd );
}

/*=============================================================================
*
* Purpose : ２バイトサイズのデータ配列を読み込む．（１６進数）
*
* Return  : 読み込みに成功すれば真，失敗すると偽を返す．
*
*============================================================================*/
extern GSboolean
GFileReadWordHexArray
(
	GFILE			hnd,				/* ファイルハンドラ		*/
	GSuint			numWord,			/* 配列の要素数			*/
	GSushort*		pWordArray			/* 配列の先頭アドレス	*/
)
{
	GSuint			i;

	if ( pWordArray != NULL )
	{
		for ( i = 0; i < numWord; i++ )
		{
			GFileReadWordHex( hnd, &pWordArray[i] );
		}
	}

	return	GFileIsSuccess( hnd );
}

/*=============================================================================
*
* Purpose : ４バイトサイズのデータ配列を読み込む．（１６進数）
*
* Return  : 読み込みに成功すれば真，失敗すると偽を返す．
*
*============================================================================*/
extern GSboolean
GFileReadDwordHexArray
(
	GFILE			hnd,				/* ファイルハンドラ		*/
	GSuint			numDword,			/* 配列の要素数			*/
	GSuint*			pDwordArray			/* 配列の先頭アドレス	*/
)
{
	GSuint			i;

	if ( pDwordArray != NULL )
	{
		for ( i = 0; i < numDword; i++ )
		{
			GFileReadDwordHex( hnd, &pDwordArray[i] );
		}
	}

	return	GFileIsSuccess( hnd );
}

/*=============================================================================
*
* Purpose : 浮動小数点のデータ配列を読み込む．
*
* Return  : 読み込みに成功すれば真，失敗すると偽を返す．
*
*============================================================================*/
extern GSboolean
GFileReadFloatArray
(
	GFILE			hnd,				/* ファイルハンドラ		*/
	GSuint			numFloat,			/* 配列の要素数			*/
	GSfloat*		pFloatArray			/* 配列の先頭アドレス	*/
)
{
	GSuint			i;

	if ( pFloatArray != NULL )
	{
		for ( i = 0; i < numFloat; i++ )
		{
			GFileReadFloat( hnd, &pFloatArray[i] );
		}
	}

	return	GFileIsSuccess( hnd );
}

/*=============================================================================
*
* Purpose : ３Ｄ座標型のデータ配列を読み込む．
*
* Return  : 読み込みに成功すれば真，失敗すると偽を返す．
*
*============================================================================*/
extern GSboolean
GFileReadVector3Array
(
	GFILE			hnd,				/* ファイルハンドラ		*/
	GSuint			numVector3,			/* 配列の要素数			*/
	GSvector3*		pVector3Array		/* 配列の先頭アドレス	*/
)
{
	GSuint			i;

	if ( pVector3Array != NULL )
	{
		for ( i = 0; i < numVector3; i++ )
		{
			GFileReadVector3( hnd, &pVector3Array[i] );
		}
	}

	return	GFileIsSuccess( hnd );
}

/*=============================================================================
*
* Purpose : カラー型のデータ配列を読み込む．
*
* Return  : 読み込みに成功すれば真，失敗すると偽を返す．
*
*============================================================================*/
extern GSboolean
GFileReadColorArray
(
	GFILE			hnd,				/* ファイルハンドラ		*/
	GSuint			numColorArray,		/* 配列の要素数			*/
	GScolor*		pColorArray			/* 配列の先頭アドレス	*/
)
{
	GSuint			i;

	if ( pColorArray != NULL )
	{
		for ( i = 0; i < numColorArray; i++ )
		{
			GFileReadColor( hnd, &pColorArray[i] );
		}
	}

	return	GFileIsSuccess( hnd );
}

/*=============================================================================
*
* Purpose : テクスチャ座標型のデータ配列を読み込む．
*
* Return  : 読み込みに成功すれば真，失敗すると偽を返す．
*
*============================================================================*/
extern GSboolean
GFileReadTexCoordArray
(
	GFILE			hnd,				/* ファイルハンドラ		*/
	GSuint			numTexCoord,		/* 配列の要素数			*/
	GSvector2*		pTexCoordArray		/* 配列の先頭アドレス	*/
)
{
	GSuint			i;

	if ( pTexCoordArray != NULL )
	{
		for ( i = 0; i < numTexCoord; i++ )
		{
			GFileReadTexCoord( hnd, &pTexCoordArray[i] );
		}
	}

	return	GFileIsSuccess( hnd );
}

/*=============================================================================
*
* Purpose : １バイトサイズのデータを読み込む．(バイナリ形式）
*
* Return  : 読み込みに成功すれば真，失敗すると偽を返す．
*
*============================================================================*/
extern GSboolean
GFileReadByteBin
(
	GFILE			hnd,				/* ファイルハンドラ		*/
	GSubyte*		pByte				/*-> 読み込んだデータ	*/
)
{
	GFile*		pGFile = (GFile*)hnd;
	if (fread(pByte, sizeof(GSubyte), 1, pGFile->pFile) != 1)
	{
		pGFile->bError = GS_TRUE;
	}
	return GFileIsSuccess(hnd);
}

/*=============================================================================
*
* Purpose : ２バイトサイズのデータを読み込む．(バイナリ形式）
*
* Return  : 読み込みに成功すれば真，失敗すると偽を返す．
*
*============================================================================*/
extern GSboolean
GFileReadWordBin
(
	GFILE			hnd,				/* ファイルハンドラ		*/
	GSushort*		pWord				/*-> 読み込んだデータ	*/
)
{
	GFile*		pGFile = (GFile*)hnd;
	if (fread(pWord, sizeof(GSushort), 1, pGFile->pFile) != 1)
	{
		pGFile->bError = GS_TRUE;
	}
	return GFileIsSuccess(hnd);
}

/*=============================================================================
*
* Purpose : ４バイトサイズのデータを読み込む．(バイナリ形式）
*
* Return  : 読み込みに成功すれば真，失敗すると偽を返す．
*
*============================================================================*/
extern GSboolean
GFileReadDwordBin
(
	GFILE			hnd,				/* ファイルハンドラ		*/
	GSuint*			pDword				/*-> 読み込んだデータ	*/
)
{
	GFile*		pGFile = (GFile*)hnd;
	if (fread(pDword, sizeof(GSuint), 1, pGFile->pFile) != 1)
	{
		pGFile->bError = GS_TRUE;
	}
	return GFileIsSuccess(hnd);
}


/*=============================================================================
*
* Purpose : 浮動小数点のデータを読み込む．(バイナリ形式）
*
* Return  : 読み込みに成功すれば真，失敗すると偽を返す．
*
*============================================================================*/
extern GSboolean
GFileReadFloatBin
(
	GFILE			hnd,				/* ファイルハンドラ		*/
	GSfloat*		pFloat				/*-> 読み込んだデータ	*/
)
{
	GFile*		pGFile = (GFile*)hnd;
	if (fread(pFloat, sizeof(GSfloat), 1, pGFile->pFile) != 1)
	{
		pGFile->bError = GS_TRUE;
	}
	return GFileIsSuccess(hnd);
}

/*=============================================================================
*
* Purpose : 文字列のデータを読み込む．(バイナリ形式）
*
* Return  : 読み込みに成功すれば真，失敗すると偽を返す．
*
*============================================================================*/
extern GSboolean
GFileReadStringBin
(
	GFILE			hnd,				/* ファイルハンドラ		*/
	GSuint			len,				/* 文字列配列のサイズ	*/
	char*			pszString			/*-> 読み込んだデータ	*/
)
{
	GFile*		pGFile = (GFile*)hnd;
	if (fread(pszString, sizeof(char), len, pGFile->pFile) != len)
	{
		pGFile->bError = GS_TRUE;
	}
	return GFileIsSuccess(hnd);
}

/*=============================================================================
*
* Purpose : ３Ｄベクトル型のデータを読み込む．(バイナリ形式）
*
* Return  : 読み込みに成功すれば真，失敗すると偽を返す．
*
*============================================================================*/
extern GSboolean
GFileReadVector3Bin
(
	GFILE			hnd,				/* ファイルハンドラ		*/
	GSvector3*		pVector3d			/*-> 読み込んだデータ	*/
)
{
	GFile*		pGFile = (GFile*)hnd;
	if (fread(pVector3d, sizeof(GSvector3), 1, pGFile->pFile) != 1)
	{
		pGFile->bError = GS_TRUE;
	}
	return GFileIsSuccess(hnd);
}

/*=============================================================================
*
* Purpose : クォータニオン型のデータを読み込む．(バイナリ形式）
*
* Return  : 読み込みに成功すれば真，失敗すると偽を返す．
*
*============================================================================*/
extern GSboolean
GFileReadQuaternionBin
(
	GFILE			hnd,				/* ファイルハンドラ		*/
	GSquaternion*	pQuaternion			/*-> 読み込んだデータ	*/
)
{
	GFile*		pGFile = (GFile*)hnd;
	if (fread(pQuaternion, sizeof(GSquaternion), 1, pGFile->pFile) != 1)
	{
		pGFile->bError = GS_TRUE;
	}
	return GFileIsSuccess(hnd);
}

/*=============================================================================
*
* Purpose : テクスチャ座標型のデータを読み込む．(バイナリ形式）
*
* Return  : 読み込みに成功すれば真，失敗すると偽を返す．
*
*============================================================================*/
extern GSboolean
GFileReadTexCoordBin
(
	GFILE			hnd,				/* ファイルハンドラ		*/
	GSvector2*		pTexcood			/*-> 読み込んだデータ	*/
)
{
	GFile*		pGFile = (GFile*)hnd;
	if (fread(pTexcood, sizeof(GSvector2), 1, pGFile->pFile) != 1)
	{
		pGFile->bError = GS_TRUE;
	}
	return GFileIsSuccess(hnd);
}

/*=============================================================================
*
* Purpose : カラー型のデータを読み込む．(バイナリ形式）
*
* Return  : 読み込みに成功すれば真，失敗すると偽を返す．
*
*============================================================================*/
extern GSboolean
GFileReadColorBin
(
	GFILE			hnd,				/* ファイルハンドラ		*/
	GScolor*		pColor				/*-> 読み込んだデータ	*/
)
{
	GFile*		pGFile = (GFile*)hnd;
	if (fread(pColor, sizeof(GScolor), 1, pGFile->pFile) != 1)
	{
		pGFile->bError = GS_TRUE;
	}
	return GFileIsSuccess(hnd);
}

/*=============================================================================
*
* Purpose : マトリクス型のデータを読み込む．(バイナリ形式）
*
* Return  : 読み込みに成功すれば真，失敗すると偽を返す．
*
*============================================================================*/
extern GSboolean
GFileReadMatrix4Bin
(
	GFILE			hnd,				/* ファイルハンドラ		*/
	GSmatrix4*		pMatrix				/*-> 読み込んだデータ	*/
)
{
	GFile*		pGFile = (GFile*)hnd;
	if (fread(pMatrix, sizeof(GSmatrix4), 1, pGFile->pFile) != 1)
	{
		pGFile->bError = GS_TRUE;
	}
	return GFileIsSuccess(hnd);
}

/*=============================================================================
*
* Purpose : １バイトサイズのデータ配列を読み込む．(バイナリ形式）
*
* Return  : 読み込みに成功すれば真，失敗すると偽を返す．
*
*============================================================================*/
extern GSboolean
GFileReadByteArrayBin
(
	GFILE			hnd,				/* ファイルハンドラ		*/
	GSuint			numByte,			/* 配列の要素数			*/
	GSubyte*		pByteArray			/* 配列の先頭アドレス	*/
)
{
	GFile*	pGFile = (GFile*)hnd;
	if (fread(pByteArray, sizeof(GSubyte), numByte, pGFile->pFile) != numByte)
	{
		pGFile->bError = GS_TRUE;
	}
	return GFileIsSuccess(hnd);
}

/*=============================================================================
*
* Purpose : ２バイトサイズのデータ配列を読み込む．(バイナリ形式）
*
* Return  : 読み込みに成功すれば真，失敗すると偽を返す．
*
*============================================================================*/
extern GSboolean
GFileReadWordArrayBin
(
	GFILE			hnd,				/* ファイルハンドラ		*/
	GSuint			numWord,			/* 配列の要素数			*/
	GSushort*		pWordArray			/* 配列の先頭アドレス	*/
)
{
	GFile*	pGFile = (GFile*)hnd;
	if (fread(pWordArray, sizeof(GSushort), numWord, pGFile->pFile) != numWord)
	{
		pGFile->bError = GS_TRUE;
	}
	return GFileIsSuccess(hnd);
}

/*=============================================================================
*
* Purpose : ４バイトサイズのデータ配列を読み込む．(バイナリ形式）
*
* Return  : 読み込みに成功すれば真，失敗すると偽を返す．
*
*============================================================================*/
extern GSboolean
GFileReadDwordArrayBin
(
	GFILE			hnd,				/* ファイルハンドラ		*/
	GSuint			numDword,			/* 配列の要素数			*/
	GSuint*			pDwordArray			/* 配列の先頭アドレス	*/
)
{
	GFile*	pGFile = (GFile*)hnd;
	if (fread(pDwordArray, sizeof(GSuint), numDword, pGFile->pFile) != numDword)
	{
		pGFile->bError = GS_TRUE;
	}
	return GFileIsSuccess(hnd);
}

/*=============================================================================
*
* Purpose : 浮動小数点のデータ配列を読み込む．(バイナリ形式）
*
* Return  : 読み込みに成功すれば真，失敗すると偽を返す．
*
*============================================================================*/
extern GSboolean
GFileReadFloatArrayBin
(
	GFILE			hnd,				/* ファイルハンドラ		*/
	GSuint			numFloat,			/* 配列の要素数			*/
	GSfloat*		pFloatArray			/* 配列の先頭アドレス	*/
)
{
	GFile*	pGFile = (GFile*)hnd;
	if (fread(pFloatArray, sizeof(GSfloat), numFloat, pGFile->pFile) != numFloat)
	{
		pGFile->bError = GS_TRUE;
	}
	return GFileIsSuccess(hnd);
}

/*=============================================================================
*
* Purpose : ３Ｄ座標型のデータ配列を読み込む．(バイナリ形式）
*
* Return  : 読み込みに成功すれば真，失敗すると偽を返す．
*
*============================================================================*/
extern GSboolean
GFileReadVector3ArrayBin
(
	GFILE			hnd,				/* ファイルハンドラ		*/
	GSuint			numVector3,			/* 配列の要素数			*/
	GSvector3*		pVector3Array		/* 配列の先頭アドレス	*/
)
{
	GFile*	pGFile = (GFile*)hnd;
	if (fread(pVector3Array, sizeof(GSvector3), numVector3, pGFile->pFile) != numVector3)
	{
		pGFile->bError = GS_TRUE;
	}
	return GFileIsSuccess(hnd);
}

/*=============================================================================
*
* Purpose : カラー型のデータ配列を読み込む．(バイナリ形式）
*
* Return  : 読み込みに成功すれば真，失敗すると偽を返す．
*
*============================================================================*/
extern GSboolean
GFileReadColorArrayBin
(
	GFILE			hnd,				/* ファイルハンドラ		*/
	GSuint			numColorArray,		/* 配列の要素数			*/
	GScolor*		pColorArray			/* 配列の先頭アドレス	*/
)
{
	GFile*	pGFile = (GFile*)hnd;
	if (fread(pColorArray, sizeof(GScolor), numColorArray, pGFile->pFile) != numColorArray)
	{
		pGFile->bError = GS_TRUE;
	}
	return GFileIsSuccess(hnd);
}

/*=============================================================================
*
* Purpose : テクスチャ座標型のデータ配列を読み込む．(バイナリ形式）
*
* Return  : 読み込みに成功すれば真，失敗すると偽を返す．
*
*============================================================================*/
extern GSboolean
GFileReadTexCoordArrayBin
(
	GFILE			hnd,				/* ファイルハンドラ		*/
	GSuint			numTexCoord,		/* 配列の要素数			*/
	GSvector2*		pTexCoordArray		/* 配列の先頭アドレス	*/
)
{
	GFile*	pGFile = (GFile*)hnd;
	if (fread(pTexCoordArray, sizeof(GSvector2), numTexCoord, pGFile->pFile) != numTexCoord)
	{
		pGFile->bError = GS_TRUE;
	}
	return GFileIsSuccess(hnd);
}

/*-----------------------------------------------------------------------------
*
* Purpose : テキストファイルのデータを読み込む．
*
* Return  : 読み込みに成功すれば真，失敗すると偽を返す．
*
*----------------------------------------------------------------------------*/
static GSboolean
GFileReadTxtFile
(
	GFILE			hnd,				/* ファイルハンドラ		*/
	const char*		farmat,				/* 変換フォーマット指定	*/
	void*			pinput				/*-> 読み込んだデータ	*/
)
{
	GFile*			pGFile = (GFile*)hnd;
	char			buffer[MAX_BUFFER_SIZE];

	/* 文字列の読み込み */
	if ( GFileReadBuffer( hnd, buffer ) == GS_FALSE )
	{
		return	GS_FALSE;
	}

	/* 文字列を指定のフォーマットで変換する */
	if ( sscanf( buffer, farmat, pinput ) == 0 )
	{
		/* 変換ができなければエラー */
		pGFile->bError = GS_TRUE;
	}

	return	GFileIsSuccess( hnd );
}

/*-----------------------------------------------------------------------------
*
* Purpose : 読み込みバッファにデータを読み込む．
*
* Return  : 読み込みに成功すれば真，失敗すると偽を返す．
*
*----------------------------------------------------------------------------*/
static GSboolean
GFileReadBuffer
(
	GFILE			hnd,				/* ファイルハンドラ			*/
	char*			pbuffer				/*-> 読み込みバッファ		*/
)
{
	GFile*			pGFile = (GFile*)hnd;
	char			c;
	int				ic;

	/* 既エラー状態であるか？ */
	if ( GFileIsError( hnd ) == GS_TRUE )
	{
		return	GS_FALSE;
	}

	/* 既にＥＯＦなのに読み込んでしまうとエラー */
	if ( GFileIsEOF( hnd ) == GS_TRUE )
	{
		pGFile->bError = GS_TRUE;

		return	GS_FALSE;
	}

	/* デリミタの文字はスキップする */
	if ( GFileReadSkipDelimiter( hnd, &c ) == GS_FALSE )
	{
		return	GS_FALSE;
	}

	/* デリミタまで読み込む */
	*pbuffer++ = c;
	while ( GS_TRUE )
	{
		/* 一文字読み込む */
		ic = fgetc( pGFile->pFile );

		/* ＥＯＦかどうか？ */
		if ( feof( pGFile->pFile ) != 0 )
		{
			pGFile->bEOF = GS_TRUE;

			break;
		}

		/* エラーであるか？ */
		if ( ferror( pGFile->pFile ) != 0 )
		{
			pGFile->bError = GS_TRUE;

			break;
		}

		/* デリミタ文字か？ */
		if ( isdelimiter( pGFile, ic ) == GS_TRUE )
		{
			break;
		}

		*pbuffer++ = (char)ic;
	}
	*pbuffer = '\0';

	return	GFileIsSuccess( hnd );
}

/*-----------------------------------------------------------------------------
*
* Purpose : デリミタとコメントを読み飛ばして１文字読み込み．
*
* Return  : 読み込みに成功すれば真，失敗すると偽を返す．
*
*----------------------------------------------------------------------------*/
static GSboolean
GFileReadSkipDelimiter
(
	GFILE			hnd,				/* ファイルハンドラ			*/
	char*			pChar				/*-> 読み込んだ文字			*/
)
{
	GFile*			pGFile = (GFile*)hnd;
	int				c;

	/* デリミタ文字をスキップする */
	do
	{
		/* 一文字読み込む */
		c = fgetc( pGFile->pFile );

		/* ＥＯＦかどうか？ */
		if ( feof( pGFile->pFile ) != 0 )
		{
			pGFile->bEOF = GS_TRUE;

			break;
		}

		/* エラーかどうか？ */
		if ( ferror( pGFile->pFile ) != 0 )
		{
			pGFile->bError = GS_TRUE;

			break;
		}

		/* もし，コメントであれば */
		if ( c == '#' )
		{
			/* コメント行は行末までスキップする */
			GFileSkipLine( pGFile );

			/* 行末は改行文字である */
			c = '\n';
		}
	} while ( isdelimiter( pGFile, c ) == GS_TRUE );

	/* ＥＯＦまたは，エラーか？ */
	if ( GFileIsEOF( pGFile ) == GS_TRUE || GFileIsError( pGFile ) == GS_TRUE )
	{
		return	GS_FALSE;
	}

	*pChar = (char)c;

	return	GS_TRUE;
}

/*-----------------------------------------------------------------------------
*
* Purpose : 行末まで読み飛ばすする．
*
* Return  : 読み飛ばすのに成功すれば真，失敗すると偽を返す．
*
*----------------------------------------------------------------------------*/
static GSboolean
GFileSkipLine
(
	GFILE		hnd						/* ファイルハンドラ			*/
)
{
	GFile*			pGFile = (GFile*)hnd;

	/* EOFかどうか？ */
	while ( feof( pGFile->pFile ) == 0 )
	{
		/* エラー状態であるか？ */
		if ( ferror( pGFile->pFile ) != 0 )
		{
			return	GS_FALSE;
		}
		/* 改行コードが入力されるまでスキップする */
		if ( fgetc( pGFile->pFile ) == '\n' )
		{
			return	GS_TRUE;
		}
	}

	return	GS_FALSE;
}

/********** End of File ******************************************************/
