/******************************************************************************
* File Name : GSvector4.h                         Ver : 1.00  Date : 2001-01-16
*
* Description :
*
*       ４Ｄベクトル計算 ヘッダファイル．
*
* Author : Moriharu Ohzu.
*
******************************************************************************/
#ifndef		_GSVECTOR4_H_
#define		_GSVECTOR4_H_

#include	"GStype.h"
#include	"GSmath.h"

/****** 関数プロトタイプ宣言 *************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*=============================================================================
*
* Purpose : ４Ｄベクトルを初期化．
*
* Return  : 初期化したベクトルを返す．
*
*============================================================================*/
GSINLINE GSvector4*
gsVector4Init
(
	GSvector4*		pOut,				/* -> 結果				*/
	GSfloat			x,					/* ｘ座標				*/
	GSfloat			y,					/* ｙ座標				*/
	GSfloat			z,					/* ｚ座標				*/
	GSfloat			w					/* ｚ座標				*/
)
{
	pOut->x = x;
	pOut->y = y;
	pOut->z = z;
	pOut->w = w;

	return	pOut;
}

/*=============================================================================
*
* Purpose : ４Ｄベクトルの加算．
*
* Return  : 加算後のベクトルを返す．
*
*============================================================================*/
GSINLINE GSvector4*
gsVector4Add
(
	GSvector4*			pOut,			/* -> 結果				*/
	const GSvector4*	pV1,			/* ４Ｄベクトル			*/
	const GSvector4*	pV2				/* ４Ｄベクトル			*/
)
{
	pOut->x = pV1->x + pV2->x;
	pOut->y = pV1->y + pV2->y;
	pOut->z = pV1->z + pV2->z;
	pOut->w = pV1->w + pV2->w;

	return	pOut;
}

/*=============================================================================
*
* Purpose : ベクトルの減算．
*
* Return  : 減算後のベクトルを返す．
*
*============================================================================*/
GSINLINE GSvector4*
gsVector4Subtract
(
	GSvector4*			pOut,			/* -> 結果				*/
	const GSvector4*	pV1,			/* ４Ｄベクトル			*/
	const GSvector4*	pV2				/* ４Ｄベクトル			*/
)
{
	pOut->x = pV1->x - pV2->x;
	pOut->y = pV1->y - pV2->y;
	pOut->z = pV1->z - pV2->z;
	pOut->w = pV1->w - pV2->w;

	return	pOut;
}

/*=============================================================================
*
* Purpose : ベクトルの乗算．
*
* Return  : 乗算後のベクトルを返す．
*
*============================================================================*/
GSINLINE GSvector4*
gsVector4Multiply

(
	GSvector4*			pOut,			/* -> 結果				*/
	const GSvector4*	pV1,			/* ４Ｄベクトル			*/
	const GSvector4*	pV2				/* ４Ｄベクトル			*/
)
{
	pOut->x = pV1->x * pV2->x;
	pOut->y = pV1->y * pV2->y;
	pOut->z = pV1->z * pV2->z;
	pOut->w = pV1->w * pV2->w;

	return	pOut;
}

/*=============================================================================
*
* Purpose : ベクトルの除算．
*
* Return  : 除算のベクトルを返す．
*
*============================================================================*/
GSINLINE GSvector4*
gsVector4Divide
(
	GSvector4*			pOut,			/* -> 結果				*/
	const GSvector4*	pV1,			/* ４Ｄベクトル			*/
	const GSvector4*	pV2				/* ４Ｄベクトル			*/
)
{
	pOut->x = pV1->x / pV2->x;
	pOut->y = pV1->y / pV2->y;
	pOut->z = pV1->z / pV2->z;
	pOut->w = pV1->w / pV2->w;

	return	pOut;
}

/*=============================================================================
*
* Purpose : ベクトルの内積をもとめる．
*
* Return  : 内積を返す．
*
*============================================================================*/
GSINLINE GSfloat
gsVector4Dot
(
	const GSvector4*	pV1,			/* ４Ｄベクトル			*/
	const GSvector4*	pV2				/* ４Ｄベクトル			*/
)
{
	return pV1->x * pV2->x + pV1->y * pV2->y + pV1->z * pV2->z + pV1->w * pV2->w;
}

