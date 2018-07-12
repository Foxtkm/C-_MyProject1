/******************************************************************************
* File Name : lklist.c                             Ver : 1.00  Date : 98-10-22
*
* Description :
*
*       連結リスト．
*
* Author : Moriharu Ohzu.
*
******************************************************************************/
#include	"lklist.h"
#include	"heap.h"

/****** データ型宣言 *********************************************************/

/*----- 連結リストノード構造体 ----------------------------------------------*/

typedef	struct node
{
	struct node*	prev;			/* 前のノードのポインタ         */
	struct node*	next;			/* 次のノードのポインタ         */
	void*			item;			/* 格納している要素             */

} NODE;

/*----- 連結リスト構造体 -----------------------------------------------------*/

typedef	struct
{
	int				count;			/* 現在の要素数                 */
	NODE			header;			/* ダミーノードヘッダ           */

} LIST;

/*----- 連結リスト反復子構造体 -----------------------------------------------*/

typedef	struct
{
	LIST*			list;			/* 連結リストのポインタ         */
	NODE*			node;			/* 参照中のノード               */

} ITER;

/****** 関数プロトタイプ宣言 *************************************************/

static NODE*	node_new( void* );
static void		node_delete( NODE* );
static void		node_remove( NODE* );
static void		node_insert_after( NODE*, NODE* );

/*=============================================================================
*
* Purpose : 連結リストの生成．
*
* Return  : 連結リストハンドラを返す．
*
*============================================================================*/
LKLIST
LkListNew
(
	void
)
{
	LIST*		lst;

	lst = new_object( sizeof( LIST ) );

	lst->count       = 0;
	lst->header.prev = &lst->header;
	lst->header.next = &lst->header;
	lst->header.item = NULL;

	return	(LKLIST)lst;
}

/*=============================================================================
*
* Purpose : 連結リストの消滅．
*
* Return  : なし．
*
*============================================================================*/
void
LkListDelete
(
	LKLIST		hnd					/* 連結リストハンドラ           */
)
{
	LIST*		lst = (LIST*)hnd;
	NODE*		node;
	NODE*		next;

	for ( node = lst->header.next; node != &lst->header; node = next )
	{
		next = node->next;

		node_delete( node );
	}
	del_object( lst );
}

/*=============================================================================
*
* Purpose : 連結リストの要素数を調べる．
*
* Return  : 連結リストの要素数を返す．
*
*============================================================================*/
int
LkListGetCount
(
	LKLIST		hnd					/* 連結リストハンドラ           */
)
{
	LIST*		lst = (LIST*)hnd;

	return	lst->count;
}

/*=============================================================================
*
* Purpose : 連結リストが空きかどうか調べる．
*
* Return  : 空きであれば真，空きでなければ偽を返す．
*
*============================================================================*/
BOOL
LkListIsEmpty
(
	LKLIST		hnd					/* 連結リストハンドラ           */
)
{
	LIST*		lst = (LIST*)hnd;

	if ( lst->count == 0 )
	{
		return	TRUE;
	}

	return	FALSE;
}

/*=============================================================================
*
* Purpose : 連結リストの先頭に要素を追加．
*
* Return  : なし．
*
*============================================================================*/
void
LkListAddHead
(
	LKLIST		hnd,				/* 連結リストハンドラ           */
	void*		item				/* 追加する要素                 */
)
{
	LIST*		lst = (LIST*)hnd;

	node_insert_after( &lst->header, node_new( item ) );

	lst->count++;
}

/*=============================================================================
*
* Purpose : 連結リストの最後尾に要素を追加．
*
* Return  : なし．
*
*============================================================================*/
void
LkListAddTail
(
	LKLIST		hnd,				/* 連結リストハンドラ           */
	void*		item				/* 追加する要素                 */
)
{
	LIST*		lst = (LIST*)hnd;

	node_insert_after( lst->header.prev, node_new( item ) );

	lst->count++;
}

