/******************************************************************************
* File Name : GSsave_dds.h                         Ver : 1.00  Date : 16-11-24
*
* Description :
*
*       テクスチャイメージをDDSで保存する．
*
* Author : Moriharu Ohzu.
*
******************************************************************************/
#ifndef GS_SAVE_DDS_FILE_H_
#define GS_SAVE_DDS_FILE_H_

#include <GStexture.h>

#ifdef __cplusplus
extern "C" {
#endif

/*=============================================================================
*
* Purpose : テクスチャイメージをDDSファイルに保存．
*
* Return  : 保存に成功すれば真，失敗したら偽を返す．
*
*============================================================================*/
extern GSboolean
gsSaveDDSFile
(
	const char*				szFlieName,		/* 読み込みファイル名	*/
	GLenum					type,			/* テクスチャタイプ		*/
	GLsizei					width,			/* 幅					*/
	GLsizei					height,			/* 高さ					*/
	GLsizei					depth,			/* 深さ					*/
	GLint					mipmap,			/* ミップマップ			*/
	const GSpixelFormat*	pFormat,		/* ピクセルフォーマット	*/
	const GLvoid*			pImage,			/* イメージデータ		*/
	GLsizei					imageSize		/* イメージデータサイズ	*/
);

#ifdef __cplusplus
}
#endif

#endif

/********** End of File ******************************************************/
