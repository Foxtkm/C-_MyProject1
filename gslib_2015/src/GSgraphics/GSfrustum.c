/******************************************************************************
* File Name : GSfrustum.c                        Ver : 1.00  Date : 2001-03-27
*
* Description :
*
*       視錐台カリング．
*
* Author : Moriharu Ohzu.
*
******************************************************************************/
#include	"GSfrustum.h"
#include	"GSmatrix4.h"
#include	"GSvector3.h"

/*=============================================================================
*
* Purpose : 視野変換行列，透視変換行列から視錐台を作成する．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsFrustumFromMatrices
(
	GSfrustum*			pFrustum,		/* 視錐台				*/
	const GSmatrix4*	pView,			/* 視野変換行列			*/
	const GSmatrix4*	pProjection		/* 透視変換行列			*/
)
{
	GSmatrix4			Clip;
	int					i;
	/* 視野変換行列と透視変換行列を保存する */
	pFrustum->View       = *pView;
	pFrustum->Projection = *pProjection;

	/* 視野変換行列と透視変換行列を合成する */
	gsMatrix4Multiply(&Clip, &pFrustum->View, &pFrustum->Projection);

	/* クリッピングプレーンを計算する */
	pFrustum->Planes[GS_FRUSTUM_RIGHT_PLANE].a=Clip.m[0][3]-Clip.m[0][0];
	pFrustum->Planes[GS_FRUSTUM_RIGHT_PLANE].b=Clip.m[1][3]-Clip.m[1][0];
	pFrustum->Planes[GS_FRUSTUM_RIGHT_PLANE].c=Clip.m[2][3]-Clip.m[2][0];
	pFrustum->Planes[GS_FRUSTUM_RIGHT_PLANE].d=Clip.m[3][3]-Clip.m[3][0];

	pFrustum->Planes[GS_FRUSTUM_LEFT_PLANE].a=Clip.m[0][3]+Clip.m[0][0];
	pFrustum->Planes[GS_FRUSTUM_LEFT_PLANE].b=Clip.m[1][3]+Clip.m[1][0];
	pFrustum->Planes[GS_FRUSTUM_LEFT_PLANE].c=Clip.m[2][3]+Clip.m[2][0];
	pFrustum->Planes[GS_FRUSTUM_LEFT_PLANE].d=Clip.m[3][3]+Clip.m[3][0];

	pFrustum->Planes[GS_FRUSTUM_BOTTOM_PLANE].a=Clip.m[0][3]+Clip.m[0][1];
	pFrustum->Planes[GS_FRUSTUM_BOTTOM_PLANE].b=Clip.m[1][3]+Clip.m[1][1];
	pFrustum->Planes[GS_FRUSTUM_BOTTOM_PLANE].c=Clip.m[2][3]+Clip.m[2][1];
	pFrustum->Planes[GS_FRUSTUM_BOTTOM_PLANE].d=Clip.m[3][3]+Clip.m[3][1];

	pFrustum->Planes[GS_FRUSTUM_TOP_PLANE].a=Clip.m[0][3]-Clip.m[0][1];
	pFrustum->Planes[GS_FRUSTUM_TOP_PLANE].b=Clip.m[1][3]-Clip.m[1][1];
	pFrustum->Planes[GS_FRUSTUM_TOP_PLANE].c=Clip.m[2][3]-Clip.m[2][1];
	pFrustum->Planes[GS_FRUSTUM_TOP_PLANE].d=Clip.m[3][3]-Clip.m[3][1];

	pFrustum->Planes[GS_FRUSTUM_FAR_PLANE].a=Clip.m[0][3]-Clip.m[0][2];
	pFrustum->Planes[GS_FRUSTUM_FAR_PLANE].b=Clip.m[1][3]-Clip.m[1][2];
	pFrustum->Planes[GS_FRUSTUM_FAR_PLANE].c=Clip.m[2][3]-Clip.m[2][2];
	pFrustum->Planes[GS_FRUSTUM_FAR_PLANE].d=Clip.m[3][3]-Clip.m[3][2];

	pFrustum->Planes[GS_FRUSTUM_NEAR_PLANE].a=Clip.m[0][3]+Clip.m[0][2];
	pFrustum->Planes[GS_FRUSTUM_NEAR_PLANE].b=Clip.m[1][3]+Clip.m[1][2];
	pFrustum->Planes[GS_FRUSTUM_NEAR_PLANE].c=Clip.m[2][3]+Clip.m[2][2];
	pFrustum->Planes[GS_FRUSTUM_NEAR_PLANE].d=Clip.m[3][3]+Clip.m[3][2];

	/* クリッピングプレーンを正規化する */
	for ( i = 0; i < 6; i++ )
	{
		gsPlaneNormalize( &pFrustum->Planes[i], &pFrustum->Planes[i] );
	}
}

