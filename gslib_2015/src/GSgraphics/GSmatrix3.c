/******************************************************************************
* File Name : GSmatrix3.c                        Ver : 1.00  Date : 1998-05-03
*
* Description :
*
*       ３行３列座標変換行列．
*
* Author : Moriharu Ohzu.
*
******************************************************************************/
#include	"GSmatrix3.h"
#include	"GSvector3.h"
#include	"GSplane.h"
#include	"GSmath.h"
#include	<float.h>

const GSmatrix3	GS_MATRIX3_IDENTITY =
{
	1.0F, 0.0F, 0.0F,
	0.0F, 1.0F, 0.0F,
	0.0F, 0.0F, 1.0F
};

/*=============================================================================
*
* Purpose : 単位行列を求める．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsMatrix3Identity
(
	GSmatrix3*		pM					/* ３Ｄマトリクス   */
)
{
	*pM = GS_MATRIX3_IDENTITY;
}

/*=============================================================================
*
* Purpose : 平行移動行列を求める．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsMatrix3Translate
(
	GSmatrix3*		pM,					/* ３Ｄマトリクス       */
	GSfloat			tx,					/* ｘ方向移動量			*/
	GSfloat			ty					/* ｙ方向移動量			*/
)
{
	pM->m[0][0] = 1.0f;
	pM->m[0][1] = 0.0f;
	pM->m[0][2] = 0.0f;

	pM->m[1][0] = 0.0f;
	pM->m[1][1] = 1.0f;
	pM->m[1][2] = 0.0f;

	pM->m[2][0] = tx;
	pM->m[2][1] = ty;
	pM->m[2][2] = 1.0f;
}

/*=============================================================================
*
* Purpose : 拡大・縮小行列を求める．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsMatrix3Scale
(
	GSmatrix3*		pM,					/* ３Ｄマトリクス       */
	GSfloat			sx,					/* ｘ方向スケール値		*/
	GSfloat			sy,					/* ｙ方向スケール値		*/
	GSfloat			sz					/* ｚ方向スケール値		*/
)
{
	pM->m[0][0] = sx;
	pM->m[0][1] = 0.0f;
	pM->m[0][2] = 0.0f;

	pM->m[1][0] = 0.0f;
	pM->m[1][1] = sy;
	pM->m[1][2] = 0.0f;

	pM->m[2][0] = 0.0f;
	pM->m[2][1] = 0.0f;
	pM->m[2][2] = sz;
}

/*=============================================================================
*
* Purpose : 回転行列を求める．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsMatrix3Rotate
(
	GSmatrix3*		pM,					/* ３Ｄマトリクス       */
	GSfloat			angle,				/* 回転角度				*/
	GSfloat			dx,					/* 回転軸のｘ成分		*/
	GSfloat			dy,					/* 回転軸のｙ成分		*/
	GSfloat			dz					/* 回転軸のｚ成分		*/
)
{
	GSfloat			sinAngle;
	GSfloat			cosAngle;
	GSfloat			oneMinusCosAngle;
	GSfloat			Len;

	sinAngle         = gsSin(angle);
	cosAngle         = gsCos(angle);
	oneMinusCosAngle = 1.0f - cosAngle;

	if ( ( Len = gsSqrt( dx * dx + dy * dy + dz * dz ) ) != 0 )
	{
		dx *= ( 1.0f / Len );
		dy *= ( 1.0f / Len );
		dz *= ( 1.0f / Len );
	}

	pM->m[0][0] = ( dx * dx ) + cosAngle * ( 1.0f - ( dx * dx ) );
	pM->m[1][0] = ( dx * dy ) * oneMinusCosAngle - sinAngle * dz;
	pM->m[2][0] = ( dx * dz ) * oneMinusCosAngle + sinAngle * dy;

	pM->m[0][1] = ( dx * dy ) * oneMinusCosAngle + sinAngle * dz;
	pM->m[1][1] = ( dy * dy ) + cosAngle * ( 1.0f - ( dy * dy ) );
	pM->m[2][1] = ( dy * dz ) * oneMinusCosAngle - sinAngle * dx;

	pM->m[0][2] = ( dx * dz ) * oneMinusCosAngle - sinAngle * dy;
	pM->m[1][2] = ( dy * dz ) * oneMinusCosAngle + sinAngle * dx;
	pM->m[2][2] = ( dz * dz ) + cosAngle * ( 1.0f - ( dz * dz ) );
}

