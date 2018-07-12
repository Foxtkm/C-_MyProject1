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
#include "GSsave_texture_file.h"
#include "GSsave_dds.h"
#include "heap.h"

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
)
{
	GSboolean result;
	/* イメージデータの取得 */
	GSvoid* image;
	GSuint  size;
	gsTextureGetImage(pTexture, &size, &image);

	/* DDSファイルの保存 */
	result = gsSaveDDSFile(
		szFlieName,
		pTexture->Type,
		pTexture->dwWidth, pTexture->dwHeight, pTexture->dwDepth,
		pTexture->Mipmap,
		&pTexture->Format,
		image, size);

	/* イメージデータの削除 */
	del_object(image);

	return result;
}

/********** End of File ******************************************************/