/*=============================================================================
*
* Purpose : ベクトルの長さを求める．（ 二乗和のみで平方根をとらない ）
*
* Return  : ベクトルの長さを返す．
*
*============================================================================*/
GSINLINE GSfloat
gsVector4LengthSq
(
	const GSvector4*		pV			/* ４Ｄベクトル			*/
)
{
	return pV->x * pV->x + pV->y * pV->y + pV->z * pV->z + pV->w * pV->w;
}

/*=============================================================================
*
* Purpose : ベクトルの長さを求める．
*
* Return  : ベクトルの長さを返す．
*
*============================================================================*/
GSINLINE GSfloat
gsVector4Length
(
	const GSvector4*		pV			/* ４Ｄベクトル			*/
)
{
	return (GSfloat)gsSqrt( gsVector4LengthSq( pV ) );
}

/*=============================================================================
*
* Purpose : ベクトルの線形補間．
*
* Return  : なし．
*
*============================================================================*/
GSINLINE GSvector4*
gsVector4Lerp
(
	GSvector4*			pOut,			/* -> 結果				*/
	const GSvector4*	pMin,			/* ４Ｄベクトル			*/
	const GSvector4*	pMax,			/* ４Ｄベクトル			*/
	GSfloat				t				/* 補間値（0.0～1.0)	*/
)
{
	pOut->x = LERP( t, pMin->x, pMax->x );
	pOut->y = LERP( t, pMin->y, pMax->y );
	pOut->z = LERP( t, pMin->z, pMax->z );
	pOut->w = LERP( t, pMin->w, pMax->w );

	return	pOut;
}

/*=============================================================================
*
* Purpose : ベクトルの２次補間．
*
* Return  : なし．
*
*============================================================================*/
GSINLINE GSvector4*
gsVcector3Quadratic
(
	GSvector4*			pOut,			/* -> 結果				*/
	const GSvector4*	pV1,			/* ４Ｄベクトル			*/
	const GSvector4*	pV2,			/* ４Ｄベクトル			*/
	const GSvector4*	pV3,			/* ４Ｄベクトル			*/
	GSfloat				t				/* 補間値（0.0～1.0)	*/
)
{
	pOut->x = QUADRATIC( t, pV1->x, pV2->x, pV3->x );
	pOut->y = QUADRATIC( t, pV1->y, pV2->y, pV3->y );
	pOut->z = QUADRATIC( t, pV1->z, pV2->z, pV3->z );
	pOut->w = QUADRATIC( t, pV1->w, pV2->w, pV3->w );

	return	pOut;
}

/*=============================================================================
*
* Purpose : ２つのベクトルの各要素の最大値から作成されたベクトルを求める．
*
* Return  : 各要素の最大値を求めたベクトルを返す．
*
*============================================================================*/
GSINLINE GSvector4*
gsVector4Maximize
(
	GSvector4*			pOut,			/* -> 結果				*/
	const GSvector4*	pV1,			/* ４Ｄベクトル			*/
	const GSvector4*	pV2				/* ４Ｄベクトル			*/
)
{
	pOut->x = ( pV1->x > pV2->x ) ? pV1->x : pV2->x;
	pOut->y = ( pV1->y > pV2->y ) ? pV1->y : pV2->y;
	pOut->z = ( pV1->z > pV2->z ) ? pV1->z : pV2->z;
	pOut->w = ( pV1->w > pV2->w ) ? pV1->w : pV2->w;

	return	pOut;
}