/*=============================================================================
*
* Purpose : 連結リストの先頭の要素を取り除く．
*
* Return  : 連結リストから取り除いた先頭の要素を返す．
*           連結リストが空きの場合はＮＵＬＬポインタを返す．
*
*============================================================================*/
void*
LkListRemoveHead
(
	LKLIST		hnd					/* 連結リストハンドラ           */
)
{
	LIST*		lst = (LIST*)hnd;
	void*		item;

	if ( lst->count == 0 )
	{
		return	NULL;
	}

	item = lst->header.next->item;

	node_remove( lst->header.next );

	lst->count--;

	return	item;
}

/*=============================================================================
*
* Purpose : 連結リストの最後尾の要素を取り除く．
*
* Return  : 連結リストから取り除いた最後尾の要素を返す．
*           連結リストが空きの場合はＮＵＬＬポインタを返す．
*
*============================================================================*/
void*
LkListRemoveTail
(
	LKLIST		hnd					/* 連結リストハンドラ           */
)
{
	LIST*		lst = (LIST*)hnd;
	void*		item;

	if ( lst->count == 0 )
	{
		return	NULL;
	}

	item = lst->header.prev->item;

	node_remove( lst->header.prev );

	lst->count--;

	return	item;
}

/*=============================================================================
*
* Purpose : 連結リストから指定要素を取り除く．
*
* Return  : 連結リストから取り除いた要素を返す．
*           連結リストが空きの場合はＮＵＬＬポインタを返す．
*
*============================================================================*/
void*
LkListRemove
(
	LKLIST		hnd,				/* 連結リストハンドラ           */
	void*		item				/* 取り除く要素                 */
)
{
	LIST*		lst = (LIST*)hnd;
	void*		find_item;
	NODE*		node;

	find_item = NULL;
	for ( node = lst->header.next; node != &lst->header; node = node->next )
	{
		if ( node->item == item )
		{
			find_item = node->item;
			node_remove( node );
			lst->count--;
			break;
		}
	}

	return	find_item;
}

/*=============================================================================
*
* Purpose : 連結リストから指定位置の要素を取り除く．
*
* Return  : 連結リストから取り除いた要素を返す．
*           連結リストが空きの場合はＮＵＬＬポインタを返す．
*
*============================================================================*/
void*
LkListRemoveAt
(
	LKLIST		hnd,				/* 連結リストハンドラ           */
	int			index				/* 取り出したい位置             */
)
{
	LIST*		lst = (LIST*)hnd;
	NODE*		node;
	void*		item;
	int			count;

	if ( index >= lst->count )
	{
		return	NULL;
	}

	node = lst->header.next;
	for ( count = 0; count < index; count++ )
	{
		 node = node->next;
	}
	item = node->item;
	node_remove( node );
	lst->count--;

	return	item;
}

/*=============================================================================
*
* Purpose : 連結リストから全ての要素を取り除く．
*
* Return  : なし．
*
*============================================================================*/
void
LkListRemoveAll
(
	LKLIST		hnd					/* 連結リストハンドラ           */
)
{
	LIST*		lst = (LIST*)hnd;
	NODE*		node;
	NODE*		next;

	for ( node = lst->header.next; node != &lst->header; node = next )
	{
		next = node->next;

		node_delete( node );
	}
	lst->count = 0;
}

/*=============================================================================
*
* Purpose : 連結リストから指定位置の要素を取り出す．
*
* Return  : 連結リストから取り出した要素を返す．
*           連結リストが空きの場合はＮＵＬＬポインタを返す．
*
*============================================================================*/
void*
LkListGetAt
(
	LKLIST		hnd,				/* 連結リストハンドラ           */
	int			index				/* 取り出したい位置             */
)
{
	LIST*		lst = (LIST*)hnd;
	int			count;
	NODE*		node;

	if ( index >= lst->count )
	{
		return	NULL;
	}

	node = lst->header.next;
	for ( count = 0; count < index; count++ )
	{
		 node = node->next;
	}

	return	node->item;
}

