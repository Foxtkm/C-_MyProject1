/******************************************************************************
* File Name : GSload_dds.c                       Ver : 1.00  Date : 2016-03-17
*
* Description :
*
*       DDSファイルからテクスチャデータを読み込む．
*
* Author : Moriharu Ohzu.
*
******************************************************************************/
#include "GSload_dds.h"
#include "DDSFile.h"
#include <opengl_ext.h>
#include <cassert>

/****** 関数プロトタイプ宣言 *************************************************/

static GLenum convert_type_dds(const DDSFile::SurfaceDesc& ddsd);
static GLenum convert_type_dx10(const DDSFile::Dx10& dx10);
static GLsizei convert_width(const DDSFile::SurfaceDesc& ddsd);
static GLsizei convert_height(const DDSFile::SurfaceDesc& ddsd);
static GLsizei convert_depth(const DDSFile::SurfaceDesc& ddsd);
static const GSpixelFormat convert_format_dds(const DDSFile::PixelFormat& ddpf);
static const GSpixelFormat convert_format_dx10(DWORD format);
static GLint convert_mipmap(const DDSFile::SurfaceDesc& ddsd);
static GLvoid* convert_image(const DDSFile& dds);
static bool comparePixelFormat(const DDSFile::PixelFormat& a, const DDSFile::PixelFormat& b);

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
)
{
	try {
		DDSFile dds(szFlieName);
		*width   = convert_width(dds.desc());
		*height  = convert_height(dds.desc());
		*depth   = convert_depth(dds.desc());
		*mipmap  = convert_mipmap(dds.desc());
		if (dds.desc().ddpfPixelFormat.dwFourCC == DDSFile::FORMAT_DX10) {
			*type = convert_type_dx10(dds.dx10_header());
			*pFormat = convert_format_dx10(dds.dx10_header().dwFormat);
		} else {
			*type = convert_type_dds(dds.desc());
			*pFormat = convert_format_dds(dds.desc().ddpfPixelFormat);
		}
		*ppImage  = convert_image(dds);
	} catch (...) {
		return GS_FALSE;
	}
	return GS_TRUE;
}

// テクスチャタイプの変換
static GLenum convert_type_dds(const DDSFile::SurfaceDesc& ddsd)
{
	if ((ddsd.ddsCaps.dwCaps2 & DDSFile::CAPS2_VOLUME) == DDSFile::CAPS2_VOLUME)
		return GL_TEXTURE_3D;
	if ((ddsd.ddsCaps.dwCaps2 & DDSFile::CAPS2_CUBEMAP) == DDSFile::CAPS2_CUBEMAP)
		return GL_TEXTURE_CUBE_MAP;
	return GL_TEXTURE_2D;
}

// テクスチャタイプの変換
static GLenum convert_type_dx10(const DDSFile::Dx10& dx10)
{
	if (dx10.dwDimension == DDSFile::DDS10_DIMENSION_2D && dx10.dwArraySize == 6) {
		return GL_TEXTURE_CUBE_MAP;
	}
	if (dx10.dwDimension == DDSFile::DDS10_DIMENSION_3D) {
		return GL_TEXTURE_3D;
	}
	return GL_TEXTURE_2D;
}

// 幅の変換
static GLsizei convert_width(const DDSFile::SurfaceDesc& ddsd)
{
	return (GLsizei)ddsd.dwWidth;
}

// 高さの変換
static GLsizei convert_height(const DDSFile::SurfaceDesc& ddsd)
{
	return (GLsizei)ddsd.dwHeight;
}

// 深さの変換
static GLsizei convert_depth(const DDSFile::SurfaceDesc& ddsd)
{
	return ((ddsd.dwFlags & DDSFile::FLAG_DEPTH) != 0) ? (GLsizei)ddsd.dwDepth : 0;
}

// ミップマップレベルの変換
static GLint convert_mipmap(const DDSFile::SurfaceDesc& ddsd)
{
	return ((ddsd.dwFlags & DDSFile::FLAG_MIPMAPCOUNT) != 0) ? (GLint)ddsd.dwMipMapCount : 1;
}

