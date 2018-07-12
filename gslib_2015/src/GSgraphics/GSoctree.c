/******************************************************************************
* File Name : GSoctree.c                         Ver : 1.00  Date : 2003-08-18
*
* Description :
*
*		オクツリー．
*
* Author : Moriharu Ohzu.
*
******************************************************************************/
#include	"GSoctree.h"
#include	"GSfrustum.h"
#include	"GScollision.h"
#include	"GSvector3.h"
#include	"GSfile_path.h"
#include	"heap.h"
#include	"gfile.h"
#include	<string.h>
#include	<float.h>

/****** 関数プロトタイプ宣言 *************************************************/

static void DrawOctreeNode( GSoctnode*, GSfrustum* );
static void DrawOctreeNodeEx(GSoctnode*, GSfrustum*);
static GSboolean SphereIsBoxInside( const GSvector3*, const GSvector3*, const GSvector3*, GSfloat );
static GSboolean LineIsInsideBox( const GSvector3*, const GSvector3*, const GSvector3*, const GSvector3* );
static GSboolean CollisionOctreeNodeAndSphere( GSoctnode*, GSvector3*, GSfloat );
static GSboolean CollisionOctreeNodeAndLine( GSoctnode*, const GSvector3*, GSvector3*, GSplane* );
static GSboolean CollisionOctreeNodeAndRay( GSoctnode*, const GSvector3*, const GSvector3*, GSvector3*, GSfloat*, GSplane* );
static GSboolean RayIsIntersectBox( const GSvector3*, const GSvector3*, const GSvector3*, const GSvector3* );
static GSboolean IsIntersect( const GSvector3*, const GSvector3*, const GSvector3*, const GSvector3*, GSfloat, GSfloat );

/*=============================================================================
*
* Purpose : オクツリーの生成．
*
* Return  : オクツリーを返す．
*
*============================================================================*/
extern GSoctree*
gsOctreeNew
(
	void
)
{
	GSoctree*		pOctree;

	pOctree = new_object( sizeof( GSoctree ) );

	memset( pOctree, 0, sizeof( GSoctree ) );

	return	pOctree;
}

/*=============================================================================
*
* Purpose : オクツリーの消滅．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsOctreeDelete
(
	GSoctree*		pOctree				/* オクツリーデータ		*/
)
{
	/* オクツリーをクリーンアップ */
	gsOctreeCleanUp( pOctree );

	/* オクツリーの削除 */
	del_object( pOctree );
}

/*=============================================================================
*
* Purpose : オクツリーのクリーンアップ．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsOctreeCleanUp
(
	GSoctree*		pOctree				/* オクツリーデータ		*/
)
{
	GSuint			i;

	/* オクツリーのノードを削除 */
	if ( pOctree->pNodes != NULL )
	{
		/* オクツリーのノードのディスプレイリストとメッシュを削除 */
		for ( i = 0; i < pOctree->numNodes; i++ )
		{
			if ( pOctree->pNodes[i].bLeafNode == GS_TRUE )
			{
				/* 面データグループを削除 */
				del_object( pOctree->pNodes[i].pMesh->pFaceGroups );

				/* メッシュデータを削除 */
				del_object( pOctree->pNodes[i].pMesh );

				/* ディスプレイリストを削除*/
				glDeleteLists( pOctree->pNodes[i].DisplayList, 1 );
			}
		}
		del_object( pOctree->pNodes );

		pOctree->pNodes = NULL;
	}
	pOctree->numNodes = 0;

	/* オクツリーのメッシュを削除 */
	if ( pOctree->pMesh != NULL )
	{
		gsMeshDelete( pOctree->pMesh );

		pOctree->pMesh = NULL;
	}
}

