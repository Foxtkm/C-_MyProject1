/******************************************************************************
* File Name : GSmatrix4.h                        Ver : 1.00  Date : 1998-05-03
*
* Description :
*
*       三次元座標変換行列計算 ヘッダファイル．
*
* Author : Moriharu Ohzu.
*
******************************************************************************/
#ifndef		_GSMATRIX4_H_
#define		_GSMATRIX4_H_

#include	"GStype.h"
#include	"GSvector2.h"
#include	"GSvector3.h"
#include	"GSvector4.h"
#include	"GSquaternion.h"
#include	"GSmatrix3.h"
#include    "GSplane.h"
#include	<float.h>

/****** 関数プロトタイプ宣言 *************************************************/

#ifdef __cplusplus
extern "C" {
#endif

extern const GSmatrix4 GS_MATRIX4_IDENTITY; /* 単位行列 */

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
	GSmatrix4*			pM				/* ３Ｄマトリクス   */
);

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
	GSmatrix4*			pM,				/* ３Ｄマトリクス       */
	GSfloat				tx,				/* ｘ方向移動量			*/
	GSfloat				ty,				/* ｙ方向移動量			*/
	GSfloat				tz				/* ｚ方向移動量			*/
);

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
	GSmatrix4*			pM,				/* ３Ｄマトリクス       */
	GSfloat				sx,				/* ｘ方向スケール値		*/
	GSfloat				sy,				/* ｙ方向スケール値		*/
	GSfloat				sz				/* ｚ方向スケール値		*/
);

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
	GSmatrix4*			pM,				/* ３Ｄマトリクス       */
	GSfloat				angle,			/* 回転角度				*/
	GSfloat				dx,				/* 回転軸のｘ成分		*/
	GSfloat				dy,				/* 回転軸のｙ成分		*/
	GSfloat				dz				/* 回転軸のｚ成分		*/
);

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
	GSmatrix4*			pM,				/* ３Ｄマトリクス       */
	GSfloat				angle			/* ｘ軸周りの角度       */
);

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
	GSmatrix4*			pM,				/* ３Ｄマトリクス       */
	GSfloat				angle			/* ｙ軸周りの角度       */
);

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
	GSmatrix4*			pM,				/* ３Ｄマトリクス       */
	GSfloat				angle			/* ｚ軸周りの角度       */
);

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
);

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
	GSmatrix4*			pOut,			/*-> 乗算の結果     */
	const GSmatrix4*	pM1,			/* ３Ｄマトリクス   */
	const GSmatrix4*	pM2				/* ３Ｄマトリクス   */
);

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
);

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
);

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
);

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
);

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
	GSmatrix4*			pOut,			/* 視野変換マトリクス    */
	const GSmatrix4*	pM				/* ３Ｄマトリクス        */
);

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
);

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
);

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
);

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
);

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
);

/*=============================================================================
*
* Purpose : 透視変換行列を求める．（右手座標系）
*
* Return  : なし．
*
*============================================================================*/
extern void
gsMatrix4FrustumRH
(
	GSmatrix4*	pM,						/* 透視変換マトリクス			*/
	GSfloat		Left,					/* クリップ面の左側の座標		*/
	GSfloat		Right,					/* クリップ面の右側の座標		*/
	GSfloat		Bottom,					/* クリップ面の下側の座標		*/
	GSfloat		Top,					/* クリップ面の上側の座標		*/
	GSfloat		zNear,					/* 近クリップ面の座標			*/
	GSfloat		zFar					/* 遠クリップ面の座標			*/
);

/*=============================================================================
*
* Purpose : 透視変換行列を求める．（左手座標系）
*
* Return  : なし．
*
*============================================================================*/
extern void
gsMatrix4FrustumLH
(
	GSmatrix4*	pM,						/* 透視変換マトリクス			*/
	GSfloat		Left,					/* クリップ面の左側の座標		*/
	GSfloat		Right,					/* クリップ面の右側の座標		*/
	GSfloat		Bottom,					/* クリップ面の下側の座標		*/
	GSfloat		Top,					/* クリップ面の上側の座標		*/
	GSfloat		zNear,					/* 近クリップ面の座標			*/
	GSfloat		zFar					/* 遠クリップ面の座標			*/
);

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
);

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
);

