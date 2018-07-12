/******************************************************************************
* File Name : GStexture.c                        Ver : 1.00  Date : 2016-03-17
*
* Description :
*
*       テクスチャ管理．
*
* Author : Moriharu Ohzu.
*
******************************************************************************/
#include "GStexture.h"
#include "heap.h"
#include <opengl_ext.h>

/****** 関数プロトタイプ宣言 *************************************************/

static void tex_image(GStexture* pTexture, const GLvoid* image);
static void tex_image_2D(GStexture* pTexture, const GLvoid* image);
static void tex_image_3D(GStexture* pTexture, const GLvoid* image);
static void tex_image_cube(GStexture* pTexture, const GLvoid* image);
static void tex_get_image(GStexture* pTexture, GLvoid* image);
static void tex_get_image_2D(GStexture* pTexture, GLvoid* image);
static void tex_get_image_3D(GStexture* pTexture, GLvoid* image);
static void tex_get_image_cube(GStexture* pTexture, GLvoid* image);
static GLsizei image_size(GStexture* pTexture, GLsizei mipLevel);
static GLsizei total_image_size(GStexture* pTexture);
static GLsizei mip_size(GLsizei size, GLint level);
static GLint mip_level(GLsizei size);
static void tex_filter(GStexture* pTexture);
static void tex_wrap(GStexture* pTexture);

/*=============================================================================
*
* Purpose : テクスチャの生成．
*
* Return  : テクスチャを返す．
*
*============================================================================*/
extern GStexture*
gsTextureNew
(
	GLenum					type,
	GLsizei					width,
	GLsizei					height,
	GLsizei					depth,
	GLint					mipmap,
	const GSpixelFormat*	format,
	const GLvoid*			pImage
)
{
	GStexture*	pTexture;
	pTexture = new_object(sizeof(GStexture));
	pTexture->Type = type;
	pTexture->dwWidth = width;
	pTexture->dwHeight = height;
	pTexture->dwDepth = depth;
	pTexture->Mipmap = mipmap;
	pTexture->Format = *format;
	glGenTextures(1, &pTexture->dwTexName);
	glBindTexture(pTexture->Type, pTexture->dwTexName);
	tex_image(pTexture, pImage);
	tex_filter(pTexture);
	tex_wrap(pTexture);
	glBindTexture(pTexture->Type, 0);
	return pTexture;
}

/*=============================================================================
*
* Purpose : テクスチャの消滅．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsTextureDelete
(
	GStexture*		pTexture			/* テクスチャ 			*/
)
{
	/* テクスチャオブジェクトの削除 */
	glDeleteTextures(1, &pTexture->dwTexName);
	/* テクスチャの消滅 */
	del_object(pTexture);
}

/*=============================================================================
*
* Purpose : ミップマップの生成．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsTextureGenerateMipmap
(
	GStexture*		pTexture			/* テクスチャ 			*/
)
{
	glBindTexture(pTexture->Type, pTexture->dwTexName);
	glGenerateMipmap(pTexture->Type);
	pTexture->Mipmap = mip_level(pTexture->dwHeight < pTexture->dwWidth ? pTexture->dwHeight : pTexture->dwWidth);
	tex_filter(pTexture);
	glBindTexture(pTexture->Type, 0);
}

/*=============================================================================
*
* Purpose : テクスチャの幅の取得．
*
* Return  : なし．
*
*============================================================================*/
extern GSuint
gsTextureGetWidth
(
	GStexture*		pTexture			/* テクスチャ 				*/
)
{
	return	pTexture->dwWidth;
}

/*=============================================================================
*
* Purpose : テクスチャの高さの取得．
*
* Return  : なし．
*
*============================================================================*/
extern GSuint
gsTextureGetHeight
(
	GStexture*		pTexture			/* テクスチャ 				*/
)
{
	return	pTexture->dwHeight;
}

/*=============================================================================
*
* Purpose : テクスチャの深さの取得．
*
* Return  : なし．
*
*============================================================================*/
extern GSuint
gsTextureGetDepth
(
	GStexture*		pTexture			/* テクスチャ 				*/
)
{
	return	pTexture->dwDepth;
}

