/******************************************************************************
* File Name : GSplane.c                          Ver : 1.00  Date : 2002-03-24
*
* Description :
*
*       平面の計算．
*
* Author : Moriharu Ohzu.
*
******************************************************************************/
#include	"GSplane.h"
#include	"GSvector3.h"
#include	<float.h>

/*=============================================================================
*
* Purpose : 平面上の座標と平面の法線ベクトルから平面を作成．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsPlaneFromPointNormal
(
	GSplane* 			pOut,			/* 平面方程式パラメータ	*/
	const GSvector3*	pPoint,			/* 平面上の座標			*/
	const GSvector3*	pNormal			/* 平面の法線ベクトル	*/
)
{
	pOut->a = pNormal->x;
	pOut->b = pNormal->y;
	pOut->c = pNormal->z;
	pOut->d = -gsVector3Dot( pNormal, pPoint );
}

/*=============================================================================
*
* Purpose : 平面上の３つの座標から平面を作成．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsPlaneFromPoints
(
	GSplane* 			pOut,			/* 平面方程式パラメータ	*/
	const GSvector3*	pV1,			/* 平面を構成する座標１	*/
	const GSvector3*	pV2,			/* 平面を構成する座標２	*/
	const GSvector3*	pV3				/* 平面を構成する座標３	*/
)
{
	GSvector3			Normal;

	/* 法線ベクトルを求める */
	gsVector3CalculateNormal( &Normal, pV1, pV2, pV3 );

	/* 法線ベクトルと座標から平面のパラメータを求める */
	gsPlaneFromPointNormal( pOut, pV1, &Normal );
}

/*=============================================================================
*
* Purpose : 平面上の点の座標を求める．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsPlanePoint
(
	const GSplane*		pPlane,			/* 平面方程式パラメータ	*/
	GSvector3*			pPoint			/*-> 平面上の点の座標	*/
)
{
	pPoint->x = pPlane->a * -pPlane->d;
	pPoint->y = pPlane->b * -pPlane->d;
	pPoint->z = pPlane->c * -pPlane->d;
}

/*=============================================================================
*
* Purpose : 平面と３次元ベクトルの内積を求める．( w = 1.0とする )
*
* Return  : 内積を返す．
*
*============================================================================*/
extern GSfloat
gsPlaneDotNormal
(
	const GSplane* 		pPlane,			/* 平面方程式パラメータ	*/
	const GSvector3*	pV				/* ベクトル				*/
)
{
	return	pPlane->a * pV->x + pPlane->b * pV->y + pPlane->c * pV->z;
}

/*=============================================================================
*
* Purpose : 平面と３次元ベクトルの内積を求める．( w = 0.0とする )
*
* Return  : 内積を返す．
*
*============================================================================*/
extern GSfloat
gsPlaneDotCoord
(
	const GSplane* 		pPlane,			/* 平面方程式パラメータ	*/
	const GSvector3*	pV				/* ベクトル				*/
)
{
	return	pPlane->a * pV->x
		  + pPlane->b * pV->y
		  + pPlane->c * pV->z
		  + pPlane->d;
}

/*=============================================================================
*
* Purpose : 平面を正規化する．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsPlaneNormalize
(
	GSplane*			pOut,			/* 平面と線分の交点の座標	*/
	const GSplane* 		pPlane			/* 平面方程式パラメータ		*/
)
{
	GSfloat				Len;

	Len = gsSqrt( SQR( pPlane->a ) + SQR( pPlane->b ) + SQR( pPlane->c ) );

	if ( Len != 0 )
	{
		pOut->a = pPlane->a / Len;
		pOut->b = pPlane->b / Len;
		pOut->c = pPlane->c / Len;
		pOut->d = pPlane->d / Len;
	}
}