/*=============================================================================
*
* Purpose : ２つのベクトルの各要素の最小値から作成されたベクトルを求める．
*
* Return  : 各要素の最小値を求めたベクトルを返す．
*
*============================================================================*/
GSINLINE GSvector4*
gsVector4Minimize
(
	GSvector4*			pOut,			/* -> 結果				*/
	const GSvector4*	pV1,			/* ４Ｄベクトル			*/
	const GSvector4*	pV2				/* ４Ｄベクトル			*/
)
{
	pOut->x = ( pV1->x < pV2->x ) ? pV1->x : pV2->x;
	pOut->y = ( pV1->y < pV2->y ) ? pV1->y : pV2->y;
	pOut->z = ( pV1->z < pV2->z ) ? pV1->z : pV2->z;
	pOut->w = ( pV1->w < pV2->w ) ? pV1->w : pV2->w;

	return	pOut;
}

/*=============================================================================
*
* Purpose : ベクトルのスケーリング．
*
* Return  : なし．
*
*============================================================================*/
GSINLINE GSvector4*
gsVector4Scale
(
	GSvector4*			pOut,			/* -> 結果				*/
	const GSvector4*	pV,				/* ４Ｄベクトル			*/
	GSfloat				s				/* スカラー				*/
)
{
	pOut->x = pV->x * s;
	pOut->y = pV->y * s;
	pOut->z = pV->z * s;
	pOut->w = pV->w * s;

	return	pOut;
}

/*=============================================================================
*
* Purpose : 逆ベクトルを求める．
*
* Return  : なし．
*
*============================================================================*/
GSINLINE GSvector4*
gsVector4Inverse
(
	GSvector4*			pOut,			/* -> 結果				*/
	const GSvector4*	pV				/* ４Ｄベクトル			*/
)
{
	pOut->x = -pV->x;
	pOut->y = -pV->y;
	pOut->z = -pV->z;
	pOut->w = -pV->w;

	return	pOut;
}

/*=============================================================================
*
* Purpose : 直線の方程式の計算を行う．
*
* Return  : なし．
*
*============================================================================*/
GSINLINE GSvector4*
gsVector4LinearEquation
(
	GSvector4*			pOut,			/* -> 結果				*/
	const GSvector4*	pP,				/* ４Ｄベクトル			*/
	const GSvector4*	pV,				/* ４Ｄベクトル			*/
	GSfloat				t				/* 係数					*/
)
{
	pOut->x = pV->x * t + pP->x;
	pOut->y = pV->y * t + pP->y;
	pOut->z = pV->z * t + pP->z;
	pOut->w = pV->w * t + pP->w;

	return	pOut;
}

/*=============================================================================
*
* Purpose : 方位角，仰角からベクトルを求める．
*
* Return  : なし．
*
*============================================================================*/
GSINLINE GSvector4*
gsVector4FromEleDir
(
	GSvector4*		pOut,				/* -> 結果			*/
	GSfloat			Elavation,			/* 仰角				*/
	GSfloat			Direction			/* 方位角			*/
)
{
	pOut->x =  gsCos( Elavation ) * gsSin( Direction );
	pOut->y = -gsSin( Elavation );
	pOut->z =  gsCos( Elavation ) * gsCos( Direction );
	pOut->w = 1.0f;

	return	pOut;
}
/*=============================================================================
*
* Purpose : ベクトルを正規化する．（ 単位ベクトル化する ）
*
* Return  : なし．
*
*============================================================================*/
GSINLINE void
gsVector4Normalize
(
	GSvector4*			pOut,			/*->単位ベクトル                */
	const GSvector4*	pV				/* ベクトル                     */
)
{
	GSfloat				len;

	if ( ( len = gsVector4Length( pV ) ) != 0 )
	{
		gsVector4Scale( pOut, pV, 1.0f / len  );
	}
}

/*=============================================================================
*
* Purpose : ２点間の距離を求める．
*
* Return  : 距離を返す．
*
*============================================================================*/
GSINLINE GSfloat
gsVector4Distance
(
	const GSvector4*	pV1,			/* 始点                         */
	const GSvector4*	pV2				/* 終点                         */
)
{
	GSvector4			V;

	gsVector4Subtract( &V, pV1, pV2 );

	return	gsVector4Length( &V );
}

