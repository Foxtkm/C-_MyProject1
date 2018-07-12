/******************************************************************************
* File Name : GSsave_texture_file.c              Ver : 1.00  Date : 2016-03-17
*
* Description :
*
*      テクスチャを画像ファイルに保存する．
*
* Author : Moriharu Ohzu.
*
******************************************************************************/
#ifndef GS_SAVE_TEXTURE_FILE_H_
#define GS_SAVE_TEXTURE_FILE_H_

#include <GStype.h>
#include <GStexture.h>

/****** 関数プロトタイプ宣言 *************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*=============================================================================
*
* Purpose : テクスチャを画像ファイルに保存する．
*
* Return  : 生成に成功すれば真，失敗したら偽を返す．
*
*============================================================================*/
extern GSboolean
gsSaveTextureFile
(
	const char*	szFlieName,		/* 保存ファイル名				*/
	GStexture*	pTexture		/* テクスチャ					*/
);

#ifdef __cplusplus
}
#endif

#endif

/********** End of File ******************************************************/
