/******************************************************************************
* File Name : GSvector3.h                         Ver : 1.00  Date : 2001-01-16
*
* Description :
*
*       ３Ｄベクトル計算 ヘッダファイル．
*
* Author : Moriharu Ohzu.
*
******************************************************************************/
#ifndef		_GSVECTOR3_H_
#define		_GSVECTOR3_H_

#include	"GStype.h"
#include	"GSmath.h"

/****** 関数プロトタイプ宣言 *************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*=============================================================================
*
* Purpose : ３Ｄベクトルを初期化．
*
* Return  : 初期化したベクトルを返す．
*
*============================================================================*/
GSINLINE GSvector3*
gsVector3Init
(
	GSvector3*		pOut,				/* -> 結果				*/
	GSfloat			x,					/* ｘ座標				*/
	GSfloat			y,					/* ｙ座標				*/
	GSfloat			z					/* ｚ座標				*/
)
{
	pOut->x = x;
	pOut->y = y;
	pOut->z = z;

	return	pOut;
}

/*=============================================================================
*
* Purpose : ３Ｄベクトルの加算．
*
* Return  : 加算後のベクトルを返す．
*
*============================================================================*/
GSINLINE GSvector3*
gsVector3Add
(
	GSvector3*			pOut,			/* -> 結果				*/
	const GSvector3*	pV1,			/* ３Ｄベクトル			*/
	const GSvector3*	pV2				/* ３Ｄベクトル			*/
)
{
	pOut->x = pV1->x + pV2->x;
	pOut->y = pV1->y + pV2->y;
	pOut->z = pV1->z + pV2->z;

	return	pOut;
}

/*=============================================================================
*
* Purpose : ベクトルの減算．
*
* Return  : 減算後のベクトルを返す．
*
*============================================================================*/
GSINLINE GSvector3*
gsVector3Subtract
(
	GSvector3*			pOut,			/* -> 結果				*/
	const GSvector3*	pV1,			/* ３Ｄベクトル			*/
	const GSvector3*	pV2				/* ３Ｄベクトル			*/
)
{
	pOut->x = pV1->x - pV2->x;
	pOut->y = pV1->y - pV2->y;
	pOut->z = pV1->z - pV2->z;

	return	pOut;
}

/*=============================================================================
*
* Purpose : ベクトルの乗算．
*
* Return  : 乗算後のベクトルを返す．
*
*============================================================================*/
GSINLINE GSvector3*
gsVector3Multiply
(
	GSvector3*			pOut,			/* -> 結果				*/
	const GSvector3*	pV1,			/* ３Ｄベクトル			*/
	const GSvector3*	pV2				/* ３Ｄベクトル			*/
)
{
	pOut->x = pV1->x * pV2->x;
	pOut->y = pV1->y * pV2->y;
	pOut->z = pV1->z * pV2->z;

	return	pOut;
}

/*=============================================================================
*
* Purpose : ベクトルの除算．
*
* Return  : 除算のベクトルを返す．
*
*============================================================================*/
GSINLINE GSvector3*
gsVector3Divide
(
	GSvector3*			pOut,			/* -> 結果				*/
	const GSvector3*	pV1,			/* ３Ｄベクトル			*/
	const GSvector3*	pV2				/* ３Ｄベクトル			*/
)
{
	pOut->x = pV1->x / pV2->x;
	pOut->y = pV1->y / pV2->y;
	pOut->z = pV1->z / pV2->z;

	return	pOut;
}