/*=============================================================================
*
* Purpose : オクツリーファイルの読み込み．
*
* Return  : なし．
*
*============================================================================*/
extern GSboolean
gsOctreeLoadFile
(
	GSoctree*		pOctree,			/* オクツリーデータ		*/
	const char*		pszFileName			/* ファイル名			*/
)
{
	GFILE			file;
	GSuint			OctreeInfo;
	GSuint			NodeID;
	GSuint			numFaceGroups;
	GSuint			FaceGroupIndex;
	GSmesh*			pMesh;
	GSoctnode*		pNode;
	GSmesh*			pNodeMesh;
	GSfacegroup*	pFaceGroup;
	char			MeshFileName[_MAX_PATH];
	char			FileName[_MAX_FNAME];
	char			Drive[_MAX_DRIVE];
	char			Dir[_MAX_DIR];
	GSuint			i, j;

	/* オクツリーファイルをオープンする */
	if ( ( file = GFileOpen( pszFileName, "r" ) ) == NULL )
	{
		/* ファイルがオープンできなかった */
		return	GS_FALSE;
	}

	/* オクツリーファイル情報の読み込み */
	GFileReadDword( file, &OctreeInfo );

	/* メッシュファイル名の読み込み */
	GFileReadString( file, FileName );

	/* フルパスのメッシュファイル名を作成する */
	gsSplitPath( GFileGetFileName( file ), Drive, Dir, NULL, NULL );
	strcpy( MeshFileName, Drive );
	strcat( MeshFileName, Dir );
	strcat( MeshFileName, FileName );

	/* メッシュデータの生成 */
	pMesh = gsMeshNew();

	/* メッシュデータの読み込み */
	if ( gsMeshLoadFile( pMesh, MeshFileName ) == GS_FALSE )
	{
		/* オクツリーファイルのクローズ */
		GFileClose( file );

		/* メッシュを消滅させる */
		gsMeshDelete( pMesh );

		return	GS_FALSE;
	}

	/* オクツリーをクリーンアップ */
	gsOctreeCleanUp( pOctree );

	/* 新しいメッシュを設定する */
	pOctree->pMesh = pMesh;

	/* ノード数の読み込み */
	GFileReadDword( file, &pOctree->numNodes );

	/* ノードの配列を生成する */
	pOctree->pNodes = new_object( sizeof( GSoctnode ) * pOctree->numNodes );

	/* ノードの配列を読み込む */
	for ( i = 0; i < pOctree->numNodes; i++ )
	{
		pNode = &pOctree->pNodes[i];

		/* ノードのデータを読み込む */
		GFileReadByte( file, &pNode->bLeafNode );
		GFileReadVector3( file, &pNode->PosMin );
		GFileReadVector3( file, &pNode->PosMax );
		GFileReadDword( file, &numFaceGroups );
		GFileReadDword( file, &FaceGroupIndex );
		for ( j = 0; j < 8; j++ )
		{
			GFileReadDword( file, &NodeID );
			if ( (GSint)NodeID != -1 )
			{
				pNode->pNodes[j] = &pOctree->pNodes[ NodeID ];
			}
			else
			{
				pNode->pNodes[j] = NULL;
			}
		}
		/* ディスプレイリストの初期化 */
		pNode->DisplayList = 0;

		/* リーフノードの場合はノード用のメッシュデータを作成する */
		pNode->pMesh = NULL;
		if ( pNode->bLeafNode == GS_TRUE )
		{
			/* メッシュデータを作成する */
			pNodeMesh = gsMeshNew();

			/* メッシュデータをコピーする */
			*pNodeMesh = *pMesh;

			/* 面データグループ数を設定 */
			pNodeMesh->numFaceGroups = numFaceGroups;

			/* 面データグループの取得 */
			pFaceGroup = &pMesh->pFaceGroups[ FaceGroupIndex ];

			/* 面データグループを作成 */
			pNodeMesh->pFaceGroups = new_object( sizeof(GSfacegroup) * numFaceGroups );

			pNodeMesh->numIndices = 0;
			for ( j = 0; j < pNodeMesh->numFaceGroups; j++ )
			{
				pNodeMesh->pFaceGroups[j] = pFaceGroup[j];
				pNodeMesh->pFaceGroups[j].StartIndices = pNodeMesh->numIndices;
				pNodeMesh->numIndices += pFaceGroup[j].numIndices;
			}

			/* 頂点インデックスを設定 */
			pNodeMesh->pIndices = &pMesh->pIndices[ pFaceGroup->StartIndices ];

			/* 平面データ配列を設定 */
			pNodeMesh->pPlanes = &pMesh->pPlanes[ pFaceGroup->StartIndices / 3 ];

			/* ノードにメッシュを設定する */
			pNode->pMesh = pNodeMesh;

			/* ディスプレイリストを作成する */
			pNode->DisplayList = glGenLists( 1 );
			glNewList( pNode->DisplayList, GL_COMPILE );
				gsMeshDraw( pNode->pMesh );
			glEndList();
		}
	}

	/* オクツリーファイルのクローズ */
	GFileClose( file );

	return	GS_TRUE;
}

