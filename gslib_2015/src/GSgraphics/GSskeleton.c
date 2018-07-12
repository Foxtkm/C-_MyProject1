/******************************************************************************
* File Name : gskeleton.c                        Ver : 1.00  Date : 2002-07-31
*
* Description :
*
*       スケルトン情報管理．
*
* Author : Moriharu Ohzu.
*
******************************************************************************/
#include	"GSskeleton.h"
#include	"GSmatrix4.h"
#include	"gfile.h"
#include	"heap.h"
#include    "GSfile_path.h"
#include	<string.h>
#include	<memory.h>

/****** 関数プロトタイプ宣言 *************************************************/

static void		 gsSkeletonLinkBone( GSskeleton* );
static void		 gsBoneAddChild( GSbone*, GSbone* );
static void		 gsBoneCalculate( GSbone*, GSmatrix4*, const GSmatrix4*, const GSmatrix4* );
static GSboolean gsBoneLoadFile( GSbone*, GFILE );
static GSboolean gsBoneLoadFileBin(GSbone*, GFILE);

/*=============================================================================
*
* Purpose : スケルトンの情報の生成．
*
* Return  : スケルトン情報を返す．
*
*============================================================================*/
extern GSskeleton*
gsSkeletonNew
(
	void
)
{
	GSskeleton*		pSkeleton;

	pSkeleton = new_object( sizeof(GSskeleton) );

	memset( pSkeleton, 0, sizeof(GSskeleton ) );

	return	pSkeleton;
}

/*=============================================================================
*
* Purpose : スケルトン情報の消滅．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsSkeletonDelete
(
	GSskeleton*		pSkeleton			/* スケルトン情報 			*/
)
{
	/* ボーンデータクリーンアップ */
	gsSkeletonCleanUp( pSkeleton );

	/* スケルトン情報を消滅 */
	del_object( pSkeleton );
}

/*=============================================================================
*
* Purpose : スケルトン情報のクリーンアップ．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsSkeletonCleanUp
(
	GSskeleton*		pSkeleton			/* スケルトン情報 			*/
)
{
	if ( pSkeleton->pBones != NULL );
	{
		del_object( pSkeleton->pBones );

		pSkeleton->pBones = NULL;
	}
	pSkeleton->numBones = 0;
}

/*=============================================================================
*
* Purpose : スケルトン情報ファイルの読み込み．
*
* Return  : 読み込みに成功すれば真，失敗すれば偽を返す．
*
*============================================================================*/
extern GSboolean
gsSkeletonLoadFileTxt
(
	GSskeleton*		pSkeleton,			/* スケルトン情報 			*/
	const char*		pszFileName			/* スケルトンファイル名		*/
)
{
	GFILE			file;
	GSboolean		fError;
	GSuint			i;

	/* スケルトン情報ファイルのオープン */
	if ( ( file = GFileOpen( pszFileName, "r" ) ) == NULL )
	{
		return	GS_FALSE;
	}

	/* スケルトン情報のクリーンアップ */
	gsSkeletonCleanUp( pSkeleton );

	/* ボーンの数を読み込む */
	GFileReadDword( file, &pSkeleton->numBones );

	/* スケルトン情報の初期化 */
	pSkeleton->pBones = new_object( sizeof( GSbone ) * pSkeleton->numBones );

	memset( pSkeleton->pBones, 0, sizeof( GSbone ) * pSkeleton->numBones );

	/* ボーン情報の読み込み */
	for ( i = 0; i < pSkeleton->numBones; i++ )
	{
		gsBoneLoadFile( &pSkeleton->pBones[i], file );
	}

	/* ファイルの読み込みが成功した */
	if ( ( fError = GFileIsSuccess( file ) ) == GS_TRUE )
	{
		/* 親子兄弟のリンクをする */
		gsSkeletonLinkBone( pSkeleton );
	}

	/* スケルトン情報ファイルをクローズする */
	GFileClose( file );

	return	fError;
}

/*=============================================================================
*
* Purpose : スケルトン情報ファイルの読み込み．
*
* Return  : 読み込みに成功すれば真，失敗すれば偽を返す．
*
*============================================================================*/
extern GSboolean
gsSkeletonLoadFileBin
(
	GSskeleton*		pSkeleton,			/* スケルトン情報 			*/
	const char*		pszFileName			/* スケルトンファイル名		*/
)
{
	GFILE			file;
	GSboolean		fError;
	GSuint			i;

	/* スケルトン情報ファイルのオープン */
	if ((file = GFileOpen(pszFileName, "rb")) == NULL)
	{
		return	GS_FALSE;
	}

	/* スケルトン情報のクリーンアップ */
	gsSkeletonCleanUp(pSkeleton);

	/* ボーンの数を読み込む */
	GFileReadDwordBin(file, &pSkeleton->numBones);

	/* スケルトン情報の初期化 */
	pSkeleton->pBones = new_object(sizeof(GSbone) * pSkeleton->numBones);

	memset(pSkeleton->pBones, 0, sizeof(GSbone) * pSkeleton->numBones);

	/* ボーン情報の読み込み */
	for (i = 0; i < pSkeleton->numBones; i++)
	{
		gsBoneLoadFileBin(&pSkeleton->pBones[i], file);
	}

	/* ファイルの読み込みが成功した */
	if ((fError = GFileIsSuccess(file)) == GS_TRUE)
	{
		/* 親子兄弟のリンクをする */
		gsSkeletonLinkBone(pSkeleton);
	}

	/* スケルトン情報ファイルをクローズする */
	GFileClose(file);

	return	fError;
}