/*=============================================================================
*
* Purpose : 連結リストの指定位置に要素を設定．
*
* Return  : なし．
*
*============================================================================*/
void
LkListSetAt
(
	LKLIST		hnd,				/* 連結リストハンドラ           */
	int			index,				/* 設定したい位置               */
	void*		item				/* 設定したい要素               */
)
{
	LIST*		lst = (LIST*)hnd;
	int			count;
	NODE*		node;

	if ( index >= lst->count )
	{
		return;
	}

	node = lst->header.next;
	for ( count = 0; count < index; count++ )
	{
		 node = node->next;
	}

	node->item = item;
}

/*=============================================================================
*
* Purpose : 連結リストの指定要素の前に要素を挿入．
*
* Return  : なし．
*
*============================================================================*/
void
LkListInsertBefore
(
	LKLIST		hnd,				/* 連結リストハンドラ           */
	void*		next,				/* 挿入したい要素の次の要素     */
	void*		item				/* 挿入したい要素               */
)
{
	LIST*		lst = (LIST*)hnd;
	NODE*		node;

	for ( node = lst->header.next; node != &lst->header; node = node->next )
	{
		if ( node->item == next )
		{
			break;
		}
	}

	node_insert_after( node->prev, node_new( item ) );

	lst->count++;
}

/*=============================================================================
*
* Purpose : 連結リストの指定要素の後ろに要素を挿入．
*
* Return  : なし．
*
*============================================================================*/
void
LkListInsertAfter
(
	LKLIST		hnd,				/* 連結リストハンドラ           */
	void*		prev,				/* 挿入したい要素の前の要素     */
	void*		item				/* 挿入したい要素               */
)
{
	LIST*		lst = (LIST*)hnd;
	NODE*		node;

	for ( node = lst->header.next; node != &lst->header; node = node->next )
	{
		if ( node->item == prev )
		{
			break;
		}
	}

	node_insert_after( node, node_new( item ) );

	lst->count++;
}

/*=============================================================================
*
* Purpose : 連結リストから指定要素のインデックスを求める．
*
* Return  : 要素のインデックスを返す．もし，要素がなかった場合－１を返す．
*
*============================================================================*/
int
LkListFindIndex
(
	LKLIST		hnd,				/* 連結リストハンドラ           */
	void* 		item				/* インデックスを求めたい要素   */
)
{
	LIST*		lst = (LIST*)hnd;
	int			count;
	NODE*		node;

	count = 0;
	for ( node = lst->header.next; node != &lst->header; node = node->next )
	{
		if ( node->item == item )
		{
			break;
		}
		count++;
	}

	if ( node == &lst->header )
	{
		return	-1;
	}

	return	count;
}

/*=============================================================================
*
* Purpose : 連結リストから要素を検索する．
*
* Return  : 検索に成功すれば発見した要素を返す．
*           検索に失敗した場合は，ＮＵＬＬポインタを返す．
*
*============================================================================*/
void*
LkListSearch
(
	LKLIST		hnd,				/* 連結リストハンドラ           */
	const void*	key,				/* 検索キーのポインタ           */
	BOOL		(*cmp)( const void* key, const void* item )
									/* 比較関数のポインタ           */
)
{
	LIST*		lst = (LIST*)hnd;
	NODE*		node;

	for ( node = lst->header.next; node != &lst->header; node = node->next )
	{
		if ( cmp( key, node->item ) == TRUE )
		{
			return	node->item;
		}
	}

	return	NULL;
}

/*=============================================================================
*
* Purpose : 連結リストから要素を検索する．
*
* Return  : 検索に成功すれば発見した要素のインデックスを返す．
*           検索に失敗した場合は，－１を返す．
*
*============================================================================*/
int
LkListSearchIndex
(
	LKLIST		hnd,				/* 連結リストハンドラ           */
	const void*	key,				/* 検索キーのポインタ           */
	BOOL		(*cmp)( const void* key, const void* item )
									/* 比較関数のポインタ           */
)
{
	LIST*		lst = (LIST*)hnd;
	NODE*		node;
	int			count;

	count = 0;
	for ( node = lst->header.next; node != &lst->header; node = node->next )
	{
		if ( cmp( key, node->item ) == TRUE )
		{
			return	count;
		}
		count++;
	}

	return	-1;
}

