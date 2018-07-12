/******************************************************************************
* File Name : GStext.c                           Ver : 1.00  Date : 2003-04-28
*
* Description :
*
*       文字列描画．
*
* Author : Moriharu Ohzu.
*
******************************************************************************/
#include	"GStext.h"
#include	"heap.h"

/****** 関数プロトタイプ宣言 *************************************************/

static HFONT CreateFontHandler( GSuint, GSuint, const char* );
static GSboolean CreateStringBitmapFont( HFONT, const char*, BYTE**, PSIZEL );
static void CreateBitmapBits( HDC, HBITMAP, BYTE**, DWORD* );

/*=============================================================================
*
* Purpose : 文字列の描画．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsTextDraw
(
	GSuint			FontCode,			/* フォントコード				*/
	GSuint			FontPoint,			/* フォントのポイント数			*/
	const char*		pFontName,			/* フォント名					*/
	const char*		pString				/* 文字列						*/
)
{
	HFONT			hFont;
	BYTE*			pBitmap;
	SIZEL			BitmapSize;

	/* 各種レンダリングモードの退避 */
	glPushAttrib( GL_ENABLE_BIT );

	/* テクスチャを無効にする */
	glDisable( GL_TEXTURE_2D );
	glDisable( GL_TEXTURE_1D );

	/* フォントを作成する */
	hFont = CreateFontHandler( FontCode, FontPoint, pFontName );

	/* ビットマップフォントの作成 */
	if ( CreateStringBitmapFont( 
			hFont, pString, &pBitmap, &BitmapSize ) == GS_TRUE )
	{
		glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );

		/* ビットマップフォントを描画する */
		glBitmap( BitmapSize.cx, BitmapSize.cy, 0.0f, (GLfloat)BitmapSize.cy, (GLfloat)BitmapSize.cx, 0.0f, pBitmap );

		/* ビットマップフォントの解放 */
		del_object( pBitmap );
	}

	/* フォントを消滅させる */
	DeleteObject( hFont );

	/* レンダリングモードの復帰 */
	glPopAttrib();
}

/*-----------------------------------------------------------------------------
*
* Purpose : 文字フォントの作成．
*
* Return  : フォントハンドラを返す．
*
*----------------------------------------------------------------------------*/
static HFONT
CreateFontHandler
(
	GSuint		FontCode,			/* フォントコード			*/
	GSuint		FontPoint,			/* フォントのポイント数		*/
	const char*	pFontName			/* フォント名				*/
)
{
	HFONT		hFont;
	DWORD		Bold;
	BOOL		bItalic;
	BOOL		bUline;
	BOOL		bSout;
	HDC			hDC;

	/* フォントの属性を設定する */
	Bold    = ( ( FontCode & GS_FONT_BOLD      ) != 0 ) ? FW_BOLD: FW_NORMAL;
	bItalic = ( ( FontCode & GS_FONT_ITALIC    ) != 0 ) ? TRUE: FALSE;
	bUline  = ( ( FontCode & GS_FONT_UNDERLINE ) != 0 ) ? TRUE: FALSE;
	bSout   = ( ( FontCode & GS_FONT_STRIKEOUT ) != 0 ) ? TRUE: FALSE;

	/* デバイスコンテキストの取得 */
	hDC = wglGetCurrentDC();

	/* フォントを作成する */
	hFont = CreateFont
	(
		FontPoint, 0, 0, 0,
		Bold, bItalic, bUline, bSout,
		SHIFTJIS_CHARSET,
		OUT_STROKE_PRECIS,
		CLIP_CHARACTER_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH,
		pFontName
	);

	return	hFont;
}