/*=============================================================================
*
* Purpose : ｘ軸周りの回転行列を求める．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsMatrix3RotateX
(
	GSmatrix3*		pM,					/* ３Ｄマトリクス       */
	GSfloat			angle				/* ｘ軸周りの角度       */
)
{
	pM->m[0][0] = 1.0f;
	pM->m[0][1] = 0.0f;
	pM->m[0][2] = 0.0f;

	pM->m[1][0] = 0.0f;

	pM->m[2][0] = 0.0f;

	pM->m[1][1] = gsCos( angle );
	pM->m[1][2] = gsSin( angle );
	pM->m[2][1] = -pM->m[1][2];
	pM->m[2][2] =  pM->m[1][1];
}

/*=============================================================================
*
* Purpose : ｙ軸周りの回転行列を求める．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsMatrix3RotateY
(
	GSmatrix3*		pM,					/* ３Ｄマトリクス       */
	GSfloat			angle				/* ｙ軸周りの角度       */
)
{
	pM->m[0][1] = 0.0f;

	pM->m[1][0] = 0.0f;
	pM->m[1][1] = 1.0f;
	pM->m[1][2] = 0.0f;

	pM->m[2][1] = 0.0f;

	pM->m[0][0] =  gsCos( angle );
	pM->m[2][0] =  gsSin( angle );
	pM->m[0][2] = -pM->m[2][0];
	pM->m[2][2] =  pM->m[0][0];
}

/*=============================================================================
*
* Purpose : ｚ軸周りの回転行列を求める．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsMatrix3RotateZ
(
	GSmatrix3*		pM,					/* ３Ｄマトリクス       */
	GSfloat			angle				/* ｚ軸周りの角度       */
)
{
	pM->m[0][2] = 0.0f;

	pM->m[1][2] = 0.0f;

	pM->m[2][0] = 0.0f;
	pM->m[2][1] = 0.0f;
	pM->m[2][2] = 1.0f;

	pM->m[0][0] =  gsCos( angle );
	pM->m[0][1] =  gsSin( angle );
	pM->m[1][0] = -pM->m[0][1];
	pM->m[1][1] =  pM->m[0][0];
}

/*=============================================================================
*
* Purpose : クォータニオンから回転行列を求める．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsMatrix3Quaternion
(
	GSmatrix3*			pM,				/* ３Ｄマトリクス       */
	const GSquaternion*	q				/* クォータニオン		*/
)
{
	GSfloat		xx, yy, zz;
	GSfloat		xy, xz, yz;
	GSfloat		wx, wy, wz;

	xx = q->x * q->x * 2.0f;
	yy = q->y * q->y * 2.0f;
	zz = q->z * q->z * 2.0f;
	xy = q->x * q->y * 2.0f;
	xz = q->x * q->z * 2.0f;
	yz = q->y * q->z * 2.0f;
	wx = q->w * q->x * 2.0f;
	wy = q->w * q->y * 2.0f;
	wz = q->w * q->z * 2.0f;

	pM->m[0][0] = 1.0f - ( yy + zz );
	pM->m[0][1] = xy + wz;
	pM->m[0][2] = xz - wy;

	pM->m[1][0] = xy - wz;
	pM->m[1][1] = 1.0f - ( xx + zz );
	pM->m[1][2] = yz + wx;

	pM->m[2][0] = xz + wy;
	pM->m[2][1] = yz - wx;
	pM->m[2][2] = 1.0f - ( xx + yy );
}