/*=============================================================================
*
* Purpose : 正射影変換行列を求める．（右手座標系）
*
* Return  : なし．
*
*============================================================================*/
#define	gsMatrix4Ortho2DRH( pM, Left, Right, Bottom, Top )			\
(																	\
	gsMatrix4OrthoRH( pM, Left, Right, Bottom, Top, -1.0f, 1.0f )	\
)

/*=============================================================================
*
* Purpose : 正射影変換行列を求める．（右手座標系）
*
* Return  : なし．
*
*============================================================================*/
#define	gsMatrix4Ortho2DLH( pM, Left, Right, Bottom, Top )			\
(																	\
	gsMatrix4OrthoLH( pM, Left, Right, Bottom, Top, -1.0f, 1.0f )	\
)

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

#ifdef __cplusplus
}
#endif

/*----- Ｃ＋＋ラッパークラス ------------------------------------------------*/

#ifdef __cplusplus

/* コンストラクタ */
GSINLINE GSmatrix4::GSmatrix4(
	GSfloat	m11, GSfloat m12, GSfloat m13, GSfloat m14,
	GSfloat	m21, GSfloat m22, GSfloat m23, GSfloat m24,
	GSfloat	m31, GSfloat m32, GSfloat m33, GSfloat m34,
	GSfloat	m41, GSfloat m42, GSfloat m43, GSfloat m44  ) :
		_11( m11 ), _12( m12 ), _13( m13 ), _14( m14 ),
		_21( m21 ), _22( m22 ), _23( m23 ), _24( m24 ),
		_31( m31 ), _32( m32 ), _33( m33 ), _34( m34 ),
		_41( m41 ), _42( m42 ), _43( m43 ), _44( m44 )
{}

GSINLINE GSmatrix4::GSmatrix4(
	const GSvector4& axisX,
	const GSvector4& axisY,
	const GSvector4& axisZ,
	const GSvector4& position ) :
		_11( axisX.x    ), _12( axisX.y    ), _13( axisX.z    ), _14( axisX.w    ),
		_21( axisY.x    ), _22( axisY.y    ), _23( axisY.z    ), _24( axisY.w    ),
		_31( axisZ.x    ), _32( axisZ.y    ), _33( axisZ.z    ), _34( axisZ.w    ),
		_41( position.x ), _42( position.y ), _43( position.z ), _44( position.w )
{}

GSINLINE GSmatrix4::GSmatrix4( const GSfloat* p ) :
		_11( p[0]  ), _12( p[1]  ), _13( p[2]  ), _14( p[3]  ),
		_21( p[4]  ), _22( p[5]  ), _23( p[6]  ), _24( p[7]  ),
		_31( p[8]  ), _32( p[9]  ), _33( p[10] ), _34( p[11] ),
		_41( p[12] ), _42( p[13] ), _43( p[14] ), _44( p[15] )
{}

GSINLINE GSmatrix4::GSmatrix4( const GSmatrix3& m3 ) :
		_11( m3._11 ), _12( m3._12 ), _13( m3._13 ), _14( 0.0f ),
		_21( m3._21 ), _22( m3._22 ), _23( m3._23 ), _24( 0.0f ),
		_31( m3._31 ), _32( m3._32 ), _33( m3._33 ), _34( 0.0f ),
		_41( 0.0f   ), _42( 0.0f   ), _43( 0.0f   ), _44( 1.0f )
{}

GSINLINE GSmatrix4::GSmatrix4( const GSquaternion& q )
{
	gsMatrix4Quaternion( this, &q );
}

/* 逆行列にする */
GSINLINE GSmatrix4& GSmatrix4::inverse()
{
	gsMatrix4Inverse( this, this );

	return	*this;
}

/* 逆行列にする */
GSINLINE GSmatrix4& GSmatrix4::inverseFast()
{
	gsMatrix4InverseFast( this, this );

	return	*this;
}

/* 転置行列にする */
GSINLINE GSmatrix4& GSmatrix4::transpose()
{
	gsMatrix4Transpose( this, this );

	return	*this;
}

