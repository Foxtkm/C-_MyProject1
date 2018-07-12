/******************************************************************************
* File Name : GSmesh.c                           Ver : 1.00  Date : 2003-01-31
*
* Description :
*
*       メッシュデータ．
*
* Author : Moriharu Ohzu.
*
******************************************************************************/
#include	"GSmesh.h"
#include	"GSvector3.h"	
#include	"GSmatrix4.h"	
#include	"GScolor.h"
#include	"GSplane.h"	
#include	"GSload_texture_file.h"
#include	"GSmath.h"
#include	"GSfile_path.h"
#include	"heap.h"
#include	"gfile.h"
#include	<string.h>
#include	<opengl_ext.h>

/****** 関数プロトタイプ宣言 *************************************************/

static void	DrawMesh(GSmaterial*, GSuint, GSfacegroup*, GSuint*, GSvector3*, GSvector3*, GSvector3*, GSvector3*, GScolor*, GSuint, GSvector2**);
static void	DrawMeshCelShade(GSmaterial*, GSuint, GSfacegroup*, GSuint, GSuint*, GSuint, GSvector3*, GSvector3*, GScolor*, GSuint, GSvector2**, GStexture*, GSvector3*, GScolor*, GSfloat);
static void	SetMaterial(GSmaterial*);
static void	LoadMaterial(GSmaterial*, GFILE);
static void	LoadMaterialBin(GSmaterial*, GFILE);
static void	TransformPoints(GSvector3*, GSuint, GSvector3*, const GSmatrix4*);
static void	TransformNormals(GSvector3*, GSuint, GSvector3*, const GSmatrix4*);
static void CalculatePlanes(GSmesh*);
static void CalculateBlendMatrices(GSmatrix4*, GSuint, GSuint, const GSblendweight*, const GSblendindices*, const GSmatrix4*);
static void CreateMeshBuffer(GSmesh*);
static void SetMaterialEx(GSmaterial*);
static void TransformVertices(GSvector3*, GSvector3*, GSvector3*, GSvector3*, GSuint, const GSvector3*, const GSvector3*, const GSvector3*, const GSvector3*, GSuint, const GSblendweight*, const GSblendindices*, const GSmatrix4*);

extern void gsSetShaderParam4f(const char*, const GSvector4*);
extern void gsSetShaderParam1f(const char*, GSfloat);

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
)
{
	GSmesh*		pMesh;

	/* メッシュの生成 */
	pMesh = new_object(sizeof(GSmesh));

	/* 全てのメンバをゼロクリアする */
	memset(pMesh, 0, sizeof(GSmesh));

	return	pMesh;
}

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
)
{
	/* メッシュをクリーンアップする */
	gsMeshCleanUp(pMesh);

	/* メッシュの消滅 */
	del_object(pMesh);
}

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
)
{
	GSuint		i;

	/* 接ベクトル配列の消滅 */
	if (pMesh->pBinormals != NULL)
	{
		del_object(pMesh->pBinormals);
	}

	/* 接ベクトル配列の消滅 */
	if (pMesh->pTangents != NULL)
	{
		del_object(pMesh->pTangents);
	}

	/* ボーンインデックス配列の消滅 */
	if (pMesh->pBlendIndices != NULL)
	{
		del_object(pMesh->pBlendIndices);
	}

	/* 頂点ウェイト配列の消滅 */
	if (pMesh->pWeights != NULL)
	{
		del_object(pMesh->pWeights);
	}

	/* テクスチャ座標配列の消滅 */
	if (pMesh->ppTexCoords != NULL)
	{
		for (i = 0; i < pMesh->numTexCoords; i++)
		{
			if (pMesh->ppTexCoords[i] != NULL)
			{
				del_object(pMesh->ppTexCoords[i]);
			}
		}
		del_object(pMesh->ppTexCoords);
	}

	/* 反射光カラー配列の消滅 */
	if (pMesh->pSpecularColors != NULL)
	{
		del_object(pMesh->pSpecularColors);
	}

	/* 拡散光カラー配列の消滅 */
	if (pMesh->pDiffuseColors != NULL)
	{
		del_object(pMesh->pDiffuseColors);
	}

	/* 法線ベクトル配列の消滅 */
	if (pMesh->pNormals != NULL)
	{
		del_object(pMesh->pNormals);
	}

	/* 頂点座標配列の消滅 */
	if (pMesh->pPoints != NULL)
	{
		del_object(pMesh->pPoints);
	}

	/* マテリアルデータの消滅 */
	if (pMesh->pMaterials != NULL)
	{
		for (i = 0; i < pMesh->numMaterials; i++)
		{
			/* テクスチャの消滅 */
			if (pMesh->pMaterials[i].pTexture != NULL)
			{
				gsTextureDelete(pMesh->pMaterials[i].pTexture);
			}
			if (pMesh->pMaterials[i].pNormalTexture != NULL)
			{
				gsTextureDelete(pMesh->pMaterials[i].pNormalTexture);
			}
			if (pMesh->pMaterials[i].pSpecularTexture != NULL)
			{
				gsTextureDelete(pMesh->pMaterials[i].pSpecularTexture);
			}
			if (pMesh->pMaterials[i].pAmbientTexture != NULL)
			{
				gsTextureDelete(pMesh->pMaterials[i].pAmbientTexture);
			}
			if (pMesh->pMaterials[i].pCubeTexture != NULL)
			{
				gsTextureDelete(pMesh->pMaterials[i].pCubeTexture);
			}
			if (pMesh->pMaterials[i].pHeightTexture != NULL)
			{
				gsTextureDelete(pMesh->pMaterials[i].pHeightTexture);
			}
			if (pMesh->pMaterials[i].pEmissiveTexture != NULL)
			{
				gsTextureDelete(pMesh->pMaterials[i].pEmissiveTexture);
			}
			if (pMesh->pMaterials[i].pMaskTexture != NULL)
			{
				gsTextureDelete(pMesh->pMaterials[i].pMaskTexture);
			}
			if (pMesh->pMaterials[i].pTexture2 != NULL)
			{
				gsTextureDelete(pMesh->pMaterials[i].pTexture2);
			}
			if (pMesh->pMaterials[i].pNormalTexture2 != NULL)
			{
				gsTextureDelete(pMesh->pMaterials[i].pNormalTexture2);
			}
		}
		del_object(pMesh->pMaterials);
	}

	/* 頂点インデックス配列の消滅 */
	if (pMesh->pIndices != NULL)
	{
		del_object(pMesh->pIndices);
	}

	/* 面データ配列の消滅 */
	if (pMesh->pFaceGroups != NULL)
	{
		del_object(pMesh->pFaceGroups);
	}

	/* 平面データ配列の消滅 */
	if (pMesh->pPlanes != NULL)
	{
		del_object(pMesh->pPlanes);
	}

	/* バッファオブジェクトの削除 */
	glDeleteVertexArrays(1, &pMesh->Buffer.ArrayBuffer);
	glDeleteBuffers(1, &pMesh->Buffer.IndexBuffer);
	glDeleteBuffers(1, &pMesh->Buffer.VertexBuffer);
	pMesh->Buffer.IndicesOffset = 0;

	/* 全てのメンバをゼロクリアする */
	memset(pMesh, 0, sizeof(GSmesh));
}

