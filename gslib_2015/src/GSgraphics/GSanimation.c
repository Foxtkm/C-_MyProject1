/******************************************************************************
* File Name : GSanimation.c                      Ver : 1.00  Date : 2002-07-31
*
* Description :
*
*       アニメーションデータ管理．
*
* Author : Moriharu Ohzu.
*
******************************************************************************/
#include	"GSanimation.h"
#include	"GSquaternion.h"
#include	"GSvector3.h"
#include	"GSmath.h"
#include	"gfile.h"
#include	"heap.h"
#include    "GSfile_path.h"
#include	<string.h>
#include	<memory.h>
#include	<math.h>

/****** 関数プロトタイプ宣言 *************************************************/

static void gsBoneFrameCleanUp( GSboneframe* );
static void gsKeyFrameCleanUp( GSkeyframe* );
static void gsAnimationInitialize( GSanimation*, GSuint );
static void gsBoneFrameInitialize( GSboneframe*, GSuint );
static void gsKeyFrameInitialize( GSkeyframe*, GSuint, GSuint, GSuint, GSuint );
static GSboolean gsBoneFrameLoadFile( GSboneframe*, GFILE );
static GSboolean gsKeyFrameLoadFile( GSkeyframe*, GFILE );
static GSboolean gsScaleKeyLoadFile( GSscalekey*, GFILE );
static GSboolean gsRotateKeyLoadFile( GSrotatekey*, GFILE );
static GSboolean gsPositionKeyLoadFile( GSpositionkey*, GFILE );
static GSboolean gsBoneFrameLoadFileBin(GSboneframe*, GFILE);
static GSboolean gsKeyFrameLoadFileBin(GSkeyframe*, GFILE);
static GSboolean gsScaleKeyLoadFileBin(GSscalekey*, GFILE);
static GSboolean gsRotateKeyLoadFileBin(GSrotatekey*, GFILE);
static GSboolean gsPositionKeyLoadFileBin(GSpositionkey*, GFILE);
static void gsBoneFrameGetKeyFrame( GSboneframe*, GSuint, GSfloat, GSvector3*, GSquaternion*, GSvector3* );
static void gsKeyFrameGetKey( GSkeyframe*, GSfloat, GSvector3*, GSquaternion*, GSvector3* );
static void gsScaleKeyGetScale( GSuint, GSscalekey*, GSfloat, GSvector3* );
static void gsRotateKeyGetRotate( GSuint, GSrotatekey*, GSfloat, GSquaternion* );
static void gsPositionKeyGetPosition( GSuint, GSpositionkey*, GSfloat, GSvector3* );

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
)
{
	GSanimation*		pAnimation;

	pAnimation = new_object( sizeof( GSanimation ) );

	memset( pAnimation, 0, sizeof( GSanimation ) );

	return	pAnimation;
}

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
	GSanimation*	pAnimation			/* アニメーションデータ			*/
)
{
	/* アニメーションデータのクリーンアップ */
	gsAnimationCleanUp( pAnimation );

	/* アニメーションデータを消滅させる */
	del_object( pAnimation );
}

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
)
{
	GSuint			i;

	/* ボーンフレーム配列のクリーンアップ */
	for ( i = 0; i < pAnimation->numAnimations; i++ )
	{
		gsBoneFrameCleanUp( &pAnimation->pAnimations[i] );
	}

	if ( pAnimation->pAnimations != NULL )
	{
		/* ボーンフレーム配列を消滅 */
		del_object( pAnimation->pAnimations );

		pAnimation->pAnimations = NULL;
	}

	pAnimation->numAnimations = 0;
}

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
)
{
	/* 各キーの値を初期化する */
	gsVector3Init( pOutScale, 1.0f, 1.0f, 1.0f );
	gsQuaternionIdentity( pOutRotate );
	gsVector3Init( pOutPosition, 0.0f, 0.0f, 0.0f );

	/* アニメーションナンバがアニメーションデータ数よりも大きい？ */
	if ( AnimationNo >= pAnimation->numAnimations )
	{
		return;
	}

	/* 指定アニメーション，指定ボーンのアニメーションキーを取得する */
	gsBoneFrameGetKeyFrame
	(
		&pAnimation->pAnimations[ AnimationNo ],
		BoneNo, fGlobalTime,
		pOutScale, pOutRotate, pOutPosition
	);
}

