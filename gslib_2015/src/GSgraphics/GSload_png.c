/******************************************************************************
* File Name : gtexload_bmp.c                     Ver : 1.00  Date : 2014-07-31
*
* Description :
*
*       pngファイルからテクスチャデータを読み込む．
*
* Author : Moriharu Ohzu.
*
******************************************************************************/
#include	"GSload_png.h"
#include	"lodepng.h"

/*=============================================================================
*
* Purpose : Pngマップファイルからテクスチャを生成する．
*
* Return  : 生成に成功すれば真，失敗したら偽を返す．
*
*============================================================================*/
extern GSboolean
gsLoadPngFile
(
	const char*			szFlieName,		/* 読み込みファイル名			*/
	GSuint*				width,			/* -> 幅						*/
	GSuint*				height,			/* -> 高さ						*/
	GSvoid**			image			/* -> 画像データ				*/
)
{
	/* pngファイルの読み込み */
	unsigned error = lodepng_decode32_file((unsigned char**)image, width, height, szFlieName);
	if (error != 0) {
		return GS_FALSE;
	}
	return GS_TRUE;
}

/********** End of File ******************************************************/
