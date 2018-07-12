/******************************************************************************
* File Name : GScollision.c                      Ver : 1.00  Date : 2002-03-24
*
* Description :
*
*       衝突判定の計算．
*
* Author : Moriharu Ohzu.
*
******************************************************************************/
#include	"GScollision.h"
#include	"GSmatrix4.h"
#include	"GSvector3.h"
#include	"GSplane.h"
#include	<float.h>

/****** 関数プロトタイプ宣言 *************************************************/

static GSboolean InsidePolygon( const GSvector3*, GSuint, const GSvector3* );
static GSboolean CollisionPolyEdgeAndSphere( const GSvector3*, GSuint, const GSvector3*, GSfloat, GSvector3* );
static void FindNearestPointOnLineSegment( const GSvector3*, const GSvector3*, const GSvector3*, GSboolean, GSvector3*, GSfloat* );
static void FindNearestPointOfParallelLineSegments( const GSvector3*, const GSvector3*, const GSvector3*, const GSvector3*, const GSvector3*, const GSvector3*, GSboolean, GSvector3*, GSvector3* );
static void AdjustNearestPoints( const GSvector3*, const GSvector3*, const GSvector3*, const GSvector3*, GSfloat, GSfloat, GSvector3*, GSvector3* );

/****** マクロ宣言 ***********************************************************/

/* 0.0f～1.0f内に収まっているかチェックする */
#define OUT_OF_RANGE(a) ( ( (a) < 0.0f  || (a) > 1.0f ) ? GS_TRUE: GS_FALSE )

/*=============================================================================
*
* Purpose : 球体と球体の衝突判定．
*
* Return  : 衝突していれば真，衝突していなければ偽を返す．
*
*============================================================================*/
extern GSboolean
gsCollisionSphereAndSphere
(
	const GSvector3*	pCenter1,		/* 球体１の中心					*/
	GSfloat				Radius1,		/* 球体１の半径					*/
	const GSvector3*	pCenter2,		/* 球体２の中心					*/
	GSfloat				Radius2			/* 球体２の半径					*/
)
{
	if ( gsVector3DistanceSq( pCenter1, pCenter2 ) <= SQR( Radius1 + Radius2 ) )
	{
		return	GS_TRUE;
	}

	return	GS_FALSE;
}

/*=============================================================================
*
* Purpose : ポリゴンと線分の衝突判定．
*
* Return  : ポリゴンと線分が交差していれば真，交差していなければ偽を返す．
*
*============================================================================*/
extern GSboolean
gsCollisionPolyAndLine
(
	const GSvector3*	pPoly,			/* ポリゴンの頂点配列				*/
	GSuint				numVertices,	/* 頂点データ数						*/
	const GSplane*		pPlane,			/* ポリゴンの平面方程式パラメータ	*/
	const GSvector3*	pLine0,			/* 線分の始点						*/
	const GSvector3*	pLine1,			/* 線分の終点						*/
	GSvector3*			pIntersectPoint	/*-> ポリゴンとの線分交点			*/
)
{
	GSvector3			Intersect;

	/* 平面と線分の交点を求める */
	if ( gsPlaneIntersectLine( &Intersect, pPlane, pLine0, pLine1 ) == GS_FALSE )
	{
		return	GS_FALSE;
	}

	/* 交点がポリゴン内に存在するか？ */
	if ( InsidePolygon( pPoly, numVertices, &Intersect ) == GS_TRUE )
	{
		/* ポリゴンとの線分交点を設定 */
		if ( pIntersectPoint != NULL )
		{
			*pIntersectPoint = Intersect;
		}

		return	GS_TRUE;
	}

	return	GS_FALSE;
}

/*=============================================================================
*
* Purpose : ポリゴンとレイの衝突判定．
*
* Return  : ポリゴンとレイが交差していれば真，交差していなければ偽を返す．
*
*============================================================================*/
extern GSboolean
gsCollisionPolyAndRay
(
	const GSvector3*	pPoly,			/* ポリゴンの頂点配列				*/
	GSuint				numVertices,	/* 頂点データ数						*/
	const GSplane*		pPlane,			/* ポリゴンの平面方程式パラメータ	*/
	const GSvector3*	pRayPos,		/* レイの始点						*/
	const GSvector3*	pRayDir,		/* レイの方向						*/
	GSvector3*			pIntersectPoint	/*-> ポリゴンとの線分交点			*/
)
{
	GSvector3			Intersect;

	/* 平面と線分の交点を求める */
	if ( gsPlaneIntersectRay( &Intersect, pPlane, pRayPos, pRayDir ) == GS_FALSE )
	{
		return	GS_FALSE;
	}

	/* 交点がポリゴン内に存在するか？ */
	if ( InsidePolygon( pPoly, numVertices, &Intersect ) == GS_TRUE )
	{
		/* ポリゴンとの線分交点を設定 */
		if ( pIntersectPoint != NULL )
		{
			*pIntersectPoint = Intersect;
		}

		return	GS_TRUE;
	}

	return	GS_FALSE;
}