/*=============================================================================
*
* Purpose : アニメーションファイルの読み込み．
*
* Return  : 読み込みに成功すれば真，失敗すれば偽を返す．
*
*============================================================================*/
extern GSboolean
gsAnimationLoadFileTxt
(
	GSanimation*	pAnimation,			/* アニメーションデータ			*/
	const char*		pszFileName			/* モーションファイル名		*/
)
{
	GFILE			file;
	GSuint			numAnimations;
	GSuint			i;

	/* アニメーションファイルのオープン */
	if ( ( file = GFileOpen( pszFileName, "r" ) ) == NULL )
	{
		return	GS_FALSE;
	}

	/* アニメーションデータのクリーンアップ */
	gsAnimationCleanUp( pAnimation );

	/* アニメーションデータ数の読み込み */
	GFileReadDword( file, &numAnimations );

	/* アニメーションデータの初期化 */
	gsAnimationInitialize( pAnimation, numAnimations );

	/* ボーンフレームの読み込み */
	for ( i = 0; i < pAnimation->numAnimations; i++ )
	{
		gsBoneFrameLoadFile( &pAnimation->pAnimations[i], file );
	}

	/* アニメーションファイルのクローズ */
	GFileClose( file );

	return	GS_TRUE;
}

/*=============================================================================
*
* Purpose : アニメーションファイルの読み込み．
*
* Return  : 読み込みに成功すれば真，失敗すれば偽を返す．
*
*============================================================================*/
extern GSboolean
gsAnimationLoadFileBin
(
	GSanimation*	pAnimation,			/* アニメーションデータ			*/
	const char*		pszFileName			/* モーションファイル名		*/
)
{
	GFILE			file;
	GSuint			numAnimations;
	GSuint			i;

	/* アニメーションファイルのオープン */
	if ((file = GFileOpen(pszFileName, "rb")) == NULL)
	{
		return	GS_FALSE;
	}

	/* アニメーションデータのクリーンアップ */
	gsAnimationCleanUp(pAnimation);

	/* アニメーションデータ数の読み込み */
	GFileReadDwordBin(file, &numAnimations);

	/* アニメーションデータの初期化 */
	gsAnimationInitialize(pAnimation, numAnimations);

	/* ボーンフレームの読み込み */
	for (i = 0; i < pAnimation->numAnimations; i++)
	{
		gsBoneFrameLoadFileBin(&pAnimation->pAnimations[i], file);
	}

	/* アニメーションファイルのクローズ */
	GFileClose(file);

	return	GS_TRUE;
}

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
)
{
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char animation_file[_MAX_PATH];
	/* ファイル名を分解する */
	gsSplitPath(pszFileName, drive, dir, fname, NULL);

	/* anmb -> anm の優先順位で読み込みをする */
	gsMakePath(animation_file, drive, dir, fname, ".anmb");
	if (gsAnimationLoadFileBin(pAnimation, animation_file) == GS_TRUE) {
		return GS_TRUE;
	}
	gsMakePath(animation_file, drive, dir, fname, ".anm");
	if (gsAnimationLoadFileTxt(pAnimation, animation_file) == GS_TRUE) {
		return GS_TRUE;
	}
	return GS_FALSE;
}

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
)
{
	if ( AnimationNo >= pAnimation->numAnimations )
	{
		return	0;
	}

	return	pAnimation->pAnimations[ AnimationNo ].numBoneFrames;
}

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
)
{
	GSboneframe*	pBoneFrame;
	GSkeyframe*		pKeyFrame;
	GSuint			EndTime;
	GSuint			i;

	if ( AnimationNo >= pAnimation->numAnimations )
	{
		return	0;
	}

	/* 終了タイムの初期化 */
	EndTime = 0;

	/* 終了タイマの一番多いキーを検索する */
	pBoneFrame = &pAnimation->pAnimations[ AnimationNo ];
	for ( i = 0; i < pBoneFrame->numBoneFrames; i++ )
	{
		pKeyFrame = &pBoneFrame->pBoneFrames[i];

		if ( pKeyFrame->numScaleKeys != 0 )
		{
			EndTime = MAX( EndTime, pKeyFrame->pScaleKeys[ pKeyFrame->numScaleKeys-1 ].Time );
		}
		if ( pKeyFrame->numRotateKeys != 0 )
		{
			EndTime = MAX( EndTime, pKeyFrame->pRotateKeys[ pKeyFrame->numRotateKeys-1 ].Time );
		}
		if ( pKeyFrame->numPositionKeys != 0 )
		{
			EndTime = MAX( EndTime, pKeyFrame->pPositionKeys[ pKeyFrame->numPositionKeys-1 ].Time );
		}
	}

	return	EndTime;
}