/*=============================================================================
*
* Purpose : メッシュファイルの読み込み．
*
* Return  : なし．
*
*============================================================================*/
extern GSboolean
gsMeshLoadFileTxt
(
	GSmesh*			pMesh,				/* メッシュデータ		*/
	const char*		pszFileName			/* ファイル名			*/
)
{
	GFILE			file;
	GSuint			bPoints;
	GSuint			numNormals;
	GSuint			bDiffuseColors;
	GSuint			bSpecularColors;
	GSuint			bBoneIndices;
	GSuint			i;
	GSuint			j;

	/* メッシュファイルをオープンする */
	if ((file = GFileOpen(pszFileName, "r")) == NULL)
	{
		/* ファイルがオープンできなかった */
		return	GS_FALSE;
	}

	/* メッシュのクリーンアップ */
	gsMeshCleanUp(pMesh);

	/* メッシュ情報フラグを読み込む */
	GFileReadDwordHex(file, &pMesh->MeshInfo);

	/* 頂点データ情報数の読み込み ( 現時点では使用していない ）*/
	GFileReadDword(file, &bPoints);

	/* 頂点データフラグの読み込み */
	GFileReadDword(file, &bPoints);

	/* 法線ベクトルデータフラグの読み込み */
	GFileReadDword(file, &numNormals);

	/* 拡散光カラーデータフラグの読み込み */
	GFileReadDword(file, &bDiffuseColors);

	/* 鏡面反射光カラーデータフラグの読み込み */
	GFileReadDword(file, &bSpecularColors);

	/* テクスチャ座標データ数の読み込み */
	GFileReadDword(file, &pMesh->numTexCoords);

	/* 頂点ウェイトデータ数の読み込み */
	GFileReadDword(file, &pMesh->numWeights);

	/* ボーンインデックスデータフラグの読み込み */
	GFileReadDword(file, &bBoneIndices);

	/* マテリアルデータ数の読み込み */
	GFileReadDword(file, &pMesh->numMaterials);

	/* 面データグループ数の読み込み */
	GFileReadDword(file, &pMesh->numFaceGroups);

	/* 頂点インデックスデータ数の読み込み */
	GFileReadDword(file, &pMesh->numIndices);

	/* 頂点データ数の読み込み */
	GFileReadDword(file, &pMesh->numVertices);

	/* マテリアルデータ配列の生成 */
	pMesh->pMaterials = new_object(sizeof(GSmaterial) * pMesh->numMaterials);

	/* マテリアルデータ配列の読み込み */
	for (i = 0; i < pMesh->numMaterials; i++)
	{
		LoadMaterial(&pMesh->pMaterials[i], file);
	}

	/* 面グループ配列の生成 */
	pMesh->pFaceGroups = new_object(sizeof(GSfacegroup) * pMesh->numFaceGroups);

	/* 面グループの配列の読み込み */
	for (i = 0; i < pMesh->numFaceGroups; i++)
	{
		/* 面情報フラグの読み込み */
		GFileReadDwordHex(file, &pMesh->pFaceGroups[i].FaceInfo);

		/* マテリアルインデックスの読み込み	*/
		GFileReadDword(file, &pMesh->pFaceGroups[i].MaterialNo);

		/* 頂点インデックスの開始位置の読み込み */
		GFileReadDword(file, &pMesh->pFaceGroups[i].StartIndices);

		/* 頂点インデックス数の読み込み */
		GFileReadDword(file, &pMesh->pFaceGroups[i].numIndices);
	}

	/* 頂点インデックス配列の読み込み */
	if (pMesh->numIndices != 0)
	{
		pMesh->pIndices = new_object(sizeof(GSuint) * pMesh->numIndices);

		GFileReadDwordArray(file, pMesh->numIndices, pMesh->pIndices);
	}

	/* 頂点データ配列の読み込み */
	if (bPoints == GS_TRUE)
	{
		pMesh->pPoints = new_object(sizeof(GSvector3) * pMesh->numVertices);

		GFileReadVector3Array(file, pMesh->numVertices, pMesh->pPoints);
	}

	/* 法線ベクトル配列の読み込み */
	if (numNormals >= 1)
	{
		pMesh->pNormals = new_object(sizeof(GSvector3) * pMesh->numVertices);

		GFileReadVector3Array(file, pMesh->numVertices, pMesh->pNormals);
	}

	/* 拡散光カラー配列の読み込み */
	if (bDiffuseColors == GS_TRUE)
	{
		pMesh->pDiffuseColors = new_object(sizeof(GScolor) * pMesh->numVertices);

		GFileReadColorArray(file, pMesh->numVertices, pMesh->pDiffuseColors);
	}

	/* 鏡面反射光カラー配列の読み込み */
	if (bSpecularColors == GS_TRUE)
	{
		pMesh->pSpecularColors = new_object(sizeof(GScolor) * pMesh->numVertices);

		GFileReadColorArray(file, pMesh->numVertices, pMesh->pSpecularColors);
	}

	/* テクスチャ座標配列の読み込み */
	if (pMesh->numTexCoords != 0)
	{
		pMesh->ppTexCoords = new_object(sizeof(GSvector2*) * pMesh->numTexCoords);

		for (i = 0; i < pMesh->numTexCoords; i++)
		{
			pMesh->ppTexCoords[i] = new_object(sizeof(GSvector2) * pMesh->numVertices);

			GFileReadTexCoordArray(file, pMesh->numVertices, pMesh->ppTexCoords[i]);
		}
	}

	if (bBoneIndices == GS_TRUE)
	{
		GSfloat*	weights;
		GSuint*		boneIndex;
		/* 頂点ウェイト配列の読み込み */
		pMesh->pWeights = new_object(sizeof(GSblendweight) * pMesh->numVertices);
		memset(pMesh->pWeights, 0, sizeof(GSblendweight) * pMesh->numVertices);
		for (i = 0; i < pMesh->numWeights; i++)
		{
			weights = new_object(sizeof(GSfloat) * pMesh->numVertices);
			GFileReadFloatArray(file, pMesh->numVertices, weights);
			for (j = 0; j < pMesh->numVertices; ++j) {
				pMesh->pWeights[j].Weights[i] = weights[j];
			}
			del_object(weights);
		}
		for (i = 0; i < pMesh->numVertices; ++i) {
			float totalWeight = 0.0f;
			for (j = 0; j < pMesh->numWeights; ++j)
			{
				totalWeight += pMesh->pWeights[i].Weights[j];
			}
			pMesh->pWeights[i].Weights[j] = 1.0f - totalWeight;
		}
		++pMesh->numWeights;

		/* ボーンインデックス配列の読み込み */
		boneIndex = new_object(sizeof(GSuint) * pMesh->numVertices);
		GFileReadDwordHexArray(file, pMesh->numVertices, boneIndex);
		pMesh->pBlendIndices = new_object(sizeof(GSblendindices) * pMesh->numVertices);
		for (i = 0; i < pMesh->numVertices; ++i) {
			pMesh->pBlendIndices[i].Indices[0] = (boneIndex[i] >>  0) & 0xff;
			pMesh->pBlendIndices[i].Indices[1] = (boneIndex[i] >>  8) & 0xff;
			pMesh->pBlendIndices[i].Indices[2] = (boneIndex[i] >> 16) & 0xff;
			pMesh->pBlendIndices[i].Indices[3] = (boneIndex[i] >> 24) & 0xff;
		}
		del_object(boneIndex);
	}

	/* 接ベクトル配列の読み込み */
	if (numNormals >= 2)
	{
		pMesh->pTangents = new_object(sizeof(GSvector3) * pMesh->numVertices);

		GFileReadVector3Array(file, pMesh->numVertices, pMesh->pTangents);
	}

	/* 従法線ベクトル配列の読み込み */
	if (numNormals >= 3)
	{
		pMesh->pBinormals = new_object(sizeof(GSvector3) * pMesh->numVertices);

		GFileReadVector3Array(file, pMesh->numVertices, pMesh->pBinormals);
	}

	/* メッシュファイルのクローズ */
	GFileClose(file);

	/* メッシュの平面データ配列を計算する */
	CalculatePlanes(pMesh);

	return	GS_TRUE;
}

