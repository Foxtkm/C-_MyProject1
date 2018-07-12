/******************************************************************************
* File Name : GSsave_dds.c                       Ver : 1.00  Date : 2016-03-17
*
* Description :
*
*       テクスチャデータをDDSファイルに保存．
*
* Author : Moriharu Ohzu.
*
******************************************************************************/
#include "GSsave_dds.h"
#include "DDSFile.h"
#include <opengl_ext.h>
#include <cassert>
#include <algorithm>

/****** 関数プロトタイプ宣言 *************************************************/

static DDSFile::SurfaceDesc convert_to_dds_surface_desc(GLenum, GLsizei, GLsizei, GLsizei, GLint, const GSpixelFormat*);
static bool operator == (const GSpixelFormat& l, const GSpixelFormat& r);
static const DDSFile::PixelFormat* convert_to_dds_pixel_format(const GSpixelFormat& format);

/*=============================================================================
*
* Purpose : DDSファイルからテクスチャイメージを作成．
*
* Return  : 生成に成功すれば真，失敗したら偽を返す．
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
)
{
	DDSFile dds(convert_to_dds_surface_desc(type, width, height, depth, mipmap, pFormat));
	dds.resize(imageSize);
	std::memcpy(&dds[0], pImage, imageSize);
	dds.save(szFlieName);
	return GS_TRUE;
}

// サーフェスディスクプリタに変換
static DDSFile::SurfaceDesc convert_to_dds_surface_desc
(
	GLenum					type,			/* テクスチャタイプ		*/
	GLsizei					width,			/* 幅					*/
	GLsizei					height,			/* 高さ					*/
	GLsizei					depth,			/* 深さ					*/
	GLint					mipmap,			/* ミップマップ			*/
	const GSpixelFormat*	pFormat			/* ピクセルフォーマット	*/
)
{
	DDSFile::SurfaceDesc surface;
	std::memset(&surface, 0, sizeof(surface));
	surface.dwSize   = 124;
	surface.dwWidth  = width;
	surface.dwHeight = height;
	surface.dwDepth  = depth;
	surface.dwMipMapCount = std::max(mipmap, 1);
	const DDSFile::PixelFormat* pixel_format = convert_to_dds_pixel_format(*pFormat);
	if (pixel_format == 0) {
		throw std::runtime_error("DDS保存 未対応フォーマットです");
	}
	surface.ddpfPixelFormat = *pixel_format;
	surface.ddsCaps.dwCaps1 = DDSFile::FLAG_HEIGHT | DDSFile::FLAG_WIDTH | DDSFile::FLAG_CAPS | DDSFile::FLAG_PIXELFORMAT | DDSFile::FLAG_MIPMAPCOUNT;
	if (type == GL_TEXTURE_CUBE_MAP) {
		surface.ddsCaps.dwCaps2 |= DDSFile::CAPS2_CUBEMAP;
	} else if (type == GL_TEXTURE_3D) {
		surface.ddsCaps.dwCaps1 |= DDSFile::FLAG_DEPTH;
		surface.ddsCaps.dwCaps2 |= DDSFile::CAPS2_VOLUME;
	}
	return surface;
}

// ピクセルフォーマットの比較関数
static bool operator == (const GSpixelFormat& l, const GSpixelFormat& r)
{
	return (l.Format         == r.Format)
		&& (l.InternalFormat == r.InternalFormat)
		&& (l.Type           == r.Type)
		&& (l.Size           == r.Size)
		&& (l.Complessed     == r.Complessed);
}

