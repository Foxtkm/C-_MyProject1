/******************************************************************************
* File Name : GSmatrix3.c                        Ver : 1.00  Date : 1998-05-03
*
* Description :
*
*       ３行３列座標変換行列計算 ヘッダファイル．
*
* Author : Moriharu Ohzu.
*
******************************************************************************/
#ifndef		_GSMATRIX3_H_
#define		_GSMATRIX3_H_

#include	"GStype.h"
#include	"GSvector2.h"
#include	"GSvector3.h"
#include	"GSquaternion.h"

/****** 関数プロトタイプ宣言 *************************************************/

#ifdef __cplusplus
extern "C" {
#endif

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
	GSmatrix3*			pM				/* ３Ｄマトリクス   */
);

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
	GSmatrix3*			pM,				/* ３Ｄマトリクス       */
	GSfloat				tx,				/* ｘ方向移動量			*/
	GSfloat				ty				/* ｙ方向移動量			*/
);

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
	GSmatrix3*			pM,				/* ３Ｄマトリクス       */
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
gsMatrix3Rotate
(
	GSmatrix3*			pM,				/* ３Ｄマトリクス       */
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
gsMatrix3RotateX
(
	GSmatrix3*			pM,				/* ３Ｄマトリクス       */
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
gsMatrix3RotateY
(
	GSmatrix3*			pM,				/* ３Ｄマトリクス       */
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
gsMatrix3RotateZ
(
	GSmatrix3*			pM,				/* ３Ｄマトリクス       */
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
gsMatrix3Quaternion
(
	GSmatrix3*			pM,				/* ３Ｄマトリクス       */
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
gsMatrix3Multiply
(
	GSmatrix3*			pOut,			/*-> 乗算の結果     */
	const GSmatrix3*	pM1,			/* ３Ｄマトリクス   */
	const GSmatrix3*	pM2				/* ３Ｄマトリクス   */
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

#ifdef __cplusplus
}
#endif

/*----- Ｃ＋＋ラッパークラス ------------------------------------------------*/

#ifdef __cplusplus


/* コンストラクタ */
GSINLINE GSmatrix3::GSmatrix3(
	GSfloat	m11, GSfloat m12, GSfloat m13,
	GSfloat	m21, GSfloat m22, GSfloat m23,
	GSfloat	m31, GSfloat m32, GSfloat m33 ) :
		_11( m11 ), _12( m12 ), _13( m13 ),
		_21( m21 ), _22( m22 ), _23( m23 ),
		_31( m31 ), _32( m32 ), _33( m33 )
{}

GSINLINE GSmatrix3::GSmatrix3(
	const GSvector3& axisX,
	const GSvector3& axisY,
	const GSvector3& axisZ ) :
	_11( axisX.x ), _12( axisX.y ), _13( axisX.z ),
	_21( axisY.x ), _22( axisY.y ), _23( axisY.z ),
	_31( axisZ.x ), _32( axisZ.y ), _33( axisZ.z )
{}

GSINLINE GSmatrix3::GSmatrix3( const GSfloat* p ) :
	_11( p[0]  ), _12( p[1]  ), _13( p[2] ),
	_21( p[3]  ), _22( p[4]  ), _23( p[5] ),
	_31( p[6]  ), _32( p[7]  ), _33( p[8] )
{}

GSINLINE GSmatrix3::GSmatrix3( const GSmatrix4& M ) :
	_11( M._11 ), _12( M._12 ), _13( M._13 ),
	_21( M._21 ), _22( M._22 ), _23( M._23 ),
	_31( M._31 ), _32( M._32 ), _33( M._33 )
{}

GSINLINE GSmatrix3::GSmatrix3( const GSquaternion& q )
{
	gsMatrix3Quaternion( this, &q );
}

/* 逆行列にする */
GSINLINE GSmatrix3& GSmatrix3::inverse()
{
	gsMatrix3Inverse( this, this );

	return	*this;
}

/* 転置行列にする */
GSINLINE GSmatrix3& GSmatrix3::transpose()
{
	gsMatrix3Transpose( this, this );

	return	*this;
}

/* 逆行列転置行列にする */
GSINLINE GSmatrix3& GSmatrix3::inverseTranspose()
{
	inverse();

	transpose();

	return	*this;
}

/* 逆行列を取得 */
GSINLINE GSmatrix3 GSmatrix3::getInverse() const
{
	GSmatrix3		result;

	gsMatrix3Inverse( &result, this );

	return	result;
}

/* 転置行列を取得 */
GSINLINE GSmatrix3 GSmatrix3::getTranspose() const
{
	GSmatrix3		result;

	gsMatrix3Transpose( &result, this );

	return	result;
}

/* 転置逆行列を取得 */
GSINLINE GSmatrix3 GSmatrix3::getInverseTranspose() const
{
	GSmatrix3		result;

	gsMatrix3Inverse( &result, this );

	gsMatrix3Transpose( &result, this );

	return	result;
}

/* スケール値の設定 */
GSINLINE GSmatrix3& GSmatrix3::setScale( GSfloat x, GSfloat y, GSfloat z )
{
	gsMatrix3Scale( this, x, y, z );

	return	*this;
}

GSINLINE GSmatrix3& GSmatrix3::setScale( const GSvector3& scaleFactor )
{
	return	setScale( scaleFactor.x, scaleFactor.y, scaleFactor.z );
}

GSINLINE GSmatrix3& GSmatrix3::setScale( GSfloat scaleFactor )
{
	return	setScale( scaleFactor, scaleFactor, scaleFactor );
}

/* 回転角度の設定 */
GSINLINE GSmatrix3& GSmatrix3::setRotation( GSfloat angle, GSfloat axisX, GSfloat axisY, GSfloat axisZ )
{
	gsMatrix3Rotate( this, angle, axisX, axisY, axisZ );

	return	*this;
}

GSINLINE GSmatrix3& GSmatrix3::setRotation( GSfloat angle, const GSvector3& axis )
{
	return setRotation( angle, axis.x, axis.y, axis.z );
}

GSINLINE GSmatrix3& GSmatrix3::setRotation( const GSquaternion& q )
{
	gsMatrix3Quaternion( this, &q );

	return	*this;
}

GSINLINE GSmatrix3& GSmatrix3::setRotationX( GSfloat angle )
{
	gsMatrix3RotateX( this, angle );

	return	*this;
}

GSINLINE GSmatrix3& GSmatrix3::setRotationY( GSfloat angle )
{
	gsMatrix3RotateY( this, angle );

	return	*this;
}

GSINLINE GSmatrix3& GSmatrix3::setRotationZ( GSfloat angle )
{
	gsMatrix3RotateZ( this, angle );

	return	*this;
}

GSINLINE GSmatrix3& GSmatrix3::setRotationYawPitchRoll( GSfloat yaw, GSfloat pitch, GSfloat roll )
{
	gsMatrix3YawPitchRoll( this, yaw, pitch, roll );

	return	*this;
}

/* 平行移動量の設定 */
GSINLINE GSmatrix3& GSmatrix3::setTranslation( GSfloat x, GSfloat y )
{
	gsMatrix3Translate( this, x, y );

	return	*this;
}

GSINLINE GSmatrix3& GSmatrix3::setTranslation( const GSvector2& translation )
{
	return	setTranslation( translation.x, translation.y );
}

/* 単位行列化する */
GSINLINE GSmatrix3& GSmatrix3::identity()
{
	gsMatrix3Identity( this );

	return	*this;
}

/* 行列の正規化　*/
GSINLINE GSmatrix3& GSmatrix3::normalize()
{
	setAxisX(getAxisX().getNormalized());
	setAxisY(getAxisY().getNormalized());
	return	*this;
}

/* 行列の正規化　*/
GSINLINE GSmatrix3 GSmatrix3::getNormalized() const
{
	GSmatrix3 result = *this;
	return	result.normalize();
}

/* 拡大縮小行列を乗算 */
GSINLINE GSmatrix3& GSmatrix3::scale( GSfloat x, GSfloat y, GSfloat z )
{
	GSmatrix3	s;

	gsMatrix3Multiply(this, this, &s.setScale(x, y, z) );

	return	*this;
}

GSINLINE GSmatrix3& GSmatrix3::scale( const GSvector3& scaleFactor )
{
	return	scale( scaleFactor.x, scaleFactor.y, scaleFactor.z );
}

GSINLINE GSmatrix3& GSmatrix3::scale( GSfloat scaleFactor )
{
	return	scale( scaleFactor, scaleFactor, scaleFactor );
}

/* 回転行列を乗算 */
GSINLINE GSmatrix3& GSmatrix3::rotate( GSfloat angle, GSfloat axisX, GSfloat axisY, GSfloat axisZ )
{
	GSmatrix3	r;

	gsMatrix3Multiply(this, this, &r.setRotation(angle, axisX, axisY, axisZ) );

	return	*this;
}

GSINLINE GSmatrix3& GSmatrix3::rotate( GSfloat angle, const GSvector3& axis )
{
	return rotate( angle, axis.x, axis.y, axis.z );
}

GSINLINE GSmatrix3& GSmatrix3::rotate( const GSquaternion& q )
{
	GSmatrix3	r;

	gsMatrix3Multiply( this, this, &r.setRotation(q) );

	return	*this;
}

GSINLINE GSmatrix3& GSmatrix3::rotateX( GSfloat angle )
{
	GSmatrix3	r;

	gsMatrix3Multiply( this, this, &r.setRotationX( angle ) );

	return	*this;
}

GSINLINE GSmatrix3& GSmatrix3::rotateY( GSfloat angle )
{
	GSmatrix3	r;

	gsMatrix3Multiply( this, this, &r.setRotationY(angle) );

	return	*this;
}

GSINLINE GSmatrix3& GSmatrix3::rotateZ( GSfloat angle )
{
	GSmatrix3	r;

	gsMatrix3Multiply( this, this, &r.setRotationZ(angle) );

	return	*this;
}

GSINLINE GSmatrix3& GSmatrix3::rotateYawPitchRoll( GSfloat yaw, GSfloat pitch, GSfloat roll )
{
	GSmatrix3	r;

	gsMatrix3Multiply( this, this, &r.setRotationYawPitchRoll(yaw, pitch, roll) );

	return	*this;
}

/* 平行移動行列を乗算 */
GSINLINE GSmatrix3& GSmatrix3::translate( GSfloat x, GSfloat y )
{
	GSmatrix3	t;

	gsMatrix3Multiply( this, this, &t.setTranslation(x, y) );

	return	*this;
}

GSINLINE GSmatrix3& GSmatrix3::translate( const GSvector2& translation )
{
	translate( translation.x, translation.y );

	return	*this;
}

/* 座標変換 */
GSINLINE GSvector2 GSmatrix3::transform( const GSvector2& v2 ) const
{
	GSvector2	result;

	gsVector2Transform( &result, &v2, this );

	return	result;
}

GSINLINE GSvector3 GSmatrix3::transform( const GSvector3& v3 ) const
{
	GSvector3	result;

	result = *this * v3;

	return	result;
}

GSINLINE GSvector2 GSmatrix3::transformNormal( const GSvector2& v2 ) const
{
	GSvector2	result;

	gsVector2TransformNormal( &result, &v2, this );

	return	result;
}

/* スケール値の取得 */
GSINLINE GSvector3 GSmatrix3::getScale() const
{
	return	GSvector3(getAxisX().length(), getAxisY().length(), getAxisZ().length());
}

/* 行列の補間　*/
GSINLINE GSmatrix3 GSmatrix3::lerp(const GSmatrix3& other, float t) const
{
	GSvector3    s = getScale().lerp(other.getScale(), t);
	GSquaternion r = GSquaternion(getNormalized()).slerp(GSquaternion(other.getNormalized()), t);
	return	GSmatrix3().identity().scale(s).rotate(r);
}

/* 軸の設定 */
GSINLINE GSmatrix3& GSmatrix3::setAxisX( const GSvector3& axisX )
{
	_11 = axisX.x; _12 = axisX.y; _13 = axisX.z;

	return	*this;
}

GSINLINE GSmatrix3& GSmatrix3::setAxisY( const GSvector3& axisY )
{
	_21 = axisY.x; _22 = axisY.y; _23 = axisY.z;

	return	*this;
}

GSINLINE GSmatrix3& GSmatrix3::setAxisZ( const GSvector3& axisZ )
{
	_31 = axisZ.x; _32 = axisZ.y; _33 = axisZ.z;

	return	*this;
}

/* 軸の取得 */
GSINLINE GSvector3 GSmatrix3::getAxisX() const
{
	return	GSvector3( _11, _12, _13 );
}

GSINLINE GSvector3 GSmatrix3::getAxisY() const
{
	return	GSvector3( _21, _22, _23 );
}

GSINLINE GSvector3 GSmatrix3::getAxisZ() const
{
	return	GSvector3( _31, _32, _33 );
}

/* ロール・ピッチ・ヨーの取得 */
GSINLINE GSfloat GSmatrix3::getRoll() const
{
	return	gsMatrix3GetRoll( this );
}

GSINLINE GSfloat GSmatrix3::getPitch() const
{
	return	gsMatrix3GetPitch( this );
}

GSINLINE GSfloat GSmatrix3::getYaw() const
{
	return	gsMatrix3GetYaw( this );
}

/* クォータニオンを取得 */
GSINLINE GSquaternion GSmatrix3::getQuaternion() const
{
	GSquaternion	q;

	gsQuaternionRotateMatrix3( &q, this );

	return	q;
}

GSINLINE GSmatrix3::operator GSfloat* () const
{
	return	(GSfloat*)v;
}

GSINLINE GSmatrix3::operator const GSfloat* () const
{
	return	(const GSfloat*)v;
}

/* 単項演算子オーバーロード */
GSINLINE GSmatrix3 GSmatrix3::operator + () const
{
	return	*this;
}

GSINLINE GSmatrix3 GSmatrix3::operator - () const
{
	return	GSmatrix3( -_11, -_12, -_13, 
					   -_21, -_22, -_23, 
					   -_31, -_32, -_33 );
}

/* 代入演算子オーバーロード */
GSINLINE GSmatrix3& GSmatrix3::operator += ( const GSmatrix3& M )
{
	gsMatrix3Add( this, this, &M );

	return	*this;
}

GSINLINE GSmatrix3& GSmatrix3::operator -= ( const GSmatrix3& M )
{
	gsMatrix3Subtract( this, this, &M );

	return	*this;
}

GSINLINE GSmatrix3& GSmatrix3::operator *= ( const GSmatrix3& M )
{
	gsMatrix3Multiply( this, this, &M );

	return	*this;
}

GSINLINE GSmatrix3& GSmatrix3::operator *= ( GSfloat s )
{
	gsMatrix3MultiplyScalar( this, this, s );

	return	*this;
}

GSINLINE GSmatrix3& GSmatrix3::operator /= ( GSfloat s )
{
	gsMatrix3MultiplyScalar( this, this, 1.0f / s );

	return	*this;
}

/* ２項演算子オーバーロード */
GSINLINE const GSmatrix3 operator + ( const GSmatrix3& m1, const GSmatrix3& m2 )
{
	GSmatrix3	result;

	gsMatrix3Add( &result, &m1, &m2 );

	return	result;
}

GSINLINE const GSmatrix3 operator - ( const GSmatrix3& m1, const GSmatrix3& m2 )
{
	GSmatrix3	result;

	gsMatrix3Subtract( &result, &m1, &m2 );

	return	result;
}

GSINLINE const GSmatrix3 operator * ( const GSmatrix3& m1, const GSmatrix3& m2 )
{
	GSmatrix3	result;

	gsMatrix3Multiply( &result, &m1, &m2 );

	return	result;
}

GSINLINE const GSmatrix3 operator * ( const GSmatrix3& m, const GSfloat s )
{
	GSmatrix3	result;

	gsMatrix3MultiplyScalar( &result, &m, s );

	return	result;
}

GSINLINE const GSmatrix3 operator * ( const GSfloat s, const GSmatrix3& m )
{
	return	m * s;
}

GSINLINE const GSmatrix3 operator / ( const GSmatrix3& m, const GSfloat s )
{
	return	m * ( 1.0f / s ) ;
}

GSINLINE bool operator == ( const GSmatrix3& m1, const GSmatrix3& m2 )
{
	for ( int i = 0; i < 3; i++ )
	{
		for ( int j = 0; j < 3; j++ )
		{
			if ( m1.m[i][j] != m2.m[i][j] ) return false;
		}
	}

	return	true;
}

GSINLINE bool operator != ( const GSmatrix3& m1, const GSmatrix3& m2 )
{
	return	!( m1 == m2 );
}

#endif

#endif

/********** End of File ******************************************************/
