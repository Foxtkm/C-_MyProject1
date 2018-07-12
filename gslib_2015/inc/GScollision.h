/******************************************************************************
* File Name : GScollision.h                      Ver : 1.00  Date : 2002-03-24
*
* Description :
*
*       衝突判定の計算 ヘッダファイル．
*
* Author : Moriharu Ohzu.
*
******************************************************************************/
#ifndef		_GSCOLLISION_H_
#define		_GSCOLLISION_H_

#include	"GStype.h"
#include	"GSmesh.h"

/****** 関数プロトタイプ宣言 *************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*=============================================================================
*
* Purpose : 球体と球体の衝突判定．
*
* Return  : 衝突していれば真，衝突していなければ偽を返す．
*
*============================================================================*/
extern GSboolean
gsCollisionSphereAndSphere
(
	const GSvector3*	pCenter1,		/* 球体１の中心					*/
	GSfloat				Radius1,		/* 球体１の半径					*/
	const GSvector3*	pCenter2,		/* 球体２の中心					*/
	GSfloat				Radius2			/* 球体２の半径					*/
);

/*=============================================================================
*
* Purpose : ポリゴンと線分の衝突判定．
*
* Return  : ポリゴンと線分が交差していれば真，交差していなければ偽を返す．
*
*============================================================================*/
extern GSboolean
gsCollisionPolyAndLine
(
	const GSvector3*	pPoly,			/* ポリゴンの頂点配列				*/
	GSuint				numVertices,	/* 頂点データ数						*/
	const GSplane*		pPlane,			/* ポリゴンの平面方程式パラメータ	*/
	const GSvector3*	pLine0,			/* 線分の始点						*/
	const GSvector3*	pLine1,			/* 線分の終点						*/
	GSvector3*			pIntersectPoint	/*-> ポリゴンとの線分交点			*/
);

/*=============================================================================
*
* Purpose : ポリゴンとレイの衝突判定．
*
* Return  : ポリゴンとレイが交差していれば真，交差していなければ偽を返す．
*
*============================================================================*/
extern GSboolean
gsCollisionPolyAndRay
(
	const GSvector3*	pPoly,			/* ポリゴンの頂点配列				*/
	GSuint				numVertices,	/* 頂点データ数						*/
	const GSplane*		pPlane,			/* ポリゴンの平面方程式パラメータ	*/
	const GSvector3*	pRayPos,		/* レイの始点						*/
	const GSvector3*	pRayDir,		/* レイの方向						*/
	GSvector3*			pIntersectPoint	/*-> ポリゴンとの線分交点			*/
);

/*=============================================================================
*
* Purpose : ポリゴンと球体の衝突判定．
*
* Return  : ポリゴンと球体が衝突していれば真，衝突していなければ偽を返す．
*
*============================================================================*/
extern GSboolean
gsCollisionPolyAndSphere
(
	const GSvector3*	pPoly,			/* ポリゴンの頂点配列				*/
	GSuint				numVertices,	/* 頂点データ数						*/
	const GSplane*		pPlane,			/* ポリゴンの平面方程式パラメータ	*/
	const GSvector3*	pCenter,		/* 球体の中心						*/
	GSfloat				Radius,			/* 球体の半径						*/
	GSvector3*			pOffset			/*-> 球体が衝突しない座標への補正値	*/
);

/*=============================================================================
*
* Purpose : メッシュと球体の衝突判定．
*
* Return  : メッシュと球体が衝突していれば真，衝突していなければ偽を返す．
*
*============================================================================*/
extern GSboolean
gsCollisionMeshAndSphere
(
	const GSmesh*		pMesh,			/* メッシュ					*/
	const GSvector3*	pCenter,		/* 球体の中心				*/
	GSfloat				Radius,			/* 球体の半径				*/
	GSvector3*			pOutCenter		/* 補正後の球の中心座標		*/
);

/*=============================================================================
*
* Purpose : メッシュとレイの衝突判定．
*
* Return  : メッシュとレイが交差していれば真，交差していなければ偽を返す．
*
*============================================================================*/
extern GSboolean
gsCollisionMeshAndRay
(
	const GSmesh*		pMesh,			/* メッシュ						*/
	const GSvector3*	pRayPos,		/* レイの始点					*/
	const GSvector3*	pRayDir,		/* レイの方向					*/
	GSvector3*			pIntersect,		/*-> メッシュとレイの交点		*/
	GSplane*			pPlane			/*-> 衝突した面の平面パラメータ	*/
);