/*=============================================================================
*
* Purpose : 行列の乗算．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsMatrix3Multiply
(
	GSmatrix3*			pOut,			/*-> 掛け算の結果   */
	const GSmatrix3*	pM1,			/* ３Ｄマトリクス   */
	const GSmatrix3*	pM2				/* ３Ｄマトリクス   */
)
{
	GSmatrix3			M;
	int					i, j;

	for ( i = 0; i < 3; i++ )
	{
		for ( j = 0; j < 3; j++ )
		{
			M.m[i][j] = pM1->m[i][0] * pM2->m[0][j]
					  + pM1->m[i][1] * pM2->m[1][j]
					  + pM1->m[i][2] * pM2->m[2][j];
		}
	}

	*pOut = M;
}

/*=============================================================================
*
* Purpose : 行列とスカラーの乗算．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsMatrix3MultiplyScalar
(
	GSmatrix3*			pOut,			/*-> 掛け算の結果   */
	const GSmatrix3*	pM1,			/* ３Ｄマトリクス   */
	GSfloat				Scalar			/* スカラー         */
)
{
	int					i, j;

	for ( i = 0; i < 3; i++ )
	{
		for ( j = 0; j < 3; j++ )
		{
			pOut->m[i][j] = pM1->m[i][j] * Scalar;
		}
	}
}

/*=============================================================================
*
* Purpose : 行列の加算．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsMatrix3Add
(
	GSmatrix3*			pOut,			/*-> 加算の結果     */
	const GSmatrix3*	pM1,			/* ３Ｄマトリクス   */
	const GSmatrix3*	pM2				/* ３Ｄマトリクス   */
)
{
	int					i, j;

	for ( i = 0; i < 3; i++ )
	{
		for ( j = 0; j < 3; j++ )
		{
			pOut->m[i][j] = pM1->m[i][j] + pM2->m[i][j];
		}
	}
}

/*=============================================================================
*
* Purpose : 行列の減算．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsMatrix3Subtract
(
	GSmatrix3*			pOut,			/*-> 減算の結果     */
	const GSmatrix3*	pM1,			/* ３Ｄマトリクス   */
	const GSmatrix3*	pM2				/* ３Ｄマトリクス   */
)
{
	int					i, j;

	for ( i = 0; i < 3; i++ )
	{
		for ( j = 0; j < 3; j++ )
		{
			pOut->m[i][j] = pM1->m[i][j] - pM2->m[i][j];
		}
	}
}

/*=============================================================================
*
* Purpose : 転置行列を求める．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsMatrix3Transpose
(
	GSmatrix3*			pOut,			/* 転置３Ｄマトリクス   */
	const GSmatrix3*	pM				/* ３Ｄマトリクス       */
)
{
	GSmatrix3			M;

	/* マトリクスの行と列を交換する */
	M.m[0][0] = pM->m[0][0];
	M.m[1][0] = pM->m[0][1];
	M.m[2][0] = pM->m[0][2];

	M.m[0][1] = pM->m[1][0];
	M.m[1][1] = pM->m[1][1];
	M.m[2][1] = pM->m[1][2];

	M.m[0][2] = pM->m[2][0];
	M.m[1][2] = pM->m[2][1];
	M.m[2][2] = pM->m[2][2];

	*pOut = M;
}