/*=============================================================================
*
* Purpose : メッシュファイルの読み込み．
*
* Return  : なし．
*
*============================================================================*/
extern GSboolean
gsMeshLoadFileBin
(
	GSmesh*			pMesh,				/* メッシュデータ		*/
	const char*		pszFileName			/* ファイル名			*/
)
{
	GFILE			file;
	GSuint			numFormat;
	GSuint			bPoints;
	GSuint			numNormals;
	GSuint			bDiffuseColors;
	GSuint			bSpecularColors;
	GSuint			bBoneIndices;
	GSuint			i;
	GSuint			j;

	/* メッシュファイルをオープンする */
	if ((file = GFileOpen(pszFileName, "rb")) == NULL)
	{
		/* ファイルがオープンできなかった */
		return	GS_FALSE;
	}

	/* メッシュのクリーンアップ */
	gsMeshCleanUp(pMesh);

	/* メッシュ情報フラグを読み込む */
	GFileReadDwordBin(file, &pMesh->MeshInfo);

	/* 頂点データ情報数の読み込み ( 現時点では使用していない ）*/
	GFileReadDwordBin(file, &numFormat);

	/* 頂点データフラグの読み込み */
	GFileReadDwordBin(file, &bPoints);

	/* 法線ベクトルデータフラグの読み込み */
	GFileReadDwordBin(file, &numNormals);

	/* 拡散光カラーデータフラグの読み込み */
	GFileReadDwordBin(file, &bDiffuseColors);

	/* 鏡面反射光カラーデータフラグの読み込み */
	GFileReadDwordBin(file, &bSpecularColors);

	/* テクスチャ座標データ数の読み込み */
	GFileReadDwordBin(file, &pMesh->numTexCoords);

	/* 頂点ウェイトデータ数の読み込み */
	GFileReadDwordBin(file, &pMesh->numWeights);

	/* ボーンインデックスデータフラグの読み込み */
	GFileReadDwordBin(file, &bBoneIndices);

	/* マテリアルデータ数の読み込み */
	GFileReadDwordBin(file, &pMesh->numMaterials);

	/* 面データグループ数の読み込み */
	GFileReadDwordBin(file, &pMesh->numFaceGroups);

	/* 頂点インデックスデータ数の読み込み */
	GFileReadDwordBin(file, &pMesh->numIndices);

	/* 頂点データ数の読み込み */
	GFileReadDwordBin(file, &pMesh->numVertices);

	/* マテリアルデータ配列の生成 */
	pMesh->pMaterials = new_object(sizeof(GSmaterial) * pMesh->numMaterials);

	/* マテリアルデータ配列の読み込み */
	for (i = 0; i < pMesh->numMaterials; i++)
	{
		LoadMaterialBin(&pMesh->pMaterials[i], file);
	}

	/* 面グループ配列の生成 */
	pMesh->pFaceGroups = new_object(sizeof(GSfacegroup) * pMesh->numFaceGroups);

	/* 面グループの配列の読み込み */
	for (i = 0; i < pMesh->numFaceGroups; i++)
	{
		/* 面情報フラグの読み込み */
		GFileReadDwordBin(file, &pMesh->pFaceGroups[i].FaceInfo);

		/* マテリアルインデックスの読み込み	*/
		GFileReadDwordBin(file, &pMesh->pFaceGroups[i].MaterialNo);

		/* 頂点インデックスの開始位置の読み込み */
		GFileReadDwordBin(file, &pMesh->pFaceGroups[i].StartIndices);

		/* 頂点インデックス数の読み込み */
		GFileReadDwordBin(file, &pMesh->pFaceGroups[i].numIndices);
	}

	/* 頂点インデックス配列の読み込み */
	if (pMesh->numIndices != 0)
	{
		pMesh->pIndices = new_object(sizeof(GSuint) * pMesh->numIndices);

		GFileReadDwordArrayBin(file, pMesh->numIndices, pMesh->pIndices);
	}

	/* 頂点データ配列の読み込み */
	if (bPoints == GS_TRUE)
	{
		pMesh->pPoints = new_object(sizeof(GSvector3) * pMesh->numVertices);

		GFileReadVector3ArrayBin(file, pMesh->numVertices, pMesh->pPoints);
	}

	/* 法線ベクトル配列の読み込み */
	if (numNormals >= 1)
	{
		pMesh->pNormals = new_object(sizeof(GSvector3) * pMesh->numVertices);

		GFileReadVector3ArrayBin(file, pMesh->numVertices, pMesh->pNormals);
	}

	/* 拡散光カラー配列の読み込み */
	if (bDiffuseColors == GS_TRUE)
	{
		pMesh->pDiffuseColors = new_object(sizeof(GScolor) * pMesh->numVertices);

		GFileReadColorArrayBin(file, pMesh->numVertices, pMesh->pDiffuseColors);
	}

	/* 鏡面反射光カラー配列の読み込み */
	if (bSpecularColors == GS_TRUE)
	{
		pMesh->pSpecularColors = new_object(sizeof(GScolor) * pMesh->numVertices);

		GFileReadColorArrayBin(file, pMesh->numVertices, pMesh->pSpecularColors);
	}

	/* テクスチャ座標配列の読み込み */
	if (pMesh->numTexCoords != 0)
	{
		pMesh->ppTexCoords = new_object(sizeof(GSvector2*) * pMesh->numTexCoords);

		for (i = 0; i < pMesh->numTexCoords; i++)
		{
			pMesh->ppTexCoords[i] = new_object(sizeof(GSvector2) * pMesh->numVertices);

			GFileReadTexCoordArrayBin(file, pMesh->numVertices, pMesh->ppTexCoords[i]);
		}
	}

	if (bBoneIndices == GS_TRUE)
	{
		GSfloat*	weights;
		GSuint*		boneIndex;
		/* 頂点ウェイト配列の読み込み */
		pMesh->pWeights = new_object(sizeof(GSblendweight) * pMesh->numVertices);
		memset(pMesh->pWeights, 0, sizeof(GSblendweight) * pMesh->numVertices);
		for (i = 0; i < pMesh->numWeights; i++)
		{
			weights = new_object(sizeof(GSfloat) * pMesh->numVertices);
			GFileReadFloatArrayBin(file, pMesh->numVertices, weights);
			for (j = 0; j < pMesh->numVertices; ++j) {
				pMesh->pWeights[j].Weights[i] = weights[j];
			}
			del_object(weights);
		}
		for (i = 0; i < pMesh->numVertices; ++i) {
			float totalWeight = 0.0f;
			for (j = 0; j < pMesh->numWeights; ++j)
			{
				totalWeight += pMesh->pWeights[i].Weights[j];
			}
			pMesh->pWeights[i].Weights[j] = 1.0f - totalWeight;
		}
		++pMesh->numWeights;

		/* ボーンインデックス配列の読み込み */
		boneIndex = new_object(sizeof(GSuint) * pMesh->numVertices);
		GFileReadDwordArrayBin(file, pMesh->numVertices, boneIndex);
		pMesh->pBlendIndices = new_object(sizeof(GSblendindices) * pMesh->numVertices);
		for (i = 0; i < pMesh->numVertices; ++i) {
			pMesh->pBlendIndices[i].Indices[0] = (boneIndex[i] >> 0) & 0xff;
			pMesh->pBlendIndices[i].Indices[1] = (boneIndex[i] >> 8) & 0xff;
			pMesh->pBlendIndices[i].Indices[2] = (boneIndex[i] >> 16) & 0xff;
			pMesh->pBlendIndices[i].Indices[3] = (boneIndex[i] >> 24) & 0xff;
		}
		del_object(boneIndex);
	}

	/* 接ベクトル配列の読み込み */
	if (numNormals >= 2)
	{
		pMesh->pTangents = new_object(sizeof(GSvector3) * pMesh->numVertices);

		GFileReadVector3ArrayBin(file, pMesh->numVertices, pMesh->pTangents);
	}

	/* 従法線ベクトル配列の読み込み */
	if (numNormals >= 3)
	{
		pMesh->pBinormals = new_object(sizeof(GSvector3) * pMesh->numVertices);

		GFileReadVector3ArrayBin(file, pMesh->numVertices, pMesh->pBinormals);
	}

	/* メッシュファイルのクローズ */
	GFileClose(file);

	/* メッシュの平面データ配列を計算する */
	CalculatePlanes(pMesh);

	return	GS_TRUE;
}

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
)
{
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char mesh_file[_MAX_PATH];
	/* ファイル名を分解する */
	gsSplitPath(pszFileName, drive, dir, fname, NULL);

	/* mshb -> msh の優先順位で読み込みをする */
	gsMakePath(mesh_file, drive, dir, fname, ".mshb");
	if (gsMeshLoadFileBin(pMesh, mesh_file) == GS_TRUE) {
		return GS_TRUE;
	}
	gsMakePath(mesh_file, drive, dir, fname, ".msh");
	if (gsMeshLoadFileTxt(pMesh, mesh_file) == GS_TRUE) {
		return GS_TRUE;
	}
	return GS_FALSE;
}