/*=============================================================================
*
* Purpose : スケルトン情報ファイルの読み込み．
*
* Return  : 読み込みに成功すれば真，失敗すれば偽を返す．
*
*============================================================================*/
extern GSboolean
gsSkeletonLoadFile
(
	GSskeleton*		pSkeleton,			/* スケルトン情報 			*/
	const char*		pszFileName			/* スケルトンファイル名		*/
)
{
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char skeleton_file[_MAX_PATH];
	/* ファイル名を分解する */
	gsSplitPath(pszFileName, drive, dir, fname, NULL);

	/* mshb -> msh の優先順位で読み込みをする */
	gsMakePath(skeleton_file, drive, dir, fname, ".sklb");
	if (gsSkeletonLoadFileBin(pSkeleton, skeleton_file) == GS_TRUE) {
		return GS_TRUE;
	}
	gsMakePath(skeleton_file, drive, dir, fname, ".skl");
	if (gsSkeletonLoadFileTxt(pSkeleton, skeleton_file) == GS_TRUE) {
		return GS_TRUE;
	}
	return GS_FALSE;
}

/*=============================================================================
*
* Purpose : 姿勢変換行列を求める．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsSkeletonCalculateOrientation
(
	GSskeleton*			pSkeleton,		/* スケルトン情報 				*/
	GSmatrix4*			pOutMatrices,	/*-> 計算後の姿勢変換行列配列	*/
	const GSmatrix4*	pParentMatrix,	/* 親の変換行列					*/
	const GSmatrix4*	pAnimMatrix		/* アニメーション用行列配列		*/
)
{
	/* 各ボーンの座標変換を行う */
	gsBoneCalculate
	(
		&pSkeleton->pBones[0],
		pOutMatrices,
		pParentMatrix,
		pAnimMatrix
	);
}

/*=============================================================================
*
* Purpose : 座標変換行列の計算を行う．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsSkeletonCalculateTransform
(
	GSskeleton*			pSkeleton,		/* スケルトン情報 			*/
	GSmatrix4*			pOutMatrices,	/*-> 座標変換行列配列		*/
	const GSmatrix4*	pOrientMatrices	/* 姿勢変換行列配列			*/
)
{
	GSuint				i;

	/* ローカル座標変換行列と姿勢変換行列を合成する */
	for ( i = 0; i < pSkeleton->numBones; i++ )
	{
		gsMatrix4Multiply
		(
			&pOutMatrices[i],
			&pSkeleton->pBones[i].matBoneOffset,
			&pOrientMatrices[i]
		);
	}
}

/*=============================================================================
*
* Purpose : ボーンデータ数を取得．
*
* Return  : ボーンデータ数を返す．
*
*============================================================================*/
extern GSuint
gsSkeletonGetNumBones
(
	GSskeleton*			pSkeleton		/* スケルトン情報 			*/
)
{
	return	pSkeleton->numBones;
}

/*-----------------------------------------------------------------------------
*
* Purpose : 各ボーンの階層構造を設定する．
*
* Return  : なし．
*
*----------------------------------------------------------------------------*/
static void
gsSkeletonLinkBone
(
	GSskeleton*		pSkeleton			/* スケルトン情報 			*/
)
{
	GSbone*			pParentBone;
	GSbone*			pChildBone;
	GSuint			i;
	GSuint			j;

	for ( i = 0; i < pSkeleton->numBones; i++ )
	{
		/* 親ボーンの取得 */
		pParentBone = &pSkeleton->pBones[i];
		for ( j = 1; j < pSkeleton->numBones; j++ )
		{
			/* 子ボーンの取得 */
			pChildBone = &pSkeleton->pBones[j];

			/* 親ボーンのＩＤを持っているボーンか？ */
			if ( pParentBone->BoneID == pChildBone->ParentBoneID )
			{
				/* 親ボーンに子ボーンをリンクさせる */
				gsBoneAddChild( pParentBone, pChildBone );
			}
		}
	}
}

