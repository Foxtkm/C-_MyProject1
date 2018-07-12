/******************************************************************************
* File Name : GSload_bmp.h                         Ver : 1.00  Date : 16-03-17
*
* Description :
*
*       ビットマップファイルからテクスチャイメージを作成 定義ヘッダファイル．
*
* Author : Moriharu Ohzu.
*
******************************************************************************/
#ifndef GS_LOAD_BMP_FILE_H_
#define GS_LOAD_BMP_FILE_H_

#include <GStype.h>

#ifdef __cplusplus
extern "C" {
#endif

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
);

#ifdef __cplusplus
}
#endif

#endif

/********** End of File ******************************************************/
