#ifndef GS_PIXEL_FORMAT_H_
#define GS_PIXEL_FORMAT_H_

#include <GStype.h>

/****** データ型宣言 *********************************************************/

/*----- ピクセル情報構造体 --------------------------------------------------*/

typedef struct
{
	GLint			InternalFormat;
	GLint			Format;
	GLsizei			Type;
	GLsizei			Size;
	GLboolean		Complessed;
} GSpixelFormat;

/*----- ピクセル情報構造体 --------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

extern const GSpixelFormat GS_PIXEL_FORMAT_UNKNOWN;
extern const GSpixelFormat GS_PIXEL_FORMAT_R32G32B32A32_FLOAT;
extern const GSpixelFormat GS_PIXEL_FORMAT_R32G32B32A32_UINT;
extern const GSpixelFormat GS_PIXEL_FORMAT_R32G32B32A32_SINT;
extern const GSpixelFormat GS_PIXEL_FORMAT_R32G32B32_FLOAT;
extern const GSpixelFormat GS_PIXEL_FORMAT_R32G32B32_UINT;
extern const GSpixelFormat GS_PIXEL_FORMAT_R32G32B32_SINT;
extern const GSpixelFormat GS_PIXEL_FORMAT_R16G16B16A16_FLOAT;
extern const GSpixelFormat GS_PIXEL_FORMAT_R16G16B16A16_UNORM;
extern const GSpixelFormat GS_PIXEL_FORMAT_R16G16B16A16_UINT;
extern const GSpixelFormat GS_PIXEL_FORMAT_R16G16B16A16_SNORM;
extern const GSpixelFormat GS_PIXEL_FORMAT_R16G16B16A16_SINT;
extern const GSpixelFormat GS_PIXEL_FORMAT_R32G32_FLOAT;
extern const GSpixelFormat GS_PIXEL_FORMAT_R32G32_UINT;
extern const GSpixelFormat GS_PIXEL_FORMAT_R32G32_SINT;
extern const GSpixelFormat GS_PIXEL_FORMAT_D32_FLOAT_S8X24_UINT;
extern const GSpixelFormat GS_PIXEL_FORMAT_R10G10B10A2_UNORM;
extern const GSpixelFormat GS_PIXEL_FORMAT_R10G10B10A2_UINT;
extern const GSpixelFormat GS_PIXEL_FORMAT_R11G11B10_FLOAT;
extern const GSpixelFormat GS_PIXEL_FORMAT_R8G8B8A8_UNORM;
extern const GSpixelFormat GS_PIXEL_FORMAT_R8G8B8A8_UNORM_SRGB;
extern const GSpixelFormat GS_PIXEL_FORMAT_R8G8B8A8_UINT;
extern const GSpixelFormat GS_PIXEL_FORMAT_R8G8B8A8_SNORM;
extern const GSpixelFormat GS_PIXEL_FORMAT_R8G8B8A8_SINT;
extern const GSpixelFormat GS_PIXEL_FORMAT_R16G16_FLOAT;
extern const GSpixelFormat GS_PIXEL_FORMAT_R16G16_UNORM;
extern const GSpixelFormat GS_PIXEL_FORMAT_R16G16_UINT;
extern const GSpixelFormat GS_PIXEL_FORMAT_R16G16_SNORM;
extern const GSpixelFormat GS_PIXEL_FORMAT_R16G16_SINT;
extern const GSpixelFormat GS_PIXEL_FORMAT_D32_FLOAT;
extern const GSpixelFormat GS_PIXEL_FORMAT_R32_FLOAT;
extern const GSpixelFormat GS_PIXEL_FORMAT_R32_UINT;
extern const GSpixelFormat GS_PIXEL_FORMAT_R32_SINT;
extern const GSpixelFormat GS_PIXEL_FORMAT_D24_UNORM_S8_UINT;
extern const GSpixelFormat GS_PIXEL_FORMAT_R8G8_UNORM;
extern const GSpixelFormat GS_PIXEL_FORMAT_R8G8_UINT;
extern const GSpixelFormat GS_PIXEL_FORMAT_R8G8_SNORM;
extern const GSpixelFormat GS_PIXEL_FORMAT_R8G8_SINT;
extern const GSpixelFormat GS_PIXEL_FORMAT_R16_FLOAT;
extern const GSpixelFormat GS_PIXEL_FORMAT_D16_UNORM;
extern const GSpixelFormat GS_PIXEL_FORMAT_R16_UNORM;
extern const GSpixelFormat GS_PIXEL_FORMAT_R16_UINT;
extern const GSpixelFormat GS_PIXEL_FORMAT_R16_SNORM;
extern const GSpixelFormat GS_PIXEL_FORMAT_R16_SINT;
extern const GSpixelFormat GS_PIXEL_FORMAT_R8_UNORM;
extern const GSpixelFormat GS_PIXEL_FORMAT_R8_UINT;
extern const GSpixelFormat GS_PIXEL_FORMAT_R8_SNORM;
extern const GSpixelFormat GS_PIXEL_FORMAT_R8_SINT;
extern const GSpixelFormat GS_PIXEL_FORMAT_A8_UNORM;
extern const GSpixelFormat GS_PIXEL_FORMAT_R9G9B9E5_SHAREDEXP;
extern const GSpixelFormat GS_PIXEL_FORMAT_BC1_UNORM;
extern const GSpixelFormat GS_PIXEL_FORMAT_BC1_UNORM_SRGB;
extern const GSpixelFormat GS_PIXEL_FORMAT_BC3_UNORM;
extern const GSpixelFormat GS_PIXEL_FORMAT_BC3_UNORM_SRGB;
extern const GSpixelFormat GS_PIXEL_FORMAT_BC5_UNORM;
extern const GSpixelFormat GS_PIXEL_FORMAT_BC5_UNORM_SRGB;
extern const GSpixelFormat GS_PIXEL_FORMAT_B5G6R5_UNORM;
extern const GSpixelFormat GS_PIXEL_FORMAT_B5G5R5A1_UNORM;
extern const GSpixelFormat GS_PIXEL_FORMAT_B8G8R8A8_UNORM;
extern const GSpixelFormat GS_PIXEL_FORMAT_B8G8R8X8_UNORM;
extern const GSpixelFormat GS_PIXEL_FORMAT_LUMINANCE;

#ifdef __cplusplus
}
#endif

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
);

#endif

/********** End of File ******************************************************/