/*=============================================================================
*
* Purpose : 連結リストの要素をソートする．
*
* Return  : なし．
*
*============================================================================*/
void
LkListSort
(
	LKLIST		hnd,				/* 連結リストハンドラ           */
	int			(*cmp)( const void* item1, const void* item2 )
									/* 比較関数のポインタ           */
)
{
	LIST*		lst = (LIST*)hnd;
	NODE*		n1;
	NODE*		n2;
	void*		item;

	for ( n1 = lst->header.next; n1 != lst->header.prev; n1 = n1->next )
	{
		for ( n2 = n1->next; n2 != &lst->header; n2 = n2->next )
		{
			if ( cmp( n1->item, n2->item ) > 0 )
			{
				item     = n1->item;
				n1->item = n2->item;
				n2->item = item;
			}
		}
	}
}

/*=============================================================================
*
* Purpose : 連結リストから最大値の要素を検索する．
*
* Return  : 連結リストの最大値の要素を返す．
*           連結リストが空きの場合はＮＵＬＬポインタを返す．
*
*============================================================================*/
void*
LkListFindMax
(
	LKLIST		hnd,				/* 連結リストハンドラ           */
	int			(*cmp)( const void* item1, const void* item2 )
									/* 比較関数のポインタ           */
)
{
	LIST*		lst = (LIST*)hnd;
	NODE*		node;
	NODE*		max_node;

	if ( lst->count == 0 )
	{
		return	NULL;
	}

	max_node = lst->header.next;
	for ( node = max_node; node != &lst->header; node = node->next )
	{
		if ( cmp( node->item, max_node->item ) > 0 )
		{
			max_node = node;
		}
	}

	return	max_node->item;
}

/*=============================================================================
*
* Purpose : 連結リストから最小値の要素を検索する．
*
* Return  : 連結リストの最小値の要素を返す．
*           連結リストが空きの場合はＮＵＬＬポインタを返す．
*
*============================================================================*/
void*
LkListFindMin
(
	LKLIST		hnd,				/* 連結リストハンドラ           */
	int			(*cmp)( const void* item1, const void* item2 )
									/* 比較関数のポインタ           */
)
{
	LIST*		lst = (LIST*)hnd;
	NODE*		node;
	NODE*		min_node;

	if ( lst->count == 0 )
	{
		return	NULL;
	}

	min_node = lst->header.next;
	for ( node = min_node; node != &lst->header; node = node->next )
	{
		if ( cmp( node->item, min_node->item ) < 0 )
		{
			min_node = node;
		}
	}

	return	min_node->item;
}

/*=============================================================================
*
* Purpose : 連結リスト反復子の生成．
*
* Return  : 連結リスト反復子ハンドラを返す．
*
*============================================================================*/
LKITER
LkListIterNew
(
	LKITER		hnd					/* 連結リスト反復子ハンドラ     */
)
{
	LIST*		lst = (LIST*)hnd;
	ITER*		iter;

	iter = new_object( sizeof( ITER ) );

	iter->list  = lst;
	iter->node  = lst->header.next;

	return	(LKITER)iter;
}

/*=============================================================================
*
* Purpose : 連結リスト反復子の消滅．
*
* Return  : なし．
*
*============================================================================*/
void
LkListIterDelete
(
	LKITER		hnd					/* 連結リスト反復子ハンドラ     */
)
{
	ITER*		iter = (ITER*)hnd;

	del_object( iter );
}