/*=============================================================================
*
* Purpose : メッシュと球体の衝突判定．
*
* Return  : メッシュと球体が衝突していれば真，衝突していなければ偽を返す．
*
*============================================================================*/
extern GSboolean
gsCollisionMeshAndSphere
(
	const GSmesh*		pMesh,			/* メッシュ					*/
	const GSvector3*	pCenter,		/* 球体の中心				*/
	GSfloat				Radius,			/* 球体の半径				*/
	GSvector3*			pOutCenter		/* 補正後の球の中心座標		*/
)
{
	GSvector3			Poly[3];
	GSplane*			pPlane;
	GSvector3			Center;
	GSvector3			Position;
	GSvector3			Offset;
	GSboolean			bCollision;
	GSboolean			bIntersects;
	GSfloat				Distance;
	GSfloat				DistanceOver;
	GSuint				i;

	Center = *pCenter;

	/* 衝突フラグの初期化 */
	bCollision  = GS_FALSE;

	/* 平面交差フラグの初期化 */
	bIntersects = GS_FALSE;

	/* 全ポリゴン平面と球体の衝突判定を行う */
	for ( i = 0; i < ( pMesh->numIndices / 3 ); i++ )
	{
		pPlane = &pMesh->pPlanes[i];

		/* 球体がポリゴン平面と交差するか調べる */
		if ( gsPlaneClassifySphere( pPlane, &Center, Radius, &Distance ) != GS_INTERSECTS_PLANE )
		{
			continue;
		}
		bIntersects = GS_TRUE;

		/* ポリゴン平面と球体の中心までの相対座標を求める */
		Offset.x = pPlane->a * Distance;
		Offset.y = pPlane->b * Distance;
		Offset.z = pPlane->c * Distance;

		/* ポリゴン平面上に球体の中心位置を補正する */
		gsVector3Subtract( &Position, &Center, &Offset );

		/* ポリゴンの３頂点を取得する */
		Poly[0] = pMesh->pPoints[ pMesh->pIndices[i*3+0] ];
		Poly[1] = pMesh->pPoints[ pMesh->pIndices[i*3+1] ];
		Poly[2] = pMesh->pPoints[ pMesh->pIndices[i*3+2] ];

		/* ポリゴン平面上に球体の中心座標が存在するか？ */
		if ( InsidePolygon( Poly, 3, &Position ) == GS_TRUE )
		{
			bCollision = GS_TRUE;

			if( Distance > 0.0f )
			{
				/* 重なっている部分の補正値を求める */
				DistanceOver = Radius - Distance;

				Center.x += pPlane->a * DistanceOver;
				Center.y += pPlane->b * DistanceOver;
				Center.z += pPlane->c * DistanceOver;
			}
		}
	}

	/* 全ての平面と交差しなければ終了 */
	if ( bIntersects == GS_FALSE )
	{
		return	bCollision;
	}

	/* 全ポリゴンエッジと球体の衝突判定を行う */
	for ( i = 0; i < ( pMesh->numIndices / 3 ); i++ )
	{
		pPlane = &pMesh->pPlanes[i];

		/* 球体がポリゴン平面と交差するか調べる */
		if ( gsPlaneClassifySphere( pPlane, &Center, Radius, NULL ) != GS_INTERSECTS_PLANE )
		{
			continue;
		}

		/* ポリゴンの３頂点を取得する */
		Poly[0] = pMesh->pPoints[ pMesh->pIndices[i*3+0] ];
		Poly[1] = pMesh->pPoints[ pMesh->pIndices[i*3+1] ];
		Poly[2] = pMesh->pPoints[ pMesh->pIndices[i*3+2] ];

		/* ポリゴンのエッジと球体の交差判定を行う */
		if ( CollisionPolyEdgeAndSphere( Poly, 3, &Center, Radius, &Offset ) == TRUE )
		{
			bCollision = GS_TRUE;

			/* 球体の中心位置を補正する */
			gsVector3Add( &Center, &Center, &Offset );
		}
	}

	/* 補正後の球体の中心座標を設定する */
	if ( pOutCenter != NULL )
	{
		*pOutCenter = Center;
	}

	return	bCollision;
}

