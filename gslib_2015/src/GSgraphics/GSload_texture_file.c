/******************************************************************************
* File Name : GSload_texture_file.c              Ver : 1.00  Date : 2016-03-17
*
* Description :
*
*       画像ファイルからテクスチャを作成する．
*
* Author : Moriharu Ohzu.
*
******************************************************************************/
#include "GSload_texture_file.h"
#include "GSload_png.h"
#include "GSload_bmp.h"
#include "GSload_dds.h"
#include "GSfile_path.h"
#include <stdlib.h>

/****** 関数プロトタイプ宣言 *************************************************/

static GStexture* create_texture_from_bmp(const char* szFlieName);
static GStexture* create_texture_from_png(const char* szFlieName);
static GStexture* create_texture_from_dds(const char* szFlieName);

/* RGBA32ピクセルフォーマット */
static const GSpixelFormat RGBA32 = { GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, 4, GL_FALSE };

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
)
{
	GStexture* pTexture = NULL;
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char texture_file[_MAX_PATH];
	gsSplitPath(szFlieName, drive, dir, fname, NULL);

	/* DDS -> png -> bmpの優先順位で読み込みをする */

	/* DDSファイルの読み込み */
	gsMakePath(texture_file, drive, dir, fname, ".dds");
	if ((pTexture = create_texture_from_dds(texture_file)) != NULL) {
		return pTexture;
	}
	/* PNGファイルの読み込み */
	gsMakePath(texture_file, drive, dir, fname, ".png");
	if ((pTexture = create_texture_from_png(texture_file)) != NULL) {
		return pTexture;
	}
	/* BMPファイルの読み込み */
	gsMakePath(texture_file, drive, dir, fname, ".bmp");
	if ((pTexture = create_texture_from_bmp(texture_file)) != NULL) {
		return pTexture;
	}
	return NULL;
}

/*-----------------------------------------------------------------------------
*
* Purpose : 画像ファイルからテクスチャを作成．
*
* Return  : なし．
*
*----------------------------------------------------------------------------*/
static GStexture*
load_texture_file
(
	const char*	szFlieName		/* 読み込みファイル名			*/
)
{
	/* ファイルの拡張子を取り出す */
	char ext[_MAX_EXT];
	gsSplitPath(szFlieName, NULL, NULL, NULL, ext);
	if (_stricmp(ext, ".bmp") == 0)
	{
		return create_texture_from_bmp(szFlieName);
	}
	if (_stricmp(ext, ".png") == 0)
	{
		return create_texture_from_png(szFlieName);
	}
	if (_stricmp(ext, ".dds") == 0)
	{
		return create_texture_from_dds(szFlieName);
	}
	return NULL;
}

/*-----------------------------------------------------------------------------
*
* Purpose : BMPファイルからテクスチャを作成．
*
* Return  : なし．
*
*----------------------------------------------------------------------------*/
static GStexture*
create_texture_from_bmp
(
	const char*	szFlieName		/* 読み込みファイル名			*/
)
{
	GSuint	width;
	GSuint	height;
	GSvoid*	image;
	GStexture* texture;
	// BMPファイルの読み込み
	if (gsLoadBmpFile(szFlieName, &width, &height, &image) == GS_FALSE)
	{
		return NULL;
	}
	// テクスチャの作成
	texture = gsTextureNew(GL_TEXTURE_2D, width, height, 1, 1, &RGBA32, image);
	// イメージデータの解放
	free(image);
	return texture;
}

/*-----------------------------------------------------------------------------
*
* Purpose : PNGファイルからテクスチャを作成．
*
* Return  : なし．
*
*----------------------------------------------------------------------------*/
static GStexture*
create_texture_from_png
(
	const char*	szFlieName		/* 読み込みファイル名			*/
)
{
	GSuint  width;
	GSuint  height;
	GSvoid*	image;
	GStexture* texture;
	// PNGファイルの読み込み
	if (gsLoadPngFile(szFlieName, &width, &height, &image) == GS_FALSE)
	{
		return NULL;
	}
	// テクスチャの作成
	texture = gsTextureNew(GL_TEXTURE_2D, width, height, 1, 1, &RGBA32, image);
	// イメージデータの解放
	free(image);
	return texture;
}

/*-----------------------------------------------------------------------------
*
* Purpose : DDSファイルからテクスチャを作成．
*
* Return  : なし．
*
*----------------------------------------------------------------------------*/
static GStexture*
create_texture_from_dds
(
	const char*	szFlieName		/* 読み込みファイル名			*/
)
{
	GLenum			type;
	GLsizei			width;
	GLsizei			height;
	GLsizei			depth;
	GLint			mipmap;
	GSpixelFormat	format;
	GLvoid*			image;
	GStexture*		texture;
	// DDSファイルの読み込み
	if (gsLoadDDSFile(szFlieName, &type, &width, &height, &depth, &mipmap, &format, &image) == GS_FALSE)
	{
		return NULL;
	}
	// テクスチャの作成
	texture = gsTextureNew(type, width, height, depth, mipmap, &format, image);
	// イメージデータの解放
	free(image);
	return texture;
}

/********** End of File ******************************************************/
