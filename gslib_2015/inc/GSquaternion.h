/******************************************************************************
* File Name : GSquaternion.h                     Ver : 1.00  Date : 2002-08-14
*
* Description :
*
*       クォータニオンの計算 ヘッダファイル．
*
* Author : Moriharu Ohzu.
*
******************************************************************************/
#ifndef		_GSQUATERNION_H_
#define		_GSQUATERNION_H_

#include	"GStype.h"
#include	"GSmath.h"

/****** 関数プロトタイプ宣言 *************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*=============================================================================
*
* Purpose : クォータニオンの初期化．
*
* Return  : なし．
*
*============================================================================*/
extern GSquaternion*
gsQuaternionInit
(
	GSquaternion*	pOut,				/*-> クォータニオン		*/
	GSfloat			x,					/* ｘ成分				*/
	GSfloat			y,					/* ｙ成分				*/
	GSfloat			z,					/* ｚ成分				*/
	GSfloat			w					/* ｗ成分				*/
);

/*=============================================================================
*
* Purpose : 単位クォータニオンを設定．
*
* Return  : なし．
*
*============================================================================*/
extern GSquaternion*
gsQuaternionIdentity
(
	GSquaternion*	pOut				/*-> クォータニオン		*/
);

/*=============================================================================
*
* Purpose : クォータニオンの内積を求める．
*
* Return  : 内積を返す．
*
*============================================================================*/
extern GSfloat
gsQuaternionDot
(
	const GSquaternion*		q1,			/* クォータニオン	*/
	const GSquaternion*		q2			/* クォータニオン	*/
);

/*=============================================================================
*
* Purpose : クォータニオンの長さを求める．
*
* Return  : 長さを返す．
*
*============================================================================*/
extern GSfloat
gsQuaternionLength
(
	const GSquaternion*		q			/* クォータニオン	*/
);

/*=============================================================================
*
* Purpose : クォータニオンの正規化．
*
* Return  : なし．
*
*============================================================================*/
extern GSquaternion*
gsQuaternionNormalize
(
	GSquaternion*		ans,			/*-> 正規クォータニオン	*/
	const GSquaternion*	q				/* クォータニオン		*/
);

/*=============================================================================
*
* Purpose : クォータニオンの乗算．
*
* Return  : なし．
*
*============================================================================*/
extern GSquaternion*
gsQuaternionMultiply
(
	GSquaternion*		ans,			/*-> ａ×ｂの結果		*/
	const GSquaternion*	a,				/* クォータニオンａ		*/
	const GSquaternion*	b				/* クォータニオンｂ		*/
);

/*=============================================================================
*
* Purpose : クォータニオンとスカラーの乗算．
*
* Return  : なし．
*
*============================================================================*/
extern GSquaternion*
gsQuaternionMultiplyScalar
(
	GSquaternion*		ans,			/*-> ａ×ｓの結果		*/
	const GSquaternion*	a,				/* スカラー				*/
	GSfloat				s				/* スカラー				*/
);

/*=============================================================================
*
* Purpose : クォータニオンの加算．
*
* Return  : なし．
*
*============================================================================*/
extern GSquaternion*
gsQuaternionAdd
(
	GSquaternion*		ans,			/*-> ａ＋ｂの結果		*/
	const GSquaternion*	a,				/* クォータニオンａ		*/
	const GSquaternion*	b				/* クォータニオンｂ		*/
);

/*=============================================================================
*
* Purpose : クォータニオンの減算．
*
* Return  : なし．
*
*============================================================================*/
extern GSquaternion*
gsQuaternionSubtract
(
	GSquaternion*		ans,			/*-> ａ－ｂの結果		*/
	const GSquaternion*	a,				/* クォータニオンａ		*/
	const GSquaternion*	b				/* クォータニオンｂ		*/
);

/*=============================================================================
*
* Purpose : 軸周りの回転角度からクォータニオンを求める．
*
* Return  : なし．
*
*============================================================================*/
extern GSquaternion*
gsQuaternionRotate
(
	GSquaternion*		q,				/* クォータニオン		*/
	GSfloat				angle,			/* 回転角度				*/
	GSfloat				vx,				/* 回転軸のｘ成分		*/
	GSfloat				vy,				/* 回転軸のｘ成分		*/
	GSfloat				vz				/* 回転軸のｘ成分		*/
);

/*=============================================================================
*
* Purpose : ヨー，ピッチ，ロールからクォータニオンを求める．
*
* Return  : なし．
*
*============================================================================*/
extern GSquaternion*
gsQuaternionYawPitchRoll
(
	GSquaternion*		q,				/* クォータニオン		*/
	GSfloat				fYaw,			/* ヨー					*/
	GSfloat				fPitch,			/* ピッチ				*/
	GSfloat				fRoll			/* ロール				*/
);

/*=============================================================================
*
* Purpose : クォータニオンの線形補間．
*
* Return  : なし．
*
*============================================================================*/
extern GSquaternion*
gsQuaternionLerp
(
	GSquaternion*		pOut,			/*-> 補間の結果			*/
	const GSquaternion*	pQ1,			/* クォータニオン1		*/
	const GSquaternion*	pQ2,			/* クォータニオン2		*/
	GSfloat				t				/* 補間値				*/
);