// ピクセルフォーマットの変換
static const GSpixelFormat convert_format_dds(const DDSFile::PixelFormat& ddpf)
{
	if ((ddpf.dwFlags & DDSFile::FORMAT_FOURCC) != 0) {
		// FOURCC変換テーブル
		static const struct {
			DDSFile::DWORD		 fourcc;
			GSpixelFormat		 ogl;
		} format_fourcc[] = {
			{ DDSFile::FORMAT_DXT1,          GS_PIXEL_FORMAT_BC1_UNORM },
			{ DDSFile::FORMAT_DXT3,          GS_PIXEL_FORMAT_BC3_UNORM },
			{ DDSFile::FORMAT_DXT5,          GS_PIXEL_FORMAT_BC5_UNORM },
			{ DDSFile::FORMAT_R32F,          GS_PIXEL_FORMAT_R32_FLOAT },
			{ DDSFile::FORMAT_G32R32F,       GS_PIXEL_FORMAT_R32G32_FLOAT },
			{ DDSFile::FORMAT_A32B32G32R32F, GS_PIXEL_FORMAT_R32G32B32A32_FLOAT },
			{ DDSFile::FORMAT_R16F,          GS_PIXEL_FORMAT_R16_FLOAT },
			{ DDSFile::FORMAT_G16R16F,       GS_PIXEL_FORMAT_R16G16_FLOAT },
			{ DDSFile::FORMAT_A16B16G16R16F, GS_PIXEL_FORMAT_R16G16B16A16_FLOAT },
		};
		// フォーマット変換
		for (int i = 0; i < (sizeof(format_fourcc) / sizeof(format_fourcc[0])); ++i) {
			if (ddpf.dwFourCC == format_fourcc[i].fourcc) {
				return format_fourcc[i].ogl;
			}
		}
	} else {
		// DDSピクセルフォーマット
		static const DDSFile::PixelFormat DDS_RGB  = { 32, DDSFile::FORMAT_RGB,  0, 32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0x00000000 };
		static const DDSFile::PixelFormat DDS_BGR  = { 32, DDSFile::FORMAT_RGB,  0, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0x00000000 };
		static const DDSFile::PixelFormat DDS_RGBA = { 32, DDSFile::FORMAT_RGBA, 0, 32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000 };
		static const DDSFile::PixelFormat DDS_BGRA = { 32, DDSFile::FORMAT_RGBA, 0, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000 };
		static const DDSFile::PixelFormat DDS_L    = { 32, DDSFile::FORMAT_LUMINANCE,   0,  8, 0x000000ff, 0x00000000, 0x00000000, 0x00000000 };
		static const DDSFile::PixelFormat DDS_A    = { 32, DDSFile::FORMAT_ALPHA, 0, 8, 0x00000000, 0x00000000, 0x00000000, 0x000000ff };
		static const DDSFile::PixelFormat DDS_AP   = { 32, DDSFile::FORMAT_ALPHAPIXELS, 0, 8, 0x00000000, 0x00000000, 0x00000000, 0x000000ff };
		// DDSピクセルフォーマット変換テーブル
		static const struct {
			DDSFile::PixelFormat dds;
			GSpixelFormat		 ogl;
		} format_dds[] = {
			{ DDS_RGB,  GS_PIXEL_FORMAT_R8G8B8A8_UNORM },
			{ DDS_BGR,  GS_PIXEL_FORMAT_B8G8R8A8_UNORM },
			{ DDS_RGBA, GS_PIXEL_FORMAT_R8G8B8A8_UNORM },
			{ DDS_BGRA, GS_PIXEL_FORMAT_B8G8R8A8_UNORM },
			{ DDS_L,    GS_PIXEL_FORMAT_LUMINANCE },
			{ DDS_A,    GS_PIXEL_FORMAT_A8_UNORM },
			{ DDS_AP,   GS_PIXEL_FORMAT_A8_UNORM }
		};
		// フォーマット変換
		for (int i = 0; i < (sizeof(format_dds) / sizeof(format_dds[0])); ++i) {
			if (comparePixelFormat(ddpf, format_dds[i].dds)) {
				return format_dds[i].ogl;
			}
		}
	}
	assert(false);
	return GS_PIXEL_FORMAT_UNKNOWN;
}