/*-----------------------------------------------------------------------------
*
* Purpose : ボーンフレームのクリーンアップ．
*
* Return  : なし．
*
*----------------------------------------------------------------------------*/
static void
gsBoneFrameCleanUp
(
	GSboneframe*		pBoneFrame			/* ボーンフレーム			*/
)
{
	GSuint			i;

	/* キーフレーム配列のクリーンアップ */
	for ( i = 0; i < pBoneFrame->numBoneFrames; i++ )
	{
		gsKeyFrameCleanUp( &pBoneFrame->pBoneFrames[i] );
	}

	if ( pBoneFrame->pBoneFrames != NULL )
	{
		/* キーフレーム配列を消滅 */
		del_object( pBoneFrame->pBoneFrames );

		pBoneFrame->pBoneFrames = NULL;
	}

	pBoneFrame->pBoneFrames = 0;
}

/*-----------------------------------------------------------------------------
*
* Purpose : キーフレームのクリーンアップ．
*
* Return  : なし．
*
*----------------------------------------------------------------------------*/
static void
gsKeyFrameCleanUp
(
	GSkeyframe*	pKeyFrame				/* キーフレーム				*/
)
{
	/* スケールキーを消滅 */
	if ( pKeyFrame->pScaleKeys != NULL )
	{
		del_object( pKeyFrame->pScaleKeys );
	}

	/* ローテートキーを消滅 */
	if ( pKeyFrame->pRotateKeys != NULL )
	{
		del_object( pKeyFrame->pRotateKeys );
	}

	/* ポジションキーを消滅 */
	if ( pKeyFrame->pPositionKeys != NULL )
	{
		del_object( pKeyFrame->pPositionKeys );
	}

	/* キーフレームデータを初期化 */
	pKeyFrame->fKeyFrameInfo   = 0;
	pKeyFrame->numScaleKeys    = 0;
	pKeyFrame->numRotateKeys   = 0;
	pKeyFrame->numPositionKeys = 0;
	pKeyFrame->pScaleKeys      = NULL;
	pKeyFrame->pRotateKeys     = NULL;
	pKeyFrame->pPositionKeys   = NULL;
}

/*-----------------------------------------------------------------------------
*
* Purpose : アニメーションデータの初期化．
*
* Return  : なし．
*
*----------------------------------------------------------------------------*/
static void
gsAnimationInitialize
(
	GSanimation*	pAnimation,			/* アニメーションデータ			*/
	GSuint			numAnimations		/* アニメーションデータ数		*/
)
{
	size_t			size;

	/* アニメーションデータのクリーンアップ */
	gsAnimationCleanUp( pAnimation );

	pAnimation->numAnimations = numAnimations;

	/* ボーンフレーム配列の生成 */
	if ( ( size = sizeof(GSboneframe) * pAnimation->numAnimations ) != 0 )
	{
		pAnimation->pAnimations = new_object( size );

		memset( pAnimation->pAnimations, 0, size );
	}
}

/*-----------------------------------------------------------------------------
*
* Purpose : ボーンフレームの初期化．
*
* Return  : なし．
*
*----------------------------------------------------------------------------*/
static void
gsBoneFrameInitialize
(
	GSboneframe*	pBoneFrame,			/* ボーンフレーム			*/
	GSuint			numBones			/* ボーンデータ数			*/
)
{
	size_t			size;

	/* ボーンフレームのクリーンアップ */
	gsBoneFrameCleanUp( pBoneFrame );

	/* ボーンフレーム数を設定 */
	pBoneFrame->numBoneFrames = numBones;

	/* ボーンフレーム配列の生成 */
	if ( ( size = sizeof(GSkeyframe) * pBoneFrame->numBoneFrames ) != 0 )
	{
		pBoneFrame->pBoneFrames = new_object( size );

		memset( pBoneFrame->pBoneFrames, 0, size );
	}
}