/* 逆行列転置行列にする */
GSINLINE GSmatrix4& GSmatrix4::inverseTranspose()
{
	inverse();

	transpose();

	return	*this;
}

/* 逆行列を取得 */
GSINLINE GSmatrix4 GSmatrix4::getInverse() const
{
	GSmatrix4		result;

	gsMatrix4Inverse( &result, this );

	return	result;
}

/* 逆行列を取得 */
GSINLINE GSmatrix4 GSmatrix4::getInverseFast() const
{
	GSmatrix4		result;

	gsMatrix4InverseFast( &result, this );

	return	result;
}

/* 転置行列を取得 */
GSINLINE GSmatrix4 GSmatrix4::getTranspose() const
{
	GSmatrix4		result;

	gsMatrix4Transpose( &result, this );

	return	result;
}

/* 転置逆行列を取得 */
GSINLINE GSmatrix4 GSmatrix4::getInverseTranspose() const
{
	GSmatrix4		result;

	gsMatrix4Inverse( &result, this );

	gsMatrix4Transpose( &result, this );

	return	result;
}

/* スケール値の設定 */
GSINLINE GSmatrix4& GSmatrix4::setScale( GSfloat x, GSfloat y, GSfloat z )
{
	gsMatrix4Scale( this, x, y, z );

	return	*this;
}

GSINLINE GSmatrix4& GSmatrix4::setScale( const GSvector3& scaleFactor )
{
	return	setScale( scaleFactor.x, scaleFactor.y, scaleFactor.z );
}

GSINLINE GSmatrix4& GSmatrix4::setScale( GSfloat scaleFactor )
{
	return	setScale( scaleFactor, scaleFactor, scaleFactor );
}

/* 回転角度の設定 */
GSINLINE GSmatrix4& GSmatrix4::setRotation( GSfloat angle, GSfloat axisX, GSfloat axisY, GSfloat axisZ )
{
	gsMatrix4Rotate( this, angle, axisX, axisY, axisZ );

	return	*this;
}

GSINLINE GSmatrix4& GSmatrix4::setRotation( GSfloat angle, const GSvector3& axis )
{
	return setRotation( angle, axis.x, axis.y, axis.z );
}

GSINLINE GSmatrix4& GSmatrix4::setRotation( const GSquaternion& q )
{
	gsMatrix4Quaternion( this, &q );

	return	*this;
}

GSINLINE GSmatrix4& GSmatrix4::setRotationX( GSfloat angle )
{
	gsMatrix4RotateX( this, angle );

	return	*this;
}

GSINLINE GSmatrix4& GSmatrix4::setRotationY( GSfloat angle )
{
	gsMatrix4RotateY( this, angle );

	return	*this;
}

GSINLINE GSmatrix4& GSmatrix4::setRotationZ( GSfloat angle )
{
	gsMatrix4RotateZ( this, angle );

	return	*this;
}

GSINLINE GSmatrix4& GSmatrix4::setRotationYawPitchRoll( GSfloat yaw, GSfloat pitch, GSfloat roll )
{
	gsMatrix4YawPitchRoll( this, yaw, pitch, roll );

	return	*this;
}

/* 平行移動量の設定 */
GSINLINE GSmatrix4& GSmatrix4::setTranslation( GSfloat x, GSfloat y, GSfloat z )
{
	gsMatrix4Translate( this, x, y, z );

	return	*this;
}

GSINLINE GSmatrix4& GSmatrix4::setTranslation( const GSvector3& translation )
{
	return	setTranslation( translation.x, translation.y, translation.z );
}

/* 視野変換行列の設定 */
GSINLINE GSmatrix4& GSmatrix4::setLookAtRH( const GSvector3& from, const GSvector3& at, const GSvector3& up )
{
	gsMatrix4LookAtRH( this, &from, &at, &up );

	return	*this;
}

GSINLINE GSmatrix4& GSmatrix4::setLookAtRH( GSfloat fromX, GSfloat formY, GSfloat formZ,
										  GSfloat atX,   GSfloat atY,   GSfloat atZ,
										  GSfloat upX,   GSfloat upY,   GSfloat upZ )
{
	return	setLookAtRH( GSvector3( fromX, formY, formZ ),
						 GSvector3( atX,     atY,   atZ ),
						 GSvector3( upX,     upY,   upZ ) );
}