/*=============================================================================
*
* Purpose : テクスチャ名のバインド．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsTextureBind
(
	GStexture*		pTexture			/* テクスチャ 				*/
)
{
	/* テクチャオブジェクトをバインドする */
	glBindTexture(pTexture->Type, pTexture->dwTexName);
	glEnable(pTexture->Type);
}

/*=============================================================================
*
* Purpose : テクスチャ名のアンバインド．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsTextureUnbind
(
	GStexture*		pTexture			/* テクスチャ 				*/
)
{
	/* テクチャオブジェクトをアンバインドする */
	glBindTexture(pTexture->Type, 0);
	glDisable(pTexture->Type);
}

/*=============================================================================
*
* Purpose : テクスチャイメージの取得．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsTextureGetImage
(
	GStexture*		pTexture,			/* テクスチャ 				*/
	GSuint*			pSize,				/*-> イメージサイズ         */
	GSvoid**		ppImage				/*-> イメージデータ         */
)
{
	*pSize = total_image_size(pTexture);
	*ppImage = new_object(*pSize);
	gsTextureBind(pTexture);
	tex_get_image(pTexture, *ppImage);
	gsTextureUnbind(pTexture);
}

/*-----------------------------------------------------------------------------
*
* Purpose : テクスチャイメージの設定．
*
* Return  : なし．
*
*----------------------------------------------------------------------------*/
static void tex_image(GStexture* pTexture, const GLvoid* image)
{
	switch (pTexture->Type)
	{
	case GL_TEXTURE_2D: tex_image_2D(pTexture, image); break;
	case GL_TEXTURE_3D: tex_image_3D(pTexture, image); break;
	case GL_TEXTURE_CUBE_MAP: tex_image_cube(pTexture, image); break;
	}
}

/*-----------------------------------------------------------------------------
*
* Purpose : 2Dテクスチャイメージの設定．
*
* Return  : なし．
*
*----------------------------------------------------------------------------*/
static void tex_image_2D(GStexture* pTexture, const GLvoid* image)
{
	const unsigned char* base = (const unsigned char*)image;
	unsigned int offset = 0;
	GLint mipLevel = 0;
	for (mipLevel = 0; mipLevel < pTexture->Mipmap; ++mipLevel)
	{
		if (pTexture->Format.Complessed == GL_FALSE)
		{
			glTexImage2D(
				pTexture->Type,
				mipLevel,
				pTexture->Format.InternalFormat,
				mip_size(pTexture->dwWidth, mipLevel),
				mip_size(pTexture->dwHeight, mipLevel),
				0,
				pTexture->Format.Format,
				pTexture->Format.Type,
				(base != NULL) ? &base[offset] : NULL
			);
		}
		else
		{
			glCompressedTexImage2D(
				pTexture->Type,
				mipLevel,
				pTexture->Format.InternalFormat,
				mip_size(pTexture->dwWidth, mipLevel),
				mip_size(pTexture->dwHeight, mipLevel),
				0,
				image_size(pTexture, mipLevel),
				(base != NULL) ? &base[offset] : NULL
			);
		}
		offset += image_size(pTexture, mipLevel);
	}
}

/*-----------------------------------------------------------------------------
*
* Purpose : 3Dテクスチャイメージの設定．
*
* Return  : なし．
*
*----------------------------------------------------------------------------*/
static void tex_image_3D(GStexture* pTexture, const GLvoid* image)
{
	const unsigned char* base = (const unsigned char*)image;
	unsigned int offset = 0;
	GLint mipLevel;
	for (mipLevel = 0; mipLevel < pTexture->Mipmap; ++mipLevel)
	{
		if (pTexture->Format.Complessed == GL_FALSE)
		{
			glTexImage3D(
				pTexture->Type,
				mipLevel,
				pTexture->Format.InternalFormat,
				mip_size(pTexture->dwWidth,  mipLevel),
				mip_size(pTexture->dwHeight, mipLevel),
				mip_size(pTexture->dwDepth,  mipLevel),
				0,
				pTexture->Format.Format,
				pTexture->Format.Type,
				(base != NULL) ? &base[offset] : NULL
			);
		}
		else
		{
			glCompressedTexImage3D(
				pTexture->Type,
				mipLevel,
				pTexture->Format.InternalFormat,
				mip_size(pTexture->dwWidth,  mipLevel),
				mip_size(pTexture->dwHeight, mipLevel),
				mip_size(pTexture->dwDepth,  mipLevel),
				0,
				image_size(pTexture, mipLevel),
				(base != NULL) ? &base[offset] : NULL
			);
		}
		offset += image_size(pTexture, mipLevel);
	}
}