/*-----------------------------------------------------------------------------
*
* Purpose : 文字フォント用ビットマップの作成．
*
* Return  : なし．
*
*----------------------------------------------------------------------------*/
static GSboolean
CreateStringBitmapFont
(
	HFONT			hFont,			/* フォント							*/
	const char*		pString,		/* 文字列							*/
	BYTE**			ppBitmap,		/*-> 文字列用ビットマップ			*/
	PSIZEL			pBitmapSize		/*-> 文字列用ビットマップのサイズ	*/
)
{
	HDC				hDC;
	BITMAP			bmp;
	HBITMAP			hbmp;
	HFONT			hFontOld;
	HBITMAP			hPrevBmp;
	HFONT			hPrevFont;
	HDC				hMemDC;
	DWORD			BitsSize;
	BITMAP			bi;
	int				len;

	/* 文字列の長さを求める */
	if ( ( len = lstrlen( pString ) ) == 0 )
	{
		return	GS_FALSE;
	}

	/* デバイスコンテキストを取得 */
	hDC = wglGetCurrentDC();


	/* 現在のデバイスコンテキストのフォントハンドラを取得 */
	hFontOld = SelectObject( hDC, hFont );

	/* ビットマップ文字列のサイズを取得する */
	GetTextExtentPoint32( hDC, pString, len, pBitmapSize );

	/* ビットマップを作成する */
	memset( &bmp, 0, sizeof( BITMAP ) );
	bmp.bmWidth      = pBitmapSize->cx;
	bmp.bmHeight     = pBitmapSize->cy;
	bmp.bmWidthBytes = ( ( pBitmapSize->cx + 7 ) / 8 + 1 ) & (~1);
	bmp.bmPlanes     = 1;
	bmp.bmBitsPixel  = 1;
	bmp.bmBits       = calloc( bmp.bmWidthBytes * pBitmapSize->cy, sizeof(BYTE) );
	hbmp = CreateBitmapIndirect( &bmp );

	/* ビットマップイメージを解放する */
	del_object( bmp.bmBits );

	/* ビットマップが作成できたか？ */
	if ( hbmp == 0 )
	{
		/* フォントハンドラを復帰する */
		SelectObject( hDC, hFontOld );

		return	GS_FALSE;
	}

	/* メモリデバイスコンテキストを作成する */
	if ( ( hMemDC = CreateCompatibleDC( hDC ) ) == 0 )
	{
		/* フォントハンドラを復帰する */
		SelectObject( hDC, hFontOld );

		/* ビットマップを解放する */
		DeleteObject( hbmp );

		return	GS_FALSE;
	}

	/* 現在のメモリデバイスコンテストのビットマップハンドラを取得 */
	hPrevBmp = SelectObject( hMemDC, hbmp );

	/* 文字列の背景色を黒に設定 */
	SetBkColor( hMemDC, RGB( 0, 0, 0 ) );

	/* 背景色で塗りつぶす */
	SetBkMode( hMemDC, OPAQUE );

	/* 文字列のカラーの設定 */
	SetTextColor( hMemDC, RGB( 255, 255, 255 ) );

	/* 現在のメモリデバイスコンテストのフォントハンドラを取得 */
	hPrevFont = SelectObject( hMemDC, hFont );

	/* テキストを描画する */
	TextOut( hMemDC, 0, 0, pString, len );

	/* ビットマップの取得 */
	GetObject( hbmp, sizeof(bi), &bi );

	/* ビットマップイメージの作成 */
	CreateBitmapBits( hMemDC, hbmp, ppBitmap, &BitsSize );

	/* ビットマップイメージのサイズを計算する */
	pBitmapSize->cx = ( ( bi.bmWidth + 31 )& ( ~31 ) );
	pBitmapSize->cy = bi.bmHeight;

	/* メモリデバイスコンテストのフォントハンドラを復帰 */
	SelectObject( hMemDC, hPrevFont );

	/* メモリデバイスコンテストのビットマップハンドラを復帰 */
	SelectObject( hMemDC, hPrevBmp );

	/* メモリデバイステキストを削除 */
	DeleteDC( hMemDC );

	/* フォントハンドラを復帰する */
	SelectObject( hDC, hFontOld );

	/* ビットマップを削除する */
	DeleteObject( hbmp );

	return	GS_TRUE;
}

/*-----------------------------------------------------------------------------
*
* Purpose : 文字フォント用ビットマップの作成．
*
* Return  : なし．
*
*----------------------------------------------------------------------------*/
static void
CreateBitmapBits
(
	HDC				hDC,			/* デバイスコンテキスト				*/
	HBITMAP			hBmp,			/* ビットマップハンドラ				*/
	BYTE**			ppBits,			/*-> ビットマップイメージ			*/
	DWORD*			pBitsSize		/*-> ビットマップイメージのサイズ	*/
)
{
	BITMAP			bi;
	struct
	{
		BITMAPINFOHEADER bih;
		RGBQUAD col[2];
	} bic;
	BITMAPINFO* 	binf;

	/* ビットマップ情報の作成 */
	binf = (BITMAPINFO *)&bic;

	/* ビットマップを取得する */
	GetObject( hBmp, sizeof(bi), &bi );

	/* ビットマップイメージのサイズを計算する */
	*pBitsSize = bi.bmHeight * ( ( ( bi.bmWidth + 31 ) & (~31)) / 8 );

	/* ビットマップのイメージを生成する */
	*ppBits = calloc( *pBitsSize, sizeof(BYTE) );

	binf->bmiHeader.biSize          = sizeof( binf->bmiHeader );
	binf->bmiHeader.biWidth         = bi.bmWidth;
	binf->bmiHeader.biHeight        = bi.bmHeight;
	binf->bmiHeader.biPlanes        = 1;
	binf->bmiHeader.biBitCount      = 1;
	binf->bmiHeader.biCompression   = BI_RGB;
	binf->bmiHeader.biSizeImage     = *pBitsSize;
	binf->bmiHeader.biXPelsPerMeter = 1;
	binf->bmiHeader.biYPelsPerMeter = 1;
	binf->bmiHeader.biClrUsed       = 0;
	binf->bmiHeader.biClrImportant  = 0;

	/* ビットマップイメージの取得 */
	GetDIBits( hDC, hBmp, 0, bi.bmHeight, *ppBits, binf, DIB_RGB_COLORS );
}

/********** End of File ******************************************************/
