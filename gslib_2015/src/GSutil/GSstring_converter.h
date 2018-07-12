/******************************************************************************
* File Name : GSstring_converter.h                            Ver : 1.00  Date : 2002-08-03
*
* Description :
*
*		文字コード変換 ヘッダファイル．．
*
* Author : Moriharu Ohzu.
*
******************************************************************************/
#ifndef	_GS_STRING_CONVERTER_H_
#define	_GS_STRING_CONVERTER_H_

#include <wchar.h>

#ifdef __cplusplus
extern "C" {
#endif

// UTF16をSJISに変換 (変換後の文字列は要free)
char* utf16_to_sjis(const wchar_t* utf16);

// SJISをUTF16に変換 (変換後の文字列は要free)
wchar_t* sjis_to_utf16(const char* sjis);

#ifdef __cplusplus
}
#endif

#endif

/********** End of File ******************************************************/
