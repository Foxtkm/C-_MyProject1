/******************************************************************************
* File Name : GSplane.h                          Ver : 1.00  Date : 2002-03-24
*
* Description :
*
*       平面の計算 ヘッダファイル．
*
* Author : Moriharu Ohzu.
*
******************************************************************************/
#ifndef		_GSPLANE_H_
#define		_GSPLANE_H_

#include	"GStype.h"
#include	"GSmath.h"
#include	"GSvector3.h"
#include	<float.h>

/****** 定数宣言 *************************************************************/

#define	GS_FRONT_PLANE			0		/* 平面の前			*/
#define	GS_BEHIND_PLANE			1		/* 平面の後ろ		*/
#define	GS_ON_PLANE				2		/* 平面上			*/
#define	GS_INTERSECTS_PLANE		3		/* 平面と交差		*/

/****** 関数プロトタイプ宣言 *************************************************/

#ifdef __cplusplus
extern "C" {
#endif

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

/*=============================================================================
*
* Purpose : 平面の線形補間．
*
* Return  : 補間後の平面方程式．
*
*============================================================================*/
extern GSplane*
gsPlaneLerp
(
	GSplane* 		pOut,			/*-> 平面方程式パラメータ	*/
	const GSplane*	pP1,			/* 開始						*/
	const GSplane*	pP2,			/* 終了						*/
	GSfloat			t				/* 補間値( 0.0f ～ 1.0f )	*/
);

/*=============================================================================
*
* Purpose : 平面の２次補間．
*
* Return  : 補間後の平面方程式．
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
);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

/* コンストラクタ */
GSINLINE GSplane::GSplane( GSfloat A, GSfloat B, GSfloat C, GSfloat D ) :
	a( A ), b( B ), c( C ), d( D )
{}

GSINLINE GSplane::GSplane( const GSvector3& N, GSfloat D ) :
	a( N.x ), b( N.y ), c( N.z ), d( D )
{}

GSINLINE GSplane::GSplane( const GSvector3& p, const GSvector3& n )
{
	gsPlaneFromPointNormal( this, &p, &n );
}

GSINLINE GSplane::GSplane( const GSvector3& v1, const GSvector3& v2, const GSvector3& v3 )
{
	gsPlaneFromPoints( this, &v1, &v2, &v3 );
}

/* 正規化する */
GSINLINE GSplane& GSplane::normalize()
{
	gsPlaneNormalize( this, this );
}

/* 正規化後の平面を得る */
GSINLINE GSplane GSplane::getNormalized()
{
	GSplane		result;

	gsPlaneNormalize( &result, this );

	return	result;
}

/* 平面上の位置を取得 */
GSINLINE GSvector3 GSplane::getPoint() const
{
	GSvector3		point;

	gsPlanePoint( this, &point );

	return	point;
}

/* 平面とベクトルの内積を求める */
GSINLINE GSfloat GSplane::dot( const GSvector3& v ) const
{
	return	gsPlaneDotNormal( this, &v );
}

/* 平面と座標の距離を求める */
GSINLINE GSfloat GSplane::distance( const GSvector3& v ) const
{
	return	gsPlaneDotCoord( this, &v );
}

/* 平面と線分が交差するか調べる */
GSINLINE bool GSplane::isIntersectLine( const GSvector3& Line0,  const GSvector3& Line1 ) const
{
	if ( gsPlaneIntersected( this, &Line0, &Line1 ) == GS_TRUE )
	{
		return	true;
	}

	return	false;
}

/* 平面と線分の交点を取得する */
GSINLINE GSvector3 GSplane::getIntersectLine( const GSvector3& Line0,  const GSvector3& Line1 ) const
{
	GSvector3		result;

	gsPlaneIntersectRay( &result, this, &Line0, &Line1 );

	return	result;
}

/* 平面と線分の交点を取得する */
GSINLINE bool GSplane::isIntersectRay( const GSvector3& rayPos,  const GSvector3& rayDir ) const
{
	(void)rayPos;
	/* 分母を求める */
	if ( dot( rayDir.getNormalized() ) == 0.0f )
	{
		/* 分母が０場合はレイと平面は平行である */
		return	false;
	}

	return	true;
}

/* 平面と線分の交点を取得する */
GSINLINE GSvector3 GSplane::getIntersectRay( const GSvector3& ray,  const GSvector3& dir ) const
{
	GSvector3		result;

	gsPlaneIntersectRay( &result, this, &ray, &dir );

	return	result;
}

/* 平面と座標の位置関係を返す */
GSINLINE GSenum GSplane::getClassifyPoint( const GSvector3& p ) const
{
	GSfloat		distance;

	return	gsPlaneClassifyPoint( this, &p, &distance );
}

/* 平面と球体の位置関係を返す */
GSINLINE GSenum GSplane::getClassifyShpere( const GSvector3& center, GSfloat radius ) const
{
	GSfloat		distance;

	return	gsPlaneClassifySphere( this, &center, radius, &distance );
}

/* 線形補間を行う */
GSINLINE GSplane GSplane::lerp( const GSplane& p, GSfloat t ) const
{
	GSplane		result;

	gsPlaneLerp( &result, this, &p, t );

	return	result;
}

/* ２次補間を行う */
GSINLINE GSplane GSplane::quadratic( const GSplane& p2, const GSplane& p3, GSfloat t ) const
{
	GSplane		result;

	gsPlaneQuadratic( &result, this, &p2, &p3, t );

	return	result;
}

/* 単項演算子オーバーロード */
GSINLINE GSplane  GSplane::operator + () const
{
	return	*this;
}

GSINLINE GSplane  GSplane::operator - () const
{
	return	GSplane( -a, -b, -c, -d );
}

/* ２項演算子オーバーロード */
GSINLINE const GSplane operator * ( const GSmatrix3& mat, const GSplane& p )
{
	GSplane				result;
	GSvector3			P;
	GSvector3			T;
	GSvector3			N;

	/* 平面上の座標を求める */
	gsPlanePoint( &p, &T );

	/* 平面上の座標を変換する */
	P.x = T.x * mat.m[0][0]
	    + T.y * mat.m[1][0]
	    + T.z * mat.m[2][0];
	P.y = T.x * mat.m[0][1]
	    + T.y * mat.m[1][1]
	    + T.z * mat.m[2][1];
	P.z = T.x * mat.m[0][2]
	    + T.y * mat.m[1][2]
	    + T.z * mat.m[2][2];

	/* 法線ベクトルの座標変換 */
	N.x = p.a * mat.m[0][0]
	    + p.b * mat.m[1][0]
	    + p.c * mat.m[2][0];
	N.y = p.a * mat.m[0][1]
	    + p.b * mat.m[1][1]
	    + p.c * mat.m[2][1];
	N.z = p.a * mat.m[0][2]
	    + p.b * mat.m[1][2]
	    + p.c * mat.m[2][2];

	/* 座標と法線から平面を求める */
	gsPlaneFromPointNormal( &result, &P, &N );

	return	result;
}

GSINLINE const GSplane operator * ( const GSplane& p, const GSmatrix3& mat )
{
	GSplane				result;
	GSvector3			P;
	GSvector3			T;
	GSvector3			N;

	/* 平面上の座標を求める */
	gsPlanePoint( &p, &T );

	/* 平面上の座標を変換する */
	P.x = T.x * mat.m[0][0]
	    + T.y * mat.m[0][1]
	    + T.z * mat.m[0][2];
	P.y = T.x * mat.m[1][0]
	    + T.y * mat.m[1][1]
	    + T.z * mat.m[1][2];
	P.z = T.x * mat.m[2][0]
	    + T.y * mat.m[2][1]
	    + T.z * mat.m[2][2];

	/* 法線ベクトルの座標変換 */
	N.x = p.a * mat.m[0][0]
	    + p.b * mat.m[0][1]
	    + p.c * mat.m[0][2];
	N.y = p.a * mat.m[1][0]
	    + p.b * mat.m[1][1]
	    + p.c * mat.m[1][2];
	N.z = p.a * mat.m[2][0]
	    + p.b * mat.m[2][1]
	    + p.c * mat.m[2][2];

	/* 座標と法線から平面を求める */
	gsPlaneFromPointNormal( &result, &P, &N );

	return	result;
}

GSINLINE const GSplane operator * ( const GSmatrix4& mat, const GSplane& p )
{
	GSplane				result;
	GSvector3			P;
	GSvector3			T;
	GSvector3			N;

	/* 平面上の座標を求める */
	gsPlanePoint( &p, &T );

	/* 平面上の座標を変換する */
	P.x = T.x * mat.m[0][0]
	    + T.y * mat.m[1][0]
	    + T.z * mat.m[2][0]
	    +       mat.m[3][0];
	P.y = T.x * mat.m[0][1]
	    + T.y * mat.m[1][1]
	    + T.z * mat.m[2][1]
	    +       mat.m[3][1];
	P.z = T.x * mat.m[0][2]
	    + T.y * mat.m[1][2]
	    + T.z * mat.m[2][2]
	    +       mat.m[3][2];

	/* 法線ベクトルの座標変換 */
	N.x = p.a * mat.m[0][0]
	    + p.b * mat.m[1][0]
	    + p.c * mat.m[2][0];
	N.y = p.a * mat.m[0][1]
	    + p.b * mat.m[1][1]
	    + p.c * mat.m[2][1];
	N.z = p.a * mat.m[0][2]
	    + p.b * mat.m[1][2]
	    + p.c * mat.m[2][2];

	/* 座標と法線から平面を求める */
	gsPlaneFromPointNormal( &result, &P, &N );

	return	result;
}

GSINLINE const GSplane operator * ( const GSplane& p, const GSmatrix4& mat )
{
	GSplane				result;
	GSvector3			P;
	GSvector3			T;
	GSvector3			N;

	/* 平面上の座標を求める */
	gsPlanePoint( &p, &T );

	/* 平面上の座標を変換する */
	P.x = T.x * mat.m[0][0]
	    + T.y * mat.m[0][1]
	    + T.z * mat.m[0][2]
	    +       mat.m[0][3];
	P.y = T.x * mat.m[1][0]
	    + T.y * mat.m[1][1]
	    + T.z * mat.m[1][2]
	    +       mat.m[1][3];
	P.z = T.x * mat.m[2][0]
	    + T.y * mat.m[2][1]
	    + T.z * mat.m[2][2]
	    +       mat.m[2][3];

	/* 法線ベクトルの座標変換 */
	N.x = p.a * mat.m[0][0]
	    + p.b * mat.m[0][1]
	    + p.c * mat.m[0][2];
	N.y = p.a * mat.m[1][0]
	    + p.b * mat.m[1][1]
	    + p.c * mat.m[1][2];
	N.z = p.a * mat.m[2][0]
	    + p.b * mat.m[2][1]
	    + p.c * mat.m[2][2];

	/* 座標と法線から平面を求める */
	gsPlaneFromPointNormal( &result, &P, &N );

	return	result;
}

GSINLINE bool operator == ( const GSplane& p1, const GSplane& p2 )
{
	return	( p1.a == p2.a && p1.b == p2.b && p1.c == p2.c && p1.d == p2.d );
}

GSINLINE bool operator != ( const GSplane& p1, const GSplane& p2 )
{
	return	!(p1 == p2 );
}

#endif

#endif

/********** End of File ******************************************************/