/*=============================================================================
*
* Purpose : オクツリーファイルの読み込み．
*
* Return  : なし．
*
*============================================================================*/
extern GSboolean
gsOctreeLoadFileEx
(
	GSoctree*		pOctree,			/* オクツリーデータ		*/
	const char*		pszFileName			/* ファイル名			*/
)
{
	GFILE			file;
	GSuint			OctreeInfo;
	GSuint			NodeID;
	GSuint			numFaceGroups;
	GSuint			FaceGroupIndex;
	GSmesh*			pMesh;
	GSoctnode*		pNode;
	GSmesh*			pNodeMesh;
	GSfacegroup*	pFaceGroup;
	char			MeshFileName[_MAX_PATH];
	char			FileName[_MAX_FNAME];
	char			Drive[_MAX_DRIVE];
	char			Dir[_MAX_DIR];
	GSuint			i, j;

	/* オクツリーファイルをオープンする */
	if ((file = GFileOpen(pszFileName, "r")) == NULL)
	{
		/* ファイルがオープンできなかった */
		return	GS_FALSE;
	}

	/* オクツリーファイル情報の読み込み */
	GFileReadDword(file, &OctreeInfo);

	/* メッシュファイル名の読み込み */
	GFileReadString(file, FileName);

	/* フルパスのメッシュファイル名を作成する */
	gsSplitPath(GFileGetFileName(file), Drive, Dir, NULL, NULL);
	strcpy(MeshFileName, Drive);
	strcat(MeshFileName, Dir);
	strcat(MeshFileName, FileName);

	/* メッシュデータの生成 */
	pMesh = gsMeshNew();

	/* メッシュデータの読み込み */
	if (gsMeshLoadFileEx(pMesh, MeshFileName) == GS_FALSE)
	{
		/* オクツリーファイルのクローズ */
		GFileClose(file);

		/* メッシュを消滅させる */
		gsMeshDelete(pMesh);

		return	GS_FALSE;
	}

	/* オクツリーをクリーンアップ */
	gsOctreeCleanUp(pOctree);

	/* 新しいメッシュを設定する */
	pOctree->pMesh = pMesh;

	/* ノード数の読み込み */
	GFileReadDword(file, &pOctree->numNodes);

	/* ノードの配列を生成する */
	pOctree->pNodes = new_object(sizeof(GSoctnode) * pOctree->numNodes);

	/* ノードの配列を読み込む */
	for (i = 0; i < pOctree->numNodes; i++)
	{
		pNode = &pOctree->pNodes[i];

		/* ノードのデータを読み込む */
		GFileReadByte(file, &pNode->bLeafNode);
		GFileReadVector3(file, &pNode->PosMin);
		GFileReadVector3(file, &pNode->PosMax);
		GFileReadDword(file, &numFaceGroups);
		GFileReadDword(file, &FaceGroupIndex);
		for (j = 0; j < 8; j++)
		{
			GFileReadDword(file, &NodeID);
			if ((GSint)NodeID != -1)
			{
				pNode->pNodes[j] = &pOctree->pNodes[NodeID];
			}
			else
			{
				pNode->pNodes[j] = NULL;
			}
		}
		/* ディスプレイリストの初期化 */
		pNode->DisplayList = 0;

		/* リーフノードの場合はノード用のメッシュデータを作成する */
		pNode->pMesh = NULL;
		if (pNode->bLeafNode == GS_TRUE)
		{
			/* メッシュデータを作成する */
			pNodeMesh = gsMeshNew();

			/* メッシュデータをコピーする */
			*pNodeMesh = *pMesh;

			/* 面データグループ数を設定 */
			pNodeMesh->numFaceGroups = numFaceGroups;

			/* 面データグループの取得 */
			pFaceGroup = &pMesh->pFaceGroups[FaceGroupIndex];

			/* 面データグループを作成 */
			pNodeMesh->pFaceGroups = new_object(sizeof(GSfacegroup) * numFaceGroups);

			pNodeMesh->numIndices = 0;
			for (j = 0; j < pNodeMesh->numFaceGroups; j++)
			{
				pNodeMesh->pFaceGroups[j] = pFaceGroup[j];
				pNodeMesh->pFaceGroups[j].StartIndices = pNodeMesh->numIndices;
				pNodeMesh->numIndices += pFaceGroup[j].numIndices;
			}

			/* 頂点インデックスを設定 */
			pNodeMesh->pIndices = &pMesh->pIndices[pFaceGroup->StartIndices];

			/* 平面データ配列を設定 */
			pNodeMesh->pPlanes = &pMesh->pPlanes[pFaceGroup->StartIndices / 3];

			// インデックスバッファのオフセット
			pNodeMesh->Buffer.IndicesOffset = pFaceGroup->StartIndices;

			/* ノードにメッシュを設定する */
			pNode->pMesh = pNodeMesh;
		}
	}

	/* オクツリーファイルのクローズ */
	GFileClose(file);

	return	GS_TRUE;
}