/*=============================================================================
*
* Purpose : 逆クォータニオンを求める．
*
* Return  : なし．
*
*============================================================================*/
extern GSquaternion*
gsQuaternionInverse
(
	GSquaternion*		pOut,			/*-> 逆クォータニオン	*/
	const GSquaternion*	q				/* クォータニオン		*/
);

/*=============================================================================
*
* Purpose : クォータニオンから軸周りの回転角度を求める．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsQuaternionToRotate
(
	const GSquaternion*	q,				/* クォータニオン		*/
	GSfloat*			pAngle,			/*-> 回転角度			*/
	GSfloat*			pVx,			/*-> 回転軸のｘ成分		*/
	GSfloat*			pVy,			/*-> 回転軸のｙ成分		*/
	GSfloat*			pVz				/*-> 回転軸のｚ成分		*/
);

/*=============================================================================
*
* Purpose : 回転行列からクォータニオンを求める．
*
* Return  : なし．
*
*============================================================================*/
extern GSquaternion*
gsQuaternionRotateMatrix3
(
	GSquaternion*		q,				/* クォータニオン		*/
	const GSmatrix3*	mat				/* 回転行列				*/
);

/*=============================================================================
*
* Purpose : 回転行列からクォータニオンを求める．
*
* Return  : なし．
*
*============================================================================*/
extern GSquaternion*
gsQuaternionRotateMatrix4
(
	GSquaternion*		q,				/* クォータニオン		*/
	const GSmatrix4*	mat				/* 回転行列				*/
);

/*=============================================================================
*
* Purpose : クォータニオンの球面線形補間．
*
* Return  : なし．
*
*============================================================================*/
extern GSquaternion*
gsQuaternionSlerp
(
	GSquaternion*		pOut,			/*-> 補間の結果			*/
	const GSquaternion*	a,				/* クォータニオン1		*/
	const GSquaternion*	b,				/* クォータニオン2		*/
	GSfloat				t				/* 補間値				*/
);

#ifdef __cplusplus
}
#endif

/*----- Ｃ＋＋ラッパークラス ------------------------------------------------*/

#ifdef __cplusplus

/* コンストラクタ */
GSINLINE GSquaternion::GSquaternion( GSfloat X, GSfloat Y, GSfloat Z, GSfloat W ) :
	x( X ), y( Y ), z( Z ), w( W )
{
}

GSINLINE GSquaternion::GSquaternion( GSfloat angle, const GSvector3& axis )
{
	gsQuaternionRotate( this, angle, axis.x, axis.y, axis.z );
}

GSINLINE GSquaternion::GSquaternion( const GSmatrix3& m )
{
	gsQuaternionRotateMatrix3( this, &m );
}

GSINLINE GSquaternion::GSquaternion( const GSmatrix4& m )
{
	gsQuaternionRotateMatrix4( this, &m );
}

GSINLINE GSquaternion::GSquaternion( const GSvector3& xAxis, const GSvector3& yAxis, const GSvector3& zAxis )
{
	GSmatrix3	M;

	M._11 = xAxis.x; M._12 = xAxis.y; M._13 = xAxis.z;
	M._21 = yAxis.x; M._22 = yAxis.y; M._23 = yAxis.z;
	M._31 = zAxis.x; M._32 = zAxis.y; M._33 = zAxis.z;

	gsQuaternionRotateMatrix3( this, &M );
}

/* 単位クォータニオン化する */
GSINLINE GSquaternion& GSquaternion::identity()
{
	gsQuaternionIdentity( this );

	return	*this;
}

/* 内積 */
GSINLINE GSfloat GSquaternion::dot( const GSquaternion& q ) const
{
	return	gsQuaternionDot( this, &q );
}

/* 長さを求める */
GSINLINE GSfloat GSquaternion::length() const
{
	gsQuaternionLength( this );
}

/* 正規化を行う */
GSINLINE GSquaternion& GSquaternion::normalize()
{
	gsQuaternionNormalize( this, this );

	return	*this;
}

/* 逆クォータニオン化する */
GSINLINE GSquaternion& GSquaternion::inverse()
{
	gsQuaternionInverse( this, this );

	return	*this;
}

/* 正規化後のクォータニオンを取得 */
GSINLINE GSquaternion GSquaternion::getNormalized() const
{
	GSquaternion	result( *this );

	result.normalize();

	return	result;
}

/* 逆クォータニオン化を取得 */
GSINLINE GSquaternion GSquaternion::getInverse () const
{
	GSquaternion	result( *this );

	result.inverse();

	return	result;
}

/* 回転角度からクォータニオンを設定 */
GSINLINE GSquaternion& GSquaternion::setRotate( GSfloat angle, GSfloat x_, GSfloat y_, GSfloat z_ )
{
	gsQuaternionRotate( this, angle, x_, y_, z_ );

	return	*this;
}