/*-----------------------------------------------------------------------------
*
* Purpose : キーフレームの初期化．
*
* Return  : なし．
*
*----------------------------------------------------------------------------*/
static void
gsKeyFrameInitialize
(
	GSkeyframe*	pKeyFrame,				/* キーフレーム				*/
	GSuint		fKeyFrameInfo,			/* キーフレーム情報			*/
	GSuint		numScaleKeys,			/* スケールキー数			*/
	GSuint		numRotateKeys,			/* ローテートキー数			*/
	GSuint		numPositionKeys			/* ポジションキー数			*/
)
{
	size_t			size;

	/* キーフレームデータのクリーンアップ */
	gsKeyFrameCleanUp( pKeyFrame );

	/* キーフレームデータの初期化 */
	pKeyFrame->fKeyFrameInfo   = fKeyFrameInfo;
	pKeyFrame->numScaleKeys    = numScaleKeys;
	pKeyFrame->numRotateKeys   = numRotateKeys;
	pKeyFrame->numPositionKeys = numPositionKeys;

	/* スケールキーの生成 */
	pKeyFrame->pScaleKeys = NULL;
	if ( ( size = sizeof(GSscalekey) * pKeyFrame->numScaleKeys ) != 0 )
	{
		pKeyFrame->pScaleKeys = new_object( size );

		memset( pKeyFrame->pScaleKeys, 0, size );
	}

	/* ローテートキーの生成 */
	pKeyFrame->pRotateKeys = NULL;
	if ( ( size = sizeof(GSrotatekey) * pKeyFrame->numRotateKeys ) != 0 )
	{
		pKeyFrame->pRotateKeys = new_object( size );

		memset( pKeyFrame->pRotateKeys, 0, size );
	}

	/* ポジションキーの生成 */
	pKeyFrame->pPositionKeys = NULL;
	if ( ( size = sizeof(GSpositionkey) * pKeyFrame->numPositionKeys ) != 0 )
	{
		pKeyFrame->pPositionKeys = new_object( size );

		memset( pKeyFrame->pPositionKeys, 0, size );
	}
}

/*-----------------------------------------------------------------------------
*
* Purpose : ボーンフレームの読み込み．
*
* Return  : 読み込みに成功すれば真，失敗すると偽を返す．
*
*----------------------------------------------------------------------------*/
static GSboolean
gsBoneFrameLoadFile
(
	GSboneframe*	pBoneFrame,			/* ボーンフレーム			*/
	GFILE			file				/* ファイルハンドラ			*/
)
{
	GSuint		i;
	GSuint		numBoneFrames = 0;

	/* ボーンデータ数の読み込み */
	GFileReadDword( file, &numBoneFrames );

	/* ボーンフレームの初期化 */
	gsBoneFrameInitialize( pBoneFrame, numBoneFrames );

	/* キーフレームデータを読み込む */
	for ( i = 0; i < pBoneFrame->numBoneFrames; i++ )
	{
		gsKeyFrameLoadFile( &pBoneFrame->pBoneFrames[i], file );
	}

	return	GFileIsSuccess( file );
}

/*-----------------------------------------------------------------------------
*
* Purpose : キーフレームの読み込み．
*
* Return  : 読み込みに成功すれば真，失敗すると偽を返す．
*
*----------------------------------------------------------------------------*/
static GSboolean
gsKeyFrameLoadFile
(
	GSkeyframe*		pKeyFrame,			/* キーフレーム				*/
	GFILE			file				/* ファイルハンドラ			*/
)
{
	GSuint		i;
	GSuint		fKeyFrameInfo   = 0;
	GSuint		numMatrixKeys   = 0;
	GSuint		numScaleKeys    = 0;
	GSuint		numRotateKeys   = 0;
	GSuint		numPositionKeys = 0;

	/* キーフレーム情報の読み込み */
	GFileReadDwordHex( file, &fKeyFrameInfo );

	/* スケールキー数の読み込み */
	GFileReadDword( file, &numScaleKeys );

	/* ローテートキー数の読み込み */
	GFileReadDword( file, &numRotateKeys );

	/* ポジションキー数の読み込み */
	GFileReadDword( file, &numPositionKeys );

	/* マトリクスキー数の読み込み（現在マトリクスキーは使用しない） */
	GFileReadDword( file, &numMatrixKeys );

	/* キーフレームデータの初期化*/
	gsKeyFrameInitialize
	(
		pKeyFrame,
		fKeyFrameInfo,
		numScaleKeys,
		numRotateKeys,
		numPositionKeys
	);

	/* スケールキーの読み込み */
	for ( i = 0 ;i < pKeyFrame->numScaleKeys; i++ )
	{
		gsScaleKeyLoadFile( &pKeyFrame->pScaleKeys[i], file );
	}

	/* ローテートキーの読み込み */
	for ( i = 0 ;i < pKeyFrame->numRotateKeys; i++ )
	{
		gsRotateKeyLoadFile( &pKeyFrame->pRotateKeys[i], file );
	}

	/* ポジションキーの読み込み */
	for ( i = 0 ;i < pKeyFrame->numPositionKeys; i++ )
	{
		gsPositionKeyLoadFile( &pKeyFrame->pPositionKeys[i], file );
	}

	return	GFileIsSuccess( file );
}

