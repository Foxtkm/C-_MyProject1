#include <gslib.h>

const GSpixelFormat GS_PIXEL_FORMAT_UNKNOWN = { 0 };
const GSpixelFormat GS_PIXEL_FORMAT_R32G32B32A32_FLOAT   = { GL_RGBA32F, GL_RGBA, GL_FLOAT, 16, GL_FALSE };
const GSpixelFormat GS_PIXEL_FORMAT_R32G32B32A32_UINT    = { GL_RGBA32UI, GL_RGBA, GL_UNSIGNED_INT, 16, GL_FALSE };
const GSpixelFormat GS_PIXEL_FORMAT_R32G32B32A32_SINT    = { GL_RGBA32I, GL_RGBA, GL_INT, 16, GL_FALSE };
const GSpixelFormat GS_PIXEL_FORMAT_R32G32B32_FLOAT      = { GL_RGB32F, GL_RGB, GL_FLOAT, 12, GL_FALSE };
const GSpixelFormat GS_PIXEL_FORMAT_R32G32B32_UINT       = { GL_RGB32UI, GL_RGB, GL_UNSIGNED_INT, 12, GL_FALSE };
const GSpixelFormat GS_PIXEL_FORMAT_R32G32B32_SINT       = { GL_RGB32I, GL_RGB, GL_INT, 12, GL_FALSE };
const GSpixelFormat GS_PIXEL_FORMAT_R16G16B16A16_FLOAT   = { GL_RGBA16F, GL_RGBA, GL_HALF_FLOAT, 8, GL_FALSE };
const GSpixelFormat GS_PIXEL_FORMAT_R16G16B16A16_UNORM   = { GL_RGBA16, GL_RGBA, GL_UNSIGNED_SHORT, 8, GL_FALSE };
const GSpixelFormat GS_PIXEL_FORMAT_R16G16B16A16_UINT    = { GL_RGBA16UI, GL_RGBA, GL_UNSIGNED_SHORT, 8, GL_FALSE };
const GSpixelFormat GS_PIXEL_FORMAT_R16G16B16A16_SNORM   = { GL_RGBA16_SNORM, GL_RGBA, GL_SHORT, 8, GL_FALSE };
const GSpixelFormat GS_PIXEL_FORMAT_R16G16B16A16_SINT    = { GL_RGBA16I, GL_RGBA, GL_SHORT, 8, GL_FALSE };
const GSpixelFormat GS_PIXEL_FORMAT_R32G32_FLOAT         = { GL_RG32F, GL_RG, GL_FLOAT, 8, GL_FALSE };
const GSpixelFormat GS_PIXEL_FORMAT_R32G32_UINT          = { GL_RGB32UI, GL_RG, GL_UNSIGNED_INT, 8, GL_FALSE };
const GSpixelFormat GS_PIXEL_FORMAT_R32G32_SINT          = { GL_RGB32I, GL_RG, GL_INT, 8, GL_FALSE };
const GSpixelFormat GS_PIXEL_FORMAT_D32_FLOAT_S8X24_UINT = { GL_DEPTH32F_STENCIL8, GL_DEPTH_STENCIL, GL_FLOAT_32_UNSIGNED_INT_24_8_REV, 4, GL_FALSE };
const GSpixelFormat GS_PIXEL_FORMAT_R10G10B10A2_UNORM    = { GL_RGB10_A2, GL_RGBA, GL_UNSIGNED_INT_10_10_10_2, 4, GL_FALSE };
const GSpixelFormat GS_PIXEL_FORMAT_R10G10B10A2_UINT     = { GL_RGB10_A2UI, GL_RGBA, GL_UNSIGNED_INT_10_10_10_2, 4, GL_FALSE };
const GSpixelFormat GS_PIXEL_FORMAT_R11G11B10_FLOAT      = { GL_R11F_G11F_B10F, GL_RGB, GL_FLOAT, 4, GL_FALSE };
const GSpixelFormat GS_PIXEL_FORMAT_R8G8B8A8_UNORM       = { GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, 4, GL_FALSE };
const GSpixelFormat GS_PIXEL_FORMAT_R8G8B8A8_UNORM_SRGB  = { GL_SRGB8_ALPHA8, GL_RGBA, GL_UNSIGNED_BYTE, 4, GL_FALSE };
const GSpixelFormat GS_PIXEL_FORMAT_R8G8B8A8_UINT        = { GL_RGBA8UI, GL_RGBA, GL_UNSIGNED_BYTE, 4, GL_FALSE };
const GSpixelFormat GS_PIXEL_FORMAT_R8G8B8A8_SNORM       = { GL_RGBA8_SNORM, GL_RGBA, GL_UNSIGNED_BYTE, 4, GL_FALSE };
const GSpixelFormat GS_PIXEL_FORMAT_R8G8B8A8_SINT        = { GL_RGBA8I, GL_RGBA, GL_BYTE, 4, GL_FALSE };
const GSpixelFormat GS_PIXEL_FORMAT_R16G16_FLOAT         = { GL_RGB16F, GL_RG, GL_HALF_FLOAT, 8, GL_FALSE };
const GSpixelFormat GS_PIXEL_FORMAT_R16G16_UNORM         = { GL_RG16, GL_RG, GL_UNSIGNED_SHORT, 8, GL_FALSE };
const GSpixelFormat GS_PIXEL_FORMAT_R16G16_UINT          = { GL_RG16UI, GL_RG, GL_UNSIGNED_SHORT, 8, GL_FALSE };
const GSpixelFormat GS_PIXEL_FORMAT_R16G16_SNORM         = { GL_RG_SNORM, GL_RG, GL_SHORT, 8, GL_FALSE };
const GSpixelFormat GS_PIXEL_FORMAT_R16G16_SINT          = { GL_RG16I, GL_RG, GL_SHORT, 8, GL_FALSE };
const GSpixelFormat GS_PIXEL_FORMAT_D32_FLOAT            = { GL_DEPTH_COMPONENT32F, GL_DEPTH_COMPONENT, GL_FLOAT, 4, GL_FALSE };
const GSpixelFormat GS_PIXEL_FORMAT_R32_FLOAT            = { GL_R32F, GL_R, GL_FLOAT, 4, GL_FALSE };
const GSpixelFormat GS_PIXEL_FORMAT_R32_UINT             = { GL_R32UI, GL_R, GL_UNSIGNED_INT, 4, GL_FALSE };
const GSpixelFormat GS_PIXEL_FORMAT_R32_SINT             = { GL_R32I, GL_R, GL_INT, 4, GL_FALSE };
const GSpixelFormat GS_PIXEL_FORMAT_D24_UNORM_S8_UINT    = { GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, 4, GL_FALSE };
const GSpixelFormat GS_PIXEL_FORMAT_R8G8_UNORM           = { GL_RG8, GL_RG, GL_UNSIGNED_BYTE, 2, GL_FALSE };
const GSpixelFormat GS_PIXEL_FORMAT_R8G8_UINT            = { GL_RG8UI, GL_RG, GL_UNSIGNED_BYTE, 2, GL_FALSE };
const GSpixelFormat GS_PIXEL_FORMAT_R8G8_SNORM           = { GL_RG8_SNORM, GL_RG, GL_BYTE, 2, GL_FALSE };
const GSpixelFormat GS_PIXEL_FORMAT_R8G8_SINT            = { GL_RG8I, GL_RG, GL_BYTE, 2, GL_FALSE };
const GSpixelFormat GS_PIXEL_FORMAT_R16_FLOAT            = { GL_R16F, GL_R, GL_HALF_FLOAT, 2, GL_FALSE };
const GSpixelFormat GS_PIXEL_FORMAT_D16_UNORM            = { GL_DEPTH_COMPONENT16, GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT, 2, GL_FALSE };
const GSpixelFormat GS_PIXEL_FORMAT_R16_UNORM            = { GL_R16, GL_R, GL_UNSIGNED_SHORT, 2, GL_FALSE };
const GSpixelFormat GS_PIXEL_FORMAT_R16_UINT             = { GL_R16UI, GL_R, GL_UNSIGNED_SHORT, 2, GL_FALSE };
const GSpixelFormat GS_PIXEL_FORMAT_R16_SNORM            = { GL_R16_SNORM, GL_R, GL_UNSIGNED_SHORT, 2, GL_FALSE };
const GSpixelFormat GS_PIXEL_FORMAT_R16_SINT             = { GL_R16I, GL_R, GL_SHORT, 2, GL_FALSE };
const GSpixelFormat GS_PIXEL_FORMAT_R8_UNORM             = { GL_R8, GL_R, GL_UNSIGNED_BYTE, 1, GL_FALSE };
const GSpixelFormat GS_PIXEL_FORMAT_R8_UINT              = { GL_R8UI, GL_R, GL_UNSIGNED_BYTE, 1, GL_FALSE };
const GSpixelFormat GS_PIXEL_FORMAT_R8_SNORM             = { GL_R8_SNORM, GL_R, GL_BYTE, 1, GL_FALSE };
const GSpixelFormat GS_PIXEL_FORMAT_R8_SINT              = { GL_R8I, GL_R, GL_BYTE, 1, GL_FALSE };
const GSpixelFormat GS_PIXEL_FORMAT_A8_UNORM             = { GL_ALPHA, GL_ALPHA, GL_UNSIGNED_BYTE, 1, GL_FALSE };
const GSpixelFormat GS_PIXEL_FORMAT_R9G9B9E5_SHAREDEXP   = { GL_RGB9_E5, GL_RGB, GL_UNSIGNED_INT_5_9_9_9_REV, 4, GL_FALSE };
const GSpixelFormat GS_PIXEL_FORMAT_BC1_UNORM            = { GL_COMPRESSED_RGB_S3TC_DXT1_EXT, GL_RGB, GL_UNSIGNED_BYTE, 8, GL_TRUE };
const GSpixelFormat GS_PIXEL_FORMAT_BC1_UNORM_SRGB       = { GL_COMPRESSED_SRGB_S3TC_DXT1_EXT,GL_RGB, GL_UNSIGNED_BYTE, 8, GL_TRUE };
const GSpixelFormat GS_PIXEL_FORMAT_BC3_UNORM            = { GL_COMPRESSED_RGBA_S3TC_DXT3_EXT, GL_RGBA, GL_UNSIGNED_BYTE, 16, GL_TRUE };
const GSpixelFormat GS_PIXEL_FORMAT_BC3_UNORM_SRGB       = { GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT3_EXT, GL_RGBA, GL_UNSIGNED_BYTE, 16, GL_TRUE };
const GSpixelFormat GS_PIXEL_FORMAT_BC5_UNORM            = { GL_COMPRESSED_RGBA_S3TC_DXT5_EXT, GL_RGBA, GL_UNSIGNED_BYTE, 16, GL_TRUE };
const GSpixelFormat GS_PIXEL_FORMAT_BC5_UNORM_SRGB       = { GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT, GL_RGBA, GL_UNSIGNED_BYTE, 16, GL_TRUE };
const GSpixelFormat GS_PIXEL_FORMAT_B5G6R5_UNORM         = { GL_RGB565, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, 2, GL_FALSE };
const GSpixelFormat GS_PIXEL_FORMAT_B5G5R5A1_UNORM       = { GL_RGB5_A1, GL_RGBA, GL_UNSIGNED_SHORT_5_5_5_1, 2,GL_FALSE };
const GSpixelFormat GS_PIXEL_FORMAT_B8G8R8A8_UNORM       = { GL_RGBA, GL_BGRA, GL_UNSIGNED_BYTE, 4, GL_FALSE };
const GSpixelFormat GS_PIXEL_FORMAT_B8G8R8X8_UNORM       = { GL_RGBA, GL_BGRA, GL_UNSIGNED_BYTE, 4, GL_FALSE };
const GSpixelFormat GS_PIXEL_FORMAT_LUMINANCE            = { GL_LUMINANCE, GL_LUMINANCE, GL_UNSIGNED_BYTE, 1, GL_FALSE };
const GSpixelFormat GS_PIXEL_FORMAT_R8G8B8_UNORM         = { GL_RGB, GL_RGB, GL_UNSIGNED_BYTE, 3, GL_FALSE };
const GSpixelFormat GS_PIXEL_FORMAT_B8G8R8_UNORM         = { GL_RGB, GL_BGR, GL_UNSIGNED_BYTE, 3, GL_FALSE };