/*=============================================================================
*
* Purpose : メッシュとレイの衝突判定．
*
* Return  : メッシュとレイが交差していれば真，交差していなければ偽を返す．
*
*============================================================================*/
extern GSboolean
gsCollisionMeshAndRay
(
	const GSmesh*		pMesh,			/* メッシュ						*/
	const GSvector3*	pRayPos,		/* レイの始点					*/
	const GSvector3*	pRayDir,		/* レイの方向					*/
	GSvector3*			pIntersect,		/*-> メッシュと線分の交点		*/
	GSplane*			pPlane			/*-> 衝突した面の平面パラメータ	*/
)
{
	GSvector3			Poly[3];
	GSplane*			pCollisionPlane;
	GSvector3			Intersect;
	GSfloat				DistanceSq;
	GSvector3			minIntersect = { 0, 0, 0 };
	GSfloat				minDistanceSq;
	GSuint				i;

	/* 衝突面の初期化 */
	pCollisionPlane = NULL;

	/* 一番近い交点との距離を初期化 */
	minDistanceSq = FLT_MAX;

	/* 全ポリゴンとレイの衝突判定を行う */
	for ( i = 0; i < ( pMesh->numIndices / 3 ); i++ )
	{
#if 0
		/* 裏面のポリゴンとは衝突判定をしない */
		if ( gsPlaneClassifyPoint( &pMesh->pPlanes[i], pRayPos, NULL ) == GS_BEHIND_PLANE )
		{
			continue;
		}
#endif
		/* ポリゴンの３頂点を取得する */
		Poly[0] = pMesh->pPoints[ pMesh->pIndices[i*3+0] ];
		Poly[1] = pMesh->pPoints[ pMesh->pIndices[i*3+1] ];
		Poly[2] = pMesh->pPoints[ pMesh->pIndices[i*3+2] ];

		/* ポリゴンとレイの衝突判定を行う */
		if ( gsCollisionPolyAndRay( Poly, 3, &pMesh->pPlanes[i], pRayPos, pRayDir, &Intersect ) == GS_TRUE )
		{
			/* レイの始点と交点の距離を求める */
			DistanceSq = gsVector3DistanceSq( pRayPos, &Intersect );

			/* レイの始点に最も近い交点を新しい交点とする */
			if ( DistanceSq < minDistanceSq )
			{
				minDistanceSq = DistanceSq;
				minIntersect  = Intersect;

				/* 衝突面のパラメータを保存 */
				pCollisionPlane = &pMesh->pPlanes[i];
			}
		}
	}

	/* 衝突した面が存在するか？ */
	if ( pCollisionPlane != NULL )
	{
		/* 衝突した面との交点を設定する */
		if ( pIntersect != NULL )
		{
			*pIntersect = minIntersect;
		}

		/* 衝突した平面のパラメータを設定する */
		if ( pPlane != NULL )
		{
			*pPlane = *pCollisionPlane;
		}

		return	GS_TRUE;
	}

	return	GS_FALSE;
}

/*=============================================================================
*
* Purpose : メッシュと線分の衝突判定．
*
* Return  : メッシュと線分が交差していれば真，交差していなければ偽を返す．
*
*============================================================================*/
extern GSboolean
gsCollisionMeshAndLine
(
	const GSmesh*		pMesh,			/* メッシュ						*/
	const GSvector3*	pLine0,			/* 線分の始点					*/
	const GSvector3*	pLine1,			/* 線分の終点					*/
	GSvector3*			pIntersect,		/*-> メッシュと線分の交点		*/
	GSplane*			pPlane			/*-> 衝突した面の平面パラメータ	*/
)
{
	GSvector3			Poly[3];
	GSplane*			pCollisionPlane;
	GSvector3			Intersect;
	GSuint				i;

	/* 終点を交点に設定 */
	Intersect = *pLine1;

	/* 衝突面の初期化 */
	pCollisionPlane = NULL;

	/* 全ポリゴンとレイの衝突判定を行う */
	for ( i = 0; i < ( pMesh->numIndices / 3 ); i++ )
	{
		/* 裏面のポリゴンとは衝突判定をしない */
		if ( gsPlaneClassifyPoint( &pMesh->pPlanes[i], pLine0, NULL ) == GS_BEHIND_PLANE )
		{
			continue;
		}

		/* ポリゴンの３頂点を取得する */
		Poly[0] = pMesh->pPoints[ pMesh->pIndices[i*3+0] ];
		Poly[1] = pMesh->pPoints[ pMesh->pIndices[i*3+1] ];
		Poly[2] = pMesh->pPoints[ pMesh->pIndices[i*3+2] ];

		/* ポリゴンと線分の衝突判定を行う */
		if ( gsCollisionPolyAndLine( Poly, 3, &pMesh->pPlanes[i], pLine0, &Intersect, &Intersect ) == GS_TRUE )
		{
			/* 衝突面のパラメータを保存 */
			pCollisionPlane = &pMesh->pPlanes[i];
		}
	}

	/* 衝突した面が存在するか？ */
	if ( pCollisionPlane != NULL )
	{
		/* 衝突した面との交点を設定する */
		if ( pIntersect != NULL )
		{
			*pIntersect = Intersect;
		}

		/* 衝突した平面のパラメータを設定する */
		if ( pPlane != NULL )
		{
			*pPlane = *pCollisionPlane;
		}

		return	GS_TRUE;
	}

	return	GS_FALSE;
}