GSINLINE GSquaternion& GSquaternion::setRotate( GSfloat angle, const GSvector3& axis )
{
	setRotate( angle, axis.x, axis.y, axis.z );

	return	*this;
}

/* 回転角度からクォータニオンを設定 */
GSINLINE GSquaternion& GSquaternion::setRotateYawPitchRoll( GSfloat yaw, GSfloat pitch, GSfloat roll )
{
	gsQuaternionYawPitchRoll( this, yaw, pitch, roll );

	return	*this;
}

/* 回転行列を設定 */
GSINLINE GSquaternion& GSquaternion::setMatrix( const GSmatrix3& m )
{
	gsQuaternionRotateMatrix3( this, &m );

	return	*this;
}

GSINLINE GSquaternion& GSquaternion::setMatrix( const GSmatrix4& m )
{
	gsQuaternionRotateMatrix4( this, &m );

	return	*this;
}

/* 軸と角度を取得 */
GSINLINE GSvector3 GSquaternion::getAxis() const
{
	GSvector3	v;
	GSfloat		angle;

	gsQuaternionToRotate( this, &angle, &v.x, &v.y, &v.z );

	return	v;
}

GSINLINE GSfloat GSquaternion::getAngle() const
{
	return	gsAcos( w ) * 2.0f;
}

/* ヨー，ピッチ，ロールの取得 */
GSINLINE GSfloat GSquaternion::getYaw() const
{
	return	gsDegreeNormalize( gsAsin( -2.0f * ( x*z - w*y ) ) );
}

GSINLINE GSfloat GSquaternion::getPitch() const
{
	return	gsAtan( 2.0f *( y*z + w*x ), w*w - x*x - y*y + z*z );
}

GSINLINE GSfloat GSquaternion::getRoll() const
{
	return	gsAtan( 2.0f * (x*y + w*z), w*w + x*x - y*y - z*z );
}

/* 球面線形補間 */
GSINLINE GSquaternion GSquaternion::slerp ( const GSquaternion& q, GSfloat t ) const
{
	GSquaternion	result;

	gsQuaternionSlerp( &result, this, &q, t );

	return	result;
}

/* 線形補間 */
GSINLINE GSquaternion GSquaternion::lerp ( const GSquaternion& q, GSfloat t ) const
{
	GSquaternion	result;

	gsQuaternionLerp( &result, this, &q, t );

	return	result;
}

/* 単項演算子オーバーロード */
GSINLINE GSquaternion GSquaternion::operator + () const
{
	return	*this;
}

GSINLINE GSquaternion GSquaternion::operator - () const
{
	return	GSquaternion( -x, -y, -z, -w );
}

/* 代入演算子オーバーロード */
GSINLINE GSquaternion& GSquaternion::operator += ( const GSquaternion& v )
{
	gsQuaternionAdd( this, this, &v );

	return	*this;
}

GSINLINE GSquaternion& GSquaternion::operator -= ( const GSquaternion& v )
{
	gsQuaternionSubtract( this, this, &v );

	return	*this;
}

GSINLINE GSquaternion& GSquaternion::operator *= ( const GSquaternion& v )
{
	gsQuaternionMultiply( this, this, &v );

	return	*this;
}

GSINLINE GSquaternion& GSquaternion::operator *= ( GSfloat s )
{
	gsQuaternionMultiplyScalar( this, this, s );

	return	*this;
}

GSINLINE GSquaternion& GSquaternion::operator /= ( GSfloat s )
{
	gsQuaternionMultiplyScalar( this, this, 1.0f / s );

	return	*this;
}

/* ２項演算子オーバーロード */
GSINLINE const GSquaternion operator + ( const GSquaternion& q1, const GSquaternion& q2 )
{
	GSquaternion	result;

	gsQuaternionAdd( &result, &q1, &q2 );

	return	result;
}

GSINLINE const GSquaternion operator - ( const GSquaternion& q1, const GSquaternion& q2 )
{
	GSquaternion	result;

	gsQuaternionSubtract( &result, &q1, &q2 );

	return	result;
}

GSINLINE const GSquaternion operator * ( const GSquaternion& q1, const GSquaternion& q2 )
{
	GSquaternion	result;

	gsQuaternionMultiply( &result, &q1, &q2 );

	return	result;
}

GSINLINE const GSquaternion operator * ( const GSquaternion& q, const GSfloat s )
{
	GSquaternion	result;

	gsQuaternionMultiplyScalar( &result, &q, s );

	return	result;
}

GSINLINE const GSquaternion operator * ( const GSfloat s, const GSquaternion& q )
{
	return	q * s;
}

GSINLINE bool operator == ( const GSquaternion& q1, const GSquaternion& q2 )
{
	return	( q1.x == q2.x && q1.y == q2.y && q1.z == q2.z && q1.w == q2.w );
}

GSINLINE bool operator != ( const GSquaternion& q1, const GSquaternion& q2 )
{
	return	!( q1 == q2 );
}

#endif

#endif

/********** End of File ******************************************************/
