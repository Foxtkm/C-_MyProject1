/******************************************************************************
* File Name : GSuniform_block.h                  Ver : 1.00  Date : 2016-11-27
*
* Description :
*
*        Uniformブロック管理 ヘッダファイル．
*
* Author : Moriharu Ohzu.
*
******************************************************************************/
#ifndef		_GSUNIFORMBLOCK_H_
#define		_GSUNIFORMBLOCK_H_

#include	"GStype.h"

/****** データ型宣言 *********************************************************/

/*----- Uniformブロック構造体 -----------------------------------------------*/

typedef struct
{
	GSuint			dwName;
	GSuint			dwSize;
} GSunifomBlock;

/****** 関数プロトタイプ宣言 *************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*=============================================================================
*
* Purpose : Uniformブロックの生成．
*
* Return  : Uniformブロックを返す．
*
*============================================================================*/
extern GSunifomBlock*
gsUnifomBlockNew
(
	GLsizei					Size,		/* バッファデータサイズ */
	const GLvoid*			pData		/* バッファデータ		*/
);

/*=============================================================================
*
* Purpose : Uniformブロックの消滅．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsUniformBlockDelete
(
	GSunifomBlock*		pUnifomBlock	/* Uniformブロック		*/
);

/*=============================================================================
*
* Purpose : Uniformブロックのデータの更新．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsUniformBlockUpdate
(
	GSunifomBlock*		pUnifomBlock,	/* Uniformブロック		            */
	GLsizei				Size,			/* 更新するバッファデータのバイト数 */
	GLsizei				Offset,			/* 更新する場所のオフセット         */
	const GLvoid*		pData			/* 更新するバッファデータ           */
);

/*=============================================================================
*
* Purpose : Uniformブロックのデータサイズの取得．
*
* Return  : なし．
*
*============================================================================*/
extern GSuint
gsUniformBlockGetSize
(
	GSunifomBlock*		pUnifomBlock	/* Uniformブロック		            */
);

/*=============================================================================
*
* Purpose : Uniformブロックのバインド．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsUniformBlockBind
(
	GSunifomBlock*		pUnifomBlock,	/* Uniformブロック		            */
	GSuint				index			/* バインド先のインデックス         */
);

/*=============================================================================
*
* Purpose : Uniformブロックのアンバインド．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsUniformBlockUnbind
(
	GSunifomBlock*		pUnifomBlock,	/* Uniformブロック		            */
	GSuint				index			/* アンバインド先のインデックス     */
);

#ifdef __cplusplus
}
#endif

#endif

/********** End of File ******************************************************/