/*=============================================================================
*
* Purpose : ポリゴンと球体の衝突判定．
*
* Return  : ポリゴンと球体が衝突していれば真，衝突していなければ偽を返す．
*
*============================================================================*/
extern GSboolean
gsCollisionPolyAndSphere
(
	const GSvector3*	pPoly,			/* ポリゴンの頂点配列				*/
	GSuint				numVertices,	/* 頂点データ数						*/
	const GSplane*		pPlane,			/* ポリゴンの平面方程式パラメータ	*/
	const GSvector3*	pCenter,		/* 球体の中心						*/
	GSfloat				Radius,			/* 球体の半径						*/
	GSvector3*			pOffset			/*-> 球体が衝突しない座標への補正値	*/
)
{
	GSfloat				Distance;
	GSfloat				DistanceOver;
	GSvector3			Offset;
	GSvector3			Position;
	GSenum				Classification;

	/* 球体がポリゴン平面と交差するか調べる */
	Classification = gsPlaneClassifySphere( pPlane, pCenter, Radius, &Distance );

	if ( Classification != GS_INTERSECTS_PLANE )
	{
		/* 交差していない場合は衝突しない */
		return	GS_FALSE;
	}

	/* ポリゴン平面と球体の中心までの相対座標を求める */
	Offset.x = pPlane->a * Distance;
	Offset.y = pPlane->b * Distance;
	Offset.z = pPlane->c * Distance;

	/* ポリゴン平面上に球体の中心位置を補正する */
	gsVector3Subtract( &Position, pCenter, &Offset );

	/* ポリゴン平面上に球体の中心座標が存在するか？ */
	if ( InsidePolygon( pPoly, numVertices, &Position ) == GS_TRUE )
	{
		/* 重なっている部分の補正値を求める */
		if ( pOffset != NULL )
		{
			DistanceOver = Radius - Distance;

			if( Distance < 0 )
			{
				DistanceOver = -DistanceOver;
			}
			pOffset->x = pPlane->a * DistanceOver;
			pOffset->y = pPlane->b * DistanceOver;
			pOffset->z = pPlane->c * DistanceOver;
		}

		return	GS_TRUE;
	}

	/* ポリゴンのエッジ部分が球体と交差しているか？ */
	return	CollisionPolyEdgeAndSphere( pPoly, numVertices, pCenter, Radius, pOffset );
}

/*=============================================================================
*
* Purpose : 球体とレイの衝突判定．
*
* Return  : レイと球体が衝突していれば真，衝突していなければ偽を返す．
*
*============================================================================*/
extern GSboolean
gsCollisionSphereAndRay
(
	const GSvector3*	pCenter,		/* 球体の中心座標			*/
	GSfloat				Radius,			/* 球体の半径				*/
	const GSvector3*	pRayPos,		/* レイの位置				*/
	const GSvector3*	pRayDir,		/* レイの方向				*/
	GSvector3*			pIntersect		/*-> レイとの交点			*/
)
{
	GSvector3			Q;
	GSfloat				a , b, c, d;
	GSfloat				t;

	/* 球体とレイの交差判定を行う */
	gsVector3Subtract( &Q, pRayPos, pCenter );
	a = gsVector3LengthSq( pRayDir );
	b = gsVector3Dot( &Q, pRayDir );
	c = gsVector3LengthSq( &Q ) - SQR( Radius );
	if ( ( d = SQR( b ) - a * c ) < 0.0f )
	{
		return	GS_FALSE;
	}

	if ( pIntersect != NULL )
	{
		/* 球体とレイの交点を求める */
		t = ( -b - gsSqrt( d ) ) / a;
		gsVector3LinearEquation( pIntersect, pRayPos, pRayDir, t );
	}

	return	GS_TRUE;
}