/*=============================================================================
*
* Purpose : ２点間の距離を求める．（ 二乗和だけで平方根を取らない ）
*
* Return  : 距離を返す．
*
*============================================================================*/
GSINLINE GSfloat
gsVector4DistanceSq
(
	const GSvector4*	pV1,			/* 始点                         */
	const GSvector4*	pV2				/* 終点                         */
)
{
	GSvector4			V;

	gsVector4Subtract( &V, pV1, pV2 );

	return	gsVector4LengthSq( &V );
}

/*=============================================================================
*
* Purpose : ２つのベクトルの作る角度を求める．
*
* Return  : 角度を返す．
*
*============================================================================*/
GSINLINE GSfloat
gsVector4InnerDegree
(
	const GSvector4*	pV1,			/* ベクトル１                   */
	const GSvector4*	pV2				/* ベクトル２                   */
)
{
	GSfloat				cos_theta;
	GSfloat				angle;

	cos_theta = gsVector4Dot( pV1, pV2 )
			  / ( gsVector4Length( pV1 ) * gsVector4Length( pV2 ) );

	angle = gsAcos( CLAMP( cos_theta, -1.0f, 1.0f ) );

	return	angle;
}

/*=============================================================================
*
* Purpose : ベクトルの座標変換を行う．
*
* Return  : なし．
*
*============================================================================*/
GSINLINE GSvector4*
gsVector4Transform
(
	GSvector4*			pOut,			/* 変換後のベクトル			*/
	const GSvector4*	pV,				/* 変換前のベクトル			*/
	const GSmatrix4*	pM				/* 座標変換行列				*/
) 
{
	GSvector4			v;

	v.x = pV->x * pM->m[0][0]
	    + pV->y * pM->m[1][0]
	    + pV->z * pM->m[2][0]
	    + pV->w * pM->m[3][0];
	v.y = pV->x * pM->m[0][1]
	    + pV->y * pM->m[1][1]
	    + pV->z * pM->m[2][1]
	    + pV->w * pM->m[3][1];
	v.z = pV->x * pM->m[0][2]
	    + pV->y * pM->m[1][2]
	    + pV->z * pM->m[2][2]
	    + pV->w * pM->m[3][2];
	v.w = pV->x * pM->m[0][3]
	    + pV->y * pM->m[1][3]
	    + pV->z * pM->m[2][3]
	    + pV->w * pM->m[3][3];

	*pOut = v;

	return	pOut;
}

/*=============================================================================
*
* Purpose : ベクトルの座標変換を行う．．
*
* Return  : なし．
*
*============================================================================*/
GSINLINE GSvector4*
gsVector4TransformCoord
(
	GSvector4*			pOut,			/* 変換後のベクトル			*/
	const GSvector4*	pV,				/* 変換前のベクトル			*/
	const GSmatrix4*	pM				/* 座標変換行列				*/
) 
{
	GSvector4			v;

	v.x = pV->x * pM->m[0][0]
	    + pV->y * pM->m[1][0]
	    + pV->z * pM->m[2][0]
	    + pV->w * pM->m[3][0];
	v.y = pV->x * pM->m[0][1]
	    + pV->y * pM->m[1][1]
	    + pV->z * pM->m[2][1]
	    + pV->w * pM->m[3][1];
	v.z = pV->x * pM->m[0][2]
	    + pV->y * pM->m[1][2]
	    + pV->z * pM->m[2][2]
	    + pV->w * pM->m[3][2];
	v.w = pV->x * pM->m[0][3]
	    + pV->y * pM->m[1][3]
	    + pV->z * pM->m[2][3]
	    + pV->w * pM->m[3][3];

	gsVector4Scale( pOut, &v, 1.0f / v.w );

	return	pOut;
}

#ifdef __cplusplus
}
#endif

/*----- Ｃ＋＋ラッパークラス ------------------------------------------------*/

#ifdef __cplusplus

/* コンストラクタ */
GSINLINE GSvector4::GSvector4( GSfloat X, GSfloat Y, GSfloat Z, GSfloat W ) :
	x( X ), y( Y ), z( Z ), w( W )
{}

GSINLINE GSvector4::GSvector4( const GSvector2& v2, GSfloat Z, GSfloat W ) :
	x( v2.x ), y( v2.y ), z( Z ), w( W )
{}

