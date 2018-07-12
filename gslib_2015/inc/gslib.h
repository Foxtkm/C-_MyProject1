/******************************************************************************
* File Name : gslib.h                            Ver : 1.00  Date : 2003-04-05
*
* Description :
*
*       ゲームシステム ヘッダファイル．
*
* Author : Moriharu Ohzu.
*
******************************************************************************/
#ifndef		_GSLIB_H_
#define		_GSLIB_H_

#include	<GStype.h>			/* 共通型定義ヘッダファイル				*/
#include	<GSmath.h>			/* 基本計算ヘッダファイル				*/
#include	<GSvector3.h>		/* ３Ｄベクトル計算ヘッダファイル		*/
#include	<GSmatrix4.h>		/* マトリクス計算ヘッダファイル			*/
#include	<GSplane.h>			/* 平面計算ヘッダファイル				*/
#include	<GScolor.h>			/* カラー計算ヘッダファイル				*/
#include	<GScollision.h>		/* 衝突判定計算ヘッダファイル			*/
#include	<GSfrustum.h>		/* 視錐台カリングヘッダファイル			*/
#include	<GSframetimer.h>	/* フレームタイマヘッダファイル			*/
#include	<GSpixel_format.h>	/* ピクセルフォーマットヘッダファイル	*/

#include	<GSgraphics.h>		/* グラフィックシステムヘッダファイル	*/
#include	<GSsound.h>			/* サウンドシステムヘッダファイル		*/
#include	<GSinput.h>			/* 入力デバイスシステムヘッダファイル	*/

#include	<GL/glut.h>
#include	<GL/wglext.h>
#include	<opengl_ext.h>

#endif

/********** End of File ******************************************************/