GSINLINE GSmatrix4& GSmatrix4::setLookAtLH( const GSvector3& from, const GSvector3& at, const GSvector3& up )
{
	gsMatrix4LookAtLH( this, &from, &at, &up );

	return	*this;
}

GSINLINE GSmatrix4& GSmatrix4::setLookAtLH( GSfloat fromX, GSfloat formY, GSfloat formZ,
										  GSfloat atX,   GSfloat atY,   GSfloat atZ,
										  GSfloat upX,   GSfloat upY,   GSfloat upZ )
{
	return	setLookAtLH( GSvector3( fromX, formY, formZ ),
						 GSvector3( atX,     atY,   atZ ),
						 GSvector3( upX,     upY,   upZ ) );
}

/* 透視変換行列の設定 */
GSINLINE GSmatrix4& GSmatrix4::setPerspectiveRH( GSfloat fovy, GSfloat aspect, GSfloat n, GSfloat f )
{
	gsMatrix4PerspectiveRH( this, fovy, aspect, n, f );

	return	*this;
}

GSINLINE GSmatrix4& GSmatrix4::setPerspectiveLH( GSfloat fovy, GSfloat aspect, GSfloat n, GSfloat f )
{
	gsMatrix4PerspectiveLH( this, fovy, aspect, n, f );

	return	*this;
}

GSINLINE GSmatrix4& GSmatrix4::setFrustumRH( GSfloat left, GSfloat right, GSfloat bottom, GSfloat top, GSfloat n, GSfloat f )
{
	gsMatrix4FrustumRH( this, left, right, bottom, top, n, f );

	return	*this;
}

GSINLINE GSmatrix4& GSmatrix4::setFrustumLH( GSfloat left, GSfloat right, GSfloat bottom, GSfloat top, GSfloat n, GSfloat f )
{
	gsMatrix4FrustumLH( this, left, right, bottom, top, n, f );

	return	*this;
}

GSINLINE GSmatrix4& GSmatrix4::setOrthoRH( GSfloat left, GSfloat right, GSfloat bottom, GSfloat top, GSfloat n, GSfloat f )
{
	gsMatrix4OrthoRH( this, left, right, bottom, top, n, f );

	return	*this;
}

GSINLINE GSmatrix4& GSmatrix4::setOrthoLH( GSfloat left, GSfloat right, GSfloat bottom, GSfloat top, GSfloat n, GSfloat f )
{
	gsMatrix4OrthoLH( this, left, right, bottom, top, n, f );

	return	*this;
}

/* 地平面シャドウマトリクスの設定 */
GSINLINE GSmatrix4& GSmatrix4::setShadow( const GSplane& plane, const GSvector3& lightPosition )
{
	gsMatrix4Shadow( this, &plane, &lightPosition );

	return	*this;
}

/* 単位行列化する */
GSINLINE GSmatrix4& GSmatrix4::identity()
{
	gsMatrix4Identity( this );

	return	*this;
}

/* 行列の正規化　*/
GSINLINE GSmatrix4& GSmatrix4::normalize()
{
	setAxisX(getAxisX().getNormalized());
	setAxisY(getAxisY().getNormalized());
	setAxisZ(getAxisZ().getNormalized());
	return	*this;
}

/* 行列の補間　*/
GSINLINE GSmatrix4 GSmatrix4::lerp(const GSmatrix4& other, float t) const
{
	GSvector3    s = getScale().lerp(other.getScale(), t);
	GSquaternion r = GSquaternion(getNormalized()).slerp(GSquaternion(other.getNormalized()), t);
	GSvector3    p = getPosition().lerp(other.getPosition(), t);
	return	GSmatrix4().identity().scale(s).rotate(r).translate(p);
}

/* 行列の正規化　*/
GSINLINE GSmatrix4 GSmatrix4::getNormalized() const
{
	GSmatrix4 result = *this;
	return	result.normalize();
}