/*=============================================================================
*
* Purpose : 点が視錐台内に存在するか調べる．
*
* Return  : 視錐台内に存在すれば真，存在しなければ偽を返す．
*
*============================================================================*/
extern GSboolean
gsFrustumIsPointInside
(
	const GSfrustum*	pFrustum,		/* 視錐台				*/
	const GSvector3*	pPoint			/* 点の座標				*/
)
{
	int					i;

	for( i = 0; i < 6; i++ )
	{
		/* クリッピングプレーンの背面に座標が存在するか？ */
		if( gsPlaneClassifyPoint( &pFrustum->Planes[i], pPoint, NULL ) == GS_BEHIND_PLANE )
		{
			/* 視錐台の外側に点が存在する */
			return	GS_FALSE;
		}
	}

	/* 視錐台の内側に点が存在する */
	return	GS_TRUE;
}

/*=============================================================================
*
* Purpose : ボックスが視錐台内に存在するか調べる．
*
* Return  : 視錐台内に存在すれば真，存在しなければ偽を返す．
*
*============================================================================*/
extern GSboolean
gsFrustumIsBoxInside
(
	const GSfrustum*	pFrustum,		/* 視錐台				*/
	const GSvector3*	pP1,			/* ボックスを表す座標１	*/
	const GSvector3*	pP2				/* ボックスを表す座標２	*/
)
{
	GSvector3			Vertices[8];
	int					i;

	/* ボックスの各頂点を求める */
	gsVector3Init( &Vertices[0], pP1->x, pP1->y, pP1->z );
	gsVector3Init( &Vertices[1], pP2->x, pP1->y, pP1->z );
	gsVector3Init( &Vertices[2], pP1->x, pP2->y, pP1->z );
	gsVector3Init( &Vertices[3], pP2->x, pP2->y, pP1->z );
	gsVector3Init( &Vertices[4], pP1->x, pP1->y, pP2->z );
	gsVector3Init( &Vertices[5], pP2->x, pP1->y, pP2->z );
	gsVector3Init( &Vertices[6], pP1->x, pP2->y, pP2->z );
	gsVector3Init( &Vertices[7], pP2->x, pP2->y, pP2->z );

	for( i = 0; i < 6; i++ )
	{
		/* ボックスの全頂点があるクリッププレーン外側に存在するか？ */
		if ( gsPlaneClassifyPoint( &pFrustum->Planes[i], &Vertices[0], NULL ) != GS_BEHIND_PLANE ) continue;
		if ( gsPlaneClassifyPoint( &pFrustum->Planes[i], &Vertices[1], NULL ) != GS_BEHIND_PLANE ) continue;
		if ( gsPlaneClassifyPoint( &pFrustum->Planes[i], &Vertices[2], NULL ) != GS_BEHIND_PLANE ) continue;
		if ( gsPlaneClassifyPoint( &pFrustum->Planes[i], &Vertices[3], NULL ) != GS_BEHIND_PLANE ) continue;
		if ( gsPlaneClassifyPoint( &pFrustum->Planes[i], &Vertices[4], NULL ) != GS_BEHIND_PLANE ) continue;
		if ( gsPlaneClassifyPoint( &pFrustum->Planes[i], &Vertices[5], NULL ) != GS_BEHIND_PLANE ) continue;
		if ( gsPlaneClassifyPoint( &pFrustum->Planes[i], &Vertices[6], NULL ) != GS_BEHIND_PLANE ) continue;
		if ( gsPlaneClassifyPoint( &pFrustum->Planes[i], &Vertices[7], NULL ) != GS_BEHIND_PLANE ) continue;

		/* 視錐台の外側にボックスが存在する */
		return	GS_FALSE;
	}

	/* 視錐台の内側にボックスが存在する */
	return	GS_TRUE;
}

/*=============================================================================
*
* Purpose : 球体が視錐台内に存在するか調べる．
*
* Return  : 視錐台内に存在すれば真，存在しなければ偽を返す．
*
*============================================================================*/
extern GSboolean
gsFrustumIsSphereInside
(
	const GSfrustum*	pFrustum,		/* 視錐台					*/
	const GSvector3*	pCenter,		/* 球体の中心座標			*/
	GSfloat				Radius			/* 球体の半径				*/
)
{
	int					i;

	for( i = 0; i < 6; i++ )
	{
		/* クリッピングプレーンの背面に球体が存在するか？ */
		if( gsPlaneClassifySphere( &pFrustum->Planes[i], pCenter, Radius, NULL ) == GS_BEHIND_PLANE )
		{
			/* 視錐台の外側に点が存在する */
			return	GS_FALSE;
		}
	}

	/* 視錐台の内側に点が存在する */
	return	GS_TRUE;
}

/*=============================================================================
*
* Purpose : 視推台のコーナーの座標を求める．
*
* Return  : 視錐台内に存在すれば真，存在しなければ偽を返す．
*
*============================================================================*/
extern void
gsFrustumGetCorners
(
	const GSfrustum*	pFrustum, 		/* 視錐台					*/
	GSvector3			Corners[8]		/*-> コーナーの座標			*/
)
{
	gsMatrix4GetFrustumCorners(Corners, &pFrustum->View, &pFrustum->Projection);
}

/********** End of File ******************************************************/