/*=============================================================================
*
* Purpose : 連結リスト反復子の参照位置を先頭に設定．
*
* Return  : 連結リストの先頭の要素を返す．
*           連結リストが空きの場合はＮＵＬＬポインタを返す．
*
*============================================================================*/
void*
LkListIterGetHeadPosition
(
	LKITER		hnd					/* 連結リスト反復子ハンドラ     */
)
{
	ITER*		iter = (ITER*)hnd;
	LIST*		lst;

	lst = iter->list;
	if ( lst->count == 0 )
	{
		return	NULL;
	}
	iter->node = lst->header.next;

	return	iter->node->item;
}

/*=============================================================================
*
* Purpose : 連結リスト反復子の参照位置を最後尾に設定．
*
* Return  : 連結リストの最後尾の要素を返す．
*           連結リストが空きの場合はＮＵＬＬポインタを返す．
*
*============================================================================*/
void*
LkListIterGetTailPosition
(
	LKITER		hnd					/* 連結リスト反復子ハンドラ     */
)
{
	ITER*		iter = (ITER*)hnd;
	LIST*		lst;

	lst = iter->list;
	if ( lst->count == 0 )
	{
		return	NULL;
	}
	iter->node = lst->header.prev;

	return	iter->node->item;
}

/*=============================================================================
*
* Purpose : 連結リスト反復子の参照位置を取り出し，参照位置を次に移動．
*
* Return  : 参照位置の要素を返す．参照位置が最後尾を超えた場合や連結リストが
*           空きの場合はＮＵＬＬポインタを返す．
*
*============================================================================*/
void*
LkListIterGetNext
(
	LKITER		hnd					/* 連結リスト反復子ハンドラ     */
)
{
	ITER*		iter = (ITER*)hnd;
	LIST*		lst;
	NODE*		node;

	lst = iter->list;
	if ( ( node = iter->node ) == &lst->header )
	{
		return	NULL;
	}
	iter->node = node->next;

	return	node->item;
}

/*=============================================================================
*
* Purpose : 連結リスト反復子の参照位置を取り出し，参照位置を前に移動．
*
* Return  : 参照位置の要素を返す．参照位置が先頭を超えた場合や連結リストが
*           空きの場合はＮＵＬＬポインタを返す．
*
*============================================================================*/
void*
LkListIterGetPrev
(
	LKITER		hnd					/* 連結リスト反復子ハンドラ     */
)
{
	ITER*		iter = (ITER*)hnd;
	LIST*		lst;
	NODE*		node;

	lst = iter->list;
	if ( ( node = iter->node ) == &lst->header )
	{
		return	NULL;
	}
	iter->node = node->prev;

	return	node->item;
}

/*-----------------------------------------------------------------------------
*
* Purpose : ノードの生成．
*
* Return  : 生成したノードを返す．
*
*----------------------------------------------------------------------------*/
static NODE*
node_new
(
	void*		item				/* 格納する要素                 */
)
{
	NODE*		node;

	node = new_object( sizeof( NODE ) );

	node->next = NULL;
	node->prev = NULL;
	node->item = item;

	return	node;
}

/*-----------------------------------------------------------------------------
*
* Purpose : ノードの消滅．
*
* Return  : なし．
*
*----------------------------------------------------------------------------*/
static void
node_delete
(
	NODE*		node				/* 消滅させるノード             */
)
{
	del_object( node );
}

/*-----------------------------------------------------------------------------
*
* Purpose : ノードを取り除く．
*
* Return  : なし．
*
*----------------------------------------------------------------------------*/
static void
node_remove
(
	NODE*		node				/* 取り除くノード               */
)
{
	NODE*		prev;
	NODE*		next;

	prev = node->prev;
	next = node->next;

	prev->next = next;
	next->prev = prev;

	node_delete( node );
}

/*-----------------------------------------------------------------------------
*
* Purpose : 次のノードに挿入．
*
* Return  : なし．
*
*----------------------------------------------------------------------------*/
static void
node_insert_after
(
	NODE*		node,				/* 挿入位置のノード             */
	NODE*		insert				/* 挿入するノード               */
)
{
	NODE*		next;

	next       = node->next;
	next->prev = insert;
	node->next = insert;

	insert->next = next;
	insert->prev = node;
}

/********** End of File ******************************************************/