/*=============================================================================
*
* Purpose : メッシュファイルの読み込み．
*
* Return  : なし．
*
*============================================================================*/
extern GSboolean
gsMeshLoadFileEx
(
	GSmesh*			pMesh,				/* メッシュデータ		*/
	const char*		pszFileName			/* ファイル名			*/
)
{
	GSboolean result;
	result = gsMeshLoadFile(pMesh, pszFileName);
	if (result == GS_TRUE)
	{
		CreateMeshBuffer(pMesh);
	}
	return result;
}

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
)
{
	/* メッシュを描画する */
	DrawMesh
	(
		pMesh->pMaterials,
		pMesh->numFaceGroups,
		pMesh->pFaceGroups,
		pMesh->pIndices,
		pMesh->pPoints,
		pMesh->pNormals,
		pMesh->pTangents,
		pMesh->pBinormals,
		pMesh->pDiffuseColors,
		pMesh->numTexCoords,
		pMesh->ppTexCoords
	);
}

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
)
{
	/* セルシェーディングを行いメッシュを描画する */
	DrawMeshCelShade
	(
		pMesh->pMaterials,
		pMesh->numFaceGroups,
		pMesh->pFaceGroups,
		pMesh->numIndices,
		pMesh->pIndices,
		pMesh->numVertices,
		pMesh->pPoints,
		pMesh->pNormals,
		pMesh->pDiffuseColors,
		pMesh->numTexCoords,
		pMesh->ppTexCoords,
		pShadeTexture,
		pShadeLight,
		pLineColor,
		fLineWidth
	);
}

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
)
{
	GSvector3*			pPoints = NULL;
	GSvector3*			pNormals = NULL;
	GSvector3*			pTangents = NULL;
	GSvector3*			pBinormals = NULL;

	/* ボーンインデックスを持っているか？ */
	if (pMesh->pBlendIndices == NULL)
	{
		/* ボーンインデックスがない場合はスキニングの計算はしない */
		gsMeshDraw(pMesh);

		return;
	}
	/* 頂点データ配列を生成する */
	pPoints = new_object(sizeof(GSvector3) * pMesh->numVertices);

	/* 法線ベクトル配列を生成する	*/
	if (pMesh->pNormals != NULL) {
		pNormals = new_object(sizeof(GSvector3) * pMesh->numVertices);
	}

	/* 接ベクトル配列を生成する	*/
	if (pMesh->pTangents != NULL) {
		pTangents = new_object(sizeof(GSvector3) * pMesh->numVertices);
	}

	/* 従法線ベクトルを生成する */
	if (pMesh->pBinormals != NULL) {
		pBinormals = new_object(sizeof(GSvector3) * pMesh->numVertices);
	}

	/* 頂点データの座標変換 */
	TransformVertices
	(
		pPoints, pNormals, pTangents, pBinormals,
		pMesh->numVertices,
		pMesh->pPoints,
		pMesh->pNormals,
		pMesh->pTangents,
		pMesh->pBinormals, 
		pMesh->numWeights,
		pMesh->pWeights,
		pMesh->pBlendIndices,
		pMatrices
	);
	/* メッシュを描画する */
	DrawMesh
	(
		pMesh->pMaterials,
		pMesh->numFaceGroups,
		pMesh->pFaceGroups,
		pMesh->pIndices,
		pPoints,
		pNormals,
		pTangents,
		pBinormals,
		pMesh->pDiffuseColors,
		pMesh->numTexCoords,
		pMesh->ppTexCoords
	);
	/* 従法線ベクトル配列を消滅させる */
	del_object(pBinormals);

	/* 接ベクトル配列を消滅させる */
	del_object(pTangents);

	/* 法線ベクトル配列を消滅させる */
	del_object(pNormals);

	/* 頂点データ配列を消滅させる */
	del_object(pPoints);
}

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
)
{
	GSvector3*			pPoints;
	GSvector3*			pNormals;

	/* ボーンインデックスを持っているか？ */
	if (pMesh->pBlendIndices == NULL)
	{
		/* ボーンインデックスがない場合はスキニングの計算はしない */
		gsMeshDrawCelShade
		(
			pMesh,
			pShadeTexture,
			pShadeLight,
			pLineColor,
			fLineWidth
		);

		return;
	}

	/* 頂点データ配列を生成する */
	pPoints = new_object(sizeof(GSvector3) * pMesh->numVertices);

	/* 法線ベクトル配列を生成する	*/
	pNormals = new_object(sizeof(GSvector3) * pMesh->numVertices);

	/* 頂点データの座標変換 */
	TransformVertices(
		pPoints, pNormals, NULL, NULL,
		pMesh->numVertices,
		pMesh->pPoints,
		pMesh->pNormals,
		NULL, NULL,
		pMesh->numWeights,
		pMesh->pWeights,
		pMesh->pBlendIndices,
		pMatrices
	);

	/* セルシェーディングを行いメッシュを描画する */
	DrawMeshCelShade
	(
		pMesh->pMaterials,
		pMesh->numFaceGroups,
		pMesh->pFaceGroups,
		pMesh->numIndices,
		pMesh->pIndices,
		pMesh->numVertices,
		pPoints,
		pNormals,
		pMesh->pDiffuseColors,
		pMesh->numTexCoords,
		pMesh->ppTexCoords,
		pShadeTexture,
		pShadeLight,
		pLineColor,
		fLineWidth
	);

	/* 法線ベクトル配列を消滅させる */
	del_object(pNormals);

	/* 頂点データ配列を消滅させる */
	del_object(pPoints);
}

/*-----------------------------------------------------------------------------
*
* Purpose : メッシュの描画．
*
* Return  : なし．
*
*----------------------------------------------------------------------------*/
static void
DrawMesh
(
	GSmaterial*		pMaterials,			/* マテリアルデータ配列	*/
	GSuint			numFaceGroups,		/* 面データグループ数	*/
	GSfacegroup*	pFaceGroups,		/* 面データグループ		*/
	GSuint*			pIndices,			/* 頂点インデックス		*/
	GSvector3*		pPoints,			/* 頂点データ配列		*/
	GSvector3*		pNormals,			/* 法線ベクトル配列		*/
	GSvector3*		pTangents,			/* 接ベクトル配列		*/
	GSvector3*		pBinormals,			/* 従法線ベクトル配列	*/
	GScolor*		pDiffuseColors,		/* 拡散光配列			*/
	GSuint			numTexCoords,		/* テクスチャ座標配列数	*/
	GSvector2**		ppTexCoords			/* テクスチャ座標配列	*/
)
{
	GScolor			CurrentColor;
	GSuint			i;

	/* 各種レンダリングモードの退避 */
	glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_LIGHTING_BIT | GL_TEXTURE_BIT);

	/* 現在のカラーを取得する */
	glGetFloatv(GL_CURRENT_COLOR, (GLfloat*)&CurrentColor);

	/* 頂点データ配列の設定 */
	glDisableClientState(GL_VERTEX_ARRAY);
	if (pPoints != NULL)
	{
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, pPoints);
	}

	/* 法線ベクトル配列の設定 */
	glDisableClientState(GL_NORMAL_ARRAY);
	if (pNormals != NULL)
	{
		glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer(GL_FLOAT, 0, pNormals);
	}

	/* 接ベクトル配列の設定 */
	glClientActiveTexture(GL_TEXTURE6);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	if (pTangents != NULL)
	{
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(3, GL_FLOAT, 0, pTangents);
	}

	/* 従法線ベクトル配列の設定 */
	glClientActiveTexture(GL_TEXTURE7);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	if (pBinormals != NULL)
	{
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(3, GL_FLOAT, 0, pBinormals);
	}

	/* カラーデータ配列の設定 */
	glDisableClientState(GL_COLOR_ARRAY);
	if (pDiffuseColors != NULL)
	{
		glEnableClientState(GL_COLOR_ARRAY);
		glColorPointer(4, GL_FLOAT, 0, pDiffuseColors);
	}

	/* テクスチャ座標2配列の設定 */
	glClientActiveTexture(GL_TEXTURE1);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	if (numTexCoords >= 2)
	{
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2, GL_FLOAT, 0, ppTexCoords[1]);
	}

	/* テクスチャ座標配列の設定 */
	glClientActiveTexture(GL_TEXTURE0);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	if (numTexCoords >= 1)
	{
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2, GL_FLOAT, 0, ppTexCoords[0]);
	}

	for (i = 0; i < numFaceGroups; i++)
	{
		/* マテリアル情報の設定をする */
		SetMaterial(&pMaterials[pFaceGroups[i].MaterialNo]);

		/* 三角形を描画する */
		glDrawElements
		(
			GL_TRIANGLES,
			pFaceGroups[i].numIndices,
			GL_UNSIGNED_INT,
			&pIndices[pFaceGroups[i].StartIndices]
		);

		/* カレントカラーを設定する */
		glColor4fv((GLfloat*)&CurrentColor);
	}


	/* テクスチャを無効にする */
	glBindTexture(GL_TEXTURE_2D, 0);

	/* レンダリングモードの復帰 */
	glPopAttrib();
}

