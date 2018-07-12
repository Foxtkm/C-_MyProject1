/******************************************************************************
* File Name : GSload_bmp.c                     Ver : 1.00  Date : 2002-07-31
*
* Description :
*
*       ビットマップファイルからテクスチャデータを読み込む．
*
* Author : Moriharu Ohzu.
*
******************************************************************************/
#include	"GSload_bmp.h"
#include	<windows.h>
#include	<stdio.h>

/****** 関数プロトタイプ宣言 *************************************************/

static BOOL	 fread_bitmapfileheader( FILE*, BITMAPFILEHEADER* );
static BOOL	 fread_bitmapbfinfheader( FILE*, BITMAPINFOHEADER* );
static BOOL	 fread_rgbquad( FILE*, RGBQUAD*, DWORD );
static BOOL	 fread_image( FILE*, GSvoid*, BITMAPFILEHEADER*, BITMAPINFOHEADER*, RGBQUAD* );
static BOOL	 fread_image_2bit( FILE*, GSvoid*, BITMAPINFOHEADER*, RGBQUAD* );
static BOOL	 fread_image_4bit( FILE*, GSvoid*, BITMAPINFOHEADER*, RGBQUAD* );
static BOOL	 fread_image_8bit( FILE*, GSvoid*, BITMAPINFOHEADER*, RGBQUAD* );
static BOOL	 fread_image_24bit( FILE*, GSvoid*, BITMAPINFOHEADER* );
static BOOL  fread_image_32bit( FILE*, GSvoid*, BITMAPINFOHEADER* );
static DWORD calc_palette_ncolors( BITMAPINFOHEADER* );
static DWORD calc_image_size( LONG, LONG, WORD );

/****** マクロ関数宣言 *******************************************************/

/* ビットマップファイルの幅のパディング値を求める */
#define	BMPFILE_WIDTH_PAD( w, b )	( ( ( 32 - (w)*(b) ) & 0x01f ) >> 3 )

/* ビットマップファイルの幅のバイト数を求める */
#define	BMPFILE_WIDTH_SIZE( w, b )						\
(														\
	( (w)*(b) + ( ( 32 - (w)*(b) ) & 0x01f ) ) >> 3		\
)

/*=============================================================================
*
* Purpose : ビットマップファイルからテクスチャイメージを作成．
*
* Return  : 生成に成功すれば真，失敗したら偽を返す．
*
*============================================================================*/
extern GSboolean
gsLoadBmpFile
(
	const char*			szFlieName,		/* 読み込みファイル名			*/
	GSuint*				width,			/* -> 幅						*/
	GSuint*				height,			/* -> 高さ						*/
	GSvoid**			image			/* -> 画像データ				*/
)
{
	FILE*				fp;
	BITMAPFILEHEADER	header;
	BITMAPINFOHEADER	info;
	RGBQUAD				palette[256];
	DWORD				n;

	/* ビットマップファイルのオープン	*/
	if ( ( fp = fopen( szFlieName, "rb" ) ) == NULL )
	{
		return	FALSE;
	}

	/* ビットマップファイルヘッダの読み込み	*/
	if ( fread_bitmapfileheader( fp, &header ) == FALSE )
	{
		fclose( fp );
		return	FALSE;
	}

	/* ビットマップファイル情報の読み込み	*/
	if ( fread_bitmapbfinfheader( fp, &info ) == FALSE )
	{
		fclose( fp );
		return	FALSE;
	}

	/* イメージデータサイズを求める */
	if ( info.biSizeImage == 0 )
	{
		info.biSizeImage = calc_image_size( info.biWidth,
											info.biHeight,
											info.biBitCount );
	}

	/* パレットデータの読み込み */
	if ( ( n = calc_palette_ncolors( &info ) ) != 0 )
	{
		if ( fread_rgbquad( fp, palette, n ) == FALSE )
		{
			fclose( fp );
			return	FALSE;
		}
	}

	/* イメージデータを読み込む */
	*width  = info.biWidth;
	*height = info.biHeight;
	*image  = malloc((*width) * (*height) * sizeof(GSbyte) * 4);
	if ( fread_image( fp, *image, &header, &info, palette ) == FALSE )
	{
		fclose( fp );
		free(*image);
		*image = NULL;
		return	FALSE;
	}

	/* ビットマップファイルのクローズ	*/
	fclose( fp );

	return	TRUE;
}

/*-----------------------------------------------------------------------------
*
* Purpose : ビットマップファイルヘッダの読み込み．
*
* Return  : 読み込みに成功すれば真，失敗すれば偽を返す．
*
*----------------------------------------------------------------------------*/
static BOOL
fread_bitmapfileheader
(
	FILE*				fp,			/* ファイルハンドラ             */
	BITMAPFILEHEADER*	pbfhdr		/* ビットマップファイルヘッダ   */
)
{
	if ( fread( pbfhdr, sizeof( BITMAPFILEHEADER ), 1, fp ) < 1 )
	{
		return	FALSE;
	}

	if ( pbfhdr->bfType != 0x4D42 )
	{
		return	FALSE;
	}

	return	TRUE;
}

