/******************************************************************************
* File Name : GSload_dds.h                         Ver : 1.00  Date : 16-03-17
*
* Description :
*
*       DDSファイルからテクスチャイメージを作成 定義ヘッダファイル．
*
* Author : Moriharu Ohzu.
*
******************************************************************************/
#ifndef GS_LOAD_DDS_FILE_H_
#define GS_LOAD_DDS_FILE_H_

#include <GStexture.h>

#ifdef __cplusplus
extern "C" {
#endif

/*=============================================================================
*
* Purpose : DDSファイルからテクスチャイメージを作成．
*
* Return  : 生成に成功すれば真，失敗したら偽を返す．
*
*============================================================================*/
extern GSboolean
gsLoadDDSFile
(
	const char*		szFlieName,		/* 読み込みファイル名		*/
	GLenum*			type,			/* -> テクスチャタイプ		*/
	GLsizei*		width,			/* -> 幅					*/
	GLsizei*		height,			/* -> 高さ					*/
	GLsizei*		depth,			/* -> 深さ					*/
	GLint*			mipmap,			/* -> ミップマップ			*/
	GSpixelFormat*	pFormat,		/* -> ピクセルフォーマット	*/
	GLvoid**		ppImage			/* -> イメージデータ		*/
);

#ifdef __cplusplus
}
#endif

#endif

/********** End of File ******************************************************/