/* 拡大縮小行列を乗算 */
GSINLINE GSmatrix4& GSmatrix4::scale( GSfloat x, GSfloat y, GSfloat z )
{
	GSmatrix4	s;

	gsMatrix4Multiply(this, this, &s.setScale(x, y, z) );

	return	*this;
}

GSINLINE GSmatrix4& GSmatrix4::scale( const GSvector3& scaleFactor )
{
	return	scale( scaleFactor.x, scaleFactor.y, scaleFactor.z );
}

GSINLINE GSmatrix4& GSmatrix4::scale( GSfloat scaleFactor )
{
	return	scale( scaleFactor, scaleFactor, scaleFactor );
}

/* 回転行列を乗算 */
GSINLINE GSmatrix4& GSmatrix4::rotate( GSfloat angle, GSfloat axisX, GSfloat axisY, GSfloat axisZ )
{
	GSmatrix4	r;

	gsMatrix4Multiply( this, this, &r.setRotation(angle, axisX, axisY, axisZ) );

	return	*this;
}

GSINLINE GSmatrix4& GSmatrix4::rotate( GSfloat angle, const GSvector3& axis )
{
	return rotate( angle, axis.x, axis.y, axis.z );
}

GSINLINE GSmatrix4& GSmatrix4::rotate( const GSquaternion& q )
{
	GSmatrix4	r;

	gsMatrix4Multiply( this, this, &r.setRotation(q) );

	return	*this;
}

GSINLINE GSmatrix4& GSmatrix4::rotateX( GSfloat angle )
{
	GSmatrix4	r;

	gsMatrix4Multiply( this, this, &r.setRotationX(angle) );

	return	*this;
}

GSINLINE GSmatrix4& GSmatrix4::rotateY( GSfloat angle )
{
	GSmatrix4	r;

	gsMatrix4Multiply( this, this, &r.setRotationY(angle) );

	return	*this;
}

GSINLINE GSmatrix4& GSmatrix4::rotateZ( GSfloat angle )
{
	GSmatrix4	r;

	gsMatrix4Multiply( this, this, &r.setRotationZ(angle) );

	return	*this;
}

GSINLINE GSmatrix4& GSmatrix4::rotateYawPitchRoll( GSfloat yaw, GSfloat pitch, GSfloat roll )
{
	GSmatrix4	r;

	gsMatrix4Multiply( this, this, &r.setRotationYawPitchRoll(yaw, pitch, roll) );

	return	*this;
}

/* 前向きベクトル優先で行列を正規化 */
GSINLINE GSmatrix4& GSmatrix4::nomalizeAxisZ()
{
	GSvector3 front = getAxisZ().getNormalized();
	GSvector3 left  = getAxisY().cross(front).getNormalized();
	GSvector3 up    = front.cross(left);
	if (left.length() < FLT_EPSILON) {
		up   = front.cross(getAxisX()).getNormalized();
		left = up.cross(front);
	}
	return setAxisX(left).setAxisY(up).setAxisZ(front);
}

/* 上向きベクトル優先で行列を正規化 */
GSINLINE GSmatrix4& GSmatrix4::nomalizeAxisY()
{
	GSvector3 up    = getAxisY().getNormalized();
	GSvector3 left  = up.cross(getAxisZ()).getNormalized();
	GSvector3 front = left.cross(up);
	if (left.length() < FLT_EPSILON) {
		front = getAxisX().cross(up).getNormalized();
		left  = up.cross(front);
	}
	return setAxisX(left).setAxisY(up).setAxisZ(front);
}

/* 左向きベクトル優先で行列を正規化 */
GSINLINE GSmatrix4& GSmatrix4::nomalizeAxisX()
{
	GSvector3 left  = getAxisX().getNormalized();
	GSvector3 up    = getAxisZ().cross(left).getNormalized();
	GSvector3 front = left.cross(up);
	if (up.length() < FLT_EPSILON) {
		front = getAxisX().cross(up).getNormalized();
		up = front.cross(left);
	}
	return setAxisX(left).setAxisY(up).setAxisZ(front);
}

