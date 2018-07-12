/******************************************************************************
* File Name : GSmesh.h                           Ver : 1.00  Date : 2002-08-03
*
* Description :
*
*		メッシュデータ ヘッダファイル．．
*
* Author : Moriharu Ohzu.
*
******************************************************************************/
#ifndef		_GSMESH_H_
#define		_GSMESH_H_

#include	"GStype.h"
#include	"GStexture.h"

/****** 定数宣言 *************************************************************/

#define	GS_MATERIAL_TEXTURE				( 1	<< 0 )	/* テクスチャ			*/
#define	GS_MATERIAL_NORMAL_TEXTURE		( 1	<< 1 )	/* 法線マップ			*/
#define	GS_MATERIAL_SPECULAR_TEXTURE	( 1	<< 2 )	/* 鏡面反射マップ		*/
#define	GS_MATERIAL_AMBIENT_TEXTURE		( 1	<< 3 )	/* 拡散反射マップ		*/
#define	GS_MATERIAL_EMISSIVE_TEXTURE	( 1	<< 4 )	/* 自己照明マップ		*/
#define	GS_MATERIAL_HEIGHT_TEXTURE		( 1	<< 5 )	/* 高さマップ			*/
#define	GS_MATERIAL_TEXTURE2			( 1	<< 6 )	/* 第２テクスチャ		*/
#define	GS_MATERIAL_NORMAL_TEXTURE2		( 1	<< 7 )	/* 第２法線マップ		*/
#define	GS_MATERIAL_MASK_TEXTURE		( 1	<< 8 )	/* マスクマップ			*/
#define	GS_MATERIAL_CUBE_TEXTURE		( 1	<< 9 )	/* キューブ環境マップ	*/

#define	GS_MATERIAL_SPHERE_MAP			( 1	<< 31 )	/* 球形環境マッピング	*/
#define	GS_MATERIAL_TEX_CLAMP			( 1	<< 30 )	/* テクスチャをクランプ	*/

/****** データ型宣言 *********************************************************/

/*----- マテリアル情報構造体 ------------------------------------------------*/

typedef	struct							/* マテリアル構造体			*/
{
	GSbitfield		MaterialInfo;		/* マテリアル情報フラグ		*/
	GScolor			Diffuse;			/* 拡散光カラー				*/
	GScolor			Ambient;			/* 環境光カラー				*/
	GScolor			Specular;			/* 鏡面反射光カラー			*/
	GScolor			Emissive;			/* 放射光カラー				*/
	GSfloat			Power;				/* ハイライトの鮮明度		*/
	GStexture*		pTexture;			/* テクスチャ				*/
	GStexture*		pNormalTexture;		/* 法線マップ				*/
	GStexture*		pSpecularTexture;	/* 鏡面反射マップ			*/
	GStexture*		pAmbientTexture;	/* 拡散反射マップ			*/
	GStexture*		pCubeTexture;		/* キューブ環境マップ		*/
	GStexture*		pHeightTexture;		/* 高さマップ				*/
	GStexture*		pEmissiveTexture;	/* 自己照明マップ			*/
	GStexture*		pMaskTexture;		/* マスクテクスチャ			*/
	GStexture*		pTexture2;			/* 第２テクスチャマップ		*/
	GStexture*		pNormalTexture2;	/* 第２法線マップ			*/

} GSmaterial;

/*----- 面データグループ構造体 ---------------------------------------------*/

typedef	struct							/* 面データグループ型		*/
{
	GSbitfield		FaceInfo;			/* 面情報フラグ				*/
	GSuint			MaterialNo;			/* マテリアルナンバ			*/
	GSuint			numIndices;			/* 頂点インデックス数		*/
	GSuint			StartIndices;		/* 頂点インデックス開始位置	*/

} GSfacegroup;

/*----- ボーンインデックス構造体 -------------------------------------------*/

typedef struct {
	GSubyte			Indices[4];			/*ボーンインデックス		*/
} GSblendindices;

/*----- ボーンインデックス構造体 -------------------------------------------*/

typedef struct {
	GSfloat			Weights[4];			/* ボーンウェイト			*/
} GSblendweight;

/*----- バッファオブジェクト構造体 -----------------------------------------*/

typedef struct {
	GLuint		ArrayBuffer;			/* 頂点配列オブジェクト				*/
	GLuint		VertexBuffer;			/* 頂点バッファオブジェクト			*/	
	GLuint		IndexBuffer;			/* インデックスバッファオブジェクト */
	GSuint		IndicesOffset;			/* インデックスバッファのオフセット */
} GSmeshbuffer;

/*----- メッシュ構造体 -----------------------------------------------------*/