/*=============================================================================
*
* Purpose : 球体と線分の衝突判定．
*
* Return  : 球体と球体が衝突していれば真，衝突していなければ偽を返す．
*
*============================================================================*/
extern GSboolean
gsCollisionSphereAndLine
(
	const GSvector3*	pCenter,		/* 球体の中心座標			*/
	GSfloat				Radius,			/* 球体の半径				*/
	const GSvector3*	pLine0,			/* 線分の始点				*/
	const GSvector3*	pLine1,			/* 線分の終点				*/
	GSvector3*			pIntersect		/*-> 線分との交点			*/
)
{
	GSvector3			Point;
	GSvector3			LineVec;
	GSfloat				DistanceSq;

	/* 線分のベクトルを求める */
	gsVector3Subtract( &LineVec, pLine1, pLine0 );

	/* 線分上の最も球体の中心座標を近い座標を求める */
	FindNearestPointOnLineSegment( pLine0, &LineVec, pCenter, GS_FALSE, &Point, NULL );

	/* 線分上の座標と球体の中心座標の距離を求める */
	DistanceSq = gsVector3DistanceSq( &Point, pCenter );

	/* 求めた距離が球体の半径よりも長い場合は衝突しない */
	if( DistanceSq > SQR( Radius ) )
	{
		return	GS_FALSE;
	}

	/* レイのベクトルを正規化する */
	gsVector3Normalize( &LineVec, &LineVec );

	/* 球体とレイの衝突判定を行う */
	return	gsCollisionSphereAndRay( pCenter, Radius, pLine0, &LineVec, pIntersect );
}

/*=============================================================================
*
* Purpose : 線分と線分の衝突判定．
*
* Return  : 線分と線分が衝突していれば真，衝突していなければ偽を返す．
*
*============================================================================*/
extern GSboolean
gsCollisionLineAndLine
(
	const GSvector3*	pA1,			/* 線分ａの始点						*/
	const GSvector3*	pA2,			/* 線分ａの終点						*/
	const GSvector3*	pB1,			/* 線分ｂの始点						*/
	const GSvector3*	pB2,			/* 線分ｂの終点						*/
	GSboolean			bInfinite,		/* 無限線分か？						*/
	GSfloat 			Epsilon,		/* 判定の閾値						*/
	GSvector3*			pPA,			/*-> 線分ａ上の線分ｂに最も近い座標	*/
	GSvector3*			pPB				/*-> 線分ｂ上の線分ａに最も近い座標	*/
)
{
	GSvector3			VA, VB, AB;
	GSfloat				L11, L22, L12;
	GSfloat				ra, rb;
	GSfloat				s, t;
	GSfloat				DetL;

	/* 線分ａ，ｂのベクトルを求める */
	gsVector3Subtract( &VA, pA2, pA1 );
	gsVector3Subtract( &VB, pB2, pB1 );

	/* 線分ａ，ｂのベクトルの二乗和を求める */
	L11 = gsVector3LengthSq( &VA );
	L22 = gsVector3LengthSq( &VB );

	if ( L11 < FLT_EPSILON )
	{
		/* 線分ａの長さが閾値よりも小さい場合 （ 点と線分の判定 ）*/
		*pPA = *pA1;

		/* 線分ａの始点と線分ｂ上の座標で最も近い座標を求める */
		FindNearestPointOnLineSegment( pB1, &VB, pA1, bInfinite, pPB, NULL );
	}
	else if ( L22 < FLT_EPSILON )
	{
		/* 線分ｂの長さが閾値よりも小さい場合 （ 点と線分の判定 ） */
		*pPB = *pB1;

		/* 線分ｂの始点と線分ａ上の座標で最も近い座標を求める */
		FindNearestPointOnLineSegment( pA1, &VA, pB1, bInfinite, pPA, NULL );
	}
	else
	{
		/* 線分ａと線分ｂが閾値よりも大きい（ 線分同士の判定 ） */
		gsVector3Subtract( &AB, pB1, pA1 );
		L12  = -gsVector3Dot( &VA, &VB );
		DetL = L11 * L22 - SQR( L12 );

		if ( ABS( DetL ) < FLT_EPSILON )
		{
			/* 交差しない線分同士の線分の最も近い点を求める */
			FindNearestPointOfParallelLineSegments
			(
				pA1, pA2, &VA,
				pB1, pB2, &VB,
				bInfinite,
				pPA, pPB
			);
		}
		else
		{
			/* 交差する線分の同士の最も近い点を求める */
			ra =  gsVector3Dot( &VA, &AB );
			rb = -gsVector3Dot( &VB, &AB );
			t  = ( L11 * rb - ra * L12 ) / DetL;
			s  = ( ra - L12 * t ) / L11;
			gsVector3LinearEquation( pPA, pA1, &VA, s );
			gsVector3LinearEquation( pPB, pB1, &VB, t );

			/* 有限線分かつどちらかの線分が少し離れている？ */
			if ( ( bInfinite == GS_FALSE )
			&& ( OUT_OF_RANGE(s) == GS_TRUE || OUT_OF_RANGE(t) == GS_TRUE ) )
			{
				/* 有限線分の同士の最も近い点の調整をする */
				AdjustNearestPoints( pA1, &VA, pB1, &VB, s, t, pPA, pPB );
			}
		}
	}

	/* 最も近い点同士の距離が閾値よりも短い場合は衝突している */
	if ( gsVector3DistanceSq( pPA, pPB ) <= SQR( Epsilon ) )
	{
		return	GS_TRUE;
	}

	return	GS_FALSE;
}

