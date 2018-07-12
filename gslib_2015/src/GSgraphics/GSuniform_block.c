/******************************************************************************
* File Name : GSuniform_block.c                  Ver : 1.00  Date : 2016-11-27
*
* Description :
*
*        Uniformブロック管理 ソースファイル．
*
* Author : Moriharu Ohzu.
*
******************************************************************************/
#include "GSuniform_block.h"
#include "heap.h"
#include <opengl_ext.h>

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
)
{
	GSunifomBlock*	pUnifomBlock;
	pUnifomBlock = new_object(sizeof(GSunifomBlock));
	pUnifomBlock->dwSize = Size;
	glGenBuffers(1, &pUnifomBlock->dwName);
	glBindBuffer(GL_UNIFORM_BUFFER, pUnifomBlock->dwName);
	glBufferData(GL_UNIFORM_BUFFER, Size, pData, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	return pUnifomBlock;
}

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
)
{
	glDeleteBuffers(1, &pUnifomBlock->dwName);
	del_object(pUnifomBlock);
}

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
)
{
	glBindBuffer(GL_UNIFORM_BUFFER, pUnifomBlock->dwName);
	glBufferSubData(GL_UNIFORM_BUFFER, Offset, Size, pData);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

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
)
{
	return pUnifomBlock->dwSize;
}

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
)
{
	glBindBufferBase(GL_UNIFORM_BUFFER, index, pUnifomBlock->dwName);
}

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
)
{
	(void)pUnifomBlock;
	glBindBufferBase(GL_UNIFORM_BUFFER, index, 0);
}

/********** End of File ******************************************************/