/*-----------------------------------------------------------------------------
*
* Purpose : スケールキーの読み込み．
*
* Return  : 読み込みに成功すれば真，失敗すると偽を返す．
*
*----------------------------------------------------------------------------*/
static GSboolean
gsScaleKeyLoadFile
(
	GSscalekey*		pKey,				/* スケールキー				*/
	GFILE			file				/* ファイルハンドラ			*/
)
{
	/* タイマ値の読み込み */
	GFileReadDword( file, &pKey->Time );

	/* スケール値の読み込み */
	GFileReadVector3( file, &pKey->Scale );

	return	GFileIsSuccess( file );
}

/*-----------------------------------------------------------------------------
*
* Purpose : ローテートキーの読み込み．
*
* Return  : 読み込みに成功すれば真，失敗すると偽を返す．
*
*----------------------------------------------------------------------------*/
static GSboolean
gsRotateKeyLoadFile
(
	GSrotatekey*	pKey,				/* ローテートキー			*/
	GFILE			file				/* ファイルハンドラ			*/
)
{
	/* タイマ値の読み込み */
	GFileReadDword( file, &pKey->Time );

	/* ローテート値の読み込み */
	GFileReadQuaternion( file, &pKey->Rotate );

	/* クォータニオンを正規化する */
	gsQuaternionNormalize( &pKey->Rotate, &pKey->Rotate );

	return	GFileIsSuccess( file );
}

/*-----------------------------------------------------------------------------
*
* Purpose : ポジションキーの読み込み．
*
* Return  : 読み込みに成功すれば真，失敗すると偽を返す．
*
*----------------------------------------------------------------------------*/
static GSboolean
gsPositionKeyLoadFile
(
	GSpositionkey*	pKey,				/* ポジションキー			*/
	GFILE			file				/* ファイルハンドラ			*/
)
{
	/* タイマ値の読み込み */
	GFileReadDword( file, &pKey->Time );

	/* ポジション値の読み込み */
	GFileReadVector3( file, &pKey->Position );

	return	GFileIsSuccess( file );
}

/*-----------------------------------------------------------------------------
*
* Purpose : ボーンフレームの読み込み．
*
* Return  : 読み込みに成功すれば真，失敗すると偽を返す．
*
*----------------------------------------------------------------------------*/
static GSboolean
gsBoneFrameLoadFileBin
(
	GSboneframe*	pBoneFrame,			/* ボーンフレーム			*/
	GFILE			file				/* ファイルハンドラ			*/
)
{
	GSuint		i;
	GSuint		numBoneFrames = 0;

	/* ボーンデータ数の読み込み */
	GFileReadDwordBin(file, &numBoneFrames);

	/* ボーンフレームの初期化 */
	gsBoneFrameInitialize(pBoneFrame, numBoneFrames);

	/* キーフレームデータを読み込む */
	for (i = 0; i < pBoneFrame->numBoneFrames; i++)
	{
		gsKeyFrameLoadFileBin(&pBoneFrame->pBoneFrames[i], file);
	}

	return	GFileIsSuccess(file);
}