/* 平行移動行列を乗算 */
GSINLINE GSmatrix4& GSmatrix4::translate( GSfloat x, GSfloat y, GSfloat z )
{
	GSmatrix4	t;

	gsMatrix4Multiply( this, this, &t.setTranslation(x, y, z) );

	return	*this;
}

GSINLINE GSmatrix4& GSmatrix4::translate( const GSvector3& translation )
{
	translate( translation.x, translation.y, translation.z );

	return	*this;
}

/* 座標変換 */
GSINLINE GSvector2 GSmatrix4::transform( const GSvector2& v2) const
{
	return GSvector2( v2.x * m[0][0] + v2.y * m[1][0] + m[3][0],
		              v2.x * m[0][1] + v2.y * m[1][1] + m[3][1] );
}

GSINLINE GSvector3 GSmatrix4::transform( const GSvector3& v3) const
{
	GSvector3	result;

	gsVector3Transform( &result, &v3, this );

	return	result;
}

GSINLINE GSvector4 GSmatrix4::transform( const GSvector4& v2) const
{
	GSvector4	result;

	gsVector4Transform( &result, &v2, this );

	return	result;
}

GSINLINE GSplane GSmatrix4::transform(const GSplane& plane) const
{
	GSplane	result;

	gsPlaneTransform( &result, &plane, this );

	return result;
}

GSINLINE GSvector2 GSmatrix4::transformCoord( const GSvector2& v2 ) const
{
	GSvector2	result;

	gsVector2TransformCoord( &result, &v2, this );

	return	result;
}

GSINLINE GSvector3 GSmatrix4::transformCoord( const GSvector3& v3 ) const
{
	GSvector3	result;

	gsVector3TransformCoord( &result, &v3, this );

	return	result;
}

GSINLINE GSvector4 GSmatrix4::transformCoord( const GSvector4& v3) const
{
	GSvector4	result;

	gsVector4TransformCoord( &result, &v3, this );

	return	result;
}

GSINLINE GSvector3 GSmatrix4::transformNormal( const GSvector3& v3) const
{
	GSvector3	result;

	gsVector3TransformNormal( &result, &v3, this );

	return	result;
}

/* 視野変換行列に変換する */
GSINLINE GSmatrix4& GSmatrix4::convertViewRH()
{
	gsMatrix4ConvViewRH( this, this );

	return	*this;
}

GSINLINE GSmatrix4& GSmatrix4::convertViewLH()
{
	gsMatrix4ConvViewLH( this, this );

	return	*this;
}

/* 軸，位置の設定 */
GSINLINE GSmatrix4& GSmatrix4::setAxisX( const GSvector3& axisX )
{
	_11 = axisX.x; _12 = axisX.y; _13 = axisX.z;

	return	*this;
}

GSINLINE GSmatrix4& GSmatrix4::setAxisY( const GSvector3& axisY )
{
	_21 = axisY.x; _22 = axisY.y; _23 = axisY.z;

	return	*this;
}

GSINLINE GSmatrix4& GSmatrix4::setAxisZ( const GSvector3& axisZ )
{
	_31 = axisZ.x; _32 = axisZ.y; _33 = axisZ.z;

	return	*this;
}

GSINLINE GSmatrix4& GSmatrix4::setPosition( const GSvector3& position )
{
	_41 = position.x; _42 = position.y; _43 = position.z;

	return	*this;
}

/* 軸，位置の取得 */
GSINLINE GSvector3 GSmatrix4::getAxisX() const
{
	return	GSvector3( _11, _12, _13 );
}

GSINLINE GSvector3 GSmatrix4::getAxisY() const
{
	return	GSvector3( _21, _22, _23 );
}

GSINLINE GSvector3 GSmatrix4::getAxisZ() const
{
	return	GSvector3( _31, _32, _33 );
}

GSINLINE GSvector3 GSmatrix4::getPosition() const
{
	return	GSvector3( _41, _42, _43 );
}

GSINLINE GSvector3 GSmatrix4::getScale() const
{
	return	GSvector3( getAxisX().length(), getAxisY().length(), getAxisZ().length() );
}

/* ロール・ピッチ・ヨーの取得 */
GSINLINE GSfloat GSmatrix4::getRoll() const
{
	return	gsMatrix4GetRoll( this );
}