/*-----------------------------------------------------------------------------
*
* Purpose : メッシュの描画．（ セルシェーディング ）
*
* Return  : なし．
*
*----------------------------------------------------------------------------*/
static void
DrawMeshCelShade
(
	GSmaterial*		pMaterials,			/* マテリアルデータ配列			*/
	GSuint			numFaceGroups,		/* 面データグループ数			*/
	GSfacegroup*	pFaceGroups,		/* 面データグループ				*/
	GSuint			numIndices,			/* 頂点インデックス数			*/
	GSuint*			pIndices,			/* 頂点インデックス				*/
	GSuint			numVertices,		/* 頂点データ数					*/
	GSvector3*		pPoints,			/* 頂点データ配列				*/
	GSvector3*		pNormals,			/* 法線ベクトル配列				*/
	GScolor*		pDiffuseColors,		/* 拡散光配列					*/
	GSuint			numTexCoords,		/* テクスチャ座標配列数			*/
	GSvector2**		ppTexCoords,		/* テクスチャ座標配列			*/
	GStexture*		pShadeTexture,		/* シェーディング用テクスチャ	*/
	GSvector3*		pShadeLight,		/* ライトの位置					*/
	GScolor*		pLineColor,			/* 輪郭線の色					*/
	GSfloat			fLineWidth			/* 輪郭線の太さ					*/
)
{
	GSmaterial*		pMaterial;
	GSvector2*		pTexCoords;
	GScolor			CurrentColor;
	GSmatrix4		tmpMat;
	GSvector3		tmpLight;
	GScolor			tmpColor;
	GSuint			i;
	(void)numTexCoords;

	/* 現在のカラーを取得する */
	glGetFloatv(GL_CURRENT_COLOR, (GLfloat*)&CurrentColor);

	/* トゥーンシェーディング用のテクスチャ座標配列を生成 */
	pTexCoords = new_object(sizeof(GSvector2) * numVertices);

	/* モデルビュー変換マトリクスを取得する */
	glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat*)&tmpMat);

	/* ライトの方向にモデルビュー変換行列の逆行列を掛け算する */
	tmpLight.x = pShadeLight->x * tmpMat.m[0][0]
		+ pShadeLight->y * tmpMat.m[0][1]
		+ pShadeLight->z * tmpMat.m[0][2];
	tmpLight.y = pShadeLight->x * tmpMat.m[1][0]
		+ pShadeLight->y * tmpMat.m[1][1]
		+ pShadeLight->z * tmpMat.m[1][2];
	tmpLight.z = pShadeLight->x * tmpMat.m[2][0]
		+ pShadeLight->y * tmpMat.m[2][1]
		+ pShadeLight->z * tmpMat.m[2][2];

	/* ライトのベクトルを正規化する */
	gsVector3Normalize(&tmpLight, &tmpLight);

	if (pNormals != NULL)
	{
		/* トゥーンシェーディングのテクスチャ座標の計算を行う */
		for (i = 0; i < numVertices; i++)
		{
			/* ライトの方向と法線ベクトルからテクスチャ座標を求める */
			pTexCoords[i].s = MAX(0.0f, gsVector3Dot(&pNormals[i], &tmpLight));
			pTexCoords[i].t = 0.0f;
		}
	}
	else
	{
		/* 法線ベクトルが存在しない場合はシェーディングできない */
		for (i = 0; i < numVertices; i++)
		{
			pTexCoords[i].s = 1.0f;
			pTexCoords[i].t = 0.0f;
		}
	}

	/* 各種レンダリングモードの退避 */
	glPushAttrib
	(
		GL_ENABLE_BIT |
		GL_DEPTH_BUFFER_BIT |
		GL_COLOR_BUFFER_BIT |
		GL_POLYGON_BIT |
		GL_LIGHTING_BIT |
		GL_TEXTURE_BIT
	);

	/* ライティングをオフにする */
	glDisable(GL_LIGHTING);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_BLEND);

	/* 頂点データ配列の設定 */
	glDisableClientState(GL_VERTEX_ARRAY);
	if (pPoints != NULL)
	{
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, pPoints);
	}

	/* カラーデータ配列の設定 */
	glDisableClientState(GL_COLOR_ARRAY);
	if (pDiffuseColors != NULL)
	{
		glEnableClientState(GL_COLOR_ARRAY);
		glColorPointer(4, GL_FLOAT, 0, pDiffuseColors);
	}

	/* 法線ベクトル配列の設定 */
	glDisableClientState(GL_NORMAL_ARRAY);
	if (pNormals != NULL)
	{
		glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer(GL_FLOAT, 0, pNormals);
	}

	for (i = 0; i < numFaceGroups; i++)
	{
		/* マテリアルデータの取得 */
		pMaterial = &pMaterials[pFaceGroups[i].MaterialNo];

		/* マテリアルの設定 */
		gsColorModulate(&tmpColor, &pMaterial->Diffuse, &CurrentColor);
		glColor4fv((GLfloat*)&tmpColor);

		/* 法線ベクトル配列を無効にする */
		glDisableClientState(GL_NORMAL_ARRAY);

		/* テクスチャ座標を有効にする */
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		/* セルシェーディング用のテクスチャをバインドする */
		if (pShadeTexture != NULL)
		{
			gsTextureBind(pShadeTexture);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glEnable(GL_TEXTURE_2D);
		}
		else
		{
			glDisable(GL_TEXTURE_2D);
		}

		/* セルシェーディング用のテクスチャ座標を設定する */
		glTexCoordPointer(2, GL_FLOAT, 0, pTexCoords);

		/* セルシェーディング用ポリゴンを描画する */
		glDrawElements
		(
			GL_TRIANGLES,
			pFaceGroups[i].numIndices,
			GL_UNSIGNED_INT,
			&pIndices[pFaceGroups[i].StartIndices]
		);

		if (pMaterial->pTexture != NULL)
		{
			/* テクスチャマッピングの設定 */
			gsTextureBind(pMaterial->pTexture);
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			glBlendFunc(GL_DST_COLOR, GL_ZERO);
			glEnable(GL_TEXTURE_2D);

			/* テクスチャをクランプするか？*/
			if ((pMaterial->MaterialInfo & GS_MATERIAL_TEX_CLAMP) != 0)
			{
				/* テクスチャをクランプに設定する */
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

				/* EDGEクランプが有効であれば使用する */
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}
			else
			{
				/* テクスチャをラップさせる */
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			}

			/* 球形環境マッピングを行うか？ */
			if ((pMaterial->MaterialInfo & GS_MATERIAL_SPHERE_MAP) != 0)
			{
				/* 球形環境マッピングを有効にする */
				glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
				glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
				glEnable(GL_TEXTURE_GEN_S);
				glEnable(GL_TEXTURE_GEN_T);
				glDisableClientState(GL_TEXTURE_COORD_ARRAY);

				/* 法線ベクトル配列を有効にする */
				glEnableClientState(GL_NORMAL_ARRAY);
			}
			else
			{
				/* 通常のテクスチャ座標配列の設定 */
				glTexCoordPointer(2, GL_FLOAT, 0, ppTexCoords[0]);
			}

			/* 通常のテクスチャを描画する */
			glDrawElements
			(
				GL_TRIANGLES,
				pFaceGroups[i].numIndices,
				GL_UNSIGNED_INT,
				&pIndices[pFaceGroups[i].StartIndices]
			);

			/* 球形環境マッピングを無効にする */
			glDisable(GL_TEXTURE_GEN_S);
			glDisable(GL_TEXTURE_GEN_T);
		}

		/* カレントカラーを復帰する */
		glColor4fv((GLfloat*)&CurrentColor);
	}

	/* 輪郭線の描画を行う */
	if (fLineWidth > 0)
	{
		float scale = fLineWidth * 0.01f;

		/* 法線方向に頂点データを拡大する */
		for (i = 0; i < numVertices; i++)
		{
			pPoints[i].x += pNormals[i].x * scale;
			pPoints[i].y += pNormals[i].y * scale;
			pPoints[i].z += pNormals[i].z * scale;
		}

		/* テクスチャを無効にする */
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisable(GL_TEXTURE_2D);

		/* 輪郭線の色を設定する */
		gsColorModulate(&tmpColor, pLineColor, &CurrentColor);
		glColor4fv((GLfloat*)&tmpColor);

		/* レンダリングモードの設定 */
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glCullFace(GL_FRONT);

		/* 輪郭線を描画する */
		glDrawElements
		(
			GL_TRIANGLES,
			numIndices,
			GL_UNSIGNED_INT,
			pIndices
		);

		/* カレントカラーを復帰する */
		glColor4fv((GLfloat*)&CurrentColor);
	}
	/* テクスチャを無効にする */
	glBindTexture(GL_TEXTURE_2D, 0);

	/* 各種レンダリングモードの復帰 */
	glPopAttrib();

	/* トゥーンシェーディング用のテクスチャ座標を消滅させる */
	del_object(pTexCoords);
}

/*-----------------------------------------------------------------------------
*
* Purpose : マテリアルの設定．
*
* Return  : なし．
*
*----------------------------------------------------------------------------*/
static void
SetMaterial
(
	GSmaterial*		pMaterial			/* マテリアルデータ	*/
)
{
	GScolor			Diffuse;
	GScolor			Ambient;
	GScolor			CurrentColor;

	/* 現在のカラーを取得する */
	glGetFloatv(GL_CURRENT_COLOR, (GLfloat*)&CurrentColor);

	/* 環境光と拡散光カラーを現在のカラーと変調する */
	gsColorModulate(&Diffuse, &pMaterial->Diffuse, &CurrentColor);
	gsColorModulate(&Ambient, &pMaterial->Ambient, &CurrentColor);

	/* マテリアル情報の設定 */
	glMaterialfv(GL_FRONT, GL_DIFFUSE, (GLfloat*)&Diffuse);
	glMaterialfv(GL_FRONT, GL_AMBIENT, (GLfloat*)&Ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, (GLfloat*)&pMaterial->Specular);
	glMaterialfv(GL_FRONT, GL_EMISSION, (GLfloat*)&pMaterial->Emissive);
	glMaterialf(GL_FRONT, GL_SHININESS, (GLfloat)pMaterial->Power);

	/* ライティングがオフの場合のマテリアル設定 */
	glColor4fv((GLfloat*)&Diffuse);

	if (pMaterial->pTexture == NULL)
	{
		/* テクスチャマッピングの設定解除 */
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_TEXTURE_1D);
	}
	else
	{
		/* テクスチャマッピングの設定 */
		glActiveTexture(GL_TEXTURE0);
		gsTextureBind(pMaterial->pTexture);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		/* テクスチャをクランプするか？*/
		if ((pMaterial->MaterialInfo & GS_MATERIAL_TEX_CLAMP) != 0)
		{
			/* テクスチャをクランプに設定する */
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

			/* EDGEクランプが有効であれば使用する */
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}
		else
		{
			/* テクスチャをラップさせる */
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		}
	}

	if ((pMaterial->MaterialInfo & GS_MATERIAL_SPHERE_MAP) != 0)
	{
		/* 球形環境マッピングを有効にする */
		glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
		glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
		glEnable(GL_TEXTURE_GEN_S);
		glEnable(GL_TEXTURE_GEN_T);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}
	else
	{
		/* 球形環境マッピングを無効にする */
		glDisable(GL_TEXTURE_GEN_S);
		glDisable(GL_TEXTURE_GEN_T);
	}
	/* 法線マップの設定 */
	if (pMaterial->pNormalTexture != 0)
	{
		glActiveTexture(GL_TEXTURE1);
		gsTextureBind(pMaterial->pNormalTexture);
	}
	/* 鏡面反射テクスチャの設定 */
	if (pMaterial->pSpecularTexture != 0)
	{
		glActiveTexture(GL_TEXTURE2);
		gsTextureBind(pMaterial->pSpecularTexture);
	}
	/* アンビエントオクルージョンテクスチャの設定 */
	if (pMaterial->pAmbientTexture != 0)
	{
		glActiveTexture(GL_TEXTURE3);
		gsTextureBind(pMaterial->pAmbientTexture);
	}
	/* 自己照明マップの設定 */
	if (pMaterial->pEmissiveTexture != 0)
	{
		glActiveTexture(GL_TEXTURE4);
		gsTextureBind(pMaterial->pEmissiveTexture);
	}
	/* 高さマップの設定 */
	if (pMaterial->pHeightTexture != 0)
	{
		glActiveTexture(GL_TEXTURE5);
		gsTextureBind(pMaterial->pHeightTexture);
	}
	/* 第２テクスチャマップの設定				*/
	if (pMaterial->pTexture2 != 0)
	{
		glActiveTexture(GL_TEXTURE6);
		gsTextureBind(pMaterial->pTexture2);
	}
	/* 第２法線マップの設定				*/
	if (pMaterial->pNormalTexture2 != 0)
	{
		glActiveTexture(GL_TEXTURE7);
		gsTextureBind(pMaterial->pNormalTexture2);
	}
	/* マスクテクスチャの設定 */
	if (pMaterial->pMaskTexture != 0)
	{
		glActiveTexture(GL_TEXTURE8);
		gsTextureBind(pMaterial->pMaskTexture);
	}
	/* キューブ環境マップテクスチャの設定 */
	if (pMaterial->pCubeTexture != 0)
	{
		glActiveTexture(GL_TEXTURE9);
		gsTextureBind(pMaterial->pCubeTexture);
	}
	glActiveTexture(GL_TEXTURE0);
}