/*=============================================================================
*
* Purpose : オクツリーの描画．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsOctreeDraw
(
	GSoctree*		pOctree				/* オクツリーデータ		*/
)
{
	GSfrustum		Frustum;
	GSmatrix4		matProj;
	GSmatrix4		matView;
	GScolor			CurrentColor;

	/* 各種レンダリングモードの退避 */
	glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_LIGHTING_BIT | GL_TEXTURE_BIT);

	/* 現在のカラーを取得する */
	glGetFloatv(GL_CURRENT_COLOR, (GLfloat*)&CurrentColor);

	/* 透視変換行列を取得 */
	glGetFloatv( GL_PROJECTION_MATRIX, (GLfloat*)&matProj );

	/* モデルビュー変換マトリクスを取得する */
	glGetFloatv( GL_MODELVIEW_MATRIX, (GLfloat*)&matView );

	/* 視錐台を作成する */
	gsFrustumFromMatrices( &Frustum, &matView, &matProj );

	/* ノードを描画する */
	DrawOctreeNode( &pOctree->pNodes[0], &Frustum );

	/* カレントカラーを復帰する */
	glColor4fv((GLfloat*)&CurrentColor);

	/* 各種レンダリングモードの復帰 */
	glPopAttrib();
}

/*=============================================================================
*
* Purpose : オクツリーの描画．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsOctreeDrawEx
(
	GSoctree*			pOctree,			/* オクツリーデータ		*/
	const GSmatrix4*	pProjectionMatrix,	/* 透視変換行列			*/
	const GSmatrix4*	pViewMatix			/* 視野変換行列			*/
)
{
	GSfrustum		Frustum;
	GScolor			CurrentColor;

	/* 各種レンダリングモードの退避 */
	glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_LIGHTING_BIT | GL_TEXTURE_BIT);

	/* 現在のカラーを取得する */
	glGetFloatv(GL_CURRENT_COLOR, (GLfloat*)&CurrentColor);

	/* 視錐台を作成する */
	gsFrustumFromMatrices(&Frustum, pViewMatix, pProjectionMatrix);

	/* ノードを描画する */
	DrawOctreeNodeEx(&pOctree->pNodes[0], &Frustum);

	/* カレントカラーを復帰する */
	glColor4fv((GLfloat*)&CurrentColor);

	/* 各種レンダリングモードの復帰 */
	glPopAttrib();
}

/*=============================================================================
*
* Purpose : オクツリーと線分の衝突判定．
*
* Return  : なし．
*
*============================================================================*/
extern GSboolean
gsOctreeCollisionLine
(
	GSoctree*			pOctree,		/* オクツリーデータ				*/
	const GSvector3*	pLine0,			/* 線分の始点					*/
	const GSvector3*	pLine1,			/* 線分の終点					*/
	GSvector3*			pIntersect,		/*-> メッシュと線分の交点		*/
	GSplane*			pPlane			/*-> 衝突した面の平面パラメータ	*/
)
{
	GSvector3			Line1;
	GSplane				Plane;

	Line1 = *pLine1;

	/* ノードと線分の衝突判定を行う */
	if ( CollisionOctreeNodeAndLine( &pOctree->pNodes[0], pLine0, &Line1, &Plane ) == GS_TRUE )
	{
		/* 交点を設定する */
		if ( pIntersect != NULL )
		{
			*pIntersect = Line1;
		}

		/* 平面パラメータを設定する */
		if ( pPlane != NULL )
		{
			*pPlane = Plane;
		}

		return	GS_TRUE;
	}

	return	GS_FALSE;
}