GSINLINE GSfloat GSmatrix4::getPitch() const
{
	return	gsMatrix4GetPitch( this );
}

GSINLINE GSfloat GSmatrix4::getYaw() const
{
	return	gsMatrix4GetYaw( this );
}

/* クォータニオンを取得 */
GSINLINE GSquaternion GSmatrix4::getQuaternion() const
{
	GSquaternion	q;

	gsQuaternionRotateMatrix4( &q, this );

	return	q;
}

GSINLINE GSmatrix4 GSmatrix4::getRotateMatrix() const {
	GSmatrix4 result(*this);
	result.setPosition(GSvector3(0.0f, 0.0f, 0.0f));
	return result;
}

GSINLINE GSmatrix4::operator GSfloat* () const
{
	return	(GSfloat*)v;
}

GSINLINE GSmatrix4::operator const GSfloat* () const
{
	return	(const GSfloat*)v;
}

/* 単項演算子オーバーロード */
GSINLINE GSmatrix4 GSmatrix4::operator + () const
{
	return	*this;
}

GSINLINE GSmatrix4 GSmatrix4::operator - () const
{
	return	GSmatrix4( -_11, -_12, -_13, -_14, 
					   -_21, -_22, -_23, -_24, 
					   -_31, -_32, -_33, -_34, 
					   -_41, -_42, -_43, -_44 );
}

/* 代入演算子オーバーロード */
GSINLINE GSmatrix4& GSmatrix4::operator += ( const GSmatrix4& M )
{
	gsMatrix4Add( this, this, &M );

	return	*this;
}

GSINLINE GSmatrix4& GSmatrix4::operator -= ( const GSmatrix4& M )
{
	gsMatrix4Subtract( this, this, &M );

	return	*this;
}

GSINLINE GSmatrix4& GSmatrix4::operator *= ( const GSmatrix4& M )
{
	gsMatrix4Multiply( this, this, &M );

	return	*this;
}

GSINLINE GSmatrix4& GSmatrix4::operator *= ( GSfloat s )
{
	gsMatrix4MultiplyScalar( this, this, s );

	return	*this;
}

GSINLINE GSmatrix4& GSmatrix4::operator /= ( GSfloat s )
{
	gsMatrix4MultiplyScalar( this, this, 1.0f / s );

	return	*this;
}

/* ２項演算子オーバーロード */
GSINLINE const GSmatrix4 operator + ( const GSmatrix4& m1, const GSmatrix4& m2 )
{
	GSmatrix4	result;

	gsMatrix4Add( &result, &m1, &m2 );

	return	result;
}

GSINLINE const GSmatrix4 operator - ( const GSmatrix4& m1, const GSmatrix4& m2 )
{
	GSmatrix4	result;

	gsMatrix4Subtract( &result, &m1, &m2 );

	return	result;
}

GSINLINE const GSmatrix4 operator * ( const GSmatrix4& m1, const GSmatrix4& m2 )
{
	GSmatrix4	result;

	gsMatrix4Multiply( &result, &m1, &m2 );

	return	result;
}

GSINLINE const GSmatrix4 operator * ( const GSmatrix4& m, const GSfloat s )
{
	GSmatrix4	result;

	gsMatrix4MultiplyScalar( &result, &m, s );

	return	result;
}

GSINLINE const GSmatrix4 operator * ( const GSfloat s, const GSmatrix4& m )
{
	return	m * s;
}

GSINLINE const GSmatrix4 operator / ( const GSmatrix4& m, const GSfloat s )
{
	return	m * ( 1.0f / s ) ;
}

GSINLINE bool operator == ( const GSmatrix4& m1, const GSmatrix4& m2 )
{
	for ( int i = 0; i < 4; i++ )
	{
		for ( int j = 0; j < 4; j++ )
		{
			if ( m1.m[i][j] != m2.m[i][j] ) return false;
		}
	}

	return	true;
}

GSINLINE bool operator != ( const GSmatrix4& m1, const GSmatrix4& m2 )
{
	return	!( m1 == m2 );
}

#endif

#endif

/********** End of File ******************************************************/