/*=============================================================================
*
* Purpose : カプセルとカプセルの衝突判定．
*
* Return  : カプセルが衝突していれば真，衝突していなければ偽を返す．
*
*============================================================================*/
extern GSboolean
gsCollisionCapsuleAndCapsule
(
	const GSvector3*	pA1,			/* カプセルａの始点					*/
	const GSvector3*	pA2,			/* カプセルａの終点					*/
	const float			Aw,				/* カプセルａの幅					*/
	const GSvector3*	pB1,			/* カプセルｂの始点					*/
	const GSvector3*	pB2,			/* カプセルｂの終点					*/
	const float			Bw,				/* カプセルｂの幅					*/
	GSvector3*			pNa,			/*-> 線分ａ上の線分ｂに最も近い座標	*/
	GSvector3*			pNb				/*-> 線分ｂ上の線分ａに最も近い座標	*/
)
{
	GSboolean result;
	GSvector3 nearA;
	GSvector3 nearB;
	result = gsCollisionLineAndLine(pA1, pA2, pB1, pB2, GS_FALSE, Aw + Bw, &nearA, &nearB);
	if (pNa != NULL)
	{
		*pNa = nearA;
	}
	if (pNb != NULL)
	{
		*pNb = nearB;
	}
	return result;
}

/*-----------------------------------------------------------------------------
*
* Purpose : 線分とある座標の最も隣接する線分上の座標を求める．
*
* Return  : なし．
*
*----------------------------------------------------------------------------*/
static void
FindNearestPointOnLineSegment
(
	const GSvector3*	pLinePos,		/* 線分の座標					*/
	const GSvector3*	pLineVec,		/* 線分のベクトル				*/
	const GSvector3	*	pPoint,			/* 調べたい座標					*/
	GSboolean			bInfinite,		/* 無限線分か？					*/
	GSvector3*			pNP,			/*-> 線分上の隣接する座標		*/
	GSfloat*			pParameter		/*-> 直線の方程式のパラメータ	*/
)
{
	GSvector3			LP;
	GSfloat				d, t;


	/* 線分のベクトルの長さが閾値よりも小さい？ */
	if ( ( d = gsVector3LengthSq( pLineVec ) ) < FLT_EPSILON )
	{
		/* 線分の始点が最も近い （ 線分は点として扱う ）*/
		*pNP = *pLinePos;

		if ( pParameter != NULL )
		{
			*pParameter = 0.0f;
		}

		return;
	}

	/* 線分の始点から点へのベクトルを求める */
	gsVector3Subtract( &LP, pPoint, pLinePos );

	/* 線分上の調べたい座標に最も近い座標のパラメータを求める */
	t = gsVector3Dot( pLineVec, &LP ) / d;

	/* 有限線分か？ */
	if ( bInfinite == GS_FALSE )
	{
		/* 有限線分の場合はtを0.0f～1.0fの間でクランプする */
		t = CLAMP( t, 0.0f, 1.0f );
	}

	/* 求めたパラメータから最も近い座標を求める */
	gsVector3LinearEquation( pNP, pLinePos, pLineVec, t );

	if ( pParameter != NULL )
	{
		*pParameter = t;
	}
}