/*=============================================================================
*
* Purpose : オクツリーとレイの衝突判定．
*
* Return  : なし．
*
*============================================================================*/
extern GSboolean
gsOctreeCollisionRay
(
	GSoctree*			pOctree,		/* オクツリーデータ				*/
	const GSvector3*	pRayPos,		/* レイの始点					*/
	const GSvector3*	pRayDir,		/* レイの方向					*/
	GSvector3*			pIntersect,		/*-> メッシュとレイの交点		*/
	GSplane*			pPlane			/*-> 衝突した面の平面パラメータ	*/
)
{
	GSvector3			Intersect;
	GSplane				Plane;
	GSfloat				DistanceSq;

	/* 一番近い交点との距離を初期化 */
	DistanceSq = FLT_MAX;

	/* ノードと線分の衝突判定を行う */
	if ( CollisionOctreeNodeAndRay( &pOctree->pNodes[0],
									pRayPos, pRayDir,
									&Intersect, &DistanceSq, &Plane ) == GS_TRUE )
	{
		/* 交点を設定する */
		if ( pIntersect != NULL )
		{
			*pIntersect = Intersect;
		}

		/* 平面パラメータを設定する */
		if ( pPlane != NULL )
		{
			*pPlane = Plane;
		}

		return	GS_TRUE;
	}

	return	GS_FALSE;
}

/*=============================================================================
*
* Purpose : オクツリーと球体の衝突判定．
*
* Return  : なし．
*
*============================================================================*/
extern GSboolean
gsOctreeCollisionSphere
(
	GSoctree*			pOctree,		/* オクツリーデータ			*/
	const GSvector3*	pCenter,		/* 球体の中心				*/
	GSfloat				Radius,			/* 球体の半径				*/
	GSvector3*			pOutCenter		/*-> 補正後の球の中心座標	*/
)
{
	GSvector3			Center;

	Center = *pCenter;

	if ( CollisionOctreeNodeAndSphere( &pOctree->pNodes[0], &Center, Radius ) == GS_TRUE )
	{
		/* 補正後の球体の中心座標を設定する */
		if ( pOutCenter != NULL )
		{
			*pOutCenter = Center;
		}

		return	GS_TRUE;
	}

	return	GS_FALSE;
}

/*-----------------------------------------------------------------------------
*
* Purpose : ノードのメッシュを描画．
*
* Return  : なし．
*
*----------------------------------------------------------------------------*/
static void
DrawOctreeNode
(
	GSoctnode*		pNode,				/* オクツリーノード		*/
	GSfrustum*		pFrustum			/* 視錐台パラメータ		*/
)
{
	GSint				i;

	/* ノードが視錐台の中に入っているか？ */
	if ( gsFrustumIsBoxInside( pFrustum, &pNode->PosMin, &pNode->PosMax ) == GS_FALSE )
	{
		/* 視錐台の中に入っていなければ描画しない */
		return;
	}

	/* リーフノードか？ */
	if ( pNode->bLeafNode == GS_TRUE )
	{
		/* リーフノードの場合はメッシュを描画する */
		glCallList( pNode->DisplayList );
	
		return;
	}

	/* 子ノードを全て描画する */
	for ( i = 0; i < 8; i++ )
	{
		if ( pNode->pNodes[i] != NULL )
		{
			DrawOctreeNode( pNode->pNodes[i], pFrustum );
		}
	}
}