/*-----------------------------------------------------------------------------
*
* Purpose : ビットマップファイル情報の読み込み．
*
* Return  : 読み込みに成功すれば真，失敗すれば偽を返す．
*
*----------------------------------------------------------------------------*/
static BOOL
fread_bitmapbfinfheader
(
	FILE*				fp,			/* ファイルハンドラ             */
	BITMAPINFOHEADER*	pbfinfo		/* ビットマップファイル情報     */
)
{
	if ( fread( pbfinfo, sizeof( BITMAPINFOHEADER ), 1, fp ) < 1 )
	{
		return	FALSE;
	}

	return	TRUE;
}

/*-----------------------------------------------------------------------------
*
* Purpose : カラーデータの読み込み．
*
* Return  : 読み込みに成功すれば真，失敗すれば偽を返す．
*
*----------------------------------------------------------------------------*/
static BOOL
fread_rgbquad
(
	FILE*				fp,			/* ファイルハンドラ             */
	RGBQUAD*			bmiColors,	/* 読み込むカラーデータ         */
	DWORD				n			/* 読み込むカラーデータ数       */
)
{
	if ( n == 0 )
	{
		return	TRUE;
	}

	if ( fread( bmiColors, sizeof( RGBQUAD ), n, fp ) < n )
	{
		return	FALSE;
	}

	return	TRUE;
}

/*-----------------------------------------------------------------------------
*
* Purpose : ビットマップファイルからイメージデータをテクスチャに読み込む．
*
* Return  : 読み込みに成功すれば真，失敗すれば偽を返す．
*
*----------------------------------------------------------------------------*/
static BOOL
fread_image
(
	FILE*				fp,			/* ファイルハンドラ             */
	GSvoid*				pImage,		/* 画像データ					*/
	BITMAPFILEHEADER*	pbfhdr,		/* ビットマップファイルヘッダ   */
	BITMAPINFOHEADER*	pbfinf,		/* ビットマップファイル情報     */
	RGBQUAD*			palette		/* ビットマップデータのポインタ */
)
{
	BOOL				error = FALSE;

	/* イメージデータまでシークする */
	fseek( fp, pbfhdr->bfOffBits, SEEK_SET );

	switch ( pbfinf->biBitCount )
	{
	case 1:		/* ２色ビットマップ			*/
		error = fread_image_2bit( fp, pImage, pbfinf, palette );
		break;

	case 4:		/* １６色ビットマップ		*/
		error = fread_image_4bit( fp, pImage, pbfinf, palette );
		break;

	case 8:		/* ２５６色ビットマップ		*/
		error = fread_image_8bit( fp, pImage, pbfinf, palette );
		break;

	case 24:	/* ２４ビットビットマップ	*/
		error = fread_image_24bit( fp, pImage, pbfinf );
		break;

	case 32:	/* ３２ビットビットマップ	*/
		error = fread_image_32bit( fp, pImage, pbfinf );
		break;
	}

	return	error;
}

/*-----------------------------------------------------------------------------
*
* Purpose : ２ｂｉｔビットマップの読み込み．
*
* Return  : 読み込みに成功すれば真，失敗すれば偽を返す．
*
*----------------------------------------------------------------------------*/
static BOOL
fread_image_2bit
(
	FILE*				fp,			/* ファイルハンドラ             */
	GSvoid*				pImage,		/* 画像データ					*/
	BITMAPINFOHEADER*	pbfinf,		/* ビットマップファイル情報     */
	RGBQUAD*			palette		/* ビットマップデータのポインタ */
)
{
	BYTE*				pteximage;
	BYTE*				ptexel;
	BYTE				image = 0;
	BYTE				pixel;
	LONG				pad;
	LONG				x;
	LONG				y;
	LONG				height;

	height = abs( pbfinf->biHeight );
	pteximage = (BYTE*)pImage;

	/* 幅のパディング値を求める */
	pad = BMPFILE_WIDTH_PAD( pbfinf->biWidth, pbfinf->biBitCount );

	for ( y = 0; y < height; y++ )
	{
		ptexel = &pteximage[ ( height-1 - y ) * pbfinf->biWidth * 4 ];

		for ( x = 0; x < pbfinf->biWidth; x++ )
		{
			/* ８ピクセルに１回読み込む */
			if ( ( x & 0x07 ) == 0 )
			{
				image = (BYTE)fgetc( fp );
			}

			/* ピクセルデータの取得 */
			pixel = ( image >> ( 7 - ( x & 0x07 ) ) ) & 1;

			/* ピクセルの色データを取得する */
			ptexel[0] = palette[ pixel ].rgbRed;
			ptexel[1] = palette[ pixel ].rgbGreen;
			ptexel[2] = palette[ pixel ].rgbBlue;
			ptexel[3] = 0xff;

			ptexel += 4;
		}
		/* パディング分をスキップする */
		fseek( fp, pad, SEEK_CUR );
	}

	/* ファイルのエラーチェックをする */
	if ( ferror( fp ) != 0 )
	{
		return	FALSE;
	}

	return	TRUE;
}

