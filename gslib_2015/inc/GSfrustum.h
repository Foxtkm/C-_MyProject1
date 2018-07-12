/******************************************************************************
* File Name : GSfrustum.h                        Ver : 1.00  Date : 2002-03-27
*
* Description :
*
*       視錐台カリングヘッダファイル．
*
* Author : Moriharu Ohzu.
*
******************************************************************************/
#ifndef		_GSFRUSTUM_H_
#define		_GSFRUSTUM_H_

#include	"GStype.h"
#include	"GSplane.h"

/****** データ型宣言 *********************************************************/

/*----- クリッピングプレーン列挙型 ------------------------------------------*/

enum
{
	GS_FRUSTUM_LEFT_PLANE = 0,			/* 左クリッピングプレーン	*/
	GS_FRUSTUM_RIGHT_PLANE,				/* 右クリッピングプレーン	*/
	GS_FRUSTUM_TOP_PLANE,				/* 上クリッピングプレーン	*/
	GS_FRUSTUM_BOTTOM_PLANE,			/* 下クリッピングプレーン	*/
	GS_FRUSTUM_NEAR_PLANE,				/* 近クリッピングプレーン	*/
	GS_FRUSTUM_FAR_PLANE				/* 遠クリッピングプレーン	*/
};

/*----- 視錐台構造体 --------------------------------------------------------*/

typedef	struct							/* 視錐台構造体				*/
{
	GSplane		Planes[6];				/* クリッピングプレーン		*/
	GSmatrix4	View;					/* 視野変換行列				*/
	GSmatrix4	Projection;				/* 透視変換行列				*/

} GSfrustum;

/****** 関数プロトタイプ宣言 *************************************************/

#ifdef __cplusplus
extern "C" {
#endif

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
);

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
);

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
);

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
);

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
);

#ifdef __cplusplus
}
#endif

#endif

/********** End of File ******************************************************/
