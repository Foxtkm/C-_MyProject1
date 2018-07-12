/******************************************************************************
* File Name : GSmatrix4.c                        Ver : 1.00  Date : 1998-05-03
*
* Description :
*
*       三次元座標変換行列．
*
* Author : Moriharu Ohzu.
*
******************************************************************************/
#include	"GSmatrix4.h"
#include	"GSvector3.h"
#include	"GSplane.h"
#include	"GSmath.h"
#include	<float.h>

const GSmatrix4 GS_MATRIX4_IDENTITY = {
	1.0F, 0.0F, 0.0F, 0.0F,
	0.0F, 1.0F, 0.0F, 0.0F,
	0.0F, 0.0F, 1.0F, 0.0F,
	0.0F, 0.0F, 0.0F, 1.0F
};

/*=============================================================================
*
* Purpose : 単位行列を求める．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsMatrix4Identity
(
	GSmatrix4*		pM					/* ３Ｄマトリクス   */
)
{
	*pM = GS_MATRIX4_IDENTITY;
}

/*=============================================================================
*
* Purpose : 平行移動行列を求める．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsMatrix4Translate
(
	GSmatrix4*		pM,					/* ３Ｄマトリクス       */
	GSfloat			tx,					/* ｘ方向移動量			*/
	GSfloat			ty,					/* ｙ方向移動量			*/
	GSfloat			tz					/* ｚ方向移動量			*/
)
{
	pM->m[0][0] = 1.0f;
	pM->m[0][1] = 0.0f;
	pM->m[0][2] = 0.0f;
	pM->m[0][3] = 0.0f;

	pM->m[1][0] = 0.0f;
	pM->m[1][1] = 1.0f;
	pM->m[1][2] = 0.0f;
	pM->m[1][3] = 0.0f;

	pM->m[2][0] = 0.0f;
	pM->m[2][1] = 0.0f;
	pM->m[2][2] = 1.0f;
	pM->m[2][3] = 0.0f;

	pM->m[3][0] = tx;
	pM->m[3][1] = ty;
	pM->m[3][2] = tz;
	pM->m[3][3] = 1.0f;
}

/*=============================================================================
*
* Purpose : 拡大・縮小行列を求める．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsMatrix4Scale
(
	GSmatrix4*		pM,					/* ３Ｄマトリクス       */
	GSfloat			sx,					/* ｘ方向スケール値		*/
	GSfloat			sy,					/* ｙ方向スケール値		*/
	GSfloat			sz					/* ｚ方向スケール値		*/
)
{
	pM->m[0][0] = sx;
	pM->m[0][1] = 0.0f;
	pM->m[0][2] = 0.0f;
	pM->m[0][3] = 0.0f;

	pM->m[1][0] = 0.0f;
	pM->m[1][1] = sy;
	pM->m[1][2] = 0.0f;
	pM->m[1][3] = 0.0f;

	pM->m[2][0] = 0.0f;
	pM->m[2][1] = 0.0f;
	pM->m[2][2] = sz;
	pM->m[2][3] = 0.0f;

	pM->m[3][0] = 0.0f;
	pM->m[3][1] = 0.0f;
	pM->m[3][2] = 0.0f;
	pM->m[3][3] = 1.0f;
}

/*=============================================================================
*
* Purpose : 回転行列を求める．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsMatrix4Rotate
(
	GSmatrix4*		pM,					/* ３Ｄマトリクス       */
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
	pM->m[3][0] = 0.0f;

	pM->m[0][1] = ( dx * dy ) * oneMinusCosAngle + sinAngle * dz;
	pM->m[1][1] = ( dy * dy ) + cosAngle * ( 1.0f - ( dy * dy ) );
	pM->m[2][1] = ( dy * dz ) * oneMinusCosAngle - sinAngle * dx;
	pM->m[3][1] = 0.0f;

	pM->m[0][2] = ( dx * dz ) * oneMinusCosAngle - sinAngle * dy;
	pM->m[1][2] = ( dy * dz ) * oneMinusCosAngle + sinAngle * dx;
	pM->m[2][2] = ( dz * dz ) + cosAngle * ( 1.0f - ( dz * dz ) );
	pM->m[3][2] = 0.0f;

	pM->m[0][3] = 0.0f;
	pM->m[1][3] = 0.0f;
	pM->m[2][3] = 0.0f;
	pM->m[3][3] = 1.0f;
}