GSINLINE GSvector4::GSvector4( const GSvector3& v3, GSfloat W ) :
	x( v3.x ), y( v3.y ), z( v3.z ), w( W )
{}

GSINLINE GSvector4::GSvector4( const GSfloat* p ) :
	x( p[0] ), y( p[1] ), z( p[2] ), w( p[3] )
{}

/* 正規化 */
GSINLINE GSvector4& GSvector4::normalize()
{
	gsVector4Normalize( this, this );

	return	*this;
}

/* 正規化後のベクトルを取得する */
GSINLINE GSvector4 GSvector4::getNormalized() const
{
	GSvector4 result( *this );

	return result.normalize();
}

/* 長さを求める */
GSINLINE GSfloat GSvector4::length() const
{
	return	gsVector4Length( this );
}

/* 長さを求める (２乗和のみ）*/
GSINLINE GSfloat GSvector4::lengthSq() const
{
	return	gsVector4LengthSq( this );
}

/* 距離を求める */
GSINLINE GSfloat GSvector4::distance( const GSvector4& V ) const
{
	return	gsVector4Distance( this, &V );
}

/* 内積を求める */
GSINLINE GSfloat GSvector4::dot( const GSvector4& V ) const
{
	return	gsVector4Dot( this, &V );
}

/* 小さい要素を求める */
GSINLINE GSvector4 GSvector4::minimize( const GSvector4& V ) const
{
	GSvector4	result;

	gsVector4Minimize( &result, this, &V );

	return result;
}

/* 大きい要素を求める */
GSINLINE GSvector4 GSvector4::maximize( const GSvector4& V ) const
{
	GSvector4	result;

	gsVector4Maximize( &result, this, &V );

	return result;
}

/* クランプする */
GSINLINE GSvector4 GSvector4::clamp( const GSvector4& l, const GSvector4& h ) const
{
	GSvector4	result;

	gsVector4Maximize(&result, &l, gsVector4Minimize(&result, this, &h));

	return result;
}


/* 線形補間を行う */
GSINLINE GSvector4 GSvector4::lerp( const GSvector4& v4, GSfloat n ) const
{
	GSvector4	result;

	gsVector4Lerp( &result, this, &v4, n );

	return result;
}

/* ２次補間を行う */
GSINLINE GSvector4 GSvector4::quadratic( const GSvector4& V2, const GSvector4& V3, GSfloat n ) const
{
	GSvector4	result;

	gsVcector3Quadratic( &result, this, &V2, &V3, n );

	return result;
}

// キャスト演算子オーバーロード
GSINLINE GSvector4::operator GSfloat* () const
{
	return (float*) this;
}

GSINLINE GSvector4::operator const GSfloat* () const
{
	return (const GSfloat*) this;
}

// 単項演算子オーバーロード
GSINLINE GSvector4	GSvector4::operator + () const
{
	return	*this;
}

GSINLINE GSvector4	GSvector4::operator - () const
{
	return	GSvector4( -x, -y, -z, -w );
}

// 代入演算子オーバーロード
GSINLINE GSvector4&	GSvector4::operator += ( const GSvector4& V )
{
	gsVector4Add( this, this, &V );

	return	*this;
}

GSINLINE GSvector4&	GSvector4::operator -= ( const GSvector4& V )
{
	gsVector4Subtract( this, this, &V );

	return	*this;
}

GSINLINE GSvector4&	GSvector4::operator *= ( const GSvector4& V )
{
	gsVector4Multiply( this, this, &V );

	return	*this;
}

GSINLINE GSvector4&	GSvector4::operator /= ( const GSvector4& V )
{
	gsVector4Divide( this, this, &V );

	return	*this;
}

GSINLINE GSvector4&	GSvector4::operator *= ( GSfloat S )
{
	gsVector4Scale( this, this, S );

	return	*this;
}

GSINLINE GSvector4&	GSvector4::operator /= ( GSfloat S )
{
	gsVector4Scale( this, this, 1.0f / S );

	return	*this;
}

