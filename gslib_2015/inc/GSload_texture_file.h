/******************************************************************************
* File Name : GSload_texture_file.h              Ver : 1.00  Date : 2016-03-17
*
* Description :
*
*       画像ファイルからテクスチャを作成する．
*
* Author : Moriharu Ohzu.
*
******************************************************************************/
#ifndef GS_LOAD_TEXTURE_FILE_H_
#define GS_LOAD_TEXTURE_FILE_H_

#include <GStexture.h>

#ifdef __cplusplus
extern "C" {
#endif

/*=============================================================================
*
* Purpose : 画像ファイルからテクスチャを生成する．
*
* Return  : 生成に成功すれば真，失敗したら偽を返す．
*
*============================================================================*/
extern GStexture*
gsLoadTextureFile
(
	const char*	szFlieName		/* 読み込みファイル名			*/
);

#ifdef __cplusplus
}
#endif

#endif

/********** End of File ******************************************************/