/*-----------------------------------------------------------------------------
*
* Purpose : ノードのメッシュを描画．
*
* Return  : なし．
*
*----------------------------------------------------------------------------*/
static void
DrawOctreeNodeEx
(
	GSoctnode*		pNode,				/* オクツリーノード		*/
	GSfrustum*		pFrustum			/* 視錐台パラメータ		*/
)
{
	GSint				i;

	/* ノードが視錐台の中に入っているか？ */
	if (gsFrustumIsBoxInside(pFrustum, &pNode->PosMin, &pNode->PosMax) == GS_FALSE)
	{
		/* 視錐台の中に入っていなければ描画しない */
		return;
	}

	/* リーフノードか？ */
	if (pNode->bLeafNode == GS_TRUE)
	{
		/* リーフノードの場合はメッシュを描画する */
		gsMeshDrawEx(pNode->pMesh);
		return;
	}

	/* 子ノードを全て描画する */
	for (i = 0; i < 8; i++)
	{
		if (pNode->pNodes[i] != NULL)
		{
			DrawOctreeNodeEx(pNode->pNodes[i], pFrustum);
		}
	}
}

/*-----------------------------------------------------------------------------
*
* Purpose : ノードと線分の衝突判定．
*
* Return  : なし．
*
*----------------------------------------------------------------------------*/
static GSboolean
CollisionOctreeNodeAndLine
(
	GSoctnode*			pNode,			/* オクツリーノード				*/
	const GSvector3*	pLine0,			/* 線分の始点					*/
	GSvector3*			pLine1,			/* 線分の終点					*/
	GSplane*			pPlane			/*-> 衝突した面の平面パラメータ	*/
)
{
	GSboolean			bCollision;
	int					i;

	/* ノードの中に線分が存在するかチェックする */
	if ( LineIsInsideBox( &pNode->PosMin, &pNode->PosMax, pLine0, pLine1 ) == GS_FALSE )
	{
		return	GS_FALSE;
	}

	/* リーフノードの場合はメッシュとの衝突判定を行う */
	if ( pNode->bLeafNode == GS_TRUE )
	{
		return	gsCollisionMeshAndLine( pNode->pMesh, pLine0, pLine1, pLine1, pPlane );
	}

	/* 全ての子ノードとの衝突判定を行う */
	bCollision = GS_FALSE;
	for ( i = 0; i < 8; i++ )
	{
		if ( pNode->pNodes[i] != NULL )
		{
			if ( CollisionOctreeNodeAndLine( pNode->pNodes[i], pLine0, pLine1, pPlane ) == GS_TRUE )
			{
				/* 衝突フラグの設定 */
				bCollision = GS_TRUE;
			}
		}
	}

	return	bCollision;
}

/*-----------------------------------------------------------------------------
*
* Purpose : ノードとレイの衝突判定．
*
* Return  : なし．
*
*----------------------------------------------------------------------------*/
static GSboolean
CollisionOctreeNodeAndRay
(
	GSoctnode*			pNode,			/* オクツリーノード				*/
	const GSvector3*	pRayPos,		/* レイの位置					*/
	const GSvector3*	pRayDir,		/* レイの方向					*/
	GSvector3*			pIntersect,		/*-> 交点の座標					*/
	GSfloat*			pDistanceSq,	/*-> 交点との距離				*/
	GSplane*			pPlane			/*-> 衝突した面の平面パラメータ	*/
)
{
	GSvector3			Intersect;
	GSplane				Plane;
	GSboolean			bCollision;
	GSfloat				DistanceSq;
	int					i;

	/* ノードの中に線分が存在するかチェックする */
	if ( RayIsIntersectBox( &pNode->PosMin, &pNode->PosMax, pRayPos, pRayDir ) == GS_FALSE )
	{
		return	GS_FALSE;
	}

	/* リーフノードの場合はメッシュとの衝突判定を行う */
	if ( pNode->bLeafNode == GS_TRUE )
	{
		if ( gsCollisionMeshAndRay( pNode->pMesh, pRayPos, pRayDir, &Intersect, &Plane ) == GS_TRUE )
		{
			/* 線分の始点と交点の距離を求める */
			DistanceSq = gsVector3DistanceSq( pRayPos, &Intersect );

			/* 線分の始点に最も近い交点を新しい交点とする */
			if ( DistanceSq < *pDistanceSq )
			{
				*pDistanceSq = DistanceSq;
				*pIntersect  = Intersect;
				*pPlane      = Plane;
			}

			return	GS_TRUE;
		}

		return	GS_FALSE;
	}

	/* 全ての子ノードとの衝突判定を行う */
	bCollision = GS_FALSE;
	for ( i = 0; i < 8; i++ )
	{
		if ( pNode->pNodes[i] != NULL )
		{
			if ( CollisionOctreeNodeAndRay( pNode->pNodes[i],
											pRayPos, pRayDir,
											pIntersect, pDistanceSq, pPlane ) == GS_TRUE )
			{
				/* 衝突フラグの設定 */
				bCollision = GS_TRUE;
			}
		}
	}

	return	bCollision;
}