/*=============================================================================
*
* Purpose : 逆行列を求める．
*
* Return  : 逆行列が求まれば真，求まらなければ偽を返す．
*
*============================================================================*/
extern	GSboolean
gsMatrix3Inverse
(
	GSmatrix3*			pOut,			/* 逆３Ｄマトリクス     */
	const GSmatrix3*	pM				/* ３Ｄマトリクス       */
)
{
	GSmatrix3			M;
	GSfloat				fDet;
	GSfloat				fInvDet;

	M.m[0][0] = pM->m[1][1] * pM->m[2][2] - pM->m[1][2]* pM->m[2][1];
	M.m[0][1] = pM->m[0][2] * pM->m[2][1] - pM->m[0][1]* pM->m[2][2];
	M.m[0][2] = pM->m[0][1] * pM->m[1][2] - pM->m[0][2]* pM->m[1][1];
	M.m[1][0] = pM->m[1][2] * pM->m[2][0] - pM->m[1][0]* pM->m[2][2];
	M.m[1][1] = pM->m[0][0] * pM->m[2][2] - pM->m[0][2]* pM->m[2][0];
	M.m[1][2] = pM->m[0][2] * pM->m[1][0] - pM->m[0][0]* pM->m[1][2];
	M.m[2][0] = pM->m[1][0] * pM->m[2][1] - pM->m[1][1]* pM->m[2][0];
	M.m[2][1] = pM->m[0][1] * pM->m[2][0] - pM->m[0][0]* pM->m[2][1];
	M.m[2][2] = pM->m[0][0] * pM->m[1][1] - pM->m[0][1]* pM->m[1][0];

	fDet = pM->m[0][0] * M.m[0][0]
	     + pM->m[0][1] * M.m[1][0]
	     + pM->m[0][2] * M.m[2][0];

	if ( fDet == 0.0 )
	{
		/* 逆行列を求める事ができない */
		gsMatrix3Identity( pOut );

		return	GS_FALSE;
	}

	fInvDet = 1.0f / fDet;

	M.m[0][0] *= fInvDet;
	M.m[0][1] *= fInvDet;
	M.m[0][2] *= fInvDet;
	M.m[1][0] *= fInvDet;
	M.m[1][1] *= fInvDet;
	M.m[1][2] *= fInvDet;
	M.m[2][0] *= fInvDet;
	M.m[2][1] *= fInvDet;
	M.m[2][2] *= fInvDet;

	*pOut = M;

	return GS_TRUE;
}

/*=============================================================================
*
* Purpose : ヨー，ピッチ，ロールから回転行列を求める．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsMatrix3YawPitchRoll
(
	GSmatrix3*			pOut,			/* 回転マトリクス					*/
	GSfloat				Yaw,			/*-> ヨー  （ｙ軸周りの回転角度）	*/
	GSfloat				Pitch,			/*-> ピッチ（ｘ軸周りの回転角度）	*/
	GSfloat				Roll			/*-> ロール（Ｚ軸周リの回転角度）	*/
)
{
	GSmatrix3			matRoll;
	GSmatrix3			matPitch;
	GSmatrix3			matYaw;

	/* ロール，ピッチ，ヨーから回転マトリクスを求める */
	gsMatrix3RotateZ( &matRoll,  Roll  );
	gsMatrix3RotateX( &matPitch, Pitch );
	gsMatrix3RotateY( &matYaw,   Yaw   );

	/* 回転マトリクスを合成する */
	gsMatrix3Multiply( pOut, &matRoll, &matPitch );
	gsMatrix3Multiply( pOut, pOut, &matYaw );
}

/*=============================================================================
*
* Purpose : 回転行列からピッチを求める．
*
* Return  : なし．
*
*============================================================================*/
extern GSfloat
gsMatrix3GetPitch
(
	const GSmatrix3*	pM				/* 行列								*/
)
{
	/* ピッチを求める */
	return	gsDegreeNormalize( gsAsin( -pM->m[2][1] ) );
}

/*=============================================================================
*
* Purpose : 回転行列からロールを求める．
*
* Return  : なし．
*
*============================================================================*/
extern GSfloat
gsMatrix3GetRoll
(
	const GSmatrix3*	pM				/* 行列								*/
)
{
	/* ロールを求める */
	return	gsAtan( pM->m[0][1], pM->m[1][1] );
}

/*=============================================================================
*
* Purpose : 回転行列からヨーを求める．
*
* Return  : なし．
*
*============================================================================*/
extern GSfloat
gsMatrix3GetYaw
(
	const GSmatrix3*	pM				/* 行列								*/
)
{
	/* ヨーを求める */
	return	gsAtan( pM->m[2][0], pM->m[2][2] );
}

/********** End of File ******************************************************/
