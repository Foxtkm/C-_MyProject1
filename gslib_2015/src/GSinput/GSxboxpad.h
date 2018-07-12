/******************************************************************************
* File Name : GSxboxpad.h                        Ver : 1.00  Date : 2016-04-16
*
* Description :
*
*       XBOXパッド入力 ヘッダファイル.
*
* Author : Moriharu Ohzu.
*
******************************************************************************/
#ifndef GS_XBOX_PAD_H_
#define GS_XBOX_PAD_H_

#include	<GStype.h>
#include	<Xinput.h>

#ifdef __cplusplus
extern "C" {
#endif

/****** データ型宣言 *********************************************************/

extern const GSushort GS_XBOX_PAD_UP;
extern const GSushort GS_XBOX_PAD_DOWN;
extern const GSushort GS_XBOX_PAD_LEFT;
extern const GSushort GS_XBOX_PAD_RIGHT;
extern const GSushort GS_XBOX_PAD_START;
extern const GSushort GS_XBOX_PAD_BACK;
extern const GSushort GS_XBOX_PAD_LEFT_THUMB;
extern const GSushort GS_XBOX_PAD_RIGHT_THUMB;
extern const GSushort GS_XBOX_PAD_LEFT_SHOULDER;
extern const GSushort GS_XBOX_PAD_RIGHT_SHOULDER;
extern const GSushort GS_XBOX_PAD_A;
extern const GSushort GS_XBOX_PAD_B;
extern const GSushort GS_XBOX_PAD_X;
extern const GSushort GS_XBOX_PAD_Y;

/*----- XBOX PADデバイス構造体 ----------------------------------------------*/

#define   GS_XBOX_PAD_MAX 4

typedef	struct
{
	int                 num_pad;					/* パッド数				*/
	XINPUT_STATE        state[2][GS_XBOX_PAD_MAX];	/* 入力バッファ			*/
	XINPUT_STATE*       newbuf;						/* 現在の入力バッファ   */
	XINPUT_STATE*       oldbuf;						/* 前回の入力バッファ   */

} GSxboxpad;

/*=============================================================================
*
* Purpose : XBOXパッドの生成．
*
* Return  : XBOXを返す．
*
*============================================================================*/
extern GSxboxpad*
XBoxPadNew
(
	void
);

/*=============================================================================
*
* Purpose : XBOXパッドの消滅．
*
* Return  : なし．
*
*============================================================================*/
extern void
XBoxPadDelete
(
	GSxboxpad*			pad		/* XBOXパッド         　　　　      */
);

/*=============================================================================
*
* Purpose : XBOXパッドの状態を読み込む．
*
* Return  : なし．
*
*============================================================================*/
extern void
XBoxPadRead
(
	GSxboxpad*			pad		/* XBOXパッド         　　　　      */
);

/*=============================================================================
*
* Purpose : ボタンが押されていか調べる．
*
* Return  : 押されていれば真，押されていなければ偽を返す．
*
*============================================================================*/
extern GSboolean
XBoxPadButtonState
(
	GSxboxpad*			pad,		/* XBOXパッド         　　　　      */
	GSuint				pad_no,		/* パッドナンバ                     */
	GSushort			button		/* 調べたいボタン名			　      */
);

/*=============================================================================
*
* Purpose : ボタンが押されたか調べる．
*
* Return  : 押されたら真，押されなければ偽を返す．
*
*============================================================================*/
extern GSboolean
XBoxPadButtonTrigger
(
	GSxboxpad*			pad,		/* XBOXパッド         　　　　      */
	GSuint				pad_no,		/* パッドナンバ                     */
	GSushort			button		/* 調べたいボタン名			　      */
);

/*=============================================================================
*
* Purpose : ボタンがが離されたか調べる．
*
* Return  : 離されたら真，離されなければ偽を返す．
*
*============================================================================*/
extern GSboolean
XBoxPadButtonDetach
(
	GSxboxpad*			pad,		/* XBOXパッド         　　　　      */
	GSuint				pad_no,		/* パッドナンバ                     */
	GSushort			button		/* 調べたいボタン名			　      */
);

/*=============================================================================
*
* Purpose : 左トリガの値を取得．
*
* Return  : 左トリガの値を返す．(0.0f ～ 1.0f)
*
*============================================================================*/
extern GSfloat
XBoxPadGetLeftTrigger
(
	GSxboxpad*			pad,		/* XBOXパッド         　　　　      */
	GSuint				pad_no		/* パッドナンバ                     */
);

/*=============================================================================
*
* Purpose : 右トリガの値を取得．
*
* Return  : 右トリガの値を返す．(0.0f ～ 1.0f正規化した値)
*
*============================================================================*/
extern GSfloat
XBoxPadGetRightTrigger
(
	GSxboxpad*			pad,		/* XBOXパッド         　　　　      */
	GSuint				pad_no		/* パッドナンバ                     */
);

/*=============================================================================
*
* Purpose : 左アナログスティックの状態を取得．
*
* Return  : 左アナログスティックの状態を返す．(-1.0f ～ 1.0fに正規化した値)
*
*============================================================================*/
extern void
XBoxPadGetLeftAxis
(
	GSxboxpad*			pad,		/* XBOXパッド         　　　　      */
	GSuint				pad_no,		/* パッドナンバ                     */
	GSvector2*			result		/* アナログスティックの状態         */
);

/*=============================================================================
*
* Purpose : 右アナログスティックの状態を取得．
*
* Return  : 右アナログスティックの状態を返す．(-1.0f ～ 1.0fに正規化した値)
*
*============================================================================*/
extern void
XBoxPadGetRightAxis
(
	GSxboxpad*			pad,		/* XBOXパッド         　　　　      */
	GSuint				pad_no,		/* パッドナンバ                     */
	GSvector2*			result		/* アナログスティックの状態         */
);

/*=============================================================================
*
* Purpose : 接続されているジョイスティック数を調べる．
*
* Return  : 接続されているジョイスティック数を返す．
*
*============================================================================*/
extern GSuint
XBoxGetPadCount
(
	GSxboxpad*			pad			/* XBOXパッド         　　　　      */
);


#ifdef __cplusplus
}
#endif

#endif

/********** End of File ******************************************************/
