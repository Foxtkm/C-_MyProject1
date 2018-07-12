/******************************************************************************
* File Name : GSfile_path.h                      Ver : 1.00  Date : 2016-06-07
*
* Description :
*
*		ファイルパスの分解と組み立て ヘッダファイル．．
*
* Author : Moriharu Ohzu.
*
******************************************************************************/
#ifndef	_GS_FILE_PATH_H_
#define	_GS_FILE_PATH_H_

#include <wchar.h>

#ifdef __cplusplus
extern "C" {
#endif

// ファイルパスの分解
void gsSplitPath(const char* path, char* drive, char* dir, char* fname, char* ext);

// ファイルパスの組み立て
void gsMakePath(char* path, const char* drive, const char* dir, const char* fname, const char* ext);

#ifdef __cplusplus
}
#endif

#endif

/********** End of File ******************************************************/
