/******************************************************************************
* File Name : GSvector2.h                         Ver : 1.00  Date : 2001-01-16
*
* Description :
*
*       ２Ｄベクトル計算 ヘッダファイル．
*
* Author : Moriharu Ohzu.
*
******************************************************************************/
#ifndef		_GSVECTOR2_H_
#define		_GSVECTOR2_H_

#include	"GStype.h"
#include	"GSmath.h"

/****** 関数プロトタイプ宣言 *************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*=============================================================================
*
* Purpose : ２Ｄベクトルを初期化．
*
* Return  : 初期化したベクトルを返す．
*
*============================================================================*/
GSINLINE GSvector2*
gsVector2Init
(
	GSvector2*		pOut,				/* -> 結果				*/
	GSfloat			x,					/* ｘ座標				*/
	GSfloat			y					/* ｙ座標				*/
)
{
	pOut->x = x;
	pOut->y = y;

	return	pOut;
}

/*=============================================================================
*
* Purpose : ２Ｄベクトルの加算．
*
* Return  : 加算後のベクトルを返す．
*
*============================================================================*/
GSINLINE GSvector2*
gsVector2Add
(
	GSvector2*			pOut,			/* -> 結果				*/
	const GSvector2*	pV1,			/* ２Ｄベクトル			*/
	const GSvector2*	pV2				/* ２Ｄベクトル			*/
)
{
	pOut->x = pV1->x + pV2->x;
	pOut->y = pV1->y + pV2->y;

	return	pOut;
}

/*=============================================================================
*
* Purpose : ベクトルの減算．
*
* Return  : 減算後のベクトルを返す．
*
*============================================================================*/
GSINLINE GSvector2*
gsVector2Subtract
(
	GSvector2*			pOut,			/* -> 結果				*/
	const GSvector2*	pV1,			/* ２Ｄベクトル			*/
	const GSvector2*	pV2				/* ２Ｄベクトル			*/
)
{
	pOut->x = pV1->x - pV2->x;
	pOut->y = pV1->y - pV2->y;

	return	pOut;
}

/*=============================================================================
*
* Purpose : ベクトルの乗算．
*
* Return  : 乗算後のベクトルを返す．
*
*============================================================================*/
GSINLINE GSvector2*
gsVector2Multiply
(
	GSvector2*			pOut,			/* -> 結果				*/
	const GSvector2*	pV1,			/* ２Ｄベクトル			*/
	const GSvector2*	pV2				/* ２Ｄベクトル			*/
)
{
	pOut->x = pV1->x * pV2->x;
	pOut->y = pV1->y * pV2->y;

	return	pOut;
}

/*=============================================================================
*
* Purpose : ベクトルの除算．
*
* Return  : 除算のベクトルを返す．
*
*============================================================================*/
GSINLINE GSvector2*
gsVector2Divide
(
	GSvector2*			pOut,			/* -> 結果				*/
	const GSvector2*	pV1,			/* ２Ｄベクトル			*/
	const GSvector2*	pV2				/* ２Ｄベクトル			*/
)
{
	pOut->x = pV1->x / pV2->x;
	pOut->y = pV1->y / pV2->y;

	return	pOut;
}

/*=============================================================================
*
* Purpose : ベクトルの外積を求める．
*
* Return  : なし．
*
*============================================================================*/
GSINLINE GSfloat
gsVector2CCW
(
	const GSvector2*	pV1,			/* ２Ｄベクトル			*/
	const GSvector2*	pV2				/* ２Ｄベクトル			*/
)
{
	return	pV1->x * pV2->y - pV1->y * pV2->x;
}

/*=============================================================================
*
* Purpose : ベクトルの内積をもとめる．
*
* Return  : 内積を返す．
*
*============================================================================*/
GSINLINE GSfloat
gsVector2Dot
(
	const GSvector2*	pV1,			/* ２Ｄベクトル			*/
	const GSvector2*	pV2				/* ２Ｄベクトル			*/
)
{
	return pV1->x * pV2->x + pV1->y * pV2->y;
}

/*=============================================================================
*
* Purpose : ベクトルの長さを求める．（ 二乗和のみで平方根をとらない ）
*
* Return  : ベクトルの長さを返す．
*
*============================================================================*/
GSINLINE GSfloat
gsVector2LengthSq
(
	const GSvector2*		pV			/* ２Ｄベクトル			*/
)
{
	return pV->x * pV->x + pV->y * pV->y;
}

