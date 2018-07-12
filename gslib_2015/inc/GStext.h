/******************************************************************************
* File Name : GStext.h                           Ver : 1.00  Date : 2003-04-28
*
* Description :
*
*       文字列描画 ヘッダファイル．
*
* Author : Moriharu Ohzu.
*
******************************************************************************/
#ifndef		_GSTEXT_H_
#define		_GSTEXT_H_

#include	"GStype.h"

/****** 定数宣言 **************************************************************/

#define	GS_FONT_NORMAL		0			/* 何も指定なし		*/
#define	GS_FONT_BOLD		( 1 << 0 )	/* 強調文字			*/
#define	GS_FONT_ITALIC		( 1 << 1 )	/* イタリック		*/
#define	GS_FONT_UNDERLINE	( 1 << 2 )	/* アンダーライン	*/
#define	GS_FONT_STRIKEOUT	( 1 << 3 )	/* 取り消線			*/

/****** 関数プロトタイプ宣言 *************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*=============================================================================
*
* Purpose : 文字列の描画．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsTextDraw
(
	GSuint			FontCode,			/* フォントコード				*/
	GSuint			FontPoint,			/* フォントのポイント数			*/
	const char*		pFontName,			/* フォント名					*/
	const char*		pString				/* 文字列						*/
);

#ifdef __cplusplus
}
#endif

#endif

/********** End of File ******************************************************/