/*-----------------------------------------------------------------------------
*
* Purpose : Cubeテクスチャイメージの設定．
*
* Return  : なし．
*
*----------------------------------------------------------------------------*/
static void tex_image_cube(GStexture* pTexture, const GLvoid* image)
{
	const unsigned char* base = (const unsigned char*)image;
	unsigned int offset = 0;
	GLenum target;
	for (target = GL_TEXTURE_CUBE_MAP_POSITIVE_X; target <= GL_TEXTURE_CUBE_MAP_NEGATIVE_Z; ++target)
	{
		GLint mipLevel;
		for (mipLevel = 0; mipLevel < pTexture->Mipmap; ++mipLevel)
		{
			if (pTexture->Format.Complessed == GL_FALSE) {
				glTexImage2D(
					target,
					mipLevel,
					pTexture->Format.InternalFormat,
					mip_size(pTexture->dwWidth,  mipLevel),
					mip_size(pTexture->dwHeight, mipLevel),
					0,
					pTexture->Format.Format,
					pTexture->Format.Type,
					(base != NULL) ? &base[offset]: NULL
				);
			}
			else
			{
				glCompressedTexImage2D(
					target,
					mipLevel,
					pTexture->Format.InternalFormat,
					mip_size(pTexture->dwWidth,  mipLevel),
					mip_size(pTexture->dwHeight, mipLevel),
					0,
					image_size(pTexture, mipLevel),
					(base != NULL) ? &base[offset] : NULL
				);
			}
			offset += image_size(pTexture, mipLevel);
		}
	}
}

/*-----------------------------------------------------------------------------
*
* Purpose : テクスチャイメージの取得．
*
* Return  : なし．
*
*----------------------------------------------------------------------------*/
static void tex_get_image(GStexture* pTexture, GLvoid* image)
{
	switch (pTexture->Type)
	{
	case GL_TEXTURE_2D: tex_get_image_2D(pTexture, image); break;
	case GL_TEXTURE_3D: tex_get_image_3D(pTexture, image); break;
	case GL_TEXTURE_CUBE_MAP: tex_get_image_cube(pTexture, image); break;
	}
}

/*-----------------------------------------------------------------------------
*
* Purpose : 2Dテクスチャイメージの取得．
*
* Return  : なし．
*
*----------------------------------------------------------------------------*/
static void tex_get_image_2D(GStexture* pTexture, GLvoid* image)
{
	unsigned char* base = (unsigned char*)image;
	unsigned int offset = 0;
	GLint mipLevel = 0;
	for (mipLevel = 0; mipLevel < pTexture->Mipmap; ++mipLevel)
	{
		if (pTexture->Format.Complessed == GL_FALSE)
		{
			glGetTexImage(
				pTexture->Type,
				mipLevel,
				pTexture->Format.Format,
				pTexture->Format.Type,
				&base[offset]
			);
		}
		else
		{
			glGetCompressedTexImage(
				pTexture->Type,
				mipLevel,
				&base[offset]
			);
		}
		offset += image_size(pTexture, mipLevel);
	}
}

/*-----------------------------------------------------------------------------
*
* Purpose : 3Dテクスチャイメージの取得．
*
* Return  : なし．
*
*----------------------------------------------------------------------------*/
static void tex_get_image_3D(GStexture* pTexture, GLvoid* image)
{
	tex_get_image_2D(pTexture, image);
}

