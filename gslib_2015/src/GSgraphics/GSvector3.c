/******************************************************************************
* File Name : GSvector3.c                         Ver : 1.00  Date : 2001-01-16
*
* Description :
*
*       ３Ｄベクトル計算
*
* Author : Moriharu Ohzu.
*
******************************************************************************/
#include	"GSvector3.h"
#include	<float.h>

/*=============================================================================
*
* Purpose : ベクトルを正規化する．（ 単位ベクトル化する ）
*
* Return  : なし．
*
*============================================================================*/
extern void
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
extern GSfloat
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
extern GSfloat
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
extern GSfloat
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
extern void
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
	    + pM->m[3][0];
	v.y = pV->x * pM->m[0][1]
	    + pV->y * pM->m[1][1]
	    + pV->z * pM->m[2][1]
	    + pM->m[3][1];
	v.z = pV->x * pM->m[0][2]
	    + pV->y * pM->m[1][2]
	    + pV->z * pM->m[2][2]
	    + pM->m[3][2];

	*pOut = v;
}

/*=============================================================================
*
* Purpose : ベクトルの座標変換を行う．．
*
* Return  : なし．
*
*============================================================================*/
extern void
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
	    + pM->m[3][0];
	v.y = pV->x * pM->m[0][1]
	    + pV->y * pM->m[1][1]
	    + pV->z * pM->m[2][1]
	    + pM->m[3][1];
	v.z = pV->x * pM->m[0][2]
	    + pV->y * pM->m[1][2]
	    + pV->z * pM->m[2][2]
	    + pM->m[3][2];
	w   = pV->x * pM->m[0][3]
	    + pV->y * pM->m[1][3]
	    + pV->z * pM->m[2][3]
	    + pM->m[3][3];

	gsVector3Scale( pOut, &v, 1.0f / w );
}

/*=============================================================================
*
* Purpose : 法線ベクトルの座標変換を行う．
*
* Return  : なし．
*
*============================================================================*/
extern void
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
}

/*=============================================================================
*
* Purpose : 面法線を求める．
*
* Return  : なし．
*
*============================================================================*/
extern void
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
}

/*=============================================================================
*
* Purpose : 反射ベクトルを求める．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsVector3Refrection
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
}

/*=============================================================================
*
* Purpose : ベクトルから仰角と方位角を求める．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsVector3ToEleDir
(
	GSfloat*			pElevation,		/*-> 仰角				*/
	GSfloat*			pDirection,		/*-> 方位角				*/
	const GSvector3*	pV				/* ベクトル				*/
)
{
	GSvector3			Normal;
	GSfloat				Len;

	/* ベクトルの長さを求める */
	if ( ( Len = gsVector3Length( pV ) ) == 0 )
	{
		/* 長さが０の場合は仰角・方位角共に０にする */
		*pDirection = 0.0f;
		*pElevation = 0.0f;

		return;
	}

	/* ベクトルを正規化する */
	Normal.x = pV->x / Len;
	Normal.y = pV->y / Len;
	Normal.z = pV->z / Len;

	/* 方位角の計算を行う */
	if ( Normal.x == 0.0f && Normal.z == 0.0f )
	{
		*pDirection = 0.0f;
	}
	else
	{
		*pDirection = gsAtan( Normal.x, Normal.z );
	}

	/* 仰角の計算を行う */
	*pElevation = gsDegreeNormalize( gsAsin( -Normal.y ) );
}

/********** End of File ******************************************************/