// ２項演算子オーバーロード
GSINLINE const GSvector4 operator + ( const GSvector4& v1, const GSvector4& v2 )
{
	GSvector4	result;

	gsVector4Add( &result, &v1, &v2 );

	return	result;
}

GSINLINE const GSvector4 operator - ( const GSvector4& v1, const GSvector4& v2 )
{
	GSvector4	result;

	gsVector4Subtract( &result, &v1, &v2 );

	return	result;
}

GSINLINE const GSvector4 operator * ( const GSvector4& v1, const GSvector4 v2 )
{
	GSvector4	result;

	gsVector4Multiply( &result, &v1, &v2 );

	return	result;
}

GSINLINE const GSvector4 operator * ( const GSvector4& v, GSfloat s )
{
	GSvector4	result;

	gsVector4Scale( &result, &v, s );

	return	result;
}

GSINLINE const GSvector4 operator * ( GSfloat s, const GSvector4& v )
{
	return	v * s;
}

GSINLINE const GSvector4 operator / ( const GSvector4& v, GSfloat s )
{
	GSvector4	result;

	gsVector4Scale( &result, &v, 1.0f / s );

	return	result;
}

GSINLINE const GSvector4 operator * ( const GSmatrix4& M, const GSvector4& v )
{
	GSfloat x = v.x * M.m[0][0] + v.y * M.m[1][0] + v.z * M.m[2][0] + v.w * M.m[3][0];
	GSfloat y = v.x * M.m[0][1] + v.y * M.m[1][1] + v.z * M.m[2][1] + v.w * M.m[3][1];
	GSfloat z = v.x * M.m[0][2] + v.y * M.m[1][2] + v.z * M.m[2][2] + v.w * M.m[3][2];
	GSfloat w = v.x * M.m[0][3] + v.y * M.m[1][3] + v.z * M.m[2][3] + v.w * M.m[3][3];

	return	GSvector4( x / w, y / w, z / w, w );
}

GSINLINE const GSvector4 operator * ( const GSvector4& v, const GSmatrix4& M )
{
	GSfloat x = v.x * M.m[0][0] + v.y * M.m[0][1] + v.z * M.m[0][2] + v.w * M.m[0][3];
	GSfloat y = v.x * M.m[1][0] + v.y * M.m[1][1] + v.z * M.m[1][2] + v.w * M.m[1][3];
	GSfloat z = v.x * M.m[2][0] + v.y * M.m[2][1] + v.z * M.m[2][2] + v.w * M.m[2][3];
	GSfloat w = v.x * M.m[3][0] + v.y * M.m[3][1] + v.z * M.m[3][2] + v.w * M.m[3][3];

	return	GSvector4( x / w, y / w, z / w, w );
}

GSINLINE bool operator == ( const GSvector4& v1, const GSvector4& v2 )
{
    return	( v1.x == v2.x && v1.y == v2.y && v1.z == v2.z && v1.z == v2.w );
}

GSINLINE bool operator != ( const GSvector4& v1, const GSvector4& v2 )
{
	return	!( v1 == v2 );
}

GSINLINE bool operator < ( const GSvector4& v1, const GSvector4& v2 )
{
	if ( v1.x != v2.x ) return v1.x < v2.x;
	if ( v1.y != v2.y ) return v1.y < v2.y;
	if ( v1.z != v2.z ) return v1.z < v2.z;
	return v1.w < v2.w;
}

GSINLINE bool operator > ( const GSvector4& v1, const GSvector4& v2 )
{
	if ( v1.x != v2.x ) return v1.x > v2.x;
	if ( v1.y != v2.y ) return v1.y > v2.y;
	if ( v1.z != v2.z ) return v1.z > v2.z;
	return v1.w > v2.w;
}

GSINLINE bool operator <= ( const GSvector4& v1, const GSvector4& v2 )
{
	return	!( v1 > v2 );
}

GSINLINE bool operator >= ( const GSvector4& v1, const GSvector4& v2 )
{
	return	!( v1 < v2 );
}

#endif

#endif

/********** End of File ******************************************************/