/*-----------------------------------------------------------------------------
*
* Purpose : キーフレームの読み込み．
*
* Return  : 読み込みに成功すれば真，失敗すると偽を返す．
*
*----------------------------------------------------------------------------*/
static GSboolean
gsKeyFrameLoadFileBin
(
	GSkeyframe*		pKeyFrame,			/* キーフレーム				*/
	GFILE			file				/* ファイルハンドラ			*/
)
{
	GSuint		i;
	GSuint		fKeyFrameInfo = 0;
	GSuint		numMatrixKeys = 0;
	GSuint		numScaleKeys = 0;
	GSuint		numRotateKeys = 0;
	GSuint		numPositionKeys = 0;

	/* キーフレーム情報の読み込み */
	GFileReadDwordBin(file, &fKeyFrameInfo);

	/* スケールキー数の読み込み */
	GFileReadDwordBin(file, &numScaleKeys);

	/* ローテートキー数の読み込み */
	GFileReadDwordBin(file, &numRotateKeys);

	/* ポジションキー数の読み込み */
	GFileReadDwordBin(file, &numPositionKeys);

	/* マトリクスキー数の読み込み（現在マトリクスキーは使用しない） */
	GFileReadDwordBin(file, &numMatrixKeys);

	/* キーフレームデータの初期化*/
	gsKeyFrameInitialize
	(
		pKeyFrame,
		fKeyFrameInfo,
		numScaleKeys,
		numRotateKeys,
		numPositionKeys
	);

	/* スケールキーの読み込み */
	for (i = 0; i < pKeyFrame->numScaleKeys; i++)
	{
		gsScaleKeyLoadFileBin(&pKeyFrame->pScaleKeys[i], file);
	}

	/* ローテートキーの読み込み */
	for (i = 0; i < pKeyFrame->numRotateKeys; i++)
	{
		gsRotateKeyLoadFileBin(&pKeyFrame->pRotateKeys[i], file);
	}

	/* ポジションキーの読み込み */
	for (i = 0; i < pKeyFrame->numPositionKeys; i++)
	{
		gsPositionKeyLoadFileBin(&pKeyFrame->pPositionKeys[i], file);
	}

	return	GFileIsSuccess(file);
}

/*-----------------------------------------------------------------------------
*
* Purpose : スケールキーの読み込み．
*
* Return  : 読み込みに成功すれば真，失敗すると偽を返す．
*
*----------------------------------------------------------------------------*/
static GSboolean
gsScaleKeyLoadFileBin
(
	GSscalekey*		pKey,				/* スケールキー				*/
	GFILE			file				/* ファイルハンドラ			*/
)
{
	/* タイマ値の読み込み */
	GFileReadDwordBin(file, &pKey->Time);

	/* スケール値の読み込み */
	GFileReadVector3Bin(file, &pKey->Scale);

	return	GFileIsSuccess(file);
}

/*-----------------------------------------------------------------------------
*
* Purpose : ローテートキーの読み込み．
*
* Return  : 読み込みに成功すれば真，失敗すると偽を返す．
*
*----------------------------------------------------------------------------*/
static GSboolean
gsRotateKeyLoadFileBin
(
	GSrotatekey*	pKey,				/* ローテートキー			*/
	GFILE			file				/* ファイルハンドラ			*/
)
{
	/* タイマ値の読み込み */
	GFileReadDwordBin(file, &pKey->Time);

	/* ローテート値の読み込み */
	GFileReadQuaternionBin(file, &pKey->Rotate);

	/* クォータニオンを正規化する */
	gsQuaternionNormalize(&pKey->Rotate, &pKey->Rotate);

	return	GFileIsSuccess(file);
}

/*-----------------------------------------------------------------------------
*
* Purpose : ポジションキーの読み込み．
*
* Return  : 読み込みに成功すれば真，失敗すると偽を返す．
*
*----------------------------------------------------------------------------*/
static GSboolean
gsPositionKeyLoadFileBin
(
	GSpositionkey*	pKey,				/* ポジションキー			*/
	GFILE			file				/* ファイルハンドラ			*/
)
{
	/* タイマ値の読み込み */
	GFileReadDwordBin(file, &pKey->Time);

	/* ポジション値の読み込み */
	GFileReadVector3Bin(file, &pKey->Position);

	return	GFileIsSuccess(file);
}