/*-----------------------------------------------------------------------------
*
* Purpose : テクスチャの読み込み．
*
* Return  : なし．
*
*----------------------------------------------------------------------------*/
static GStexture*
LoadTexture
(
	GFILE		file					/* ファイル			*/
)
{
	GStexture*	pTexture;
	char		TexFileName[_MAX_PATH];
	char		Drive[_MAX_DRIVE];
	char		Dir[_MAX_DIR];

	/* オープンしているメッシュファイルのパスを取得する */
	gsSplitPath(GFileGetFileName(file), Drive, Dir, NULL, NULL);
	strcpy(TexFileName, Drive);
	strcat(TexFileName, Dir);

	/* テクスチャファイル名の読み込み */
	GFileReadString(file, TexFileName + strlen(TexFileName));

	/* テクスチャを生成 */
	pTexture = gsLoadTextureFile(TexFileName);

	/* ミップマップを生成する */
	if (pTexture != NULL && pTexture->Mipmap == 1) {
		gsTextureGenerateMipmap(pTexture);
	}
	return pTexture;
}

/*-----------------------------------------------------------------------------
*
* Purpose : マテリアルの読み込み．
*
* Return  : なし．
*
*----------------------------------------------------------------------------*/
static void
LoadMaterial
(
	GSmaterial*	pMaterial,				/* マテリアル		*/
	GFILE		file					/* ファイル			*/
)
{
	/* マテリアル情報フラグの読み込み */
	GFileReadDwordHex(file, &pMaterial->MaterialInfo);

	/* 拡散光カラーの読み込み */
	GFileReadColor(file, &pMaterial->Diffuse);

	/* 環境光カラーの読み込み */
	GFileReadColor(file, &pMaterial->Ambient);

	/* 鏡面反射光カラーの読み込み */
	GFileReadColor(file, &pMaterial->Specular);

	/* 放射光カラーの読み込み */
	GFileReadColor(file, &pMaterial->Emissive);

	/* ハイライトの鮮明度の読み込み */
	GFileReadFloat(file, &pMaterial->Power);
	pMaterial->Power = MAX(pMaterial->Power, 1.0f);

	/* テクスチャの読み込み */
	pMaterial->pTexture = NULL;
	if ((pMaterial->MaterialInfo & GS_MATERIAL_TEXTURE) != 0)
	{
		pMaterial->pTexture = LoadTexture(file);
	}
	/* 法線マップの読み込み	*/
	pMaterial->pNormalTexture = NULL;
	if ((pMaterial->MaterialInfo & GS_MATERIAL_NORMAL_TEXTURE) != 0)
	{
		pMaterial->pNormalTexture = LoadTexture(file);
	}
	/* 鏡面反射マップの読み込み	*/
	pMaterial->pSpecularTexture = NULL;
	if ((pMaterial->MaterialInfo & GS_MATERIAL_SPECULAR_TEXTURE) != 0)
	{
		pMaterial->pSpecularTexture = LoadTexture(file);
	}
	/* アンビエントオクルージョンマップの読み込み	*/
	pMaterial->pAmbientTexture = NULL;
	if ((pMaterial->MaterialInfo & GS_MATERIAL_AMBIENT_TEXTURE) != 0)
	{
		pMaterial->pAmbientTexture = LoadTexture(file);
	}
	/* 自己照明マップの読み込み			*/
	pMaterial->pEmissiveTexture = NULL;
	if ((pMaterial->MaterialInfo & GS_MATERIAL_EMISSIVE_TEXTURE) != 0)
	{
		pMaterial->pEmissiveTexture = LoadTexture(file);
	}
	/* 高さマップの読み込み */
	pMaterial->pHeightTexture = NULL;
	if ((pMaterial->MaterialInfo & GS_MATERIAL_HEIGHT_TEXTURE) != 0)
	{
		pMaterial->pHeightTexture = LoadTexture(file);
	}
	/* 第２テクスチャマップ				*/
	pMaterial->pTexture2 = NULL;
	if ((pMaterial->MaterialInfo & GS_MATERIAL_TEXTURE2) != 0)
	{
		pMaterial->pTexture2 = LoadTexture(file);
	}
	/* 第２法線マップ			*/
	pMaterial->pNormalTexture2 = NULL;
	if ((pMaterial->MaterialInfo & GS_MATERIAL_NORMAL_TEXTURE2) != 0)
	{
		pMaterial->pNormalTexture2 = LoadTexture(file);
	}
	/* マスクテクスチャの読み込み			*/
	pMaterial->pMaskTexture = NULL;
	if ((pMaterial->MaterialInfo & GS_MATERIAL_MASK_TEXTURE) != 0)
	{
		pMaterial->pMaskTexture = LoadTexture(file);
	}
	/* キューブ環境マップの読み込み	*/
	pMaterial->pCubeTexture = NULL;
	if ((pMaterial->MaterialInfo & GS_MATERIAL_CUBE_TEXTURE) != 0)
	{
		pMaterial->pCubeTexture = LoadTexture(file);
	}
}

/*-----------------------------------------------------------------------------
*
* Purpose : テクスチャの読み込み．
*
* Return  : なし．
*
*----------------------------------------------------------------------------*/
static GStexture*
LoadTextureBin
(
	GFILE		file					/* ファイル			*/
)
{
	static const GSuint MAX_TEXTURE_FILE_NAME = 128;
	GStexture*	pTexture;
	char		TexFileName[_MAX_PATH];
	char		Drive[_MAX_DRIVE];
	char		Dir[_MAX_DIR];

	/* オープンしているメッシュファイルのパスを取得する */
	gsSplitPath(GFileGetFileName(file), Drive, Dir, NULL, NULL);
	strcpy(TexFileName, Drive);
	strcat(TexFileName, Dir);

	/* テクスチャファイル名の読み込み */
	GFileReadStringBin(file, MAX_TEXTURE_FILE_NAME, TexFileName + strlen(TexFileName));

	/* テクスチャを生成 */
	pTexture = gsLoadTextureFile(TexFileName);

	/* ミップマップを生成する */
	if (pTexture != NULL && pTexture->Mipmap == 1) {
		gsTextureGenerateMipmap(pTexture);
	}
	return pTexture;
}