/*-----------------------------------------------------------------------------
*
* Purpose : ４ｂｉｔビットマップの読み込み．
*
* Return  : 読み込みに成功すれば真，失敗すれば偽を返す．
*
*----------------------------------------------------------------------------*/
static BOOL
fread_image_4bit
(
	FILE*				fp,			/* ファイルハンドラ             */
	GSvoid*				pImage,		/* 画像データ					*/
	BITMAPINFOHEADER*	pbfinf,		/* ビットマップファイル情報     */
	RGBQUAD*			palette		/* ビットマップデータのポインタ */
)
{
	BYTE*				pteximage;
	BYTE*				ptexel;
	BYTE				image = 0;
	BYTE				pixel;
	LONG				pad;
	LONG				x;
	LONG				y;
	LONG				height;

	height = abs( pbfinf->biHeight );
	pteximage = (BYTE*)pImage;

	/* 幅のパディング値を求める */
	pad = BMPFILE_WIDTH_PAD( pbfinf->biWidth, pbfinf->biBitCount );

	for ( y = 0; y < height; y++ )
	{
		ptexel = &pteximage[ ( height-1 - y ) * pbfinf->biWidth * 4 ];

		for ( x = 0; x < pbfinf->biWidth; x++ )
		{
			/* ２ピクセルに１回読み込む */
			if ( ( x & 0x01 ) == 0 )
			{
				image = (BYTE)fgetc( fp );
			}

			/* ピクセルデータの取得 */
			pixel = ( image >> ( ( ~x & 1 ) << 2 ) ) & 0x0f;

			/* ピクセルの色データを取得する */
			ptexel[0] = palette[ pixel ].rgbRed;
			ptexel[1] = palette[ pixel ].rgbGreen;
			ptexel[2] = palette[ pixel ].rgbBlue;
			ptexel[3] = 0xff;

			ptexel += 4;
		}
		/* パディング分をスキップする */
		fseek( fp, pad, SEEK_CUR );
	}

	/* ファイルのエラーチェックをする */
	if ( ferror( fp ) != 0 )
	{
		return	FALSE;
	}

	return	TRUE;
}

/*-----------------------------------------------------------------------------
*
* Purpose : ８ｂｉｔビットマップの読み込み．
*
* Return  : 読み込みに成功すれば真，失敗すれば偽を返す．
*
*----------------------------------------------------------------------------*/
static BOOL
fread_image_8bit
(
	FILE*				fp,			/* ファイルハンドラ             */
	GSvoid*				pImage,		/* 画像データ					*/
	BITMAPINFOHEADER*	pbfinf,		/* ビットマップファイル情報     */
	RGBQUAD*			palette		/* ビットマップデータのポインタ */
)
{
	BYTE*				pteximage;
	BYTE*				ptexel;
	BYTE				pixel;
	LONG				pad;
	LONG				x;
	LONG				y;
	LONG				height;

	height = abs( pbfinf->biHeight );

	/* テクスチャイメージのロック */
	pteximage = (BYTE*)pImage;

	/* 幅のパディング値を求める */
	pad = BMPFILE_WIDTH_PAD( pbfinf->biWidth, pbfinf->biBitCount );

	for ( y = 0; y < height; y++ )
	{
		ptexel = &pteximage[ ( height-1 - y ) * pbfinf->biWidth * 4 ];

		for ( x = 0; x < pbfinf->biWidth; x++ )
		{
			/* ピクセルの色データを取得する */
			pixel = (BYTE)fgetc( fp );

			/* ピクセルの色データを取得する */
			ptexel[0] = palette[ pixel ].rgbRed;
			ptexel[1] = palette[ pixel ].rgbGreen;
			ptexel[2] = palette[ pixel ].rgbBlue;
			ptexel[3] = 0xff;

			ptexel += 4;
		}
		/* パディング分をスキップする */
		fseek( fp, pad, SEEK_CUR );
	}

	/* ファイルのエラーチェックをする */
	if ( ferror( fp ) != 0 )
	{
		return	FALSE;
	}

	return	TRUE;
}

