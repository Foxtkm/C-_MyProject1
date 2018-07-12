/******************************************************************************
* File Name : GSquaternion.c                      Ver : 1.00  Date : 2002-08-14
*
* Description :
*
*       クォータニオンの計算．
*
* Author : Moriharu Ohzu.
*
******************************************************************************/
#include	"GSquaternion.h"
#include	"GSmath.h"

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
* Purpose : 単位クォータニオンを設定．
*
* Return  : なし．
*
*============================================================================*/
extern GSquaternion*
gsQuaternionIdentity
(
	GSquaternion*	pOut				/*-> クォータニオン		*/
)
{
	gsQuaternionInit( pOut, 0.0f, 0.0f, 0.0f, 1.0f );

	return	pOut;
}

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
)
{
	return	q1->x*q2->x + q1->y*q2->y + q1->z*q2->z + q1->w*q2->w;
}

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
)
{
	return	gsSqrt( gsQuaternionDot( q, q ) );
}

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
)
{
	GSfloat				l;

	l = gsQuaternionLength( q );
	l = ( l > 0.0f ) ? ( 1.0f / l ): 0.0f;

	ans->x = q->x * l;
	ans->y = q->y * l;
	ans->z = q->z * l;
	ans->w = q->w * l;

	return	ans;
}

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
)
{
	GSquaternion		tmp;

	tmp.x =  a->x * b->w + a->y * b->z - a->z * b->y + a->w * b->x;
	tmp.y = -a->x * b->z + a->y * b->w + a->z * b->x + a->w * b->y;
	tmp.z =  a->x * b->y - a->y * b->x + a->z * b->w + a->w * b->z;
	tmp.w = -a->x * b->x - a->y * b->y - a->z * b->z + a->w * b->w;

	*ans = tmp;

	return	ans;
}

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
)
{
	ans->x = a->x * s;
	ans->y = a->y * s;
	ans->z = a->z * s;
	ans->w = a->w * s;

	return	ans;
}

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
)
{
	ans->x = a->x + b->x;
	ans->y = a->y + b->y;
	ans->z = a->z + b->z;
	ans->w = a->w + b->w;

	return	ans;
}

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
)
{
	ans->x = a->x - b->x;
	ans->y = a->y - b->y;
	ans->z = a->z - b->z;
	ans->w = a->w - b->w;

	return	ans;
}

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
)
{
	q->x = gsSin( angle / 2.0f ) * vx;
	q->y = gsSin( angle / 2.0f ) * vy;
	q->z = gsSin( angle / 2.0f ) * vz;
	q->w = gsCos( angle / 2.0f );

	return	q;
}

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
)
{
	GSfloat				fSinYaw;
	GSfloat				fSinPitch;
	GSfloat				fSinRoll;
	GSfloat				fCosYaw;
	GSfloat				fCosPitch;
	GSfloat				fCosRoll;

	fSinYaw   = gsSin( fYaw   / 2.0f );
	fSinPitch = gsSin( fPitch / 2.0f );
	fSinRoll  = gsSin( fRoll  / 2.0f );
	fCosYaw   = gsCos( fYaw   / 2.0f );
	fCosPitch = gsCos( fPitch / 2.0f );
	fCosRoll  = gsCos( fRoll  / 2.0f );

	q->x = fSinRoll * fCosPitch * fCosYaw - fCosRoll * fSinPitch * fSinYaw;
	q->y = fCosRoll * fSinPitch * fCosYaw + fSinRoll * fCosPitch * fSinYaw;
	q->z = fCosRoll * fCosPitch * fSinYaw - fSinRoll * fSinPitch * fCosYaw;
	q->w = fCosRoll * fCosPitch * fCosYaw + fSinRoll * fSinPitch * fSinYaw;

	return	q;
}

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
)
{
	pOut->x = LERP( t, pQ1->x, pQ2->x );
	pOut->y = LERP( t, pQ1->y, pQ2->y );
	pOut->z = LERP( t, pQ1->z, pQ2->z );
	pOut->w = LERP( t, pQ1->w, pQ2->w );

	gsQuaternionNormalize( pOut, pOut );

	return	pOut;
}

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
)
{
	GSfloat				l;

	l = gsQuaternionLength( q );
	l = ( l > 0.0f ) ? ( 1.0f / l ): 0.0f;

	pOut->x = -q->x * l;
	pOut->y = -q->y * l;
	pOut->z = -q->z * l;
	pOut->w =  q->w * l;

	return	pOut;
}

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
)
{
	*pAngle = gsAcos( q->w ) * 2.0f;
	*pVx    = q->x / gsSin( *pAngle / 2.0f );
	*pVy    = q->y / gsSin( *pAngle / 2.0f );
	*pVz    = q->z / gsSin( *pAngle / 2.0f );
}

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
)
{
	GSfloat				tr;
	GSfloat				fourD;

	tr = mat->m[0][0] + mat->m[1][1] + mat->m[2][2] + 1.0f;
	if ( tr >= 1.0f )
	{
		fourD = 2.0f * gsSqrt( tr );
		q->w = fourD / 4.0f;
		q->x = ( mat->m[1][2] - mat->m[2][1] ) / fourD;
		q->y = ( mat->m[2][0] - mat->m[0][2] ) / fourD;
		q->z = ( mat->m[0][1] - mat->m[1][0] ) / fourD;
	}
	else
	{
		GSfloat		qa[4];
		int			i, j, k;

		if ( mat->m[0][0] > mat->m[1][1] )
		{
			i = 0;
		}
		else
		{
			i = 1;
		}
		if ( mat->m[2][2] > mat->m[i][i] )
		{
			i = 2;
		}
		j = ( i + 1 ) % 3;
		k = ( j + 1 ) % 3;

		tr = mat->m[i][i] - mat->m[j][j] - mat->m[k][k] + 1.0f;

		fourD = 2.0f * gsSqrt( tr );

		qa[i] = fourD / 4.0f;
		qa[j] = ( mat->m[j][i] + mat->m[i][j] ) / fourD;
		qa[k] = ( mat->m[k][i] + mat->m[i][k] ) / fourD;
		qa[3] = ( mat->m[j][k] - mat->m[k][j] ) / fourD;

		q->x = qa[0];
		q->y = qa[1];
		q->z = qa[2];
		q->w = qa[3];
	}

	return	q;
}

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
)
{
	GSfloat				tr;
	GSfloat				fourD;

	tr = mat->m[0][0] + mat->m[1][1] + mat->m[2][2] + 1.0f;
	if ( tr >= 1.0f )
	{
		fourD = 2.0f * gsSqrt( tr );
		q->w = fourD / 4.0f;
		q->x = ( mat->m[1][2] - mat->m[2][1] ) / fourD;
		q->y = ( mat->m[2][0] - mat->m[0][2] ) / fourD;
		q->z = ( mat->m[0][1] - mat->m[1][0] ) / fourD;
	}
	else
	{
		GSfloat		qa[4];
		int			i, j, k;

		if ( mat->m[0][0] > mat->m[1][1] )
		{
			i = 0;
		}
		else
		{
			i = 1;
		}
		if ( mat->m[2][2] > mat->m[i][i] )
		{
			i = 2;
		}
		j = ( i + 1 ) % 3;
		k = ( j + 1 ) % 3;

		tr = mat->m[i][i] - mat->m[j][j] - mat->m[k][k] + 1.0f;

		fourD = 2.0f * gsSqrt( tr );

		qa[i] = fourD / 4.0f;
		qa[j] = ( mat->m[j][i] + mat->m[i][j] ) / fourD;
		qa[k] = ( mat->m[k][i] + mat->m[i][k] ) / fourD;
		qa[3] = ( mat->m[j][k] - mat->m[k][j] ) / fourD;

		q->x = qa[0];
		q->y = qa[1];
		q->z = qa[2];
		q->w = qa[3];
	}

	return	q;
}

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
)
{
	GSquaternion		tb;
	GSfloat				fCosTheta;
	GSfloat				fBeta;

	fCosTheta = a->x * b->x + a->y * b->y + a->z * b->z + a->w * b->w;

	if( fCosTheta < 0.0f ) 
	{
		fCosTheta = -fCosTheta;
		tb.x      = -b->x;
		tb.y      = -b->y; 
		tb.z      = -b->z;
		tb.w      = -b->w;
	}
	else
	{
		tb = *b;
	}

	fBeta = 1.0f - t;

	if ( ( 1.0f - fCosTheta ) > 0.001f ) 
	{
		GSfloat fTheta;

		fTheta = (GSfloat)acos( fCosTheta );

		fBeta  = (GSfloat)( sin( fTheta*fBeta  ) / sin( fTheta ) );
		t      = (GSfloat)( sin( fTheta*t      ) / sin( fTheta ) );
	}

	pOut->x = fBeta * a->x + t * tb.x;
	pOut->y = fBeta * a->y + t * tb.y;
	pOut->z = fBeta * a->z + t * tb.z;
	pOut->w = fBeta * a->w + t * tb.w;

	gsQuaternionNormalize( pOut, pOut );

	return	pOut;
}

/********** End of File ******************************************************/
