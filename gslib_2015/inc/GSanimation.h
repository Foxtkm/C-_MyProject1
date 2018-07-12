/******************************************************************************
* File Name : GSanimation.h                      Ver : 1.00  Date : 2002-08-03
*
* Description :
*
*		アニメーションデータ管理 ヘッダファイル．．
*
* Author : Moriharu Ohzu.
*
******************************************************************************/
#ifndef		_GSANIMATION_H_
#define		_GSANIMATION_H_

#include	"GStype.h"

/****** データ型宣言 *********************************************************/

/*----- スケールキー構造体 --------------------------------------------------*/

typedef	struct							/* スケールキー型			*/
{
	GSuint			Time;				/* フレームタイマ			*/
	GSvector3		Scale;				/* スケール値				*/

} GSscalekey;

/*----- ローテートキー構造体 ------------------------------------------------*/

typedef	struct							/* ローテートキー型			*/
{
	GSuint			Time;				/* フレームタイマ			*/
	GSquaternion	Rotate;				/* ローテート値				*/

} GSrotatekey;

/*----- ポジションキー構造体 ------------------------------------------------*/

typedef	struct							/* ポジションキー型			*/
{
	GSuint			Time;				/* フレームタイマ			*/
	GSvector3		Position;			/* ポジション値				*/

} GSpositionkey;

/*----- キーフレーム型構造体 ------------------------------------------------*/

typedef	struct							/* キーフレーム型			*/
{
	GSuint			fKeyFrameInfo;		/* キーフレーム情報			*/
	GSuint			numScaleKeys;		/* スケールキー数			*/
	GSuint			numRotateKeys;		/* ローテートキー数			*/
	GSuint			numPositionKeys;	/* ポジションキー数			*/
	GSscalekey*		pScaleKeys;			/* スケールキー 			*/
	GSrotatekey*	pRotateKeys;		/* ローテートキー 			*/
	GSpositionkey*	pPositionKeys;		/* ポジションキー 			*/

} GSkeyframe;

/*----- ボーンフレーム構造体 ------------------------------------------------*/

typedef	struct							/* ボーンフレーム型			*/
{
	GSuint			numBoneFrames;		/* ボーンフレーム数			*/
	GSkeyframe*		pBoneFrames;		/* ボーンフレームデータ数	*/

} GSboneframe;

/*----- アニメーションデータ構造体 ------------------------------------------*/

typedef	struct							/* アニメーションデータ型	*/
{
	GSuint			numAnimations;		/* アニメーションデータ数	*/
	GSboneframe*	pAnimations;		/* アニメーションデータ		*/

} GSanimation;

/****** 関数プロトタイプ宣言 *************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*=============================================================================
*
* Purpose : アニメーションデータの生成．
*
* Return  : アニメーションデータを返す．
*
*============================================================================*/
extern GSanimation*
gsAnimationNew
(
	void
);

/*=============================================================================
*
* Purpose : アニメーションデータの消滅．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsAnimationDelete
(
	GSanimation*		pAnimation				/* アニメーションデータ			*/
);

/*=============================================================================
*
* Purpose : アニメーションデータのクリーンアップ．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsAnimationCleanUp
(
	GSanimation*	pAnimation			/* アニメーションデータ			*/
);


/*=============================================================================
*
* Purpose : アニメーションキーの取得．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsAnimationGetKeyFrame
(
	GSanimation*	pAnimation,			/* アニメーションデータ			*/
	GSuint			AnimationNo,		/* 取得するモーションナンバ		*/
	GSuint			BoneNo,				/* 取得するボーンナンバ			*/
	GSfloat			fGlobalTime,		/* 取得するキーのタイマ			*/
	GSvector3*		pOutScale,			/*-> スケール値					*/
	GSquaternion*	pOutRotate,			/*-> ローテート値				*/
	GSvector3*		pOutPosition		/*-> ポジション値				*/
);

/*=============================================================================
*
* Purpose : アニメーションファイルの読み込み．
*
* Return  : 読み込みに成功すれば真，失敗すれば偽を返す．
*
*============================================================================*/
extern GSboolean
gsAnimationLoadFile
(
	GSanimation*	pAnimation,			/* アニメーションデータ			*/
	const char*		pszFileName			/* モーションファイル名		*/
);

/*=============================================================================
*
* Purpose : 指定アニメーションのボーン数を返す．
*
* Return  : ボーン数を返す．
*
*============================================================================*/
extern GSuint
gsAnimationGetNumBones
(
	GSanimation*	pAnimation,			/* アニメーションデータ		*/
	GSuint			AnimationNo			/* アニメーションナンバ		*/
);

/*=============================================================================
*
* Purpose : アニメーションの終了タイマを調べる．
*
* Return  : アニメーションの終了タイマを返す．
*
*============================================================================*/
extern GSuint
gsAnimationGetEndTime
(
	GSanimation*	pAnimation,			/* アニメーションデータ		*/
	GSuint			AnimationNo			/* アニメーションナンバ		*/
);

#ifdef __cplusplus
}
#endif

#endif

/********** End of File ******************************************************/