/*-----------------------------------------------------------------------------
*
* Purpose : ２４ｂｉｔビットマップの読み込み．
*
* Return  : 読み込みに成功すれば真，失敗すれば偽を返す．
*
*----------------------------------------------------------------------------*/
static BOOL
fread_image_24bit
(
	FILE*				fp,			/* ファイルハンドラ             */
	GSvoid*				pImage,		/* 画像データ					*/
	BITMAPINFOHEADER*	pbfinf		/* ビットマップファイル情報     */
)
{
	BYTE*				pteximage;
	BYTE*				ptexel;
	LONG				pad;
	LONG				x;
	LONG				y;
	LONG				height;

	height = abs( pbfinf->biHeight );

	/* テクスチャイメージのロック */
	pteximage = (BYTE*)pImage;

	/* 幅のパディング値を求める */
	pad = BMPFILE_WIDTH_PAD( pbfinf->biWidth, pbfinf->biBitCount );

	for ( y = 0; y < height; y++ )
	{
		ptexel = &pteximage[ ( height-1 - y ) * pbfinf->biWidth * 4 ];

		for ( x = 0; x < pbfinf->biWidth; x++ )
		{
			/* ピクセルを読み込む */
			ptexel[2] = (BYTE)fgetc( fp );	/* Blue  */
			ptexel[1] = (BYTE)fgetc(fp);	/* Green */
			ptexel[0] = (BYTE)fgetc(fp);	/* Red   */
			ptexel[3] = (BYTE)0xff;			/* Alpha */

			ptexel += 4;
		}
		/* パディング分をスキップする */
		fseek( fp, pad, SEEK_CUR );
	}

	/* ファイルのエラーチェックをする */
	if ( ferror( fp ) != 0 )
	{
		return	FALSE;
	}

	return	TRUE;
}

/*-----------------------------------------------------------------------------
*
* Purpose : ３２ｂｉｔビットマップの読み込み．
*
* Return  : 読み込みに成功すれば真，失敗すれば偽を返す．
*
*----------------------------------------------------------------------------*/
static BOOL
fread_image_32bit
(
	FILE*				fp,			/* ファイルハンドラ             */
	GSvoid*				pImage,		/* 画像データ					*/
	BITMAPINFOHEADER*	pbfinf		/* ビットマップファイル情報     */
)
{
	BYTE*				pteximage;
	BYTE*				ptexel;
	LONG				pad;
	LONG				x;
	LONG				y;
	LONG				height;

	height = abs( pbfinf->biHeight );

	/* テクスチャイメージのロック */
	pteximage = (BYTE*)pImage;

	/* 幅のパディング値を求める */
	pad = BMPFILE_WIDTH_PAD( pbfinf->biWidth, pbfinf->biBitCount );

	for ( y = 0; y < height; y++ )
	{
		ptexel = &pteximage[ ( height-1 - y ) * pbfinf->biWidth * 4 ];

		for ( x = 0; x < pbfinf->biWidth; x++ )
		{
			/* ピクセルを読み込む */
			ptexel[2] = (BYTE)fgetc( fp );	/* Blue  */
			ptexel[1] = (BYTE)fgetc(fp);	/* Green */
			ptexel[0] = (BYTE)fgetc(fp);	/* Red   */
			ptexel[3] = (BYTE)fgetc(fp);	/* Alpha */

			ptexel += 4;
		}
		/* パディング分をスキップする */
		fseek( fp, pad, SEEK_CUR );
	}

	/* ファイルのエラーチェックをする */
	if ( ferror( fp ) != 0 )
	{
		return	FALSE;
	}

	return	TRUE;
}

/*-----------------------------------------------------------------------------
*
* Purpose : ビットマップファイル情報からカラーデータ数を求める．
*
* Return  : カラーデータ数を返す．
*
*----------------------------------------------------------------------------*/
static DWORD
calc_palette_ncolors
(
	BITMAPINFOHEADER*	pbfinf		/* ビットマップファイル情報     */
)
{
	DWORD				ncolor;

	ncolor = 0;
	if ( pbfinf->biBitCount <= 8 )
	{
		if ( pbfinf->biClrUsed == 0 )
		{
			ncolor = 1 << pbfinf->biBitCount;
		}
		else
		{
			ncolor = pbfinf->biClrUsed;
		}
	}

	return	ncolor;
}

/*-----------------------------------------------------------------------------
*
* Purpose : イメージサイズを求める．
*
* Return  : イメージサイズをバイト数で返す．
*
*----------------------------------------------------------------------------*/
static DWORD
calc_image_size
(
	LONG				width,		/* ビットマップの幅             */
	LONG				height,		/* ビットマップの高さ           */
	WORD				bits		/* ビットマップのビット深度     */
)
{
	/* 高さが負の場合は正する */
	height = ( height < 0 ) ? -height: height;

	/* 幅と高さを掛け合せるとバイト数になる */
	return	BMPFILE_WIDTH_SIZE( width, bits ) * height;
}

/********** End of File ******************************************************/