/*-----------------------------------------------------------------------------
*
* Purpose : マテリアルの読み込み．
*
* Return  : なし．
*
*----------------------------------------------------------------------------*/
static void
LoadMaterialBin
(
	GSmaterial*	pMaterial,				/* マテリアル		*/
	GFILE		file					/* ファイル			*/
)
{
	/* マテリアル情報フラグの読み込み */
	GFileReadDwordBin(file, &pMaterial->MaterialInfo);

	/* 拡散光カラーの読み込み */
	GFileReadColorBin(file, &pMaterial->Diffuse);

	/* 環境光カラーの読み込み */
	GFileReadColorBin(file, &pMaterial->Ambient);

	/* 鏡面反射光カラーの読み込み */
	GFileReadColorBin(file, &pMaterial->Specular);

	/* 放射光カラーの読み込み */
	GFileReadColorBin(file, &pMaterial->Emissive);

	/* ハイライトの鮮明度の読み込み */
	GFileReadFloatBin(file, &pMaterial->Power);
	pMaterial->Power = MAX(pMaterial->Power, 1.0f);

	/* テクスチャの読み込み */
	pMaterial->pTexture = NULL;
	if ((pMaterial->MaterialInfo & GS_MATERIAL_TEXTURE) != 0)
	{
		pMaterial->pTexture = LoadTextureBin(file);
	}
	/* 法線マップの読み込み	*/
	pMaterial->pNormalTexture = NULL;
	if ((pMaterial->MaterialInfo & GS_MATERIAL_NORMAL_TEXTURE) != 0)
	{
		pMaterial->pNormalTexture = LoadTextureBin(file);
	}
	/* 鏡面反射マップの読み込み	*/
	pMaterial->pSpecularTexture = NULL;
	if ((pMaterial->MaterialInfo & GS_MATERIAL_SPECULAR_TEXTURE) != 0)
	{
		pMaterial->pSpecularTexture = LoadTextureBin(file);
	}
	/* アンビエントオクルージョンマップの読み込み	*/
	pMaterial->pAmbientTexture = NULL;
	if ((pMaterial->MaterialInfo & GS_MATERIAL_AMBIENT_TEXTURE) != 0)
	{
		pMaterial->pAmbientTexture = LoadTextureBin(file);
	}
	/* 自己照明マップの読み込み			*/
	pMaterial->pEmissiveTexture = NULL;
	if ((pMaterial->MaterialInfo & GS_MATERIAL_EMISSIVE_TEXTURE) != 0)
	{
		pMaterial->pEmissiveTexture = LoadTextureBin(file);
	}
	/* 高さマップの読み込み */
	pMaterial->pHeightTexture = NULL;
	if ((pMaterial->MaterialInfo & GS_MATERIAL_HEIGHT_TEXTURE) != 0)
	{
		pMaterial->pHeightTexture = LoadTextureBin(file);
	}
	/* 第２テクスチャマップ				*/
	pMaterial->pTexture2 = NULL;
	if ((pMaterial->MaterialInfo & GS_MATERIAL_TEXTURE2) != 0)
	{
		pMaterial->pTexture2 = LoadTextureBin(file);
	}
	/* 第２法線マップ			*/
	pMaterial->pNormalTexture2 = NULL;
	if ((pMaterial->MaterialInfo & GS_MATERIAL_NORMAL_TEXTURE2) != 0)
	{
		pMaterial->pNormalTexture2 = LoadTextureBin(file);
	}
	/* マスクテクスチャの読み込み			*/
	pMaterial->pMaskTexture = NULL;
	if ((pMaterial->MaterialInfo & GS_MATERIAL_MASK_TEXTURE) != 0)
	{
		pMaterial->pMaskTexture = LoadTextureBin(file);
	}
	/* キューブ環境マップの読み込み	*/
	pMaterial->pCubeTexture = NULL;
	if ((pMaterial->MaterialInfo & GS_MATERIAL_CUBE_TEXTURE) != 0)
	{
		pMaterial->pCubeTexture = LoadTextureBin(file);
	}
}

/*-----------------------------------------------------------------------------
*
* Purpose : メッシュの平面パラメータの計算をする．
*
* Return  : なし．
*
*----------------------------------------------------------------------------*/
static void
CalculatePlanes
(
	GSmesh*			pMesh				/* メッシュ					*/
)
{
	GSuint			i;

	/* 既にプレーンが生成されている場合は消滅させる */
	if (pMesh->pPlanes != NULL)
	{
		del_object(pMesh->pPlanes);
	}

	/* 平面パラメータの配列を生成する */
	pMesh->pPlanes = new_object(sizeof(GSplane) * (pMesh->numIndices / 3));

	/* 全ポリゴンとレイの衝突判定を行う */
	for (i = 0; i < (pMesh->numIndices / 3); i++)
	{
		/* ポリゴンの３頂点を取得する */
		gsPlaneFromPoints(
			&pMesh->pPlanes[i],
			&pMesh->pPoints[pMesh->pIndices[i * 3 + 0]],
			&pMesh->pPoints[pMesh->pIndices[i * 3 + 1]],
			&pMesh->pPoints[pMesh->pIndices[i * 3 + 2]]
		);
	}
}

/*=============================================================================
*
* Purpose : メッシュの描画．
*
* Return  : なし．
*
=============================================================================*/
extern void
gsMeshDrawEx
(
	GSmesh*			pMesh				/* メッシュ構造体		*/
)
{
	const GLushort* base = 0;
	GSuint			i;

	/* レンダリングモードの退避 */
	glPushAttrib(GL_TEXTURE_BIT);

	/* バッファオブジェクトの設定 */
	glBindVertexArray(pMesh->Buffer.ArrayBuffer);
	for (i = 0; i < pMesh->numFaceGroups; ++i)
	{
		/* マテリアル情報の設定をする */
		SetMaterialEx(&pMesh->pMaterials[pMesh->pFaceGroups[i].MaterialNo]);
		/* 三角形を描画する */
		glDrawElements
		(
			GL_TRIANGLES,
			pMesh->pFaceGroups[i].numIndices,
			GL_UNSIGNED_INT,
			&base[pMesh->pFaceGroups[i].StartIndices + pMesh->Buffer.IndicesOffset]
		);
	}
	glBindVertexArray(0);

	/* レンダリングモードの復帰 */
	glPopAttrib();
}

/*-----------------------------------------------------------------------------
*
* Purpose : マテリアルの設定．
*
* Return  : なし．
*
*----------------------------------------------------------------------------*/
static void
SetMaterialEx
(
	GSmaterial*		pMaterial			/* マテリアルデータ	*/
)
{
	/* マテリアル情報の設定 */
	gsSetShaderParam4f("gs_MaterialAmbient", &pMaterial->Ambient);
	gsSetShaderParam4f("gs_MaterialDiffuse", &pMaterial->Diffuse);
	gsSetShaderParam4f("gs_MaterialSpecular", &pMaterial->Specular);
	gsSetShaderParam4f("gs_MaterialEmission", &pMaterial->Emissive);
	gsSetShaderParam1f("gs_MaterialShininess", pMaterial->Power);
	/* テクスチャマッピングの設定 */
	if (pMaterial->pTexture != NULL)
	{
		glActiveTexture(GL_TEXTURE0);
		gsTextureBind(pMaterial->pTexture);
		/* テクスチャをクランプするか？*/
		if ((pMaterial->MaterialInfo & GS_MATERIAL_TEX_CLAMP) != 0)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		}
	}
	/* 法線マップの設定 */
	if (pMaterial->pNormalTexture != NULL)
	{
		glActiveTexture(GL_TEXTURE1);
		gsTextureBind(pMaterial->pNormalTexture);
	}
	/* 鏡面反射テクスチャの設定 */
	if (pMaterial->pSpecularTexture != NULL)
	{
		glActiveTexture(GL_TEXTURE2);
		gsTextureBind(pMaterial->pSpecularTexture);
	}
	/* アンビエントオクルージョンテクスチャの設定 */
	if (pMaterial->pAmbientTexture != NULL)
	{
		glActiveTexture(GL_TEXTURE3);
		gsTextureBind(pMaterial->pAmbientTexture);
	}
	/* 自己照明マップの設定 */
	if (pMaterial->pEmissiveTexture != NULL)
	{
		glActiveTexture(GL_TEXTURE4);
		gsTextureBind(pMaterial->pEmissiveTexture);
	}
	/* 高さマップの設定 */
	if (pMaterial->pHeightTexture != NULL)
	{
		glActiveTexture(GL_TEXTURE5);
		gsTextureBind(pMaterial->pHeightTexture);
	}
	/* 第２テクスチャマップの設定				*/
	if (pMaterial->pTexture2 != NULL)
	{
		glActiveTexture(GL_TEXTURE6);
		gsTextureBind(pMaterial->pTexture2);
	}
	/* 第２法線マップの設定				*/
	if (pMaterial->pNormalTexture2 != NULL)
	{
		glActiveTexture(GL_TEXTURE7);
		gsTextureBind(pMaterial->pNormalTexture2);
	}
	/* マスクテクスチャの設定 */
	if (pMaterial->pMaskTexture != NULL)
	{
		glActiveTexture(GL_TEXTURE8);
		gsTextureBind(pMaterial->pMaskTexture);
	}
	/* キューブ環境マップテクスチャの設定 */
	if (pMaterial->pCubeTexture != NULL)
	{
		glActiveTexture(GL_TEXTURE9);
		gsTextureBind(pMaterial->pCubeTexture);
	}
	glActiveTexture(GL_TEXTURE0);
}