// ピクセルフォーマットの変換
static const DDSFile::PixelFormat* convert_to_dds_pixel_format(const GSpixelFormat& format)
{
	// DDSピクセルフォーマット
	static const DDSFile::PixelFormat DDS_RGB  = { 32, DDSFile::FORMAT_RGB,  0, 32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0x00000000 };
	static const DDSFile::PixelFormat DDS_BGR  = { 32, DDSFile::FORMAT_RGB,  0, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0x00000000 };
	static const DDSFile::PixelFormat DDS_RGBA = { 32, DDSFile::FORMAT_RGBA, 0, 32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000 };
	static const DDSFile::PixelFormat DDS_BGRA = { 32, DDSFile::FORMAT_RGBA, 0, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000 };
	static const DDSFile::PixelFormat DDS_L    = { 32, DDSFile::FORMAT_LUMINANCE,   0,  8, 0x000000ff, 0x00000000, 0x00000000, 0x00000000 };
	static const DDSFile::PixelFormat DDS_A    = { 32, DDSFile::FORMAT_ALPHA, 0, 8, 0x00000000, 0x00000000, 0x00000000, 0x000000ff };
	static const DDSFile::PixelFormat DDS_AP   = { 32, DDSFile::FORMAT_ALPHAPIXELS, 0, 8, 0x00000000, 0x00000000, 0x00000000, 0x000000ff };
	// DDSピクセルフォーマット(FOURCC)
	static const DDSFile::PixelFormat DDS_DXT1          = { 32, DDSFile::FORMAT_FOURCC, DDSFile::FORMAT_DXT1,          0, 0, 0, 0, 0 };
	static const DDSFile::PixelFormat DDS_DXT3          = { 32, DDSFile::FORMAT_FOURCC, DDSFile::FORMAT_DXT3,          0, 0, 0, 0, 0 };
	static const DDSFile::PixelFormat DDS_DXT5          = { 32, DDSFile::FORMAT_FOURCC, DDSFile::FORMAT_DXT5,          0, 0, 0, 0, 0 };
	static const DDSFile::PixelFormat DDS_G32R32F       = { 32, DDSFile::FORMAT_FOURCC, DDSFile::FORMAT_G32R32F,       0, 0, 0, 0, 0 };
	static const DDSFile::PixelFormat DDS_A32B32G32R32F = { 32, DDSFile::FORMAT_FOURCC, DDSFile::FORMAT_A32B32G32R32F, 0, 0, 0, 0, 0 };
	static const DDSFile::PixelFormat DDS_R16F          = { 32, DDSFile::FORMAT_FOURCC, DDSFile::FORMAT_R16F,          0, 0, 0, 0, 0 };
	static const DDSFile::PixelFormat DDS_G16R16F       = { 32, DDSFile::FORMAT_FOURCC, DDSFile::FORMAT_G16R16F,       0, 0, 0, 0, 0 };
	static const DDSFile::PixelFormat DDS_A16B16G16R16F = { 32, DDSFile::FORMAT_FOURCC, DDSFile::FORMAT_A16B16G16R16F, 0, 0, 0, 0, 0 };
	// DDSピクセルフォーマット変換テーブル
	static const struct {
		DDSFile::PixelFormat dds;
		GSpixelFormat		 ogl;
	} format_dds[] = {
		{ DDS_RGBA, GS_PIXEL_FORMAT_R8G8B8A8_UNORM },
		{ DDS_BGRA, GS_PIXEL_FORMAT_B8G8R8A8_UNORM },
		{ DDS_L,    GS_PIXEL_FORMAT_LUMINANCE },
		{ DDS_A,    GS_PIXEL_FORMAT_A8_UNORM },
		{ DDS_AP,   GS_PIXEL_FORMAT_A8_UNORM }
	};
	// FOURCC変換テーブル
	static const struct {
		DDSFile::PixelFormat fourcc;
		GSpixelFormat		 ogl;
	} format_fourcc[] = {
		{ DDS_DXT1,          GS_PIXEL_FORMAT_BC1_UNORM },
		{ DDS_DXT3,          GS_PIXEL_FORMAT_BC3_UNORM },
		{ DDS_DXT5,          GS_PIXEL_FORMAT_BC5_UNORM },
		{ DDS_R16F,          GS_PIXEL_FORMAT_R32_FLOAT },
		{ DDS_G16R16F,       GS_PIXEL_FORMAT_R32G32_FLOAT },
		{ DDS_A32B32G32R32F, GS_PIXEL_FORMAT_R32G32B32A32_FLOAT },
		{ DDS_R16F,          GS_PIXEL_FORMAT_R16_FLOAT },
		{ DDS_G16R16F,       GS_PIXEL_FORMAT_R16G16_FLOAT },
		{ DDS_A16B16G16R16F, GS_PIXEL_FORMAT_R16G16B16A16_FLOAT },
	};
	for (int i = 0; i < (sizeof(format_dds) / sizeof(format_dds[0])); ++i) {
		if (format_dds[i].ogl == format) {
			return &format_dds[i].dds;
		}
	}
	for (int i = 0; i < (sizeof(format_fourcc) / sizeof(format_fourcc[0])); ++i) {
		if (format_fourcc[i].ogl == format) {
			return &format_fourcc[i].fourcc;
		}
	}
	assert(false);
	return 0;
}

/********** End of File ******************************************************/