/*-----------------------------------------------------------------------------
*
* Purpose : Cubeテクスチャイメージの取得．
*
* Return  : なし．
*
*----------------------------------------------------------------------------*/
static void tex_get_image_cube(GStexture* pTexture, GLvoid* image)
{
	unsigned char* base = (unsigned char*)image;
	unsigned int offset = 0;
	GLenum target;
	for (target = GL_TEXTURE_CUBE_MAP_POSITIVE_X; target <= GL_TEXTURE_CUBE_MAP_NEGATIVE_Z; ++target)
	{
		GLint mipLevel;
		for (mipLevel = 0; mipLevel < pTexture->Mipmap; ++mipLevel)
		{
			if (pTexture->Format.Complessed == GL_FALSE)
			{
				glGetTexImage(
					target,
					mipLevel,
					pTexture->Format.Format,
					pTexture->Format.Type,
					&base[offset]
				);
			}
			else
			{
				glGetCompressedTexImage(
					target,
					mipLevel,
					&base[offset]
				);
			}
			offset += image_size(pTexture, mipLevel);
		}
	}
}

/*-----------------------------------------------------------------------------
*
* Purpose : イメージサイズを求める．
*
* Return  : イメージサイズを返す．
*
*----------------------------------------------------------------------------*/
static GLsizei image_size(GStexture* pTexture, GLsizei mipLevel)
{
	GLsizei w = mip_size(pTexture->dwWidth,  mipLevel);
	GLsizei h = mip_size(pTexture->dwHeight, mipLevel);
	GLsizei d = mip_size(pTexture->dwDepth,  mipLevel);
	if (pTexture->Format.Complessed == GL_TRUE)
	{
		w = (w + 3) / 4;
		h = (h + 3) / 4;
	}
	return w * h * d * pTexture->Format.Size;
}

/*-----------------------------------------------------------------------------
*
* Purpose : 総イメージサイズを計算する．
*
* Return  : なし．
*
*----------------------------------------------------------------------------*/
static GLsizei total_image_size(GStexture* pTexture)
{
	GLsizei result = 0;
	GLint mipLevel;
	for (mipLevel = 0; mipLevel < pTexture->Mipmap; ++mipLevel)
	{
		result += image_size(pTexture, mipLevel);
	}
	return (pTexture->Type == GL_TEXTURE_CUBE_MAP) ? result * 6 : result;
}

/*-----------------------------------------------------------------------------
*
* Purpose : ミップマップサイズを求める．
*
* Return  : ミップマップサイズを返す．
*
*----------------------------------------------------------------------------*/
static GLsizei mip_size(GLsizei size, GLint level)
{
	const GLint result = (size >> level);
	return (result <= 0) ? 1 : result;
}

/*-----------------------------------------------------------------------------
*
* Purpose : ミップマップレベルを求める．
*
* Return  : ミップマップレベルを返す．
*
*----------------------------------------------------------------------------*/
static GLint mip_level(GLsizei size)
{
	GLint level = 1;
	while ((size >> level) > 0)
	{
		++level;
	}
	return level;
}

/*-----------------------------------------------------------------------------
*
* Purpose : テクスチャフィルタの設定．
*
* Return  : なし．
*
*----------------------------------------------------------------------------*/
static void tex_filter(GStexture* pTexture)
{
	if (pTexture->Mipmap <= 1)
	{
		glTexParameteri(pTexture->Type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(pTexture->Type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
	{
		glTexParameteri(pTexture->Type, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(pTexture->Type, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	}
}

/*-----------------------------------------------------------------------------
*
* Purpose : テクスチャラップモードの設定．
*
* Return  : なし．
*
*----------------------------------------------------------------------------*/
static void tex_wrap(GStexture* pTexture)
{
	if (pTexture->Type == GL_TEXTURE_CUBE_MAP)
	{
		glTexParameteri(pTexture->Type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(pTexture->Type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(pTexture->Type, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}
	else
	{
		glTexParameteri(pTexture->Type, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(pTexture->Type, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(pTexture->Type, GL_TEXTURE_WRAP_R, GL_REPEAT);
	}
}

/********** End of File ******************************************************/