/*=============================================================================
*
* Purpose : 球体とレイの衝突判定．
*
* Return  : レイと球体が衝突していれば真，衝突していなければ偽を返す．
*
*============================================================================*/
extern GSboolean
gsCollisionSphereAndRay
(
	const GSvector3*	pCenter,		/* 球体の中心座標				*/
	GSfloat				Radius,			/* 球体の半径					*/
	const GSvector3*	pRayPos,		/* レイの位置					*/
	const GSvector3*	pRayDir,		/* レイの方向					*/
	GSvector3*			pIntersect		/*-> レイとの交点				*/
);

/*=============================================================================
*
* Purpose : メッシュと線分の衝突判定．
*
* Return  : メッシュと線分が交差していれば真，交差していなければ偽を返す．
*
*============================================================================*/
extern GSboolean
gsCollisionMeshAndLine
(
	const GSmesh*		pMesh,			/* メッシュ						*/
	const GSvector3*	pLine0,			/* 線分の始点					*/
	const GSvector3*	pLine1,			/* 線分の終点					*/
	GSvector3*			pIntersect,		/*-> メッシュと線分の交点		*/
	GSplane*			pPlane			/*-> 衝突した面の平面パラメータ	*/
);

/*=============================================================================
*
* Purpose : 球体と線分の衝突判定．
*
* Return  : 球体と球体が衝突していれば真，衝突していなければ偽を返す．
*
*============================================================================*/
extern GSboolean
gsCollisionSphereAndLine
(
	const GSvector3*	pCenter,		/* 球体の中心座標			*/
	GSfloat				Radius,			/* 球体の半径				*/
	const GSvector3*	pLine0,			/* 線分の始点				*/
	const GSvector3*	pLine1,			/* 線分の終点				*/
	GSvector3*			pIntersect		/*-> 線分との交点			*/
);

/*=============================================================================
*
* Purpose : 線分と線分の衝突判定．
*
* Return  : 線分と線分が衝突していれば真，衝突していなければ偽を返す．
*
*============================================================================*/
extern GSboolean
gsCollisionLineAndLine
(
	const GSvector3*	pA1,			/* 線分ａの始点						*/
	const GSvector3*	pA2,			/* 線分ａの終点						*/
	const GSvector3*	pB1,			/* 線分ｂの始点						*/
	const GSvector3*	pB2,			/* 線分ｂの終点						*/
	GSboolean			bInfinite,		/* 無限線分か？						*/
	GSfloat				Epsilon,		/* 判定の閾値						*/
	GSvector3*			pPA,			/*-> 線分ａ上の線分ｂに最も近い座標	*/
	GSvector3*			pPB				/*-> 線分ｂ上の線分ａに最も近い座標	*/
);

/*=============================================================================
*
* Purpose : カプセルとカプセルの衝突判定．
*
* Return  : カプセルが衝突していれば真，衝突していなければ偽を返す．
*
*============================================================================*/
extern GSboolean
gsCollisionCapsuleAndCapsule
(
	const GSvector3*	pA1,			/* カプセルａの始点					*/
	const GSvector3*	pA2,			/* カプセルａの終点					*/
	const float			Aw,				/* カプセルａの幅					*/
	const GSvector3*	pB1,			/* カプセルｂの始点					*/
	const GSvector3*	pB2,			/* カプセルｂの終点					*/
	const float			Bw,				/* カプセルｂの幅					*/
	GSvector3*			pNa,			/*-> 線分ａ上の線分ｂに最も近い座標	*/
	GSvector3*			pNb				/*-> 線分ｂ上の線分ａに最も近い座標	*/
);

#ifdef __cplusplus
}
#endif

/****** マクロ関数宣言 *******************************************************/

/*=============================================================================
*
* Purpose : 点と矩形の衝突判定．
*
* Return  : 衝突していれば真，衝突していなければ偽を返す．
*
*============================================================================*/
#define	gsCollisionPointAndRect( p, r )										\
(																			\
   ( ( (r)->left <= (p)->x ) && ( (r)->right  >= (p)->x )					\
  && ( (r)->top  <= (p)->y ) && ( (r)->bottom >= (p)->y ) )					\
  ? GS_TRUE: GS_FALSE														\
)

/*=============================================================================
*
* Purpose : 矩形と矩形の衝突判定．
*
* Return  : 衝突していれば真，衝突していなければ偽を返す．
*
*============================================================================*/
#define	gsCollisionRectAndRect( r1, r2 )									\
(																			\
   ( ( (r2)->left <= (r1)->right  ) && ( (r1)->left <= (r2)->right  )		\
  && ( (r2)->top  <= (r1)->bottom ) && ( (r1)->top  <= (r2)->bottom ) )		\
  ? GS_TRUE: GS_FALSE														\
)

#endif

/********** End of File ******************************************************/