typedef	struct							/* メッシュ型				*/
{
	GSbitfield		MeshInfo;			/* メッシュ情報				*/
	GSuint			numMaterials;		/* マテリアルデータ数		*/
	GSuint			numFaceGroups;		/* レンダリング面情報数		*/
	GSuint			numIndices;			/* 頂点インデックス数		*/
	GSuint			numVertices;		/* 頂点データ数 			*/
	GSuint			numTexCoords;		/* テクスチャ座標数			*/
	GSuint			numWeights;			/* 頂点ウェイト数			*/
	GSmaterial*		pMaterials;			/* マテリアルデータ配列		*/
	GSfacegroup*	pFaceGroups;		/* レンダリング面情報		*/
	GSuint*			pIndices;			/* 頂点インデックス			*/
	GSvector3*		pPoints;			/* 頂点座標配列				*/
	GSvector3*		pNormals;			/* 法線ベクトル配列			*/
	GScolor*		pDiffuseColors;		/* 拡散光カラー配列			*/
	GScolor*		pSpecularColors;	/* 鏡面反射光カラー配列		*/
	GSvector2**		ppTexCoords;		/* テクスチャ座標配列		*/
	GSblendweight*	pWeights;			/* 頂点ウェイト配列			*/
	GSblendindices*	pBlendIndices;		/* ボーンインデックス配列	*/
	GSvector3*		pTangents;			/* 接ベクトル配列			*/
	GSvector3*		pBinormals;			/* 従法線ベクトル配列		*/
	GSplane*		pPlanes;			/* 平面データ配列			*/
	GSvector3		Center;				/* メッシュの境界球の中心	*/
	GSfloat			fRadius;			/* メッシュの境界球の半径	*/
	GSmeshbuffer	Buffer;				/* バッファオブジェクト		*/
} GSmesh;

/****** 関数プロトタイプ宣言 *************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*=============================================================================
*
* Purpose : メッシュの生成．
*
* Return  : メッシュを返す．
*
*============================================================================*/
extern GSmesh*
gsMeshNew
(
	void
);

/*=============================================================================
*
* Purpose : メッシュの消滅．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsMeshDelete
(
	GSmesh*			pMesh				/* メッシュデータ		*/
);

/*=============================================================================
*
* Purpose : メッシュのクリーンアップ．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsMeshCleanUp
(
	GSmesh*			pMesh				/* メッシュデータ		*/
);

/*=============================================================================
*
* Purpose : メッシュファイルの読み込み．
*
* Return  : なし．
*
*============================================================================*/
extern GSboolean
gsMeshLoadFile
(
	GSmesh*			pMesh,				/* メッシュデータ		*/
	const char*		pszFileName			/* ファイル名			*/
);

/*=============================================================================
*
* Purpose : メッシュファイルの読み込み．(シェーダー専用）
*
* Return  : なし．
*
*============================================================================*/
extern GSboolean
gsMeshLoadFileEx
(
	GSmesh*			pMesh,				/* メッシュデータ		*/
	const char*		pszFileName			/* ファイル名			*/
);

/*=============================================================================
*
* Purpose : メッシュの描画．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsMeshDraw
(
	GSmesh*			pMesh				/* メッシュデータ		*/
);

/*=============================================================================
*
* Purpose : メッシュの描画．（ セルシェーディング ）
*
* Return  : なし．
*
*============================================================================*/
extern void
gsMeshDrawCelShade
(
	GSmesh*			pMesh,				/* メッシュデータ				*/
	GStexture*		pShadeTexture,		/* シェーディング用テクスチャ	*/
	GSvector3*		pShadeLight,		/* ライトの位置					*/
	GScolor*		pLineColor,			/* 輪郭線の色					*/
	GSfloat			fLineWidth			/* 輪郭線の太さ					*/
);

/*=============================================================================
*
* Purpose : スキニングメッシュの描画．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsMeshDrawSkin
(
	GSmesh*				pMesh,			/* メッシュデータ		*/
	const GSmatrix4*	pMatrices		/* 変換行列配列			*/
);

/*=============================================================================
*
* Purpose : スキニングメッシュの描画．（ セルシェーディング ）
*
* Return  : なし．
*
*============================================================================*/
extern void
gsMeshDrawSkinCelShade
(
	GSmesh*				pMesh,			/* メッシュデータ				*/
	const GSmatrix4*	pMatrices,		/* 変換行列配列					*/
	GStexture*			pShadeTexture,	/* シェーディング用テクスチャ	*/
	GSvector3*			pShadeLight,	/* ライトの位置					*/
	GScolor*			pLineColor,		/* 輪郭線の色					*/
	GSfloat				fLineWidth		/* 輪郭線の太さ					*/
);

/*=============================================================================
*
* Purpose : メッシュの描画．（シェーダー専用）
*
* Return  : なし．
*
=============================================================================*/
extern void
gsMeshDrawEx
(
	GSmesh*			pMesh				/* メッシュ構造体		*/
);

#ifdef __cplusplus
}
#endif

#endif

/********** End of File ******************************************************/