/*-----------------------------------------------------------------------------
*
* Purpose : 線分同士が交差しない場合の線分同士で最も近い点を求める．
*
* Return  : なし．
*
*----------------------------------------------------------------------------*/
static void
FindNearestPointOfParallelLineSegments
(
	const GSvector3*	pA1,			/* 線分ａの始点							*/
	const GSvector3*	pA2,			/* 線分ａの終点							*/
	const GSvector3*	pVA,			/* 線分ａの始点から終点へのベクトル		*/
	const GSvector3*	pB1,			/* 線分ｂの始点							*/
	const GSvector3*	pB2,			/* 線分ｂの終点							*/
	const GSvector3*	pVB,			/* 線分のの始点から終点へのベクトル		*/
	GSboolean			bInfinite,		/* 無限線分か？							*/
	GSvector3*			pPA,			/*-> 線分ａ上の線分ｂに最も近い座標		*/
	GSvector3*			pPB				/*-> 線分ｂ上の線分ａに最も近い座標		*/
)
{
	float				s1, s2;
	float				t;
	GSvector3			tmpPoint;

	/* 線分ａ上の座標で最も線分ｂの始点に近い座標を求める */
	FindNearestPointOnLineSegment( pA1, pVA, pB1, GS_TRUE, pPA, &s1 );

	/* 無限線分か？ */
	if ( bInfinite == GS_TRUE )
	{
		/* 無限線分の場合は線分ｂの始点を最も近い座標とする */
		*pPB = *pB1;
	}
	else
	{
		/* 線分ａ上の座標で最も線分ｂの終点に近い座標を求める */
		FindNearestPointOnLineSegment( pA1, pVA, pB2, GS_TRUE, &tmpPoint, &s2 );

		if ( ( s1 < 0.0f ) && ( s2 < 0.0f ) )
		{
			*pPA = *pA1;
			*pPB = ( s1 < s2 ) ? *pB2: *pB1;
		}
		else if ( ( s1 > 1.0f ) && ( s2 > 1.0f ) )
		{
			*pPA = *pA2;
			*pPB = ( s1 < s2 ) ? *pB1: *pB2;
		}
		else
		{
			t = 0.5f * ( CLAMP( s1, 0.0f, 1.0f ) + CLAMP( s2, 0.0f, 1.0f ) );
			gsVector3LinearEquation( pPA, pA1, pVA, t );
			FindNearestPointOnLineSegment( pB1, pVB, pPA, GS_TRUE, pPB, NULL );
		}
	}
}

/*-----------------------------------------------------------------------------
*
* Purpose : 線分同士の最も近い点の調整をする．
*
* Return  : なし．
*
*----------------------------------------------------------------------------*/
static void
AdjustNearestPoints
(
	const GSvector3*	pA1,			/* 線分ａの始点						*/
	const GSvector3*	pVA,			/* 線分ａの始点から終点へのベクトル	*/
	const GSvector3*	pB1,			/* 線分ｂの始点						*/
	const GSvector3*	pVB,			/* 線分ｂの始点から終点へのベクトル	*/
	GSfloat				s,				/* 線分ａの直線の方程式のパラメータ	*/
	GSfloat				t,				/* 線分ｂの直線の方程式のパラメータ	*/
	GSvector3*			pPA,			/*-> 線分ａ上の線分ｂに最も近い座標	*/
	GSvector3*			pPB				/*-> 線分ｂ上の線分ａに最も近い座標	*/
)
{
	if ( OUT_OF_RANGE( s ) == GS_TRUE && OUT_OF_RANGE( t ) == GS_TRUE )
	{
		s = CLAMP( s, 0.0f, 1.0f );
		gsVector3LinearEquation( pPA, pA1, pVA, s );
		FindNearestPointOnLineSegment( pB1, pVB, pPA, GS_TRUE, pPB, &t );

		if ( OUT_OF_RANGE( t ) == GS_TRUE )
		{
			t = CLAMP( t, 0.0f, 1.0f );
			gsVector3LinearEquation( pPB, pB1, pVB, t );
			FindNearestPointOnLineSegment( pA1, pVA, pPB, GS_FALSE, pPA, NULL );
			FindNearestPointOnLineSegment( pB1, pVB, pPA, GS_FALSE, pPB, NULL );
		}
	}
	else if ( OUT_OF_RANGE( s ) == GS_TRUE )
	{
		s = CLAMP( s, 0.0f, 1.0f );
		gsVector3LinearEquation( pPA, pA1, pVA, s );
		FindNearestPointOnLineSegment( pB1, pVB, pPA, GS_FALSE, pPB, NULL );
	}
	else if ( OUT_OF_RANGE( t ) == GS_TRUE )
	{
		t = CLAMP( t, 0.0f, 1.0f );
		gsVector3LinearEquation( pPB, pB1, pVB, t );
		FindNearestPointOnLineSegment( pA1, pVA, pPB, GS_FALSE, pPA, NULL );
	}
}