// ピクセルフォーマットの変換
static const GSpixelFormat convert_format_dx10(DWORD format)
{
	// 変換テーブル
	static const struct {
		DWORD				dds;
		GSpixelFormat		ogl;
	} convet_array[] = {
		{ DDSFile::DXGI_FORMAT_R32G32B32A32_FLOAT, GS_PIXEL_FORMAT_R32G32B32A32_FLOAT },
		{ DDSFile::DXGI_FORMAT_R32G32B32A32_UINT, GS_PIXEL_FORMAT_R32G32B32A32_UINT },
		{ DDSFile::DXGI_FORMAT_R32G32B32A32_SINT, GS_PIXEL_FORMAT_R32G32B32A32_SINT },
		{ DDSFile::DXGI_FORMAT_R32G32B32_FLOAT,GS_PIXEL_FORMAT_R32G32B32_FLOAT },
		{ DDSFile::DXGI_FORMAT_R32G32B32_UINT,GS_PIXEL_FORMAT_R32G32B32_UINT },
		{ DDSFile::DXGI_FORMAT_R32G32B32_SINT,GS_PIXEL_FORMAT_R32G32B32_SINT },
		{ DDSFile::DXGI_FORMAT_R16G16B16A16_FLOAT,GS_PIXEL_FORMAT_R16G16B16A16_FLOAT },
		{ DDSFile::DXGI_FORMAT_R16G16B16A16_UNORM,GS_PIXEL_FORMAT_R16G16B16A16_UNORM },
		{ DDSFile::DXGI_FORMAT_R16G16B16A16_UINT,GS_PIXEL_FORMAT_R16G16B16A16_UINT },
		{ DDSFile::DXGI_FORMAT_R16G16B16A16_SNORM,GS_PIXEL_FORMAT_R16G16B16A16_SNORM },
		{ DDSFile::DXGI_FORMAT_R16G16B16A16_SINT,GS_PIXEL_FORMAT_R16G16B16A16_SINT },
		{ DDSFile::DXGI_FORMAT_R32G32_FLOAT,GS_PIXEL_FORMAT_R32G32_FLOAT },
		{ DDSFile::DXGI_FORMAT_R32G32_UINT,GS_PIXEL_FORMAT_R32G32_UINT },
		{ DDSFile::DXGI_FORMAT_R32G32_SINT,GS_PIXEL_FORMAT_R32G32_SINT },
		{ DDSFile::DXGI_FORMAT_D32_FLOAT_S8X24_UINT,GS_PIXEL_FORMAT_D32_FLOAT_S8X24_UINT },
		{ DDSFile::DXGI_FORMAT_R10G10B10A2_UNORM,GS_PIXEL_FORMAT_R10G10B10A2_UNORM },
		{ DDSFile::DXGI_FORMAT_R10G10B10A2_UINT,GS_PIXEL_FORMAT_R10G10B10A2_UINT },
		{ DDSFile::DXGI_FORMAT_R11G11B10_FLOAT,GS_PIXEL_FORMAT_R11G11B10_FLOAT },
		{ DDSFile::DXGI_FORMAT_R8G8B8A8_UNORM,GS_PIXEL_FORMAT_R8G8B8A8_UNORM },
		{ DDSFile::DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,GS_PIXEL_FORMAT_R8G8B8A8_UNORM_SRGB },
		{ DDSFile::DXGI_FORMAT_R8G8B8A8_UINT,GS_PIXEL_FORMAT_R8G8B8A8_UINT },
		{ DDSFile::DXGI_FORMAT_R8G8B8A8_SNORM,GS_PIXEL_FORMAT_R8G8B8A8_SNORM },
		{ DDSFile::DXGI_FORMAT_R8G8B8A8_SINT,GS_PIXEL_FORMAT_R8G8B8A8_SINT },
		{ DDSFile::DXGI_FORMAT_R16G16_FLOAT,GS_PIXEL_FORMAT_R16G16_FLOAT },
		{ DDSFile::DXGI_FORMAT_R16G16_UNORM,GS_PIXEL_FORMAT_R16G16_UNORM },
		{ DDSFile::DXGI_FORMAT_R16G16_UINT,GS_PIXEL_FORMAT_R16G16_UINT },
		{ DDSFile::DXGI_FORMAT_R16G16_SNORM,GS_PIXEL_FORMAT_R16G16_SNORM },
		{ DDSFile::DXGI_FORMAT_R16G16_SINT,GS_PIXEL_FORMAT_R16G16_SINT },
		{ DDSFile::DXGI_FORMAT_D32_FLOAT,GS_PIXEL_FORMAT_D32_FLOAT },
		{ DDSFile::DXGI_FORMAT_R32_FLOAT,GS_PIXEL_FORMAT_R32_FLOAT },
		{ DDSFile::DXGI_FORMAT_R32_UINT,GS_PIXEL_FORMAT_R32_UINT },
		{ DDSFile::DXGI_FORMAT_R32_SINT,GS_PIXEL_FORMAT_R32_SINT },
		{ DDSFile::DXGI_FORMAT_D24_UNORM_S8_UINT,GS_PIXEL_FORMAT_D24_UNORM_S8_UINT },
		{ DDSFile::DXGI_FORMAT_R8G8_UNORM,GS_PIXEL_FORMAT_R8G8_UNORM },
		{ DDSFile::DXGI_FORMAT_R8G8_UINT,GS_PIXEL_FORMAT_R8G8_UINT },
		{ DDSFile::DXGI_FORMAT_R8G8_SNORM,GS_PIXEL_FORMAT_R8G8_SNORM },
		{ DDSFile::DXGI_FORMAT_R8G8_SINT,GS_PIXEL_FORMAT_R8G8_SINT },
		{ DDSFile::DXGI_FORMAT_R16_FLOAT,GS_PIXEL_FORMAT_R16_FLOAT },
		{ DDSFile::DXGI_FORMAT_D16_UNORM,GS_PIXEL_FORMAT_D16_UNORM },
		{ DDSFile::DXGI_FORMAT_R16_UNORM,GS_PIXEL_FORMAT_R16_UNORM },
		{ DDSFile::DXGI_FORMAT_R16_UINT,GS_PIXEL_FORMAT_R16_UINT },
		{ DDSFile::DXGI_FORMAT_R16_SNORM,GS_PIXEL_FORMAT_R16_SNORM },
		{ DDSFile::DXGI_FORMAT_R16_SINT,GS_PIXEL_FORMAT_R16_SINT },
		{ DDSFile::DXGI_FORMAT_R8_UNORM,GS_PIXEL_FORMAT_R8_UNORM },
		{ DDSFile::DXGI_FORMAT_R8_UINT,GS_PIXEL_FORMAT_R8_UINT },
		{ DDSFile::DXGI_FORMAT_R8_SNORM,GS_PIXEL_FORMAT_R8_SNORM },
		{ DDSFile::DXGI_FORMAT_R8_SINT,GS_PIXEL_FORMAT_R8_SINT },
		{ DDSFile::DXGI_FORMAT_A8_UNORM,GS_PIXEL_FORMAT_A8_UNORM },
		{ DDSFile::DXGI_FORMAT_R9G9B9E5_SHAREDEXP,GS_PIXEL_FORMAT_R9G9B9E5_SHAREDEXP },
		{ DDSFile::DXGI_FORMAT_BC1_UNORM,GS_PIXEL_FORMAT_BC1_UNORM },
		{ DDSFile::DXGI_FORMAT_BC1_UNORM_SRGB,GS_PIXEL_FORMAT_BC1_UNORM_SRGB },
		{ DDSFile::DXGI_FORMAT_BC3_UNORM,GS_PIXEL_FORMAT_BC1_UNORM },
		{ DDSFile::DXGI_FORMAT_BC3_UNORM_SRGB,GS_PIXEL_FORMAT_BC1_UNORM_SRGB },
		{ DDSFile::DXGI_FORMAT_BC5_UNORM,GS_PIXEL_FORMAT_BC5_UNORM },
		{ DDSFile::DXGI_FORMAT_BC5_SNORM,GS_PIXEL_FORMAT_BC5_UNORM_SRGB },
		{ DDSFile::DXGI_FORMAT_B5G6R5_UNORM,GS_PIXEL_FORMAT_B5G6R5_UNORM },
		{ DDSFile::DXGI_FORMAT_B5G5R5A1_UNORM,GS_PIXEL_FORMAT_B5G5R5A1_UNORM },
		{ DDSFile::DXGI_FORMAT_B8G8R8A8_UNORM,GS_PIXEL_FORMAT_B8G8R8A8_UNORM },
		{ DDSFile::DXGI_FORMAT_B8G8R8X8_UNORM,GS_PIXEL_FORMAT_B8G8R8X8_UNORM }
	};
	// フォーマット変換
	for (int i = 0; i < (sizeof(convet_array) / sizeof(convet_array[0])); ++i) {
		if (convet_array[i].dds == format) {
			return convet_array[i].ogl;
		}
	}
	assert(false);
	return GS_PIXEL_FORMAT_UNKNOWN;
}

// イメージデータの変換
static void* convert_image(const DDSFile& dds)
{
	void* image = malloc(dds.size());
	if (image == NULL) throw std::bad_alloc();
	memcpy(image, &dds[0], dds.size());
	return image;
}

// フォーマットの比較
static bool comparePixelFormat(const DDSFile::PixelFormat& a, const DDSFile::PixelFormat& b)
{
	if (a.dwSize != b.dwSize)						return false;
	if (a.dwFlags != b.dwFlags)						return false;
	if (a.dwRGBBitCount != b.dwRGBBitCount)         return false;
	if (a.dwFourCC != b.dwFourCC)					return false;
	if (a.dwRBitMask != b.dwRBitMask)				return false;
	if (a.dwGBitMask != b.dwGBitMask)				return false;
	if (a.dwBBitMask != b.dwBBitMask)				return false;
	if (a.dwRGBAlphaBitMask != b.dwRGBAlphaBitMask) return false;
	return true;
}

/********** End of File ******************************************************/
