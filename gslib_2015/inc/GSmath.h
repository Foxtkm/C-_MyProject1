/******************************************************************************
* File Name : GSmath.h                              Ver : 1.00  Date : 98-10-22
*
* Description :
*
*       ゲームシステム 基本計算 定義ヘッダファイル．
*
* Author : Moriharu Ohzu.
*
******************************************************************************/
#ifndef		_GSMATH_H_
#define		_GSMATH_H_

#include	"GStype.h"
#include	<math.h>
#include	<stdio.h>
#include	<time.h>

/****** 定数宣言 *************************************************************/

#define	GS_PI	3.14159265358979323846264f	/* 円周率               */

/****** マクロ関数宣言 *******************************************************/

/* ａを四捨五入して整数型に変換する */
#define	ROUND( a )			(int)( ( (a) > 0 ) ? ( (a)+0.5F ): ( (a)-0.5F ) )

/* ａの絶対値を求める */
#define	ABS( a )			( ( (a) < 0 ) ? -(a) : (a) )

/* ａ，ｂの小さい方の値を求める */
#define	MIN( a, b )			( ( (a) < (b) ) ? (a) : (b) )

/* ａ，ｂ大きい方の値を求める */
#define	MAX( a , b )		( ( (a) > (b) ) ? (a) : (b) )

/* ａが正なら１，負なら－１，ゼロなら０を求める */
#define	ZSGN( a )			( ( (a) < 0 ) ? -1 : (a) > 0 ? 1 : 0 )

/* ａが正またはゼロなら１，負なら－１を求める */
#define	SGN( a )			( ( (a) < 0 ) ? -1 : 1 )

/* ａを二乗する */
#define	SQR( a )			( (a) * (a) )

/* ｖをｌからｈの値に留める */
#define	CLAMP( v, l, h )	MAX( l, MIN( v, h ) )

/* ｌからｈまでを線形補間する （ ａは 0.0 から 1.0 ) */
#define	LERP( a, l, h )		( (l) + ( ( (h) - (l) ) * (a) ) )

/* ｖ１，ｖ２，ｖ３を２次補間する （ ａは 0.0 から 1.0 ) */
#define	QUADRATIC( a, v1, v2, v3 )										\
(																		\
	(v1)*(1.0f-(a))*(1.0f-(a)) + 2*(v2)*(a)*(1.0f-(a)) + (v3)*(a)*(a)	\
)

/* 配列の要素数を求める */
#define	ARRAY_LENGTH( a )	( sizeof( a ) / sizeof( a[0] ) )

/* ラジアンを角度に変換 */
#define	RAD_TO_DEG( rad )	( (rad) * ( 1.0f / GS_PI * 180.0f ) )

/* 角度をラジアンに変換 */
#define	DEG_TO_RAD( deg )	( (deg) * ( 1.0f / 180.0f * GS_PI ) )

/* 角度の正規化をする */
#define	gsDegreeNormalize( x )												\
(																			\
	( (x) >= 0.0f ) ? (GSfloat)fmod( x, 360.0f ): (GSfloat)fmod( x, 360.0f ) + 360.0f \
)

/* 三角関数関係 */
#define	gsCos( x )		( (GSfloat)cos( DEG_TO_RAD( x ) ) )
#define	gsSin( x )		( (GSfloat)sin( DEG_TO_RAD( x ) ) )
#define	gsTan( x )		( (GSfloat)tan( DEG_TO_RAD( x ) ) )
#define	gsAcos( x )		( (GSfloat)RAD_TO_DEG( acos( x ) ) )
#define	gsAsin( x )		( (GSfloat)RAD_TO_DEG( asin( x ) ) )
#define	gsAtan( x, y )	( (GSfloat)gsDegreeNormalize( RAD_TO_DEG( atan2( y, x ) ) ) )

/* 平方根を求める */
#define	gsSqrt( x )		( (GSfloat)sqrt( x ) )

/* 乱数の初期化を行う */
#define	gsRandamize()	( srand( (unsigned int)time(NULL) ), srand( rand() ) )

/* 浮動小数点の乱数値を求める*/
#define	gsRandf( min, max )										\
(																\
	( (float)rand() / RAND_MAX ) * ( (max) - (min) ) + (min)	\
)

/* 乱数値を求める*/
#define	gsRand( min, max )										\
(																\
	( rand() % (int)( (max) - (min) + 1 ) ) + (int)(min)		\
)

#endif

/********** End of File ******************************************************/