/*-----------------------------------------------------------------------------
*
* Purpose : 指定ボーンのキーフレームデータの取得．
*
* Return  : なし．
*
*----------------------------------------------------------------------------*/
static void
gsBoneFrameGetKeyFrame
(
	GSboneframe*	pBoneFrame,			/* ボーンフレームキーフレーム	*/
	GSuint			BoneNo,				/* 取得するボーンのＩＤ			*/
	GSfloat			fGlobalTime,		/* 取得するキーのタイマ			*/
	GSvector3*		pOutScale,			/*-> スケール値					*/
	GSquaternion*	pOutRotate,			/*-> ローテート値				*/
	GSvector3*		pOutPosition		/*-> ポジション値				*/
)
{
	/* ボーンナンバがボーンフレーム数よりも大きい？ */
	if ( BoneNo >= pBoneFrame->numBoneFrames )
	{
		return;
	}

	/* キーフレームデータからスケール値，ローテート値，ポジション値を取得 */
	gsKeyFrameGetKey
	(
		&pBoneFrame->pBoneFrames[ BoneNo ],
		fGlobalTime,
		pOutScale, pOutRotate, pOutPosition
	);
}

/*-----------------------------------------------------------------------------
*
* Purpose : キーフレームデータの取得．
*
* Return  : なし．
*
*----------------------------------------------------------------------------*/
static void
gsKeyFrameGetKey
(
	GSkeyframe*		pKeyFrame,			/* マトリクスキー配列			*/
	GSfloat			fGlobalTime,		/* 取得するキーのタイマ			*/
	GSvector3*		pOutScale,			/*-> スケール値					*/
	GSquaternion*	pOutRoate,			/*-> ローテート値				*/
	GSvector3*		pOutPosition		/*-> ポジション値				*/
)
{
	/* 各キーの値を初期化する */
	gsVector3Init( pOutScale, 1.0f, 1.0f, 1.0f );
	gsQuaternionIdentity( pOutRoate );
	gsVector3Init( pOutPosition, 0.0f, 0.0f, 0.0f );

	/* スケールキーを持っているか？ */
	if ( pKeyFrame->pScaleKeys != NULL )
	{
		/* スケールキーを取得する */
		gsScaleKeyGetScale
		(
			pKeyFrame->numScaleKeys,
			pKeyFrame->pScaleKeys,
			fGlobalTime,
			pOutScale
		);
	}

	/* ローテートキーを持っているか？ */
	if ( pKeyFrame->pRotateKeys != NULL )
	{
		/* ローテートキーを取得する */
		gsRotateKeyGetRotate
		(
			pKeyFrame->numRotateKeys,
			pKeyFrame->pRotateKeys,
			fGlobalTime,
			pOutRoate
		);
	}

	/* ポジションキーを持っているか？ */
	if ( pKeyFrame->pPositionKeys != NULL )
	{
		/* ポジションキーを取得する */
		gsPositionKeyGetPosition
		(
			pKeyFrame->numPositionKeys,
			pKeyFrame->pPositionKeys,
			fGlobalTime,
			pOutPosition
		);
	}
}

/*-----------------------------------------------------------------------------
*
* Purpose : スケールキーの取得．
*
* Return  : なし．
*
*----------------------------------------------------------------------------*/
static void
gsScaleKeyGetScale
(
	GSuint			numScaleKeys,		/* スケールキー数		*/
	GSscalekey*		pScaleKeys,			/* スケールキー配列		*/
	GSfloat			fGlobalTime,		/* 取得するキーのタイマ	*/
	GSvector3*		pScale				/*-> 取得したスケール値	*/
)
{
	GSfloat			fTime;
	GSfloat			fTime1;
	GSfloat			fTime2;
	GSfloat			fLerpValue;
	GSuint			iKey;
	GSuint			iKey1;
	GSuint			iKey2;

	fTime = (float)fmod( fGlobalTime, pScaleKeys[numScaleKeys-1].Time );
	iKey1 = 0;
	iKey2 = 0;

	for ( iKey = 0 ;iKey < numScaleKeys ; iKey++ )
	{
		if ( (float)pScaleKeys[iKey].Time > fTime )
		{
			iKey2 = iKey;

			if ( iKey > 0 )
			{
				iKey1= iKey - 1;
			}
			else
			{
				iKey1 = iKey;
			}

			break;
		}
	}
	fTime1 = (float)pScaleKeys[iKey1].Time;
	fTime2 = (float)pScaleKeys[iKey2].Time;

	if ( ( fTime2 - fTime1 ) ==0 )
	{
		fLerpValue = 0;
	}
	else
	{
		fLerpValue =  ( fTime - fTime1 )  / ( fTime2 - fTime1 );
	}

	/* スケールキーの線形補間を行う */
	gsVector3Lerp
	(
		pScale,
		&pScaleKeys[iKey1].Scale, 
		&pScaleKeys[iKey2].Scale,
		fLerpValue
	);
}