/*-----------------------------------------------------------------------------
*
* Purpose : 親ボーンに子ボーンを設定する．
*
* Return  : なし．
*
*----------------------------------------------------------------------------*/
static void
gsBoneAddChild
(
	GSbone*		pParentBone,			/* 親ボーン					*/
	GSbone*		pChildBone				/* 子ボーン					*/
)
{
	/* 各ポインタのリンクをする */
	pChildBone->pParent  = pParentBone;
	pChildBone->pSibling = pParentBone->pChild;
	pParentBone->pChild  = pChildBone;
}

/*-----------------------------------------------------------------------------
*
* Purpose : ボーンの変換行列の計算を行う．
*
* Return  : なし．
*
*----------------------------------------------------------------------------*/
static void
gsBoneCalculate
(
	GSbone*				pBone,			/* ボーン情報				*/
	GSmatrix4*			pOutMatrices,	/*-> 計算後の変換行列配列	*/
	const GSmatrix4*	pParentMatrix,	/* 親の変換行列				*/
	const GSmatrix4*	pAnimMatrices	/* アニメーション用行列配列	*/
)
{
	GSmatrix4*			pOutMatrix;

	/* 変換先の行列を取得 */
	pOutMatrix = &pOutMatrices[ pBone->BoneID ];

	/* 行列の初期化 */
	gsMatrix4Identity( pOutMatrix );

	/* アニメーション変換行列の取得 */
	if (pAnimMatrices != NULL)
	{
		*pOutMatrix = pAnimMatrices[pBone->BoneID];
	}

	/* 親からの相対座標をモデリング変換行列に加える */
	pOutMatrix->m[3][0] += pBone->ParentBoneOffset.x;
	pOutMatrix->m[3][1] += pBone->ParentBoneOffset.y;
	pOutMatrix->m[3][2] += pBone->ParentBoneOffset.z;

	/* 親の変換行列を合成する */
	if ( pParentMatrix != NULL )
	{
		/* 親の変換行列と合成 */
		gsMatrix4Multiply( pOutMatrix, pOutMatrix, pParentMatrix );
	}

	/* 兄弟を持っていれば兄弟を計算させる */
	if ( pBone->pSibling != NULL )
	{
		gsBoneCalculate
		(
			pBone->pSibling,
			pOutMatrices,
			pParentMatrix,
			pAnimMatrices
		);
	}

	/* 子供を持っていれば子供を計算させる */
	if ( pBone->pChild != NULL )
	{
		gsBoneCalculate
		(
			pBone->pChild,
			pOutMatrices,
			pOutMatrix,
			pAnimMatrices
		);
	}
}

/*-----------------------------------------------------------------------------
*
* Purpose : ボーン情報の読み込み．
*
* Return  : 読み込みに成功すれば真，失敗すると偽を返す．
*
*----------------------------------------------------------------------------*/
static GSboolean
gsBoneLoadFile
(
	GSbone*		pBone,					/* ボーン情報				*/
	GFILE		file					/* ファイルハンドラ			*/
)
{
	/* ボーンＩＤの読み込み 	*/
	GFileReadDword( file, &pBone->BoneID );

	/* 親ボーンＩＤの読み込み */
	GFileReadDword( file, &pBone->ParentBoneID );

	/* 親からの相対座標の読み込み */
	GFileReadVector3( file, &pBone->ParentBoneOffset );

	/* ローカル座標変換行列の読み込み */
	GFileReadMatrix4( file, &pBone->matBoneOffset );

	/* 親子兄弟のポインタを初期化 */
	pBone->pParent  = NULL;
	pBone->pSibling = NULL;
	pBone->pChild   = NULL;

	return	GFileIsSuccess( file );
}

/*-----------------------------------------------------------------------------
*
* Purpose : ボーン情報の読み込み．
*
* Return  : 読み込みに成功すれば真，失敗すると偽を返す．
*
*----------------------------------------------------------------------------*/
static GSboolean
gsBoneLoadFileBin
(
	GSbone*		pBone,					/* ボーン情報				*/
	GFILE		file					/* ファイルハンドラ			*/
)
{
	/* ボーンＩＤの読み込み 	*/
	GFileReadDwordBin(file, &pBone->BoneID);

	/* 親ボーンＩＤの読み込み */
	GFileReadDwordBin(file, &pBone->ParentBoneID);

	/* 親からの相対座標の読み込み */
	GFileReadVector3Bin(file, &pBone->ParentBoneOffset);

	/* ローカル座標変換行列の読み込み */
	GFileReadMatrix4Bin(file, &pBone->matBoneOffset);

	/* 親子兄弟のポインタを初期化 */
	pBone->pParent = NULL;
	pBone->pSibling = NULL;
	pBone->pChild = NULL;

	return	GFileIsSuccess(file);
}

/********** End of File ******************************************************/