/*-----------------------------------------------------------------------------
*
* Purpose : ノードと球体の衝突判定．
*
* Return  : なし．
*
*----------------------------------------------------------------------------*/
static GSboolean
CollisionOctreeNodeAndSphere
(
	GSoctnode*			pNode,			/* オクツリーノード			*/
	GSvector3*			pCenter,		/* 球体の中心				*/
	GSfloat				Radius			/* 球体の半径				*/
)
{
	GSboolean			bCollision;
	int					i;

	/* ノードの中に球体が存在するかチェックする */
	if ( SphereIsBoxInside( &pNode->PosMin, &pNode->PosMax, pCenter, Radius ) == GS_FALSE )
	{
		return	GS_FALSE;
	}

	/* リーフノードの場合はメッシュとの衝突判定を行う */
	if ( pNode->bLeafNode == GS_TRUE )
	{
		return	gsCollisionMeshAndSphere( pNode->pMesh, pCenter, Radius, pCenter );
	}

	/* 全ての子ノードとの衝突判定を行う */
	bCollision = GS_FALSE;
	for ( i = 0; i < 8; i++ )
	{
		if ( pNode->pNodes[i] != NULL )
		{
			if ( CollisionOctreeNodeAndSphere( pNode->pNodes[i], pCenter, Radius ) == GS_TRUE )
			{
				bCollision = GS_TRUE;
			}
		}
	}

	return	bCollision;
}

/*-----------------------------------------------------------------------------
*
* Purpose : 線分がボックス内に存在するかチェックする．
*
* Return  : なし．
*
*----------------------------------------------------------------------------*/
static GSboolean
LineIsInsideBox
(
	const GSvector3*	pMin,			/* ボックスの最小座標	*/
	const GSvector3*	pMax,			/* ボックスの最大座標	*/
	const GSvector3*	pLine0,			/* 線分の始点			*/
	const GSvector3*	pLine1			/* 線分の終点			*/
)
{
	GSvector3			Dir;

	/* 完全にボックスよりも外にあるか？ */
	if ( ( pLine0->x < pMin->x && pLine1->x < pMin->x )
	  || ( pLine0->y < pMin->y && pLine1->y < pMin->y )
	  || ( pLine0->z < pMin->z && pLine1->z < pMin->z )
	  || ( pLine0->x > pMax->x && pLine1->x > pMax->x )
	  || ( pLine0->y > pMax->y && pLine1->y > pMax->y )
	  || ( pLine0->z > pMax->z && pLine1->z > pMax->z ) )
	{
		return	GS_FALSE;
	}

	/* 始点がボックス内に存在するか？ */
	if ( ( pLine0->x >= pMin->x && pLine0->x <= pMax->x )
	  && ( pLine0->y >= pMin->y && pLine0->y <= pMax->y )
	  && ( pLine0->z >= pMin->z && pLine0->z <= pMax->z ) )
	{
		return	GS_TRUE;
	}

	/* 終点がボックス内に存在するか？ */
	if ( ( pLine1->x >= pMin->x && pLine1->x <= pMax->x )
	  && ( pLine1->y >= pMin->y && pLine1->y <= pMax->y )
	  && ( pLine1->z >= pMin->z && pLine1->z <= pMax->z ) )
	{
		return	GS_TRUE;
	}

	/* 線分がボックスと交差するか？ */
	gsVector3Subtract( &Dir, pLine1, pLine0 ); 
	gsVector3Normalize( &Dir, &Dir );

	return	RayIsIntersectBox( pMin,  pMax, pLine0, &Dir );
}