/*=============================================================================
*
* Purpose : ベクトルの長さを求める．
*
* Return  : ベクトルの長さを返す．
*
*============================================================================*/
GSINLINE GSfloat
gsVector2Length
(
	const GSvector2*		pV			/* ２Ｄベクトル			*/
)
{
	return (GSfloat)gsSqrt( gsVector2LengthSq( pV ) );
}

/*=============================================================================
*
* Purpose : ベクトルの線形補間．
*
* Return  : なし．
*
*============================================================================*/
GSINLINE GSvector2*
gsVector2Lerp
(
	GSvector2*			pOut,			/* -> 結果				*/
	const GSvector2*	pMin,			/* ２Ｄベクトル			*/
	const GSvector2*	pMax,			/* ２Ｄベクトル			*/
	GSfloat				t				/* 補間値（0.0～1.0)	*/
)
{
	pOut->x = LERP( t, pMin->x, pMax->x );
	pOut->y = LERP( t, pMin->y, pMax->y );

	return	pOut;
}

/*=============================================================================
*
* Purpose : ベクトルの２次補間．
*
* Return  : なし．
*
*============================================================================*/
GSINLINE GSvector2*
gsVector2Quadratic
(
	GSvector2*			pOut,			/* -> 結果				*/
	const GSvector2*	pV1,			/* ２Ｄベクトル			*/
	const GSvector2*	pV2,			/* ２Ｄベクトル			*/
	const GSvector2*	pV3,			/* ２Ｄベクトル			*/
	GSfloat				t				/* 補間値（0.0～1.0)	*/
)
{
	pOut->x = QUADRATIC( t, pV1->x, pV2->x, pV3->x );
	pOut->y = QUADRATIC( t, pV1->y, pV2->y, pV3->y );

	return	pOut;
}

/*=============================================================================
*
* Purpose : ２つのベクトルの各要素の最大値から作成されたベクトルを求める．
*
* Return  : 各要素の最大値を求めたベクトルを返す．
*
*============================================================================*/
GSINLINE GSvector2*
gsVector2Maximize
(
	GSvector2*			pOut,			/* -> 結果				*/
	const GSvector2*	pV1,			/* ２Ｄベクトル			*/
	const GSvector2*	pV2				/* ２Ｄベクトル			*/
)
{
	pOut->x = ( pV1->x > pV2->x ) ? pV1->x : pV2->x;
	pOut->y = ( pV1->y > pV2->y ) ? pV1->y : pV2->y;

	return	pOut;
}

/*=============================================================================
*
* Purpose : ２つのベクトルの各要素の最小値から作成されたベクトルを求める．
*
* Return  : 各要素の最小値を求めたベクトルを返す．
*
*============================================================================*/
GSINLINE GSvector2*
gsVector2Minimize
(
	GSvector2*			pOut,			/* -> 結果				*/
	const GSvector2*	pV1,			/* ２Ｄベクトル			*/
	const GSvector2*	pV2				/* ２Ｄベクトル			*/
)
{
	pOut->x = ( pV1->x < pV2->x ) ? pV1->x : pV2->x;
	pOut->y = ( pV1->y < pV2->y ) ? pV1->y : pV2->y;

	return	pOut;
}

/*=============================================================================
*
* Purpose : ベクトルのスケーリング．
*
* Return  : なし．
*
*============================================================================*/
GSINLINE GSvector2*
gsVector2Scale
(
	GSvector2*			pOut,			/* -> 結果				*/
	const GSvector2*	pV,				/* ２Ｄベクトル			*/
	GSfloat				s				/* スカラー				*/
)
{
	pOut->x = pV->x * s;
	pOut->y = pV->y * s;

	return	pOut;
}

/*=============================================================================
*
* Purpose : 逆ベクトルを求める．
*
* Return  : なし．
*
*============================================================================*/
GSINLINE GSvector2*
gsVector2Inverse
(
	GSvector2*			pOut,			/* -> 結果				*/
	const GSvector2*	pV				/* ２Ｄベクトル			*/
)
{
	pOut->x = -pV->x;
	pOut->y = -pV->y;

	return	pOut;
}