/*=============================================================================
*
* Purpose : 平面の座標変換を行う．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsPlaneTransform
(
	GSplane*			pOut,			/*-> 変換後の平面方程式パラメータ	*/
	const GSplane*		pPlane,			/* 平面方程式パラメータ				*/
	const GSmatrix4*	pM				/* 座標変換行列						*/
)
{
	GSvector3			P;
	GSvector3			N;

	/* 平面上の座標を求める */
	gsPlanePoint( pPlane, &P );

	/* 平面上の座標を変換する */
	gsVector3TransformCoord( &P, &P, pM );

	/* 法線ベクトルの座標変換 */
	N.x = pPlane->a * pM->m[0][0]
	    + pPlane->b * pM->m[1][0]
	    + pPlane->c * pM->m[2][0];
	N.y = pPlane->a * pM->m[0][1]
	    + pPlane->b * pM->m[1][1]
	    + pPlane->c * pM->m[2][1];
	N.z = pPlane->a * pM->m[0][2]
	    + pPlane->b * pM->m[1][2]
	    + pPlane->c * pM->m[2][2];

	/* 座標と法線から平面を求める */
	gsPlaneFromPointNormal( pOut, &P, &N );
}

/*=============================================================================
*
* Purpose : 平面と線分が交差するか調べる．
*
* Return  : 交差すれば真，交差しなければ偽を返す．
*
*============================================================================*/
extern GSboolean
gsPlaneIntersected
(
	const GSplane* 		pPlane,			/* 平面方程式パラメータ	*/
	const GSvector3*	pLine0,			/* 線分の始点			*/
	const GSvector3*	pLine1			/* 線分の終点			*/
)
{
	GSfloat				Distance1;
	GSfloat				Distance2;

	/* 平面と始点の距離を求める */
	Distance1 = gsPlaneDotCoord( pPlane, pLine0 );

	/* 平面と終点の距離を求める */
	Distance2 = gsPlaneDotCoord( pPlane, pLine1 );

	/* 始点と終点の距離が同符号の場合は交差しない */
	if ( ( Distance1 * Distance2 ) > 0 )
	{
		return	GS_FALSE;
	}

	return	GS_TRUE;
}

/*=============================================================================
*
* Purpose : 平面とレイが交差する座標を求める．
*
* Return  : 交差すれば真，交差しなければ偽を返す．
*
*============================================================================*/
extern GSboolean
gsPlaneIntersectRay
(
	GSvector3*			pOut,			/* 平面と線分の交点の座標	*/
	const GSplane* 		pPlane,			/* 平面方程式パラメータ		*/
	const GSvector3*	pRayPos,		/* レイの始点				*/
	const GSvector3*	pRayDir			/* レイの方向				*/
)
{
	GSvector3			Normal;
	GSfloat				Denominator;
	GSfloat				Numerator;
	GSfloat				Distance;

	/* 線分の方向を正規化する */
	gsVector3Normalize( &Normal, pRayDir );

	/* 分母を求める */
	Denominator = gsPlaneDotNormal( pPlane, &Normal );

	/* 分母が０の場合は平面と直線が平行となる */
	if( Denominator == 0.0f )
	{
		return	GS_FALSE;
	}

	if ( pOut != NULL )
	{
		/* 分子を求める */
		Numerator = -gsPlaneDotCoord( pPlane, pRayPos );

		/* 始点と平面の距離を求める */
		Distance = Numerator / Denominator;

		/* 交点を求める */
		gsVector3LinearEquation( pOut, pRayPos, &Normal, Distance );
	}

	return	GS_TRUE;
}

/*=============================================================================
*
* Purpose : 平面と線分が交差する座標を求める．
*
* Return  : 交差すれば真，交差しなければ偽を返す．
*
*============================================================================*/
extern GSboolean
gsPlaneIntersectLine
(
	GSvector3*			pOut,			/* 平面と線分の交点の座標	*/
	const GSplane* 		pPlane,			/* 平面方程式パラメータ		*/
	const GSvector3*	pLine0,			/* 線分の始点				*/
	const GSvector3*	pLine1			/* 線分の終点				*/
)
{
	GSvector3			Direction;

	/* 平面と線分が交点を持つか？ */
	if ( gsPlaneIntersected( pPlane, pLine0, pLine1 ) == GS_FALSE )
	{
		return	GS_FALSE;
	}

	gsVector3Subtract( &Direction, pLine1, pLine0 ); 

	/* 平面と線分の交点を求める */
	return	gsPlaneIntersectRay( pOut, pPlane, pLine0, &Direction );
}