/*-----------------------------------------------------------------------------
*
* Purpose : バッファオブジェクトの作成．
*
* Return  : なし．
*
*----------------------------------------------------------------------------*/
static void
CreateMeshBuffer
(
	GSmesh*			pMesh				/* メッシュ構造体		*/
)
{
	GLint	offset_points          = -1;
	GLint	offset_weights         = -1;
	GLint	offset_normals         = -1;
	GLint	offset_diffuse_colors  = -1;
	GLint	offset_specular_colors = -1;
	GLint	offset_blend_indices   = -1;
	GLint	offset_texcoord0       = -1;
	GLint	offset_texcoord1       = -1;
	GLint	offset_tangents        = -1;
	GLint	offset_binormals       = -1;
	GLuint	vertex_buffer_size     = 0;
	unsigned char* base = 0;
	/* 頂点バッファのオフセットとサイズを計算する */
	if (pMesh->pPoints != 0)
	{
		offset_points = vertex_buffer_size;
		vertex_buffer_size += pMesh->numVertices * sizeof(GSvector3);
	}
	if (pMesh->pWeights != 0)
	{
		offset_weights = vertex_buffer_size;
		vertex_buffer_size += pMesh->numVertices * sizeof(GSblendweight);
	}
	if (pMesh->pNormals != NULL)
	{
		offset_normals = vertex_buffer_size;
		vertex_buffer_size += pMesh->numVertices * sizeof(GSvector3);
	}
	if (pMesh->pDiffuseColors != NULL)
	{
		offset_diffuse_colors = vertex_buffer_size;
		vertex_buffer_size += pMesh->numVertices * sizeof(GScolor);
	}
	if (pMesh->pSpecularColors != NULL)
	{
		offset_specular_colors = vertex_buffer_size;
		vertex_buffer_size += pMesh->numVertices * sizeof(GScolor);
	}
	if (pMesh->pBlendIndices != NULL)
	{
		offset_blend_indices = vertex_buffer_size;
		vertex_buffer_size += pMesh->numVertices * sizeof(GSblendindices);
	}
	if (pMesh->numTexCoords >= 1 && pMesh->ppTexCoords[0] != NULL)
	{
		offset_texcoord0 = vertex_buffer_size;
		vertex_buffer_size += pMesh->numVertices * sizeof(GSvector2);
	}
	if (pMesh->numTexCoords >= 2 && pMesh->ppTexCoords[1] != NULL)
	{
		offset_texcoord1 = vertex_buffer_size;
		vertex_buffer_size += pMesh->numVertices * sizeof(GSvector2);
	}
	if (pMesh->pTangents != NULL)
	{
		offset_tangents = vertex_buffer_size;
		vertex_buffer_size += pMesh->numVertices * sizeof(GSvector3);
	}
	if (pMesh->pBinormals != NULL)
	{
		offset_binormals = vertex_buffer_size;
		vertex_buffer_size += pMesh->numVertices * sizeof(GSvector3);
	}
	/* 頂点配列オブジェクトの作成 */
	glGenVertexArrays(1, &pMesh->Buffer.ArrayBuffer);
	glBindVertexArray(pMesh->Buffer.ArrayBuffer);
	/* インデックスバッファの作成 */
	glGenBuffers(1, &pMesh->Buffer.IndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pMesh->Buffer.IndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, pMesh->numIndices * sizeof(GLuint), pMesh->pIndices, GL_STATIC_DRAW);
	/* 頂点バッファの作成 */
	glGenBuffers(1, &pMesh->Buffer.VertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, pMesh->Buffer.VertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_size, 0, GL_STATIC_DRAW);
	if (offset_points != -1)
	{
		glBufferSubData(GL_ARRAY_BUFFER, offset_points, pMesh->numVertices * sizeof(GSvector3), pMesh->pPoints);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, &base[offset_points]);
	}
	if (offset_weights != -1)
	{
		glBufferSubData(GL_ARRAY_BUFFER, offset_weights, pMesh->numVertices * sizeof(GSblendweight), pMesh->pWeights);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, &base[offset_weights]);
	}
	if (offset_normals != -1)
	{
		glBufferSubData(GL_ARRAY_BUFFER, offset_normals, pMesh->numVertices * sizeof(GSvector3), pMesh->pNormals);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, &base[offset_normals]);
	}
	if (offset_diffuse_colors != -1)
	{
		glBufferSubData(GL_ARRAY_BUFFER, offset_diffuse_colors, pMesh->numVertices * sizeof(GScolor), pMesh->pDiffuseColors);
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 0, &base[offset_diffuse_colors]);
	}
	if (offset_specular_colors != -1)
	{
		glBufferSubData(GL_ARRAY_BUFFER, offset_specular_colors, pMesh->numVertices * sizeof(GScolor), pMesh->pSpecularColors);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 0, &base[offset_specular_colors]);
	}
	if (offset_blend_indices != -1)
	{
		glBufferSubData(GL_ARRAY_BUFFER, offset_blend_indices, pMesh->numVertices * sizeof(GSblendindices), pMesh->pBlendIndices);
		glEnableVertexAttribArray(7);
		glVertexAttribPointer(7, 4, GL_UNSIGNED_BYTE, GL_FALSE, 0, &base[offset_blend_indices]);
	}
	if (offset_texcoord0 != -1)
	{
		glBufferSubData(GL_ARRAY_BUFFER, offset_texcoord0, pMesh->numVertices * sizeof(GSvector2), pMesh->ppTexCoords[0]);
		glEnableVertexAttribArray(8);
		glVertexAttribPointer(8, 2, GL_FLOAT, GL_FALSE, 0, &base[offset_texcoord0]);
	}
	if (offset_texcoord1 != -1)
	{
		glBufferSubData(GL_ARRAY_BUFFER, offset_texcoord1, pMesh->numVertices  * sizeof(GSvector2), pMesh->ppTexCoords[1]);
		glEnableVertexAttribArray(9);
		glVertexAttribPointer(8, 2, GL_FLOAT, GL_FALSE, 0, &base[offset_texcoord1]);
	}
	if (offset_tangents != -1)
	{
		glBufferSubData(GL_ARRAY_BUFFER, offset_tangents, pMesh->numVertices * sizeof(GSvector3), pMesh->pTangents);
		glEnableVertexAttribArray(14);
		glVertexAttribPointer(14, 3, GL_FLOAT, GL_FALSE, 0, &base[offset_tangents]);
	}
	if (offset_binormals != -1)
	{
		glBufferSubData(GL_ARRAY_BUFFER, offset_binormals, pMesh->numVertices * sizeof(GSvector3), pMesh->pBinormals);
		glEnableVertexAttribArray(15);
		glVertexAttribPointer(15, 3, GL_FLOAT, GL_FALSE, 0, &base[offset_binormals]);
	}
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

/*-----------------------------------------------------------------------------
*
* Purpose : 頂点ブレンド用行列の計算．
*
* Return  : なし．
*
*----------------------------------------------------------------------------*/
static void
TransformVertices
(
	GSvector3*				pOutPoints,		/*-> 頂点データ	            */
	GSvector3*				pOutNormals,	/*-> 計算後の法線ベクトル	*/
	GSvector3*				pOutTangents,	/*-> 計算後の接ベクトル	    */
	GSvector3*				pOutBinormals,	/*-> 計算後の従法線ベクトル */
	GSuint					numVertices,	/* 頂点データ数				*/
	const GSvector3*		pPoints,		/* 頂点データ	            */
	const GSvector3*		pNormals,		/* 法線ベクトル	            */
	const GSvector3*		pTangents,		/* 接ベクトル	            */
	const GSvector3*		pBinromals,		/* 従法線ベクトル	        */
	GSuint					numWeights,		/* ウェイト数				*/
	const GSblendweight*	pWeights,		/* ボーンウェイト配列		*/
	const GSblendindices*	pBoneIndices,	/* ボーンインデックス配列	*/
	const GSmatrix4*		pBoneMetrices	/* ボーンの変換行列			*/
)
{
	GSuint i;
	for (i = 0; i < numVertices; ++i) {
		GSmatrix4 boneMatrix = { 0 };
		GSuint j;
		for (j = 0; j < numWeights; ++j) {
			const GSfloat(*s)[4] = pBoneMetrices[pBoneIndices[i].Indices[j]].m;
			GSfloat(*d)[4] = boneMatrix.m;
			GSfloat w = pWeights[i].Weights[j];
			d[0][0] += s[0][0] * w; d[0][1] += s[0][1] * w; d[0][2] += s[0][2] * w;
			d[1][0] += s[1][0] * w; d[1][1] += s[1][1] * w; d[1][2] += s[1][2] * w;
			d[2][0] += s[2][0] * w; d[2][1] += s[2][1] * w; d[2][2] += s[2][2] * w;
			d[3][0] += s[3][0] * w; d[3][1] += s[3][1] * w; d[3][2] += s[3][2] * w;
		}
		gsVector3Transform(&pOutPoints[i], &pPoints[i], &boneMatrix);
		if (pNormals != NULL) {
			gsVector3TransformNormal(&pOutNormals[i], &pNormals[i], &boneMatrix);
		}
		if (pTangents != NULL) {
			gsVector3TransformNormal(&pOutTangents[i], &pTangents[i], &boneMatrix);
		}
		if (pBinromals != NULL) {
			gsVector3TransformNormal(&pOutBinormals[i], &pBinromals[i], &boneMatrix);
		}
	}
}
/********** End of File ******************************************************/