/*-----------------------------------------------------------------------------
*
* Purpose : ポリゴン内に交点が存在するか調べる．
*
* Return  : 交点が存在すれば真，存在しなければ偽を返す．
*
*----------------------------------------------------------------------------*/
static GSboolean
InsidePolygon
(
	const GSvector3*	pPoly,			/* ポリゴンの頂点配列	*/
	GSuint				numVertices,	/* 頂点データ数			*/
	const GSvector3*	pPoint			/* 平面との交点			*/
)
{
#define	MATCH_FACTOR	0.99f			/* 比較時の許容範囲		*/
	GSfloat				Angle;
	GSfloat				CosTheta;
	GSfloat				LenV1, LenV2;
	GSvector3			V1, V2;
	GSuint				i;

	/* ポリゴンの各頂点と交点を結ぶ線分の角度の合計を求める */
	Angle = 0.0f;
	for ( i = 0; i < numVertices; i++ )
	{
		gsVector3Subtract( &V1, &pPoly[i], pPoint );
		gsVector3Subtract( &V2, &pPoly[ (i + 1) % numVertices ], pPoint );

		/* 頂点と交点の距離を求める */
		if ( ( LenV1 = gsVector3Length( &V1 ) ) < 0.0001f
		  || ( LenV2 = gsVector3Length( &V2 ) ) < 0.0001f )
		{
			return	GS_TRUE;
		}

		/* 二つのベクトルなす角度を求める */
		CosTheta = gsVector3Dot( &V1, &V2 ) / ( LenV1 * LenV2 );

		/* 角度の合計を求める */
		Angle += gsAcos( CLAMP( CosTheta, -1.0f, 1.0f ) );
	}

	/* 合計した角度が３６０度であれば，ポリゴン上に交点が存在する */
	if( Angle >= ( MATCH_FACTOR * 360.0f ) )
	{
		return	GS_TRUE;
	}

	return	GS_FALSE;
}

/*-----------------------------------------------------------------------------
*
* Purpose : ポリゴンのエッジと球体が衝突しているか調べる．
*
* Return  : 衝突していれば真，衝突していなければ偽を返す．
*
*----------------------------------------------------------------------------*/
static GSboolean
CollisionPolyEdgeAndSphere
(
	const GSvector3*	pPoly,			/* ポリゴンの頂点配列				*/
	GSuint				numVertices,	/* 頂点データ数						*/
	const GSvector3*	pCenter,		/* 球体の中心						*/
	GSfloat				Radius,			/* 球体の半径						*/
	GSvector3*			pOffset			/*-> 球体が衝突しない座標への補正値	*/
)
{
	GSvector3			Point;
	GSvector3			Vector;
	GSfloat				DistanceSq;
	GSuint				i;

	for ( i = 0; i < numVertices; i++ )
	{
		/* 球体の中心座標とポリゴンを結ぶ線分上の最も近い座標を求める */
		gsVector3Subtract( &Vector, &pPoly[ (i+1) % numVertices ], &pPoly[i] );
		FindNearestPointOnLineSegment( &pPoly[i], &Vector, pCenter, GS_FALSE, &Point, NULL );

		/* 線分上の座標と球体の中心座標の距離を求める */
		DistanceSq = gsVector3DistanceSq( &Point, pCenter );

		/* 求めた距離が球体半径よりも短い場合は線分と球体が交差している */
		if ( DistanceSq < SQR( Radius ) )
		{
			/* 補正値を求める */
			if ( pOffset != NULL )
			{
				/* 隣接点から球体の中心への単位ベクトルを求める */
				gsVector3Subtract( pOffset, pCenter, &Point );
				gsVector3Normalize( pOffset, pOffset );

				/* 交差している部分を補正する */
				gsVector3Scale( pOffset, pOffset, Radius - gsSqrt( DistanceSq ) );
			}

			/* 衝突している */
			return	GS_TRUE;
		}
	}

	/* 衝突していない */
	return GS_FALSE;
}

/********** End of File ******************************************************/