/*=============================================================================
*
* Purpose : ｘ軸周りの回転行列を求める．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsMatrix4RotateX
(
	GSmatrix4*		pM,					/* ３Ｄマトリクス       */
	GSfloat			angle				/* ｘ軸周りの角度       */
)
{
	pM->m[0][0] = 1.0f;
	pM->m[0][1] = 0.0f;
	pM->m[0][2] = 0.0f;
	pM->m[0][3] = 0.0f;

	pM->m[1][0] = 0.0f;
	pM->m[1][3] = 0.0f;

	pM->m[2][0] = 0.0f;
	pM->m[2][3] = 0.0f;

	pM->m[3][0] = 0.0f;
	pM->m[3][1] = 0.0f;
	pM->m[3][2] = 0.0f;
	pM->m[3][3] = 1.0f;

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
gsMatrix4RotateY
(
	GSmatrix4*		pM,					/* ３Ｄマトリクス       */
	GSfloat			angle				/* ｙ軸周りの角度       */
)
{
	pM->m[0][1] = 0.0f;
	pM->m[0][3] = 0.0f;

	pM->m[1][0] = 0.0f;
	pM->m[1][1] = 1.0f;
	pM->m[1][2] = 0.0f;
	pM->m[1][3] = 0.0f;

	pM->m[2][1] = 0.0f;
	pM->m[2][3] = 0.0f;

	pM->m[3][0] = 0.0f;
	pM->m[3][1] = 0.0f;
	pM->m[3][2] = 0.0f;
	pM->m[3][3] = 1.0f;

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
gsMatrix4RotateZ
(
	GSmatrix4*		pM,					/* ３Ｄマトリクス       */
	GSfloat			angle				/* ｚ軸周りの角度       */
)
{
	pM->m[0][2] = 0.0f;
	pM->m[0][3] = 0.0f;

	pM->m[1][2] = 0.0f;
	pM->m[1][3] = 0.0f;

	pM->m[2][0] = 0.0f;
	pM->m[2][1] = 0.0f;
	pM->m[2][2] = 1.0f;
	pM->m[2][3] = 0.0f;

	pM->m[3][0] = 0.0f;
	pM->m[3][1] = 0.0f;
	pM->m[3][2] = 0.0f;
	pM->m[3][3] = 1.0f;

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
gsMatrix4Quaternion
(
	GSmatrix4*			pM,				/* ３Ｄマトリクス       */
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
	pM->m[0][3] = 0.0f;

	pM->m[1][0] = xy - wz;
	pM->m[1][1] = 1.0f - ( xx + zz );
	pM->m[1][2] = yz + wx;
	pM->m[1][3] = 0.0f;

	pM->m[2][0] = xz + wy;
	pM->m[2][1] = yz - wx;
	pM->m[2][2] = 1.0f - ( xx + yy );
	pM->m[2][3] = 0.0f;

	pM->m[3][0] = 0.0f;
	pM->m[3][1] = 0.0f;
	pM->m[3][2] = 0.0f;
	pM->m[3][3] = 1.0f;
}

/*=============================================================================
*
* Purpose : 行列の乗算．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsMatrix4Multiply
(
	GSmatrix4*			pOut,			/*-> 掛け算の結果   */
	const GSmatrix4*	pM1,			/* ３Ｄマトリクス   */
	const GSmatrix4*	pM2				/* ３Ｄマトリクス   */
)
{
	GSmatrix4			M;
	int					i, j;

	for ( i = 0; i < 4; i++ )
	{
		for ( j = 0; j < 4; j++ )
		{
			M.m[i][j] = pM1->m[i][0] * pM2->m[0][j]
					  + pM1->m[i][1] * pM2->m[1][j]
					  + pM1->m[i][2] * pM2->m[2][j]
					  + pM1->m[i][3] * pM2->m[3][j];
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
gsMatrix4MultiplyScalar
(
	GSmatrix4*			pOut,			/*-> 掛け算の結果   */
	const GSmatrix4*	pM1,			/* ３Ｄマトリクス   */
	GSfloat				Scalar			/* スカラー         */
)
{
	int					i, j;

	for ( i = 0; i < 4; i++ )
	{
		for ( j = 0; j < 4; j++ )
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
gsMatrix4Add
(
	GSmatrix4*			pOut,			/*-> 加算の結果     */
	const GSmatrix4*	pM1,			/* ３Ｄマトリクス   */
	const GSmatrix4*	pM2				/* ３Ｄマトリクス   */
)
{
	int					i, j;

	for ( i = 0; i < 4; i++ )
	{
		for ( j = 0; j < 4; j++ )
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
gsMatrix4Subtract
(
	GSmatrix4*			pOut,			/*-> 減算の結果     */
	const GSmatrix4*	pM1,			/* ３Ｄマトリクス   */
	const GSmatrix4*	pM2				/* ３Ｄマトリクス   */
)
{
	int					i, j;

	for ( i = 0; i < 4; i++ )
	{
		for ( j = 0; j < 4; j++ )
		{
			pOut->m[i][j] = pM1->m[i][j] - pM2->m[i][j];
		}
	}
}

/*=============================================================================
*
* Purpose : 透視変換行列を求める．（右手座標系）
*
* Return  : なし．
*
*============================================================================*/
extern void
gsMatrix4PerspectiveRH
(
	GSmatrix4*	pM,						/* 透視変換マトリクス			*/
	GSfloat		Fov,					/* 視野角						*/
	GSfloat		Aspect,					/* アスペクト比					*/
	GSfloat 	zNear,					/* 近クリップ面までの距離		*/
	GSfloat 	zFar					/* 遠クリップ面までの距離		*/
)
{
	GSfloat			f;

	f = gsCos( Fov / 2.0f ) / gsSin( Fov / 2.0f );

	pM->m[0][0] = f / Aspect;
	pM->m[0][1] = 0;
	pM->m[0][2] = 0;
	pM->m[0][3] = 0;

	pM->m[1][0] = 0;
	pM->m[1][1] = f;
	pM->m[1][2] = 0;
	pM->m[1][3] = 0;

	pM->m[2][0] = 0;
	pM->m[2][1] = 0;
	pM->m[2][2] = ( zFar + zNear ) / ( zNear - zFar );
	pM->m[2][3] = -1.0f;

	pM->m[3][0] = 0;
	pM->m[3][1] = 0;
	pM->m[3][2] = ( 2.0f * zFar * zNear ) / ( zNear - zFar );
	pM->m[3][3] = 0;
}

/*=============================================================================
*
* Purpose : 透視変換行列を求める．（左手座標系）
*
* Return  : なし．
*
*============================================================================*/
extern void
gsMatrix4PerspectiveLH
(
	GSmatrix4*	pM,						/* 透視変換マトリクス			*/
	GSfloat		Fov,					/* 視野角						*/
	GSfloat		Aspect,					/* アスペクト比					*/
	GSfloat 	zNear,					/* 近クリップ面までの距離		*/
	GSfloat 	zFar					/* 遠クリップ面までの距離		*/
)
{
	GSfloat		f;

	f = gsCos( Fov / 2.0f ) / gsSin( Fov / 2.0f );

	pM->m[0][0] = f / Aspect;
	pM->m[0][1] = 0;
	pM->m[0][2] = 0;
	pM->m[0][3] = 0;

	pM->m[1][0] = 0;
	pM->m[1][1] = f;
	pM->m[1][2] = 0;
	pM->m[1][3] = 0;

	pM->m[2][0] = 0;
	pM->m[2][1] = 0;
	pM->m[2][2] = ( zFar + zNear ) / ( zNear - zFar );
	pM->m[2][3] = 1.0f;

	pM->m[3][0] = 0;
	pM->m[3][1] = 0;
	pM->m[3][2] = ( 2.0f * zFar * zNear ) / ( zNear - zFar );
	pM->m[3][3] = 0;
}

/*=============================================================================
*
* Purpose : 透視変換行列を求める．（右手座標系）
*
* Return  : なし．
*
*============================================================================*/
void
gsMatrix4FrustumRH
(
	GSmatrix4*	pM,						/* 透視変換マトリクス			*/
	GSfloat		Left,					/* クリップ面の左側の座標		*/
	GSfloat		Right,					/* クリップ面の右側の座標		*/
	GSfloat		Bottom,					/* クリップ面の下側の座標		*/
	GSfloat		Top,					/* クリップ面の上側の座標		*/
	GSfloat		zNear,					/* 近クリップ面の座標			*/
	GSfloat		zFar					/* 遠クリップ面の座標			*/
)
{
	pM->m[0][0] = ( 2.0f * zNear ) / ( Right - Left );
	pM->m[0][1] = 0.0f;  
	pM->m[0][2] = 0.0f;
	pM->m[0][3] = 0.0f;

	pM->m[1][0] = 0.0f;  
	pM->m[1][1] = ( 2.0f * zNear ) / ( Top - Bottom );
	pM->m[1][2] = 0.0f;
	pM->m[1][3] = 0.0f;

	pM->m[2][0] = ( Right + Left ) / ( Right - Left );
	pM->m[2][1] = ( Top + Bottom ) / ( Top - Bottom );
	pM->m[2][2] = -( zFar + zNear ) / ( zFar - zNear );
	pM->m[2][3] = -1.0f;

	pM->m[3][0] = 0.0f;
	pM->m[3][1] = 0.0f;
	pM->m[3][2] = -( 2.0f * zFar * zNear ) / ( zFar - zNear );
	pM->m[3][3] = 0.0f;
}

/*=============================================================================
*
* Purpose : 透視変換行列を求める．（左手座標系）
*
* Return  : なし．
*
*============================================================================*/
void
gsMatrix4FrustumLH
(
	GSmatrix4*	pM,						/* 透視変換マトリクス			*/
	GSfloat		Left,					/* クリップ面の左側の座標		*/
	GSfloat		Right,					/* クリップ面の右側の座標		*/
	GSfloat		Bottom,					/* クリップ面の下側の座標		*/
	GSfloat		Top,					/* クリップ面の上側の座標		*/
	GSfloat		zNear,					/* 近クリップ面の座標			*/
	GSfloat		zFar					/* 遠クリップ面の座標			*/
)
{
	pM->m[0][0] = ( 2.0f * zNear ) / ( Right - Left );
	pM->m[0][1] = 0.0f;  
	pM->m[0][2] = 0.0f;
	pM->m[0][3] = 0.0f;

	pM->m[1][0] = 0.0f;  
	pM->m[1][1] = ( 2.0f * zNear ) / ( Top - Bottom );
	pM->m[1][2] = 0.0f;
	pM->m[1][3] = 0.0f;

	pM->m[2][0] = ( Right + Left ) / ( Right - Left );
	pM->m[2][1] = ( Top + Bottom ) / ( Top - Bottom );
	pM->m[2][2] = -( zFar + zNear ) / ( zFar - zNear );
	pM->m[2][3] = 1.0f;

	pM->m[3][0] = 0.0f;
	pM->m[3][1] = 0.0f;
	pM->m[3][2] = -( 2.0f * zFar * zNear ) / ( zFar - zNear );
	pM->m[3][3] = 0.0f;
}

/*=============================================================================
*
* Purpose : 正射影変換行列を求める．（右手座標系）
*
* Return  : なし．
*
*============================================================================*/
extern void
gsMatrix4OrthoRH
(
	GSmatrix4*	pM,						/* 正射影変換マトリクス		*/
	GSfloat		Left,					/* クリップ面の左側の座標	*/
	GSfloat		Right,					/* クリップ面の右側の座標	*/
	GSfloat		Bottom,					/* クリップ面の下側の座標	*/
	GSfloat		Top,					/* クリップ面の上側の座標	*/
	GSfloat		zNear,					/* 近クリップ面の座標		*/
	GSfloat		zFar					/* 遠クリップ面の座標		*/
)
{
	pM->m[0][0] = 2.0f / ( Right - Left );
	pM->m[0][1] = 0;
	pM->m[0][2] = 0;
	pM->m[0][3] = 0; 

	pM->m[1][0] = 0;
	pM->m[1][1] = 2.0f / ( Top - Bottom );
	pM->m[1][2] = 0;
	pM->m[1][3] = 0;

	pM->m[2][0] = 0;
	pM->m[2][1] = 0;
	pM->m[2][2] = -2.0f / ( zFar - zNear );
	pM->m[2][3] = 0; 

	pM->m[3][0] = -( Right + Left   ) / ( Right - Left   );
	pM->m[3][1] = -( Top   + Bottom ) / ( Top   - Bottom );
	pM->m[3][2] = -( zFar  + zNear  ) / ( zFar  - zNear  );
	pM->m[3][3] = 1;
}

/*=============================================================================
*
* Purpose : 正射影変換行列を求める．（左手座標系）
*
* Return  : なし．
*
*============================================================================*/
extern void
gsMatrix4OrthoLH
(
	GSmatrix4*	pM,						/* 正射影変換マトリクス		*/
	GSfloat		Left,					/* クリップ面の左側の座標	*/
	GSfloat		Right,					/* クリップ面の右側の座標	*/
	GSfloat		Bottom,					/* クリップ面の下側の座標	*/
	GSfloat		Top,					/* クリップ面の上側の座標	*/
	GSfloat		zNear,					/* 近クリップ面の座標		*/
	GSfloat		zFar					/* 遠クリップ面の座標		*/
)
{
	pM->m[0][0] = 2.0f / ( Right - Left );
	pM->m[0][1] = 0;
	pM->m[0][2] = 0;
	pM->m[0][3] = 0; 

	pM->m[1][0] = 0;
	pM->m[1][1] = 2.0f / ( Top - Bottom );
	pM->m[1][2] = 0;
	pM->m[1][3] = 0;

	pM->m[2][0] = 0;
	pM->m[2][1] = 0;
	pM->m[2][2] = 2.0f / ( zFar - zNear );
	pM->m[2][3] = 0; 

	pM->m[3][0] = -( Right + Left   ) / ( Right - Left   );
	pM->m[3][1] = -( Top   + Bottom ) / ( Top   - Bottom );
	pM->m[3][2] = -( zFar  + zNear  ) / ( zFar  - zNear  );
	pM->m[3][3] = 1;
}

/*=============================================================================
*
* Purpose : 視野変換行列を求める．（右手座標系）
*
* Return  : なし．
*
*============================================================================*/
extern void
gsMatrix4LookAtRH
(
	GSmatrix4*			pM,				/* 視点変換マトリクス   */
	const GSvector3*	from,			/* 視点の座標           */
	const GSvector3*	at,				/* 目標点の座標         */
	const GSvector3*	up				/* 視点の上側の方向     */
)
{
	GSvector3			xaxis;
	GSvector3			yaxis;
	GSvector3			zaxis;

	/* ｚ方向のベクトルを求める */
	gsVector3Subtract( &zaxis, from, at );
	gsVector3Normalize( &zaxis, &zaxis );

	/* ｘ方向のベクトルを求める */
	gsVector3Cross( &xaxis, up, &zaxis );
	gsVector3Normalize( &xaxis, &xaxis );

	/* ｙ方向のベクトルを求める */
	gsVector3Cross( &yaxis, &zaxis, &xaxis );

	/* 回転行列に各ベクトルを設定する */
	pM->m[0][0] = xaxis.x;
	pM->m[0][1] = yaxis.x;
	pM->m[0][2] = zaxis.x;
	pM->m[0][3] = 0.0f;

	pM->m[1][0] = xaxis.y;
	pM->m[1][1] = yaxis.y;
	pM->m[1][2] = zaxis.y;
	pM->m[1][3] = 0.0f;

	pM->m[2][0] = xaxis.z;
	pM->m[2][1] = yaxis.z;
	pM->m[2][2] = zaxis.z;
	pM->m[2][3] = 0.0f;

	/* 視点の位置を設定する */
	pM->m[3][0] = -gsVector3Dot( from, &xaxis );
	pM->m[3][1] = -gsVector3Dot( from, &yaxis );
	pM->m[3][2] = -gsVector3Dot( from, &zaxis );
	pM->m[3][3] = 1.0f;
}

/*=============================================================================
*
* Purpose : 視野変換行列を求める．（左手座標系）
*
* Return  : なし．
*
*============================================================================*/
extern void
gsMatrix4LookAtLH
(
	GSmatrix4*			pM,				/* 視点変換マトリクス   */
	const GSvector3*	from,			/* 視点の座標           */
	const GSvector3*	at,				/* 目標点の座標         */
	const GSvector3*	up				/* 視点の上側の方向     */
)
{
	GSvector3			xaxis;
	GSvector3			yaxis;
	GSvector3			zaxis;

	/* ｚ方向のベクトルを求める */
	gsVector3Subtract( &zaxis, at, from );
	gsVector3Normalize( &zaxis, &zaxis );

	/* ｘ方向のベクトルを求める */
	gsVector3Cross( &xaxis, up, &zaxis );
	gsVector3Normalize( &xaxis, &xaxis );

	/* ｙ方向のベクトルを求める */
	gsVector3Cross( &yaxis, &zaxis, &xaxis );

	/* 回転行列に各ベクトルを設定する */
	pM->m[0][0] = xaxis.x;
	pM->m[0][1] = yaxis.x;
	pM->m[0][2] = zaxis.x;
	pM->m[0][3] = 0.0f;

	pM->m[1][0] = xaxis.y;
	pM->m[1][1] = yaxis.y;
	pM->m[1][2] = zaxis.y;
	pM->m[1][3] = 0.0f;

	pM->m[2][0] = xaxis.z;
	pM->m[2][1] = yaxis.z;
	pM->m[2][2] = zaxis.z;
	pM->m[2][3] = 0.0f;

	/* 視点の位置を設定する */
	pM->m[3][0] = -gsVector3Dot( from, &xaxis );
	pM->m[3][1] = -gsVector3Dot( from, &yaxis );
	pM->m[3][2] = -gsVector3Dot( from, &zaxis );
	pM->m[3][3] = 1.0f;
}

/*=============================================================================
*
* Purpose : 視野変換行列に変換する．（右手座標系）
*
* Return  : なし．
*
*============================================================================*/
extern void
gsMatrix4ConvViewRH
(
	GSmatrix4*			pOut,			/* 視野変換マトリクス     */
	const GSmatrix4*	pM				/* ３Ｄマトリクス       */
)
{
	GSvector3 from;
	GSvector3 at;
	GSvector3 up;
	from.x = pM->m[3][0];
	from.y = pM->m[3][1];
	from.z = pM->m[3][2];
	at.x  = pM->m[2][0] + pM->m[3][0];
	at.y  = pM->m[2][1] + pM->m[3][1];
	at.z  = pM->m[2][2] + pM->m[3][2];
	up.x  = pM->m[1][0];
	up.y  = pM->m[1][1];
	up.z  = pM->m[1][2];
	gsMatrix4LookAtRH(pOut, &from, &at, &up);
}

/*=============================================================================
*
* Purpose : 視野変換行列の逆行変換をする．（右手座標系）
*
* Return  : なし．
*
*============================================================================*/
extern void
gsMatrix4InverseLookAtRH
(
	const GSmatrix4*	pM,				/* 視点変換マトリクス    */
	GSvector3*			pFrom,			/*-> 視点の座標          */
	GSvector3*			pAt,			/*-> 目標点の座標        */
	GSvector3*			pUp				/*-> 視点の上側の方向    */
)
{
	GSmatrix4			invMat;
	gsMatrix4InverseFast(&invMat, pM);
	if (pFrom != NULL) {
		pFrom->x = invMat.m[3][0];
		pFrom->y = invMat.m[3][1];
		pFrom->z = invMat.m[3][2];
	}
	if (pAt != NULL) {
		pAt->x = invMat.m[3][0] - invMat.m[2][0];
		pAt->y = invMat.m[3][1] - invMat.m[2][1];
		pAt->z = invMat.m[3][2] - invMat.m[2][2];
	}
	if (pUp != NULL) {
		pUp->x = invMat.m[1][0];
		pUp->y = invMat.m[1][1];
		pUp->z = invMat.m[1][2];
	}
}

/*=============================================================================
*
* Purpose : 視野変換行列に変換する．（左手座標系）
*
* Return  : なし．
*
*============================================================================*/
extern void
gsMatrix4ConvViewLH
(
	GSmatrix4*			pOut,			/* 視野変換マトリクス     */
	const GSmatrix4*	pM				/* ３Ｄマトリクス       */
)
{
	GSvector3 from;
	GSvector3 at;
	GSvector3 up;
	from.x = pM->m[3][0];
	from.y = pM->m[3][1];
	from.z = pM->m[3][2];
	at.x   = pM->m[2][0] + pM->m[3][0];
	at.y   = pM->m[2][1] + pM->m[3][1];
	at.z   = pM->m[2][2] + pM->m[3][2];
	up.x   = pM->m[1][0];
	up.y   = pM->m[1][1];
	up.z   = pM->m[1][2];
	gsMatrix4LookAtLH(pOut, &from, &at, &up);
}

/*=============================================================================
*
* Purpose : 透視変換行列の逆変換．（右手座標系）
*
* Return  : なし．
*
*============================================================================*/
extern void
gsMatrix4InversePerspectiveRH
(
	const GSmatrix4*	pM,				/* 透視変換マトリクス			*/
	GSfloat*			pFov,			/*-> 視野角						*/
	GSfloat*			pAspect,		/*-> アスペクト比				*/
	GSfloat*		 	pNear,			/*-> 近クリップ面までの距離		*/
	GSfloat* 			pFar			/*-> 遠クリップ面までの距離		*/
)
{
	static const GSvector3 NEAR_CLIP = { 0.0f, 0.0f, -1.0f };
	static const GSvector3 FAR_CLIP = { 0.0f, 0.0f,  1.0f };
	static const GSvector3 FAR_RIGHT = { 1.0f, 0.0f,  1.0f };
	static const GSvector3 FAR_TOP = { 0.0f, 1.0f,  1.0f };
	GSmatrix4 invProjection;
	GSvector3 invNear;
	GSvector3 invFar;
	GSvector3 invFarRight;
	GSvector3 invFarTop;
	float result_n;
	float result_f;
	float result_aspect;
	float result_fov;
	gsMatrix4Inverse(&invProjection, pM);
	gsVector3TransformCoord(&invNear, &NEAR_CLIP, &invProjection);
	gsVector3TransformCoord(&invFar, &FAR_CLIP, &invProjection);
	gsVector3TransformCoord(&invFarRight, &FAR_RIGHT, &invProjection);
	gsVector3TransformCoord(&invFarTop, &FAR_TOP, &invProjection);
	result_n = -invNear.z;
	result_f = -invFar.z;
	result_aspect = invFarRight.x / invFarTop.y;
	result_fov = gsAtan(result_f, invFarRight.x / result_aspect) * 2.0f;
	if (pNear   != NULL) *pNear   = result_n;
	if (pFar    != NULL) *pFar    = result_f;
	if (pAspect != NULL) *pAspect = result_aspect;
	if (pFov    != NULL) *pFov    = result_fov;
}

/*=============================================================================
*
* Purpose : 透視変換行列の逆変換．（右手座標系）
*
* Return  : なし．
*
*============================================================================*/
extern void
gsMatrix4InverseOrthoRH
(
	const GSmatrix4*	pM,				/* 正射影変換マトリクス		*/
	GSfloat*			pLeft,			/*-> クリップ面の左側の座標	*/
	GSfloat*			pRight,			/*-> クリップ面の右側の座標	*/
	GSfloat*			pBottom,		/*-> クリップ面の下側の座標	*/
	GSfloat*			pTop,			/*-> クリップ面の上側の座標	*/
	GSfloat*			pNear,			/*-> 近クリップ面の座標		*/
	GSfloat*			pFar			/*-> 遠クリップ面の座標		*/
)
{
	static const GSvector3 MIN_POSITION = { -1.0f, -1.0f, -1.0f };
	static const GSvector3 MAX_POSITION = { 1.0f,  1.0f,  1.0f };
	GSmatrix4 invProjection;
	GSvector3 invMin;
	GSvector3 invMax;
	gsMatrix4Inverse(&invProjection, pM);
	gsVector3TransformCoord(&invMin, &MIN_POSITION, &invProjection);
	gsVector3TransformCoord(&invMax, &MAX_POSITION, &invProjection);
	if (pLeft   != NULL) *pLeft   =  invMin.x;
	if (pRight  != NULL) *pRight  =  invMax.x;
	if (pBottom != NULL) *pBottom =  invMin.y;
	if (pTop    != NULL) *pTop    =  invMax.y;
	if (pNear   != NULL) *pNear   = -invMin.z;
	if (pFar    != NULL) *pFar    = -invMax.z;
}

/*=============================================================================
*
* Purpose : スクリーン座標変換行列を求める．（左上原点）
*
* Return  : なし．
*
*============================================================================*/
extern void
gsMatrix4Screen
(
	GSmatrix4*			pM,				/* スクリーン座標変換行列	*/
	GSfloat				width,			/* ビューポートの幅			*/
	GSfloat				height			/* ビューポートの高さ		*/
)
{
	GSfloat w = width  / 2.0f;
	GSfloat h = height / 2.0f;
	pM->m[0][0] = w;
	pM->m[0][1] = 0.0f;
	pM->m[0][2] = 0.0f;
	pM->m[0][3] = 0.0f;
	pM->m[1][0] = 0.0f;
	pM->m[1][1] = -h;
	pM->m[1][2] = 0.0f;
	pM->m[1][3] = 0.0f;
	pM->m[2][0] = 0.0f;
	pM->m[2][1] = 0.0f;
	pM->m[2][2] = 1.0f;
	pM->m[2][3] = 0.0f;
	pM->m[3][0] = w;
	pM->m[3][1] = h;
	pM->m[3][2] = 0.0f;
	pM->m[3][3] = 1.0f;
}

/*=============================================================================
*
* Purpose : 転置行列を求める．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsMatrix4Transpose
(
	GSmatrix4*			pOut,			/* 転置３Ｄマトリクス   */
	const GSmatrix4*	pM				/* ３Ｄマトリクス       */
)
{
	int					i, j;
	GSmatrix4			M;

	/* マトリクスの行と列を交換する */
	for ( i = 0; i < 4; i++ )
	{
		for ( j = 0; j < 4; j++ )
		{
			M.m[i][j] = pM->m[j][i];
		}
	}
	*pOut = M;
}

/*=============================================================================
*
* Purpose : 逆行列を求める．（ スケーリングされていない行列のみ有効 ）
*
* Return  : なし．
*
*============================================================================*/
extern void
gsMatrix4InverseFast
(
	GSmatrix4*			pOut,			/* 逆３Ｄマトリクス     */
	const GSmatrix4*	pM				/* ３Ｄマトリクス       */
)
{
	GSmatrix4			M;

	M.m[0][0] = pM->m[0][0];
	M.m[1][0] = pM->m[0][1];
	M.m[2][0] = pM->m[0][2];

	M.m[0][1] = pM->m[1][0];
	M.m[1][1] = pM->m[1][1];
	M.m[2][1] = pM->m[1][2];

	M.m[0][2] = pM->m[2][0];
	M.m[1][2] = pM->m[2][1];
	M.m[2][2] = pM->m[2][2];

	M.m[3][0] = -( pM->m[3][0] * M.m[0][0]
	             + pM->m[3][1] * M.m[1][0]
	             + pM->m[3][2] * M.m[2][0] );
	M.m[3][1] = -( pM->m[3][0] * M.m[0][1] 
	             + pM->m[3][1] * M.m[1][1]
	             + pM->m[3][2] * M.m[2][1] );
	M.m[3][2] = -( pM->m[3][0] * M.m[0][2]
	             + pM->m[3][1] * M.m[1][2]
	             + pM->m[3][2] * M.m[2][2] );

	M.m[0][3] = pM->m[0][3];
	M.m[1][3] = pM->m[1][3];
	M.m[2][3] = pM->m[2][3];
	M.m[3][3] = pM->m[3][3];

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
gsMatrix4Inverse
(
	GSmatrix4*			pOut,			/* 逆３Ｄマトリクス     */
	const GSmatrix4*	pM				/* ３Ｄマトリクス       */
)
{
	GSmatrix4			M;
	GSfloat				d12;
	GSfloat				d13;
	GSfloat				d23;
	GSfloat				d24;
	GSfloat				d34;
	GSfloat				d41;
	GSfloat				invDet;
	GSfloat				det;

/* 可読性を良くするための一時マクロ定義 */
#define MAT( r,c )	(pM->m[(c)][(r)])

#define m11 MAT(0,0)
#define m12 MAT(0,1)
#define m13 MAT(0,2)
#define m14 MAT(0,3)
#define m21 MAT(1,0)
#define m22 MAT(1,1)
#define m23 MAT(1,2)
#define m24 MAT(1,3)
#define m31 MAT(2,0)
#define m32 MAT(2,1)
#define m33 MAT(2,2)
#define m34 MAT(2,3)
#define m41 MAT(3,0)
#define m42 MAT(3,1)
#define m43 MAT(3,2)
#define m44 MAT(3,3)

	d12 = ( m31 * m42 - m41 * m32 );
	d13 = ( m31 * m43 - m41 * m33 );
	d23 = ( m32 * m43 - m42 * m33 );
	d24 = ( m32 * m44 - m42 * m34 );
	d34 = ( m33 * m44 - m43 * m34 );
	d41 = ( m34 * m41 - m44 * m31 );

	M.m[0][0] =  ( m22 * d34 - m23 * d24 + m24 * d23 );
	M.m[0][1] = -( m21 * d34 + m23 * d41 + m24 * d13 );
	M.m[0][2] =  ( m21 * d24 + m22 * d41 + m24 * d12 );
	M.m[0][3] = -( m21 * d23 - m22 * d13 + m23 * d12 );

	det = m11 * M.m[0][0]
		+ m12 * M.m[0][1]
		+ m13 * M.m[0][2]
		+ m14 * M.m[0][3];

	if( det == 0.0 )
	{
		/* 逆行列を求める事ができない */
		gsMatrix4Identity( pOut );

		return	GS_FALSE;
	}

	invDet = 1.0f / det;

	M.m[0][0] *= invDet;
	M.m[0][1] *= invDet;
	M.m[0][2] *= invDet;
	M.m[0][3] *= invDet;

	M.m[1][0] = -( m12 * d34 - m13 * d24 + m14 * d23 ) * invDet;
	M.m[1][1] =  ( m11 * d34 + m13 * d41 + m14 * d13 ) * invDet;
	M.m[1][2] = -( m11 * d24 + m12 * d41 + m14 * d12 ) * invDet;
	M.m[1][3] =  ( m11 * d23 - m12 * d13 + m13 * d12 ) * invDet;

	d12 = m11 * m22 - m21 * m12;
	d13 = m11 * m23 - m21 * m13;
	d23 = m12 * m23 - m22 * m13;
	d24 = m12 * m24 - m22 * m14;
	d34 = m13 * m24 - m23 * m14;
	d41 = m14 * m21 - m24 * m11;

	M.m[2][0] =  ( m42 * d34 - m43 * d24 + m44 * d23 ) * invDet;
	M.m[2][1] = -( m41 * d34 + m43 * d41 + m44 * d13 ) * invDet;
	M.m[2][2] =  ( m41 * d24 + m42 * d41 + m44 * d12 ) * invDet;
	M.m[2][3] = -( m41 * d23 - m42 * d13 + m43 * d12 ) * invDet;
	M.m[3][0] = -( m32 * d34 - m33 * d24 + m34 * d23 ) * invDet;
	M.m[3][1] =  ( m31 * d34 + m33 * d41 + m34 * d13 ) * invDet;
	M.m[3][2] = -( m31 * d24 + m32 * d41 + m34 * d12 ) * invDet;
	M.m[3][3] =  ( m31 * d23 - m32 * d13 + m33 * d12 ) * invDet;

	*pOut = M;

/* 一時マクロを全て削除する */
#undef m11
#undef m12
#undef m13
#undef m14
#undef m21
#undef m22
#undef m23
#undef m24
#undef m31
#undef m32
#undef m33
#undef m34
#undef m41
#undef m42
#undef m43
#undef m44
#undef MAT

	return	GS_TRUE;
}

/*=============================================================================
*
* Purpose : 地平面シャドウマトリクスを求める．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsMatrix4Shadow
(
	GSmatrix4*			pM,			/* 影変換行列			*/
	const GSplane*		pPlane,		/* 平面のパラメータ		*/
	const GSvector3*	pLightPos	/* ライトの位置			*/
)
{
	GSfloat		dot;

	dot = gsPlaneDotCoord( pPlane, pLightPos );

	pM->m[0][0] = dot - pLightPos->x * pPlane->a;
	pM->m[1][0] = -pLightPos->x * pPlane->b;
	pM->m[2][0] = -pLightPos->x * pPlane->c;
	pM->m[3][0] = -pLightPos->x * pPlane->d;

	pM->m[0][1] = -pLightPos->y * pPlane->a;
	pM->m[1][1] = dot - pLightPos->y * pPlane->b;
	pM->m[2][1] = -pLightPos->y * pPlane->c;
	pM->m[3][1] = -pLightPos->y * pPlane->d;

	pM->m[0][2] = -pLightPos->z * pPlane->a;
	pM->m[1][2] = -pLightPos->z * pPlane->b;
	pM->m[2][2] = dot - pLightPos->z * pPlane->c;
	pM->m[3][2] = -pLightPos->z * pPlane->d;

	pM->m[0][3] = -pPlane->a;
	pM->m[1][3] = -pPlane->b;
	pM->m[2][3] = -pPlane->c;
	pM->m[3][3] = dot - pPlane->d;
}

/*=============================================================================
*
* Purpose : ヨー，ピッチ，ロールから回転行列を求める．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsMatrix4YawPitchRoll
(
	GSmatrix4*			pOut,			/* 回転マトリクス					*/
	GSfloat				Yaw,			/*-> ヨー  （ｙ軸周りの回転角度）	*/
	GSfloat				Pitch,			/*-> ピッチ（ｘ軸周りの回転角度）	*/
	GSfloat				Roll			/*-> ロール（Ｚ軸周リの回転角度）	*/
)
{
	GSmatrix4			matRoll;
	GSmatrix4			matPitch;
	GSmatrix4			matYaw;

	/* ロール，ピッチ，ヨーから回転マトリクスを求める */
	gsMatrix4RotateZ( &matRoll,  Roll  );
	gsMatrix4RotateX( &matPitch, Pitch );
	gsMatrix4RotateY( &matYaw,   Yaw   );

	/* 回転マトリクスを合成する */
	gsMatrix4Multiply( pOut, &matRoll, &matPitch );
	gsMatrix4Multiply( pOut, pOut, &matYaw );
}

/*=============================================================================
*
* Purpose : 回転行列からピッチを求める．
*
* Return  : なし．
*
*============================================================================*/
extern GSfloat
gsMatrix4GetPitch
(
	const GSmatrix4*	pM				/* 行列								*/
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
gsMatrix4GetRoll
(
	const GSmatrix4*	pM				/* 行列								*/
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
gsMatrix4GetYaw
(
	const GSmatrix4*	pM				/* 行列								*/
)
{
	/* ヨーを求める */
	return	gsAtan(pM->m[2][2], pM->m[2][0]);
}

/*=============================================================================
*
* Purpose : 視推台のコーナーの座標を求める．
*
* Return  : 視錐台内に存在すれば真，存在しなければ偽を返す．
*
*============================================================================*/
extern void
gsMatrix4GetFrustumCorners
(
	GSvector3			Corners[8],			/*-> 視推台のコーナーの座標		*/
	const GSmatrix4*	pView,				/* 視野変換行列			*/
	const GSmatrix4*	pProjection			/* 透視変換行列			*/
)
{
	static const GSvector3 NEAR_FAR_POSITION[] = {
		{ -1.0f,  1.0f,  -1.0f }, // near left top
		{ -1.0f, -1.0f,  -1.0f }, // near left bottom
		{  1.0f, -1.0f,  -1.0f }, // near right bottom
		{  1.0f,  1.0f,  -1.0f }, // near right top
		{ -1.0f,  1.0f,   1.0f }, // far left top
		{ -1.0f, -1.0f,   1.0f }, // far left bottom
		{  1.0f, -1.0f,   1.0f }, // far right bottom
		{  1.0f,  1.0f,   1.0f }, // far right top
	};
	GSmatrix4 view_projection;
	GSmatrix4 inv_view_projection;
	gsMatrix4Multiply(&view_projection, pView, pProjection);
	gsMatrix4Inverse(&inv_view_projection, &view_projection);
	for (int i = 0; i < 8; ++i) {
		gsVector3TransformCoord(&Corners[i], &NEAR_FAR_POSITION[i], &inv_view_projection);
	}
}

/********** End of File ******************************************************/