/*=============================================================================
*
* Purpose : gslib用ピクセルフォーマットを取得
*
* Return  : ピクセルフォーマットを返す．
*
*============================================================================*/
extern const GSpixelFormat*
gsGetPixelFormat
(
	GLint			InternalFormat,
	GLint			Format,
	GLsizei			Type
)
{
	static const GSpixelFormat* format[] = {
		&GS_PIXEL_FORMAT_R8G8B8A8_UNORM,
		&GS_PIXEL_FORMAT_R8G8B8_UNORM,
		&GS_PIXEL_FORMAT_B8G8R8_UNORM,
		&GS_PIXEL_FORMAT_R8G8B8A8_UNORM_SRGB,
		&GS_PIXEL_FORMAT_R8G8B8A8_UINT,
		&GS_PIXEL_FORMAT_R8G8B8A8_SNORM,
		&GS_PIXEL_FORMAT_R8G8B8A8_SINT,
		&GS_PIXEL_FORMAT_R32G32B32A32_FLOAT,
		&GS_PIXEL_FORMAT_R32G32B32A32_UINT,
		&GS_PIXEL_FORMAT_R32G32B32A32_SINT,
		&GS_PIXEL_FORMAT_R32G32B32_FLOAT,
		&GS_PIXEL_FORMAT_R32G32B32_UINT,
		&GS_PIXEL_FORMAT_R32G32B32_SINT,
		&GS_PIXEL_FORMAT_R16G16B16A16_FLOAT,
		&GS_PIXEL_FORMAT_R16G16B16A16_UNORM,
		&GS_PIXEL_FORMAT_R16G16B16A16_UINT,
		&GS_PIXEL_FORMAT_R16G16B16A16_SNORM,
		&GS_PIXEL_FORMAT_R16G16B16A16_SINT,
		&GS_PIXEL_FORMAT_R32G32_FLOAT,
		&GS_PIXEL_FORMAT_R32G32_UINT,
		&GS_PIXEL_FORMAT_R32G32_SINT,
		&GS_PIXEL_FORMAT_D32_FLOAT_S8X24_UINT,
		&GS_PIXEL_FORMAT_R10G10B10A2_UNORM,
		&GS_PIXEL_FORMAT_R10G10B10A2_UINT,
		&GS_PIXEL_FORMAT_R11G11B10_FLOAT,
		&GS_PIXEL_FORMAT_R16G16_FLOAT,
		&GS_PIXEL_FORMAT_R16G16_UNORM,
		&GS_PIXEL_FORMAT_R16G16_UINT,
		&GS_PIXEL_FORMAT_R16G16_SNORM,
		&GS_PIXEL_FORMAT_R16G16_SINT,
		&GS_PIXEL_FORMAT_D32_FLOAT,
		&GS_PIXEL_FORMAT_R32_FLOAT,
		&GS_PIXEL_FORMAT_R32_UINT,
		&GS_PIXEL_FORMAT_R32_SINT,
		&GS_PIXEL_FORMAT_D24_UNORM_S8_UINT,
		&GS_PIXEL_FORMAT_R8G8_UNORM,
		&GS_PIXEL_FORMAT_R8G8_UINT,
		&GS_PIXEL_FORMAT_R8G8_SNORM,
		&GS_PIXEL_FORMAT_R8G8_SINT,
		&GS_PIXEL_FORMAT_R16_FLOAT,
		&GS_PIXEL_FORMAT_D16_UNORM,
		&GS_PIXEL_FORMAT_R16_UNORM,
		&GS_PIXEL_FORMAT_R16_UINT,
		&GS_PIXEL_FORMAT_R16_SNORM,
		&GS_PIXEL_FORMAT_R16_SINT,
		&GS_PIXEL_FORMAT_R8_UNORM,
		&GS_PIXEL_FORMAT_R8_UINT,
		&GS_PIXEL_FORMAT_R8_SNORM,
		&GS_PIXEL_FORMAT_R8_SINT,
		&GS_PIXEL_FORMAT_A8_UNORM,
		&GS_PIXEL_FORMAT_R9G9B9E5_SHAREDEXP,
		&GS_PIXEL_FORMAT_BC1_UNORM,
		&GS_PIXEL_FORMAT_BC1_UNORM_SRGB,
		&GS_PIXEL_FORMAT_BC3_UNORM,
		&GS_PIXEL_FORMAT_BC3_UNORM_SRGB,
		&GS_PIXEL_FORMAT_BC5_UNORM,
		&GS_PIXEL_FORMAT_BC5_UNORM_SRGB,
		&GS_PIXEL_FORMAT_B5G6R5_UNORM,
		&GS_PIXEL_FORMAT_B5G5R5A1_UNORM,
		&GS_PIXEL_FORMAT_B8G8R8A8_UNORM,
		&GS_PIXEL_FORMAT_B8G8R8X8_UNORM,
		&GS_PIXEL_FORMAT_LUMINANCE
	};
	int i;
	for (i = 0; i < (sizeof(format) / sizeof(format[0])); ++i) {
		if ((format[i]->InternalFormat == InternalFormat)
		 && (format[i]->Format         == Format)
		 && (format[i]->Type           == Type))
		{
			return format[i];
		}
	}
	return NULL;
}

/********** End of File ******************************************************/