/*-----------------------------------------------------------------------------
*
* Purpose : 球体がボックス内に存在するかチェックする．
*
* Return  : なし．
*
*----------------------------------------------------------------------------*/
static GSboolean
SphereIsBoxInside
(
	const GSvector3*	pMin,			/* ボックスの最小座標		*/
	const GSvector3*	pMax,			/* ボックスの最大座標		*/
	const GSvector3*	pCenter,		/* 球体の中心座標			*/
	GSfloat				Radius			/* 球体の半径				*/
)
{
	GSfloat				DistanceSq;

	DistanceSq = 0.0f;

	if( pCenter->x > pMax->x )
	{
		DistanceSq += SQR( pCenter->x - pMax->x );
	}
	else if( pCenter->x < pMin->x )
	{
		DistanceSq += SQR( pMin->x - pCenter->x );
	}

	if( pCenter->y > pMax->y )
	{
		DistanceSq += SQR( pCenter->y - pMax->y );
	}
	else if( pCenter->y < pMin->y )
	{
		DistanceSq += SQR( pMin->y - pCenter->y );
	}

	if( pCenter->z > pMax->z )
	{
		DistanceSq += SQR( pCenter->z - pMax->z );
	}
	else if( pCenter->z < pMin->z )
	{
		DistanceSq += SQR( pMin->z - pCenter->z );
	}

	if ( DistanceSq <= SQR( Radius ) )
	{
		return	GS_TRUE;
	}

	return	GS_FALSE;
}

/*-----------------------------------------------------------------------------
*
* Purpose : レイとボックスが交差するかチェックする．
*
* Return  : なし．
*
*----------------------------------------------------------------------------*/
static GSboolean
RayIsIntersectBox
(
	const GSvector3*	pMin,			/* ボックスの最小座標		*/
	const GSvector3*	pMax,			/* ボックスの最大座標		*/
	const GSvector3*	pPos,			/* レイの位置				*/
	const GSvector3*	pDir			/* レイの方向				*/
)
{
	/* ボックスの何れかの面がレイが交差するか調べる */
	if ( IsIntersect( pMin, pMax, pPos, pDir, -pPos->y + pMax->y,  pDir->y ) == GS_TRUE
	  || IsIntersect( pMin, pMax, pPos, pDir,  pPos->y - pMin->y, -pDir->y ) == GS_TRUE
	  || IsIntersect( pMin, pMax, pPos, pDir, -pPos->x + pMax->x,  pDir->x ) == GS_TRUE
	  || IsIntersect( pMin, pMax, pPos, pDir,  pPos->x - pMin->x, -pDir->x ) == GS_TRUE
	  || IsIntersect( pMin, pMax, pPos, pDir, -pPos->z + pMax->z,  pDir->z ) == GS_TRUE
	  || IsIntersect( pMin, pMax, pPos, pDir,  pPos->z - pMin->z, -pDir->z ) == GS_TRUE )
	{
		return	GS_TRUE;
	}

	return	GS_FALSE;
}

/*-----------------------------------------------------------------------------
*
* Purpose : レイとボックスの何れかの面が交差するかチェックする．
*
* Return  : なし．
*
*----------------------------------------------------------------------------*/
static GSboolean
IsIntersect
(
	const GSvector3*	pMin,			/* ボックスの最小座標		*/
	const GSvector3*	pMax,			/* ボックスの最大座標		*/
	const GSvector3*	pPos,			/* レイの位置				*/
	const GSvector3*	pDir,			/* レイの方向				*/
	GSfloat				Numerator,		/* 分子						*/
	GSfloat				Denominator		/* 分母						*/
)
{
#define	MACH_FACTOR		0.0001f
	GSvector3			V;

	/* 線分と面が平行か？ */
	if ( Denominator == 0.0f )
	{
		return	GS_FALSE;
	}

	/* 交点を求める */
	gsVector3LinearEquation( &V, pPos, pDir, Numerator / Denominator );

	/* 交点がボックス内に存在するか？ */
	if ( ( V.x >= ( pMin->x - MACH_FACTOR ) && V.x <= ( pMax->x + MACH_FACTOR ) )
	  && ( V.y >= ( pMin->y - MACH_FACTOR ) && V.y <= ( pMax->y + MACH_FACTOR ) )
 	  && ( V.z >= ( pMin->z - MACH_FACTOR ) && V.z <= ( pMax->z + MACH_FACTOR ) ) )
	{
		return	GS_TRUE;
	}

	return	GS_FALSE;
}

/********** End of File ******************************************************/