/*-----------------------------------------------------------------------------
*
* Purpose : ローテートキーの取得．
*
* Return  : なし．
*
*----------------------------------------------------------------------------*/
static void
gsRotateKeyGetRotate
(
	GSuint			numRotateKeys,		/* ローテートキー数			*/
	GSrotatekey*	pRotateKeys,		/* ローテートキー配列		*/
	GSfloat			fGlobalTime,		/* 取得するキーのタイマ		*/
	GSquaternion*	pRotate				/*-> 取得したローテート値	*/
)
{
	GSfloat			fTime;
	GSfloat			fTime1;
	GSfloat			fTime2;
	GSfloat			fLerpValue;
	GSuint			iKey;
	GSuint			iKey1;
	GSuint			iKey2;

	fTime = (float)fmod( fGlobalTime, pRotateKeys[numRotateKeys-1].Time );
	iKey1 = 0;
	iKey2 = 0;
	for ( iKey = 0 ;iKey < numRotateKeys; iKey++ )
	{
		if ( (float)pRotateKeys[iKey].Time > fTime )
		{
			iKey2 = iKey;

			if ( iKey > 0 )
			{
				iKey1 = iKey - 1;
			}
			else
			{
				iKey1 = iKey;
			}
			break;
		}
	}

	fTime1 = (float)pRotateKeys[iKey1].Time;
	fTime2 = (float)pRotateKeys[iKey2].Time;

	if ( ( fTime2 - fTime1 ) == 0 )
	{
		fLerpValue = 0;
	}
	else
	{
		fLerpValue =  ( fTime - fTime1 )  / ( fTime2 - fTime1 );
	}

	/* ローテートキーの線形補間を行う */
	gsQuaternionSlerp
	(
		pRotate,
		&pRotateKeys[iKey1].Rotate,
		&pRotateKeys[iKey2].Rotate,
		fLerpValue
	);
}

/*-----------------------------------------------------------------------------
*
* Purpose : ポジションキーの取得．
*
* Return  : なし．
*
*----------------------------------------------------------------------------*/
static void
gsPositionKeyGetPosition
(
	GSuint			numPositionKeys,	/* ポジションキー数		*/
	GSpositionkey*	pPositionKeys,		/* ポジションキー配列	*/
	GSfloat			fGlobalTime,		/* 取得するキーのタイマ	*/
	GSvector3*		pPosition			/*-> 取得したポジション	*/
)
{
	GSfloat			fTime;
	GSfloat			fTime1;
	GSfloat			fTime2;
	GSfloat			fLerpValue;
	GSuint			iKey;
	GSuint			iKey1;
	GSuint			iKey2;

	fTime = (float)fmod( fGlobalTime, pPositionKeys[numPositionKeys-1].Time );
	iKey1 = 0;
	iKey2 = 0;
	for ( iKey = 0 ;iKey < numPositionKeys ; iKey++ )
	{
		if ( (float)pPositionKeys[iKey].Time > fTime )
		{
			iKey2 = iKey;

			if ( iKey > 0 )
			{
				iKey1 = iKey - 1;
			}
			else
			{
				iKey1 = iKey;
			}
			break;
		}
	}
	fTime1 = (float)pPositionKeys[iKey1].Time;
	fTime2 = (float)pPositionKeys[iKey2].Time;

	if ( ( fTime2 - fTime1 ) ==0 )
	{
		fLerpValue = 0;
	}
	else
	{
		fLerpValue =  ( fTime - fTime1 )  / ( fTime2 - fTime1 );
	}

	/* ポジションキーの線形補間を行う */
	gsVector3Lerp
	(
		pPosition,
		&pPositionKeys[iKey1].Position,
		&pPositionKeys[iKey2].Position,
		fLerpValue
	);
}

/********** End of File ******************************************************/