/*=============================================================================
*
* Purpose : 平面と座標の位置関係を返す．
*
* Return  : 平面の正面，平面の裏面，平面上にあるの何れかを返す．
*
*           正面   : GS_FRONT_PLANE
*           背面   : GS_BEHIND_PLANE
*           平面上 : GS_ON_PLANE
*
*============================================================================*/
extern GSenum
gsPlaneClassifyPoint
(
	const GSplane* 		pPlane,			/* 平面方程式パラメータ	*/
	const GSvector3*	pPoint,			/* 点の座標				*/
	GSfloat*			pDistance		/* 平面と点の距離		*/
)
{
	GSfloat				Distance;

	/* 平面と点との距離を求める */
	Distance = gsPlaneDotCoord( pPlane, pPoint );

	if ( pDistance != NULL )
	{
		*pDistance = Distance;
	}

	/* 平面の前面に点が存在するか？*/
	if( Distance > FLT_EPSILON )
	{
		return	GS_FRONT_PLANE;
	}

	/* 平面の背面に点が存在するか？*/
	if( Distance < -FLT_EPSILON )
	{
		return	GS_BEHIND_PLANE;
	}

	/* 平面上に点が存在する */
	return	GS_ON_PLANE;
}

/*=============================================================================
*
* Purpose : 平面と球体の位置関係を返す．
*
* Return  : 平面の正面，平面の裏面，平面と交差の何れかを返す．
*
*           正面 : GS_FRONT_PLANE
*           背面 : GS_BEHIND_PLANE
*           交差 : GS_INTERSECTS_PLANE
*
*============================================================================*/
extern GSenum
gsPlaneClassifySphere
(
	const GSplane* 		pPlane,			/* 平面方程式パラメータ		*/
	const GSvector3*	pCenter,		/* 球体の中心座標			*/
	GSfloat				Radius,			/* 球体の半径				*/
	GSfloat*			pDistance		/* 平面と球体の中心との距離	*/
)
{
	GSfloat				Distance;

	/* 平面と中心座標点の距離を求める */
	Distance = gsPlaneDotCoord( pPlane, pCenter );

	if ( pDistance != NULL )
	{
		*pDistance = Distance;
	}

	/* 平面と球体が交差しているか？*/
	if( ABS( Distance ) < Radius )
	{
		return	GS_INTERSECTS_PLANE;
	}

	/* 平面の前面に球体が存在するか？*/
	if( Distance >= Radius )
	{
		return	GS_FRONT_PLANE;
	}

	/* 平面上の背面に球体が存在する */
	return	GS_BEHIND_PLANE;
}

/*=============================================================================
*
* Purpose : 平面の線形補間．
*
* Return  : 交差すれば真，交差しなければ偽を返す．
*
*============================================================================*/
extern GSplane*
gsPlaneLerp
(
	GSplane* 		pOut,			/*-> 平面方程式パラメータ	*/
	const GSplane*	pP1,			/* 開始						*/
	const GSplane*	pP2,			/* 終了						*/
	GSfloat			t				/* 補間値( 0.0f ～ 1.0f )	*/
)
{
	pOut->a = LERP( pP1->a, pP2->a, t );
	pOut->b = LERP( pP1->b, pP2->b, t );
	pOut->c = LERP( pP1->c, pP2->c, t );
	pOut->d = LERP( pP1->d, pP2->d, t );

	gsVector3Normalize( &pOut->normal, &pOut->normal );

	return	pOut;
}

/*=============================================================================
*
* Purpose : 平面の２次補間．
*
* Return  : 交差すれば真，交差しなければ偽を返す．
*
*============================================================================*/
extern GSplane*
gsPlaneQuadratic
(
	GSplane* 		pOut,			/*-> 平面方程式パラメータ	*/
	const GSplane*	pP1,			/* 開始						*/
	const GSplane*	pP2,			/* 中間						*/
	const GSplane*	pP3,			/* 終了						*/
	GSfloat			t				/* 補間値( 0.0f ～ 1.0f )	*/
)
{
	pOut->a = QUADRATIC( pP1->a, pP2->a, pP3->a, t );
	pOut->b = QUADRATIC( pP1->b, pP2->b, pP3->b, t );
	pOut->c = QUADRATIC( pP1->c, pP2->c, pP3->c, t );
	pOut->d = QUADRATIC( pP1->d, pP2->d, pP3->d, t );

	gsVector3Normalize( &pOut->normal, &pOut->normal );

	return	pOut;
}

/********** End of File ******************************************************/