/*=============================================================================
*
* Purpose : ベクトルの外積を求める．
*
* Return  : なし．
*
*============================================================================*/
GSINLINE GSvector3*
gsVector3Cross
(
	GSvector3*			pOut,			/* -> 結果				*/
	const GSvector3*	pV1,			/* ３Ｄベクトル			*/
	const GSvector3*	pV2				/* ３Ｄベクトル			*/
)
{
	pOut->x = pV1->y * pV2->z - pV1->z * pV2->y;
	pOut->y = pV1->z * pV2->x - pV1->x * pV2->z;
	pOut->z = pV1->x * pV2->y - pV1->y * pV2->x;

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
gsVector3Dot
(
	const GSvector3*	pV1,			/* ３Ｄベクトル			*/
	const GSvector3*	pV2				/* ３Ｄベクトル			*/
)
{
	return pV1->x * pV2->x + pV1->y * pV2->y + pV1->z * pV2->z;
}

/*=============================================================================
*
* Purpose : ベクトルの長さを求める．（ 二乗和のみで平方根をとらない ）
*
* Return  : ベクトルの長さを返す．
*
*============================================================================*/
GSINLINE GSfloat
gsVector3LengthSq
(
	const GSvector3*		pV			/* ３Ｄベクトル			*/
)
{
	return pV->x * pV->x + pV->y * pV->y + pV->z * pV->z;
}

/*=============================================================================
*
* Purpose : ベクトルの長さを求める．
*
* Return  : ベクトルの長さを返す．
*
*============================================================================*/
GSINLINE GSfloat
gsVector3Length
(
	const GSvector3*		pV			/* ３Ｄベクトル			*/
)
{
	return (GSfloat)gsSqrt( gsVector3LengthSq( pV ) );
}

/*=============================================================================
*
* Purpose : ベクトルの線形補間．
*
* Return  : なし．
*
*============================================================================*/
GSINLINE GSvector3*
gsVector3Lerp
(
	GSvector3*			pOut,			/* -> 結果				*/
	const GSvector3*	pMin,			/* ３Ｄベクトル			*/
	const GSvector3*	pMax,			/* ３Ｄベクトル			*/
	GSfloat				t				/* 補間値（0.0～1.0)	*/
)
{
	pOut->x = LERP( t, pMin->x, pMax->x );
	pOut->y = LERP( t, pMin->y, pMax->y );
	pOut->z = LERP( t, pMin->z, pMax->z );

	return	pOut;
}

/*=============================================================================
*
* Purpose : ベクトルの２次補間．
*
* Return  : なし．
*
*============================================================================*/
GSINLINE GSvector3*
gsVector3Quadratic
(
	GSvector3*			pOut,			/* -> 結果				*/
	const GSvector3*	pV1,			/* ３Ｄベクトル			*/
	const GSvector3*	pV2,			/* ３Ｄベクトル			*/
	const GSvector3*	pV3,			/* ３Ｄベクトル			*/
	GSfloat				t				/* 補間値（0.0～1.0)	*/
)
{
	pOut->x = QUADRATIC( t, pV1->x, pV2->x, pV3->x );
	pOut->y = QUADRATIC( t, pV1->y, pV2->y, pV3->y );
	pOut->z = QUADRATIC( t, pV1->z, pV2->z, pV3->z );

	return	pOut;
}

/*=============================================================================
*
* Purpose : ２つのベクトルの各要素の最大値から作成されたベクトルを求める．
*
* Return  : 各要素の最大値を求めたベクトルを返す．
*
*============================================================================*/
GSINLINE GSvector3*
gsVector3Maximize
(
	GSvector3*			pOut,			/* -> 結果				*/
	const GSvector3*	pV1,			/* ３Ｄベクトル			*/
	const GSvector3*	pV2				/* ３Ｄベクトル			*/
)
{
	pOut->x = ( pV1->x > pV2->x ) ? pV1->x : pV2->x;
	pOut->y = ( pV1->y > pV2->y ) ? pV1->y : pV2->y;
	pOut->z = ( pV1->z > pV2->z ) ? pV1->z : pV2->z;

	return	pOut;
}

/*=============================================================================
*
* Purpose : ２つのベクトルの各要素の最小値から作成されたベクトルを求める．
*
* Return  : 各要素の最小値を求めたベクトルを返す．
*
*============================================================================*/
GSINLINE GSvector3*
gsVector3Minimize
(
	GSvector3*			pOut,			/* -> 結果				*/
	const GSvector3*	pV1,			/* ３Ｄベクトル			*/
	const GSvector3*	pV2				/* ３Ｄベクトル			*/
)
{
	pOut->x = ( pV1->x < pV2->x ) ? pV1->x : pV2->x;
	pOut->y = ( pV1->y < pV2->y ) ? pV1->y : pV2->y;
	pOut->z = ( pV1->z < pV2->z ) ? pV1->z : pV2->z;

	return	pOut;
}

/*=============================================================================
*
* Purpose : ベクトルのスケーリング．
*
* Return  : なし．
*
*============================================================================*/
GSINLINE GSvector3*
gsVector3Scale
(
	GSvector3*			pOut,			/* -> 結果				*/
	const GSvector3*	pV,				/* ３Ｄベクトル			*/
	GSfloat				s				/* スカラー				*/
)
{
	pOut->x = pV->x * s;
	pOut->y = pV->y * s;
	pOut->z = pV->z * s;

	return	pOut;
}

/*=============================================================================
*
* Purpose : 逆ベクトルを求める．
*
* Return  : なし．
*
*============================================================================*/
GSINLINE GSvector3*
gsVector3Inverse
(
	GSvector3*			pOut,			/* -> 結果				*/
	const GSvector3*	pV				/* ３Ｄベクトル			*/
)
{
	pOut->x = -pV->x;
	pOut->y = -pV->y;
	pOut->z = -pV->z;

	return	pOut;
}

/*=============================================================================
*
* Purpose : 直線の方程式の計算を行う．
*
* Return  : なし．
*
*============================================================================*/
GSINLINE GSvector3*
gsVector3LinearEquation
(
	GSvector3*			pOut,			/* -> 結果				*/
	const GSvector3*	pP,				/* ３Ｄベクトル			*/
	const GSvector3*	pV,				/* ３Ｄベクトル			*/
	GSfloat				t				/* 係数					*/
)
{
	pOut->x = pV->x * t + pP->x;
	pOut->y = pV->y * t + pP->y;
	pOut->z = pV->z * t + pP->z;

	return	pOut;
}

/*=============================================================================
*
* Purpose : 方位角，仰角からベクトルを求める．
*
* Return  : なし．
*
*============================================================================*/
GSINLINE GSvector3*
gsVector3FromEleDir
(
	GSvector3*		pOut,				/* -> 結果			*/
	GSfloat			Elavation,			/* 仰角				*/
	GSfloat			Direction			/* 方位角			*/
)
{
	pOut->x =  gsCos( Elavation ) * gsSin( Direction );
	pOut->y = -gsSin( Elavation );
	pOut->z =  gsCos( Elavation ) * gsCos( Direction );

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
gsVector3Normalize
(
	GSvector3*			pOut,			/*->単位ベクトル                */
	const GSvector3*	pV				/* ベクトル                     */
)
{
	GSfloat				len;

	if ( ( len = gsVector3Length( pV ) ) != 0 )
	{
		gsVector3Scale( pOut, pV, 1.0f / len  );
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
gsVector3Distance
(
	const GSvector3*	pV1,			/* 始点                         */
	const GSvector3*	pV2				/* 終点                         */
)
{
	GSvector3			V;

	gsVector3Subtract( &V, pV1, pV2 );

	return	gsVector3Length( &V );
}

/*=============================================================================
*
* Purpose : ２点間の距離を求める．（ 二乗和だけで平方根を取らない ）
*
* Return  : 距離を返す．
*
*============================================================================*/
GSINLINE GSfloat
gsVector3DistanceSq
(
	const GSvector3*	pV1,			/* 始点                         */
	const GSvector3*	pV2				/* 終点                         */
)
{
	GSvector3			V;

	gsVector3Subtract( &V, pV1, pV2 );

	return	gsVector3LengthSq( &V );
}

/*=============================================================================
*
* Purpose : ２つのベクトルの作る角度を求める．
*
* Return  : 角度を返す．
*
*============================================================================*/
GSINLINE GSfloat
gsVector3InnerDegree
(
	const GSvector3*	pV1,			/* ベクトル１                   */
	const GSvector3*	pV2				/* ベクトル２                   */
)
{
	GSfloat				cos_theta;
	GSfloat				angle;

	cos_theta = gsVector3Dot( pV1, pV2 )
			  / ( gsVector3Length( pV1 ) * gsVector3Length( pV2 ) );

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
GSINLINE GSvector3*
gsVector3Transform
(
	GSvector3*			pOut,			/* 変換後のベクトル			*/
	const GSvector3*	pV,				/* 変換前のベクトル			*/
	const GSmatrix4*	pM				/* 座標変換行列				*/
) 
{
	GSvector3			v;

	v.x = pV->x * pM->m[0][0]
	    + pV->y * pM->m[1][0]
	    + pV->z * pM->m[2][0]
	    +         pM->m[3][0];
	v.y = pV->x * pM->m[0][1]
	    + pV->y * pM->m[1][1]
	    + pV->z * pM->m[2][1]
	    +         pM->m[3][1];
	v.z = pV->x * pM->m[0][2]
	    + pV->y * pM->m[1][2]
	    + pV->z * pM->m[2][2]
	    +         pM->m[3][2];

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
GSINLINE GSvector3*
gsVector3TransformCoord
(
	GSvector3*			pOut,			/* 変換後のベクトル			*/
	const GSvector3*	pV,				/* 変換前のベクトル			*/
	const GSmatrix4*	pM				/* 座標変換行列				*/
) 
{
	GSvector3			v;
	GSfloat				w;

	v.x = pV->x * pM->m[0][0]
	    + pV->y * pM->m[1][0]
	    + pV->z * pM->m[2][0]
	    +         pM->m[3][0];
	v.y = pV->x * pM->m[0][1]
	    + pV->y * pM->m[1][1]
	    + pV->z * pM->m[2][1]
	    +         pM->m[3][1];
	v.z = pV->x * pM->m[0][2]
	    + pV->y * pM->m[1][2]
	    + pV->z * pM->m[2][2]
	    +         pM->m[3][2];
	w   = pV->x * pM->m[0][3]
	    + pV->y * pM->m[1][3]
	    + pV->z * pM->m[2][3]
	    +         pM->m[3][3];

	gsVector3Scale( pOut, &v, 1.0f / w );

	return	pOut;
}

/*=============================================================================
*
* Purpose : 法線ベクトルの座標変換を行う．
*
* Return  : なし．
*
*============================================================================*/
GSINLINE GSvector3*
gsVector3TransformNormal
(
	GSvector3*			pOut,			/* 変換後の法線ベクトル		*/
	const GSvector3*	pV,				/* 変換前の法線ベクトル		*/
	const GSmatrix4*	pM				/* 座標変換行列				*/
) 
{
	GSvector3			v;

	v.x = pV->x * pM->m[0][0]
	    + pV->y * pM->m[1][0]
	    + pV->z * pM->m[2][0];
	v.y = pV->x * pM->m[0][1]
	    + pV->y * pM->m[1][1]
	    + pV->z * pM->m[2][1];
	v.z = pV->x * pM->m[0][2]
	    + pV->y * pM->m[1][2]
	    + pV->z * pM->m[2][2];

	*pOut = v;

	return	pOut;
}

/*=============================================================================
*
* Purpose : 面法線を求める．
*
* Return  : なし．
*
*============================================================================*/
GSINLINE GSvector3*
gsVector3CalculateNormal
(
	GSvector3*			pOut,			/*-> 法線ベクトル		*/
	const GSvector3*	pV0,			/* 三角形の頂点座標０	*/
	const GSvector3*	pV1,			/* 三角形の頂点座標１	*/
	const GSvector3*	pV2				/* 三角形の頂点座標２	*/
)
{
	GSvector3			V1;
	GSvector3			V2;

	/* ３角形の頂点からベクトルを求める */
	gsVector3Subtract( &V1, pV0, pV1 );
	gsVector3Subtract( &V2, pV1, pV2 );

	/* ２つのベクトルの外積を求める */
	gsVector3Cross( pOut, &V1, &V2 );

	/* ベクトルのを正規化する */
	gsVector3Normalize( pOut, pOut );

	return	pOut;
}

/*=============================================================================
*
* Purpose : 反射ベクトルを求める．
*
* Return  : なし．
*
*============================================================================*/
GSINLINE GSvector3*
gsVector3Reflection
(
	GSvector3*			pOut,			/*-> 反射ベクトル		*/
	const GSvector3*	pI,				/* 入射ベクトル			*/
	const GSvector3*	pN				/* 法線ベクトル			*/
)
{
	GSvector3			v;
	GSvector3			t;

	/* Out = 2 * ( -I dot N ) * N + I */
	gsVector3Inverse( &v, pI );
	gsVector3Scale( &t, pN, gsVector3Dot( &v, pN ) * 2.0f );
	gsVector3Add( pOut, &t, pI );

	return	pOut;
}

/*=============================================================================
*
* Purpose : 屈折ベクトルを求める．
*
* Return  : なし．
*
*============================================================================*/
GSINLINE GSvector3*
gsVector3Refraction
(
	GSvector3*			pOut,			/*-> 反射ベクトル		*/
	const GSvector3*	pI,				/* 入射ベクトル			*/
	const GSvector3*	pN,				/* 法線ベクトル			*/
	GSfloat				Eta				/* 屈折率比				*/
)
{
	GSvector3			T1, T2;
	GSfloat				cosI;
	GSfloat				cosT2;

	gsVector3Inverse( &T1, pI );
	cosI  = gsVector3Dot( &T1, pN );
	cosT2 = 1.0f - Eta * Eta * ( 1.0f - cosI * cosI );

	if ( cosT2 <= 0.0f )
	{
		gsVector3Init( pOut, 0.0f, 0.0f, 0.0f );
	}
	else
	{
		gsVector3Scale( &T1, pI, Eta );
		gsVector3Scale( &T2, pN, Eta * cosI - gsSqrt( ABS( cosT2 ) ) );
		gsVector3Add( pOut, &T1, &T2 );
	}

	return	pOut;
}

/*=============================================================================
*
* Purpose : ベクトルからヨーを求める．
*
* Return  : なし．
*
*============================================================================*/
GSINLINE GSfloat
gsVector3GetYaw
(
	const GSvector3*	pV				/* ベクトル				*/
)
{
	GSfloat				x, z;
	GSfloat				l;

	/* ベクトルの長さを求める */
	if ( ( l = gsVector3Length( pV ) ) == 0 )
	{
		/* 長さが０の場合は０にする */
		return 0.0f;
	}

	/* ベクトルを正規化する */
	x = pV->x / l;
	z = pV->z / l;

	/* ヨーの計算を行う */
	return	gsAtan( x, z );
}

/*=============================================================================
*
* Purpose : ベクトルからピッチを求める．
*
* Return  : ピッチを返す．
*
*============================================================================*/
GSINLINE GSfloat
gsVector3GetPitch
(
	const GSvector3*	pV				/* ベクトル				*/
)
{
	GSfloat				y;
	GSfloat				l;

	/* ベクトルの長さを求める */
	if ( ( l = gsVector3Length( pV ) ) == 0 )
	{
		/* 長さが０の場合は０度にする */
		return	0.0f;
	}

	/* ベクトルを正規化する */
	y = pV->y / l;

	/* ピッチの計算を行う */
	return	gsDegreeNormalize( gsAsin( -y ) );
}

/*=============================================================================
*
* Purpose : ベクトルから仰角と方位角を求める．
*
* Return  : なし．
*
*============================================================================*/
GSINLINE void
gsVector3ToEleDir
(
	GSfloat*			pElevation,		/*-> 仰角				*/
	GSfloat*			pDirection,		/*-> 方位角				*/
	const GSvector3*	pV				/* ベクトル				*/
)
{
	if ( pElevation != NULL )
	{
		*pElevation = gsVector3GetPitch( pV );
	}
	if ( pDirection != NULL )
	{
		*pDirection = gsVector3GetYaw( pV );
	}
}

/*=============================================================================
*
* Purpose : 重心座標を求める
*
* Return  : なし．
*
*============================================================================*/
GSINLINE void
gsCalculateBarycentric(
	const GSvector3*	pV0,		/* 三角形の座標1			*/
	const GSvector3*	pV1,		/* 三角形の座標2			*/
	const GSvector3*	pV2,		/* 三角形の座標3			*/
	const GSvector3*	pPoint,		/* 重心座標を求めたい位置	*/
	float*				pU,			/*-> 重心座標1              */
	float*				pV,			/*-> 重心座標2              */
	float*				pW			/*-> 重心座標3              */
)
{
	GSvector3 v0;
	GSvector3 v1;
	GSvector3 v2;
	float d00;
	float d01;
	float d11;
	float d20;
	float d21;
	float denom;
	gsVector3Subtract(&v0, pV1, pV0);
	gsVector3Subtract(&v1, pV2, pV0);
	gsVector3Subtract(&v2, pPoint, pV0);
	d00 = gsVector3Dot(&v0, &v0);
	d01 = gsVector3Dot(&v0, &v1);
	d11 = gsVector3Dot(&v1, &v1);
	d20 = gsVector3Dot(&v2, &v0);
	d21 = gsVector3Dot(&v2, &v1);
	denom = d00 * d11 - d01 * d01;
	*pV = (d11 * d20 - d01 * d21) / denom;
	*pW = (d00 * d21 - d01 * d20) / denom;
	*pU = 1.0f - *pV - *pW;
}

/*=============================================================================
*
* Purpose : 重心座標から座標を求める
*
* Return  : なし．
*
*============================================================================*/
GSINLINE void
gsVector3Barycentric
(
	GSvector3*			pOut,		/*-> 重心座標から求めた座標	*/
	const GSvector3*	pV0,		/* 三角形の座標1			*/
	const GSvector3*	pV1,		/* 三角形の座標2			*/
	const GSvector3*	pV2,		/* 三角形の座標3		    */
	float				u,			/* 重心座標1                */
	float				v,			/* 重心座標2              　*/
	float				w			/* 重心座標3                */
)
{
	GSvector3 v0;
	GSvector3 v1;
	GSvector3 v2;
	gsVector3Scale(&v0, pV0, u);
	gsVector3Scale(&v1, pV1, v);
	gsVector3Scale(&v2, pV2, w);
	gsVector3Add(pOut, &v0,  &v1);
	gsVector3Add(pOut, pOut, &v2);
}

/*=============================================================================
*
* Purpose : 頂点配列の座標変換
*
* Return  : なし．
*
*============================================================================*/
GSINLINE void
gsVector3ArrayTransform
(
	GSvector3			Out[],		/*-> 座標変換後の頂点配列	*/
	const GSvector3		In[],		/* 頂点配列					*/
	GSuint				Size,		/* 頂点配列のサイズ			*/
	const GSmatrix4*	pMatrix		/* 座標変換行列				*/
)
{
	GSuint i;
	for (i = 0; i < Size; ++i)
	{
		gsVector3Transform(&Out[i], &In[i], pMatrix);
	}
}

/*=============================================================================
*
* Purpose : 頂点配列の座標変換（透視変換あり）
*
* Return  : なし．
*
*============================================================================*/
GSINLINE void
gsVector3ArrayTransformCoord
(
	GSvector3			Out[],		/*-> 座標変換後の頂点配列	*/
	const GSvector3		In[],		/* 頂点配列					*/
	GSuint				Size,		/* 頂点配列のサイズ			*/
	const GSmatrix4*	pMatrix		/* 座標変換行列				*/
)
{
	GSuint i;
	for (i = 0; i < Size; ++i)
	{
		gsVector3TransformCoord(&Out[i], &In[i], pMatrix);
	}
}

/*=============================================================================
*
* Purpose : 頂点配列の座標変換
*
* Return  : なし．
*
*============================================================================*/
GSINLINE void
gsVector3ArrayTransformNormal
(
	GSvector3			Out[],		/*-> 座標変換後の頂点配列	*/
	const GSvector3		In[],		/* 頂点配列					*/
	GSuint				Size,		/* 頂点配列のサイズ			*/
	const GSmatrix4*	pMatrix		/* 座標変換行列				*/
)
{
	GSuint i;
	for (i = 0; i < Size; ++i)
	{
		gsVector3TransformNormal(&Out[i], &In[i], pMatrix);
	}
}

/*=============================================================================
*
* Purpose : 頂点配列の座標変換
*
* Return  : なし．
*
*============================================================================*/
GSINLINE void
gsVector3ArrayNormalize
(
	GSvector3			Out[],		/*-> 座標変換後の頂点配列	*/
	const GSvector3		In[],		/* 頂点配列					*/
	GSuint				Size		/* 頂点配列のサイズ			*/
)
{
	GSuint i;
	for (i = 0; i < Size; ++i)
	{
		gsVector3Normalize(&Out[i], &In[i]);
	}
}

#ifdef __cplusplus
}
#endif

/*----- Ｃ＋＋ラッパークラス ------------------------------------------------*/

#ifdef __cplusplus

/* コンストラクタ */
GSINLINE GSvector3::GSvector3( GSfloat X, GSfloat Y, GSfloat Z ) :
	x( X ), y( Y ), z( Z )
{}

GSINLINE GSvector3::GSvector3( const GSvector2& v2, GSfloat Z ) :
	x( v2.x ), y( v2.y ), z( Z )
{}

GSINLINE GSvector3::GSvector3( const GSvector4& v4 ) :
	x( v4.x ), y( v4.y ), z( v4.z )
{}

GSINLINE GSvector3::GSvector3( const GSfloat* p ) :
	x( p[0] ), y( p[1] ), z( p[2] )
{}

/* 正規化 */
GSINLINE GSvector3& GSvector3::normalize()
{
	gsVector3Normalize( this, this );

	return	*this;
}

/* 正規化後のベクトルを取得する */
GSINLINE GSvector3 GSvector3::getNormalized() const
{
	GSvector3 result( *this );

	return	result.normalize();
}

/* 長さを求める */
GSINLINE GSfloat GSvector3::length() const
{
	return	gsVector3Length( this );
}

/* 長さを求める (２乗和のみ）*/
GSINLINE GSfloat GSvector3::lengthSq() const
{
	return	gsVector3LengthSq( this );
}

/* 距離を求める */
GSINLINE GSfloat GSvector3::distance( const GSvector3& V ) const
{
	return	gsVector3Distance( this, &V );
}

/* 内積を求める */
GSINLINE GSfloat GSvector3::dot( const GSvector3& V ) const
{
	return	gsVector3Dot( this, &V );
}

/* 外積を求める */
GSINLINE GSvector3 GSvector3::cross( const GSvector3& V ) const
{
	GSvector3	result;

	gsVector3Cross( &result, this, &V );

	return result;
}

/* 小さい要素を求める */
GSINLINE GSvector3 GSvector3::minimize( const GSvector3& V ) const
{
	GSvector3	result;

	gsVector3Minimize( &result, this, &V );

	return result;
}

/* 大きい要素を求める */
GSINLINE GSvector3 GSvector3::maximize( const GSvector3& V ) const
{
	GSvector3	result;

	gsVector3Maximize( &result, this, &V );

	return result;
}

/* クランプする */
GSINLINE GSvector3 GSvector3::clamp( const GSvector3& l, const GSvector3& h ) const
{
	GSvector3	result;

	gsVector3Maximize(&result, &l, gsVector3Minimize(&result, this, &h));

	return result;
}

/* 線形補間を行う */
GSINLINE GSvector3 GSvector3::lerp( const GSvector3& V, GSfloat T ) const
{
	GSvector3	result;

	gsVector3Lerp( &result, this, &V, T );

	return result;
}

/* ２次補間を行う */
GSINLINE GSvector3 GSvector3::quadratic( const GSvector3& V2, const GSvector3& V3, GSfloat n ) const
{
	GSvector3	result;

	gsVector3Quadratic( &result, this, &V2, &V3, n );

	return result;
}

/* 反射ベクトルを求める */
GSINLINE GSvector3 GSvector3::reflect( const GSvector3& N ) const
{
	GSvector3	result;

	gsVector3Reflection( &result, this, &N );

	return result;
}

/* 屈折ベクトルを求める */
GSINLINE GSvector3 GSvector3::refract( const GSvector3& N, GSfloat Eta ) const
{
	GSvector3	result;

	gsVector3Refraction( &result, this, &N, Eta );

	return result;
}

/* ２つベクトルのなす角度を求める */
GSINLINE GSfloat GSvector3::degree( const GSvector3& V ) const
{
	return gsVector3InnerDegree( this, &V );
}

/* ヨーを求める */
GSINLINE GSfloat GSvector3::getYaw() const
{
	return	gsVector3GetYaw( this );
}

/* ピッチを求める */
GSINLINE GSfloat GSvector3::getPitch() const
{
	return	gsVector3GetPitch( this );
}

/* ピッチ・ヨーからベクトルを求める */
GSINLINE GSvector3& GSvector3::fromPitchYaw( GSfloat picth, GSfloat yaw )
{
	gsVector3FromEleDir( this, picth, yaw );

	return	*this;
}

/** ピッチ・ヨーからベクトルを求める */
GSINLINE  GSvector3 GSvector3::createFromPitchYaw( GSfloat picth, GSfloat yaw )
{
	GSvector3 result;

	gsVector3FromEleDir( &result, picth, yaw );

	return	result;
}

/* 法線ベクトルを求める */
GSINLINE GSvector3 GSvector3::calculateNormal( const GSvector3& V1, const GSvector3& V2 ) const
{
	GSvector3	result;

	gsVector3CalculateNormal( &result, this, &V1, &V2 );

	return result;
}

/* ２つのベクトルがなす角度を求める */
GSINLINE GSfloat GSvector3::innerDegree(const GSvector3& other) const
{
	return	gsVector3InnerDegree(this, &other);
}

// キャスト演算子オーバーロード
GSINLINE GSvector3::operator GSfloat* () const
{
	return (float*) this;
}

GSINLINE GSvector3::operator const GSfloat* () const
{
	return (const GSfloat*) this;
}

// 単項演算子オーバーロード
GSINLINE GSvector3	GSvector3::operator + () const
{
	return	*this;
}

GSINLINE GSvector3	GSvector3::operator - () const
{
	return	GSvector3( -x, -y, -z );
}

// 代入演算子オーバーロード
GSINLINE GSvector3&	GSvector3::operator += ( const GSvector3& V )
{
	gsVector3Add( this, this, &V );

	return	*this;
}

GSINLINE GSvector3&	GSvector3::operator -= ( const GSvector3& V )
{
	gsVector3Subtract( this, this, &V );

	return	*this;
}

GSINLINE GSvector3&	GSvector3::operator *= ( const GSvector3& V )
{
	gsVector3Multiply( this, this, &V );

	return	*this;
}

GSINLINE GSvector3&	GSvector3::operator /= ( const GSvector3& V )
{
	gsVector3Divide( this, this, &V );

	return	*this;
}

GSINLINE GSvector3&	GSvector3::operator *= ( GSfloat S )
{
	gsVector3Scale( this, this, S );

	return	*this;
}

GSINLINE GSvector3&	GSvector3::operator /= ( GSfloat S )
{
	gsVector3Scale( this, this, 1.0f / S );

	return	*this;
}

// ２項演算子オーバーロード
GSINLINE const GSvector3 operator + ( const GSvector3& v1, const GSvector3& v2 )
{
	GSvector3	result;

	gsVector3Add( &result, &v1, &v2 );

	return	result;
}

GSINLINE const GSvector3 operator - ( const GSvector3& v1, const GSvector3& v2 )
{
	GSvector3	result;

	gsVector3Subtract( &result, &v1, &v2 );

	return	result;
}

GSINLINE const GSvector3 operator * ( const GSvector3& v1, const GSvector3 v2 )
{
	GSvector3	result;

	gsVector3Multiply( &result, &v1, &v2 );

	return	result;
}

GSINLINE const GSvector3 operator * ( const GSvector3& v, GSfloat s )
{
	GSvector3	result;

	gsVector3Scale( &result, &v, s );

	return	result;
}

GSINLINE const GSvector3 operator / ( const GSvector3& v, GSfloat s )
{
	GSvector3	result;

	gsVector3Scale( &result, &v, 1.0f / s);

	return	result;
}


GSINLINE const GSvector3 operator * ( GSfloat s, const GSvector3& v )
{
	return	v * s;
}

GSINLINE const GSvector3 operator * ( const GSmatrix4& M, const GSvector3& v )
{
	GSfloat x = v.x * M.m[0][0] + v.y * M.m[0][1] + v.z * M.m[0][2] + M.m[0][3];
	GSfloat y = v.x * M.m[1][0] + v.y * M.m[1][1] + v.z * M.m[1][2] + M.m[1][3];
	GSfloat z = v.x * M.m[2][0] + v.y * M.m[2][1] + v.z * M.m[2][2] + M.m[2][3];
	GSfloat w = v.x * M.m[3][0] + v.y * M.m[3][1] + v.z * M.m[3][2] + M.m[3][3];

	return	GSvector3( x / w, y / w, z / w );
}

GSINLINE const GSvector3 operator * ( const GSvector3& v, const GSmatrix4& M )
{
	GSfloat x = v.x * M.m[0][0] + v.y * M.m[1][0] + v.z * M.m[2][0] + M.m[3][0];
	GSfloat y = v.x * M.m[0][1] + v.y * M.m[1][1] + v.z * M.m[2][1] + M.m[3][1];
	GSfloat z = v.x * M.m[0][2] + v.y * M.m[1][2] + v.z * M.m[2][2] + M.m[3][2];
	GSfloat w = v.x * M.m[0][3] + v.y * M.m[1][3] + v.z * M.m[2][3] + M.m[3][3];

	return	GSvector3( x / w, y / w, z / w );
}

GSINLINE const GSvector3 operator * ( const GSmatrix3& M, const GSvector3& v )
{
	return	GSvector3( v.x * M.m[0][0] + v.y * M.m[0][1] + v.z * M.m[0][2],
					   v.x * M.m[1][0] + v.y * M.m[1][1] + v.z * M.m[1][2],
					   v.x * M.m[2][0] + v.y * M.m[2][1] + v.z * M.m[2][2] );
}

GSINLINE const GSvector3 operator * ( const GSvector3& v, const GSmatrix3& M )
{
	return GSvector3( v.x * M.m[0][0] + v.y * M.m[1][0] + v.z * M.m[2][0],
					  v.x * M.m[0][1] + v.y * M.m[1][1] + v.z * M.m[2][1],
					  v.x * M.m[0][2] + v.y * M.m[1][2] + v.z * M.m[2][2] );
}

GSINLINE bool operator == ( const GSvector3& v1, const GSvector3& v2 )
{
    return	( v1.x == v2.x && v1.y == v2.y && v1.z == v2.z );
}

GSINLINE bool operator != ( const GSvector3& v1, const GSvector3& v2 )
{
	return	!( v1 == v2 );
}

GSINLINE bool operator < ( const GSvector3& v1, const GSvector3& v2 )
{
	if ( v1.x != v2.x ) return v1.x < v2.x;
	if ( v1.y != v2.y ) return v1.y < v2.y;
	return v1.z < v2.z;
}

GSINLINE bool operator > ( const GSvector3& v1, const GSvector3& v2 )
{
	if ( v1.x != v2.x ) return v1.x > v2.x;
	if ( v1.y != v2.y ) return v1.y > v2.y;
	return v1.z > v2.z;
}

GSINLINE bool operator <= ( const GSvector3& v1, const GSvector3& v2 )
{
	return	!( v1 > v2 );
}

GSINLINE bool operator >= ( const GSvector3& v1, const GSvector3& v2 )
{
	return	!( v1 < v2 );
}

#endif


#endif

/********** End of File ******************************************************/