/*=============================================================================
*
* Purpose : 直線の方程式の計算を行う．
*
* Return  : なし．
*
*============================================================================*/
GSINLINE GSvector2*
gsVector2LinearEquation
(
	GSvector2*			pOut,			/* -> 結果				*/
	const GSvector2*	pP,				/* ２Ｄベクトル			*/
	const GSvector2*	pV,				/* ２Ｄベクトル			*/
	GSfloat				t				/* 係数					*/
)
{
	pOut->x = pV->x * t + pP->x;
	pOut->y = pV->y * t + pP->y;

	return	pOut;
}

/*=============================================================================
*
* Purpose : 方位角からベクトルを求める．
*
* Return  : なし．
*
*============================================================================*/
GSINLINE GSvector2*
gsVector2FromDirection
(
	GSvector2*		pOut,				/* -> 結果			*/
	GSfloat			Direction			/* 方位角			*/
)
{
	pOut->x = gsCos( Direction );
	pOut->y = gsSin( Direction );

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
gsVector2Normalize
(
	GSvector2*			pOut,			/*->単位ベクトル                */
	const GSvector2*	pV				/* ベクトル                     */
)
{
	GSfloat				len;

	if ( ( len = gsVector2Length( pV ) ) != 0 )
	{
		gsVector2Scale( pOut, pV, 1.0f / len  );
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
gsVector2Distance
(
	const GSvector2*	pV1,			/* 始点                         */
	const GSvector2*	pV2				/* 終点                         */
)
{
	GSvector2			V;

	gsVector2Subtract( &V, pV1, pV2 );

	return	gsVector2Length( &V );
}

/*=============================================================================
*
* Purpose : ２点間の距離を求める．（ 二乗和だけで平方根を取らない ）
*
* Return  : 距離を返す．
*
*============================================================================*/
GSINLINE GSfloat
gsVector2DistanceSq
(
	const GSvector2*	pV1,			/* 始点                         */
	const GSvector2*	pV2				/* 終点                         */
)
{
	GSvector2			V;

	gsVector2Subtract( &V, pV1, pV2 );

	return	gsVector2LengthSq( &V );
}

/*=============================================================================
*
* Purpose : ２つのベクトルの作る角度を求める．
*
* Return  : 角度を返す．
*
*============================================================================*/
GSINLINE GSfloat
gsVector2InnerDegree
(
	const GSvector2*	pV1,			/* ベクトル１                   */
	const GSvector2*	pV2				/* ベクトル２                   */
)
{
	GSfloat				cos_theta;
	GSfloat				angle;

	cos_theta = gsVector2Dot( pV1, pV2 )
			  / ( gsVector2Length( pV1 ) * gsVector2Length( pV2 ) );

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
GSINLINE GSvector2*
gsVector2Transform
(
	GSvector2*			pOut,			/* 変換後のベクトル			*/
	const GSvector2*	pV,				/* 変換前のベクトル			*/
	const GSmatrix3*	pM				/* 座標変換行列				*/
) 
{
	GSvector2			v;

	v.x = pV->x * pM->m[0][0]
	    + pV->y * pM->m[1][0]
	    +         pM->m[2][0];
	v.y = pV->x * pM->m[0][1]
	    + pV->y * pM->m[1][1]
	    +         pM->m[2][1];

	*pOut = v;

	return	pOut;
}

/*=============================================================================
*
* Purpose : 法線ベクトルの座標変換を行う．
*
* Return  : なし．
*
*============================================================================*/
GSINLINE GSvector2*
gsVector2TransformNormal
(
	GSvector2*			pOut,			/* 変換後の法線ベクトル		*/
	const GSvector2*	pV,				/* 変換前の法線ベクトル		*/
	const GSmatrix3*	pM				/* 座標変換行列				*/
) 
{
	GSvector2			v;

	v.x = pV->x * pM->m[0][0]
	    + pV->y * pM->m[1][0];
	v.y = pV->x * pM->m[0][1]
	    + pV->y * pM->m[1][1];

	*pOut = v;

	return	pOut;
}

/*=============================================================================
*
* Purpose : 座標変換を行う．（３Ｄ変換行列を使用する）
*
* Return  : なし．
*
*============================================================================*/
GSINLINE GSvector2*
gsVector2TransformCoord
(
	GSvector2*			pOut,			/* 変換後の法線ベクトル		*/
	const GSvector2*	pV,				/* 変換前の法線ベクトル		*/
	const GSmatrix4*	pM				/* 座標変換行列				*/
) 
{
	GSvector2			v;
	GSfloat				w;

	v.x = pV->x * pM->m[0][0]
	    + pV->y * pM->m[1][0]
	    +         pM->m[3][0];
	v.y = pV->x * pM->m[0][1]
	    + pV->y * pM->m[1][1]
	    +         pM->m[3][1];
	w   = pV->x * pM->m[0][3]
	    + pV->y * pM->m[1][3]
	    +         pM->m[3][3];

	gsVector2Scale( &v, &v, 1.0f / w );

	*pOut = v;

	return	pOut;
}

/*=============================================================================
*
* Purpose : 反射ベクトルを求める．
*
* Return  : なし．
*
*============================================================================*/
GSINLINE GSvector2*
gsVector2Reflection
(
	GSvector2*			pOut,			/*-> 反射ベクトル		*/
	const GSvector2*	pI,				/* 入射ベクトル			*/
	const GSvector2*	pN				/* 法線ベクトル			*/
)
{
	GSvector2			v;
	GSvector2			t;

	/* Out = 2 * ( -I dot N ) * N + I */
	gsVector2Inverse( &v, pI );
	gsVector2Scale( &t, pN, gsVector2Dot( &v, pN ) * 2.0f );
	gsVector2Add( pOut, &t, pI );

	return	pOut;
}

/*=============================================================================
*
* Purpose : 屈折ベクトルを求める．
*
* Return  : なし．
*
*============================================================================*/
GSINLINE GSvector2*
gsVector2Refraction
(
	GSvector2*			pOut,			/*-> 反射ベクトル		*/
	const GSvector2*	pI,				/* 入射ベクトル			*/
	const GSvector2*	pN,				/* 法線ベクトル			*/
	GSfloat				Eta				/* 屈折率比				*/
)
{
	GSvector2			T1, T2;
	GSfloat				cosI;
	GSfloat				cosT2;

	gsVector2Inverse( &T1, pI );
	cosI  = gsVector2Dot( &T1, pN );
	cosT2 = 1.0f - Eta * Eta * ( 1.0f - cosI * cosI );

	if ( cosT2 <= 0.0f )
	{
		gsVector2Init( pOut, 0.0f, 0.0f );
	}
	else
	{
		gsVector2Scale( &T1, pI, Eta );
		gsVector2Scale( &T2, pN, Eta * cosI - gsSqrt( ABS( cosT2 ) ) );
		gsVector2Add( pOut, &T1, &T2 );
	}

	return	pOut;
}

/*=============================================================================
*
* Purpose : 方位角を求める．
*
* Return  : なし．
*
*============================================================================*/
GSINLINE GSfloat
gsVector2GetDirection
(
	const GSvector2*	pV				/* ベクトル				*/
)
{
	GSfloat				x, y;
	GSfloat				l;

	/* ベクトルの長さを求める */
	if ( ( l = gsVector2Length( pV ) ) == 0 )
	{
		/* 長さが０の場合は０にする */
		return 0.0f;
	}

	/* ベクトルを正規化する */
	x = pV->x / l;
	y = pV->y / l;

	/* 方角の計算を行う */
	return	gsAtan( x, y );
}

#ifdef __cplusplus
}
#endif

/*----- Ｃ＋＋ラッパークラス ------------------------------------------------*/

#ifdef __cplusplus

GSINLINE GSvector2::GSvector2( GSfloat X, GSfloat Y ) :
	x( X ), y( Y )
{}

GSINLINE GSvector2::GSvector2( const GSvector3& v3 ) :
	x( v3.x ), y( v3.y )
{}

GSINLINE GSvector2::GSvector2( const GSvector4& v4 ) :
	x( v4.x ), y( v4.y )
{}

GSINLINE GSvector2::GSvector2( const GSfloat* p ) :
	x( p[0] ), y( p[1] )
{}

/* 正規化 */
GSINLINE GSvector2& GSvector2::normalize()
{
	gsVector2Normalize( this, this );

	return	*this;
}

/* 正規化後のベクトルを取得する */
GSINLINE GSvector2 GSvector2::getNormalized() const
{
	GSvector2 result( *this );

	return	result.normalize();
}

/* 長さを求める */
GSINLINE GSfloat GSvector2::length() const
{
	return	gsVector2Length( this );
}

/* 長さを求める (２乗和のみ）*/
GSINLINE GSfloat GSvector2::lengthSq() const
{
	return	gsVector2LengthSq( this );
}

/* 距離を求める */
GSINLINE GSfloat GSvector2::distance( const GSvector2& V ) const
{
	return	gsVector2Distance( this, &V );
}

/* 内積を求める */
GSINLINE GSfloat GSvector2::dot( const GSvector2& V ) const
{
	return	gsVector2Dot( this, &V );
}

/* 外積を求める */
GSINLINE GSfloat GSvector2::CCW( const GSvector2& V ) const
{
	return	gsVector2CCW( this, &V );
}

/* 小さい要素を求める */
GSINLINE GSvector2 GSvector2::minimize( const GSvector2& V ) const
{
	GSvector2	result;

	gsVector2Minimize( &result, this, &V );

	return result;
}

/* 大きい要素を求める */
GSINLINE GSvector2 GSvector2::maximize( const GSvector2& V ) const
{
	GSvector2	result;

	gsVector2Maximize( &result, this, &V );

	return result;
}

/* クランプする */
GSINLINE GSvector2 GSvector2::clamp( const GSvector2& l, const GSvector2& h ) const
{
	GSvector2	result;

	gsVector2Maximize(&result, &l, gsVector2Minimize(&result, this, &h));

	return result;
}


/* 線形補間を行う */
GSINLINE GSvector2 GSvector2::lerp( const GSvector2& V, GSfloat n ) const
{
	GSvector2	result;

	gsVector2Lerp( &result, this, &V, n );

	return result;
}

/* ２次補間を行う */
GSINLINE GSvector2 GSvector2::quadratic( const GSvector2& V2, const GSvector2& V3, GSfloat n ) const
{
	GSvector2	result;

	gsVector2Quadratic( &result, this, &V2, &V3, n );

	return result;
}

/* 反射ベクトルを求める */
GSINLINE GSvector2 GSvector2::reflect( const GSvector2& N ) const
{
	GSvector2	result;

	gsVector2Reflection( &result, this, &N );

	return result;
}

/* 屈折ベクトルを求める */
GSINLINE GSvector2 GSvector2::refract( const GSvector2& N, GSfloat Eta ) const
{
	GSvector2	result;

	gsVector2Refraction( &result, this, &N, Eta );

	return result;
}

/* ２つベクトルのなす角度を求める */
GSINLINE GSfloat GSvector2::degree( const GSvector2& V ) const
{
	return gsVector2InnerDegree( this, &V );
}

/* 方位角を求める */
GSINLINE GSfloat GSvector2::getDirection() const
{
	return	gsVector2GetDirection( this );
}

/* 方位角からベクトルを求める */
GSINLINE GSvector2& GSvector2::fromDirection( GSfloat direction )
{
	gsVector2FromDirection( this, direction );

	return	*this;
}

/* 方位角からベクトルを求める */
GSINLINE GSvector2 GSvector2::createFromDirection(GSfloat direction)
{
	GSvector2 result;

	gsVector2FromDirection(&result, direction);

	return	result;
}

/* ２つのベクトルがなす角度を求める */
GSINLINE GSfloat GSvector2::innerDegree(const GSvector2& other) const
{
	return	gsVector2InnerDegree(this, &other);
}

// キャスト演算子オーバーロード
GSINLINE GSvector2::operator GSfloat* () const
{
	return (float*) this;
}

GSINLINE GSvector2::operator const GSfloat* () const
{
	return (const GSfloat*) this;
}

// 単項演算子オーバーロード
GSINLINE GSvector2	GSvector2::operator + () const
{
	return	*this;
}

GSINLINE GSvector2	GSvector2::operator - () const
{
	return	GSvector2( -x, -y );
}

// 代入演算子オーバーロード
GSINLINE GSvector2&	GSvector2::operator += ( const GSvector2& V )
{
	gsVector2Add( this, this, &V );

	return	*this;
}

GSINLINE GSvector2&	GSvector2::operator -= ( const GSvector2& V )
{
	gsVector2Subtract( this, this, &V );

	return	*this;
}

GSINLINE GSvector2&	GSvector2::operator *= ( const GSvector2& V )
{
	gsVector2Multiply( this, this, &V );

	return	*this;
}

GSINLINE GSvector2&	GSvector2::operator /= ( const GSvector2& V )
{
	gsVector2Divide( this, this, &V );

	return	*this;
}

GSINLINE GSvector2&	GSvector2::operator *= ( GSfloat S )
{
	gsVector2Scale( this, this, S );

	return	*this;
}

GSINLINE GSvector2&	GSvector2::operator /= ( GSfloat S )
{
	gsVector2Scale( this, this, 1.0f / S );

	return	*this;
}

// ２項演算子オーバーロード
GSINLINE const GSvector2 operator + ( const GSvector2& v1, const GSvector2& v2 )
{
	GSvector2	result;

	gsVector2Add( &result, &v1, &v2 );

	return	result;
}

GSINLINE const GSvector2 operator - ( const GSvector2& v1, const GSvector2& v2 ) 
{
	GSvector2	result;

	gsVector2Subtract( &result, &v1, &v2 );

	return	result;
}

GSINLINE const GSvector2 operator * ( const GSvector2& v1, const GSvector2 v2 )
{
	GSvector2	result;

	gsVector2Multiply( &result, &v1, &v2 );

	return	result;
}

GSINLINE const GSvector2 operator * ( const GSvector2& v, GSfloat s )
{
	GSvector2	result;

	gsVector2Scale( &result, &v, s );

	return	result;
}

GSINLINE const GSvector2 operator * ( GSfloat s, const GSvector2& v )
{
	return	v * s;
}

GSINLINE const GSvector2 operator / ( const GSvector2& v, GSfloat s )
{
	GSvector2	result;

	gsVector2Scale( &result, &v, 1.0f / s );

	return	result;
}

GSINLINE const GSvector2 operator * ( const GSmatrix3& M, const GSvector2& v )
{
	return	GSvector2( v.x * M.m[0][0] + v.y * M.m[1][0] + M.m[2][0],
					   v.x * M.m[0][1] + v.y * M.m[1][1] + M.m[2][1] );
}

GSINLINE const GSvector2 operator * ( const GSvector2& v, const GSmatrix3& M )
{
	return GSvector2( v.x * M.m[0][0] + v.y * M.m[0][1] + M.m[0][2],
					  v.x * M.m[1][0] + v.y * M.m[1][1] + M.m[1][2] );
}

GSINLINE const GSvector2 operator * ( const GSmatrix4& M, const GSvector2& v )
{
	GSfloat x = v.x * M.m[0][0] + v.y * M.m[1][0] + M.m[3][0];
	GSfloat y = v.x * M.m[0][1] + v.y * M.m[1][1] + M.m[3][1];
	GSfloat w = v.x * M.m[0][3] + v.y * M.m[1][3] + M.m[3][3];

	return	GSvector2( x / w, y / w );
}

GSINLINE const GSvector2 operator * ( const GSvector2& v, const GSmatrix4& M )
{
	GSfloat x = v.x * M.m[0][0] + v.y * M.m[0][1] + M.m[0][3];
	GSfloat y = v.x * M.m[1][0] + v.y * M.m[1][1] + M.m[1][3];
	GSfloat w = v.x * M.m[3][0] + v.y * M.m[3][1] + M.m[3][3];

	return	GSvector2( x / w, y / w );
}

GSINLINE bool operator == ( const GSvector2& v1, const GSvector2& v2 )
{
    return	( v1.x == v2.x && v1.y == v2.y );
}

GSINLINE bool operator != ( const GSvector2& v1, const GSvector2& v2 )
{
	return	!( v1 == v2 );
}

GSINLINE bool operator < ( const GSvector2& v1, const GSvector2& v2 )
{
	if ( v1.x != v2.x ) return v1.x < v2.x;
	return v1.y < v2.y;
}

GSINLINE bool operator > ( const GSvector2& v1, const GSvector2& v2 )
{
	if ( v1.x != v2.x ) return v1.x > v2.x;
	return v1.y > v2.y;
}

GSINLINE bool operator <= ( const GSvector2& v1, const GSvector2& v2 )
{
	return	!( v1 > v2 );
}

GSINLINE bool operator >= ( const GSvector2& v1, const GSvector2& v2 )
{
	return	!( v1 < v2 );
}

#endif


#endif

/********** End of File ******************************************************/
