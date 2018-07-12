/******************************************************************************
* File Name : GSgraphics.c                       Ver : 1.00  Date : 2002-07-31
*
* Description :
*
*       ゲームシステム用 グラフィックス処理．
*
* Author : Moriharu Ohzu.
*
******************************************************************************/
#include	"GSgraphics.h"
#include	"GSvector3.h"
#include	"GSmatrix4.h"
#include	"GSquaternion.h"
#include	"GScolor.h"
#include    "GSload_texture_file.h"
#include	"GSshader.h"
#include	"GSrender_target.h"
#include    "GSuniform_block.h"
#include	<stddef.h>
#include	<GL/glu.h>

/****** 定数宣言 *************************************************************/

#define	GS_MESH_MAX			 1024	/* メッシュデータの最大数		*/
#define	GS_SKELETON_MAX		 1024	/* スケルトンデータの最大数		*/
#define	GS_ANIMATION_MAX	 1024	/* アニメーションデータの最大数	*/
#define	GS_TEXTURE_MAX		 1024	/* テクスチャデータの最大数		*/
#define	GS_OCTREE_MAX		 1024	/* オクツリーデータの最大数		*/
#define	GS_SHADER_MAX		 1024	/* シェーダーデータの最大数		*/
#define	GS_RENDER_TARGET_MAX 1024	/* レンダーターゲットの最大数   */
#define	GS_UNIFORM_BLOCK_MAX 1024	/* UniformBlockの最大数         */

#define	GS_MATRIX_MAX		256		/* 変換行列の最大数				*/

/****** 関数プロトタイプ宣言 *************************************************/

void initialize_opengl_ext(void);	/* OpenGL拡張の初期化 */

/****** 変数宣言 *************************************************************/

/* メッシュ，スケルトン，アニメーション,テクスチャデータ */
static GSmesh*		    s_pMeshs[ GS_MESH_MAX ];
static GSskeleton*	    s_pSkeletons[ GS_SKELETON_MAX ];
static GSanimation*	    s_pAnimations[ GS_ANIMATION_MAX ];
static GStexture*	    s_pTextures[ GS_TEXTURE_MAX ];
static GSoctree*	    s_pOctrees[ GS_OCTREE_MAX ];
static GSshaderProgram*	s_pShaders[ GS_SHADER_MAX ];
static GSrendertarget*  s_pRenderTargets[ GS_RENDER_TARGET_MAX ];
static GSunifomBlock*   s_pUniformBlocks[ GS_UNIFORM_BLOCK_MAX ];

/* セルシェーディング用パラメータ */
static GSvector3	s_CelShadeLightPosition = { 1.0f, 1.0f, 1.0f };
static GSfloat		s_CelShadeLineWidth     = 1.5f;
static GSuint		s_CelShadeTexture       = 0;
static GScolor		s_CelShadeLineColor		= { 0.0f, 0.0f, 0.0f, 1.0f };

/* テクスチャのカラーキーのモード */
static GSenum		s_TexColorKeyMode = GS_TEXCOLOR_KEY_DISABLE;
static GSbyte		s_TexColorKeyR = 0;
static GSbyte		s_TexColorKeyG = 0;
static GSbyte		s_TexColorKeyB = 0;

/* バインド中のスケルトンナンバ */
static GSuint		s_BindSkeleton = 0;

/* アニメーション変換行列 */
static GSmatrix4	s_AnimationMatrices[ GS_MATRIX_MAX ];

/* 姿勢変換行列 */
static GSmatrix4	s_SkeletonMatrices[ GS_MATRIX_MAX ];

/* 座標変換行列 */
static GSmatrix4	s_TransformMatrices[ GS_MATRIX_MAX ];

/* 各種状態フラグ */
static GSboolean	s_bSkinMesh          = GS_TRUE;
static GSboolean	s_bCalculateSkeleton = GS_TRUE;
static GSboolean	s_bCelShade          = GS_FALSE;

/* 文字フォント情報 */
static GSbitfield	s_FontCode     = GS_FONT_NORMAL;
static GSuint		s_FontPoint    = 10;
static char			s_FontName[32] = "ＭＳ ゴシック";

/* テキストの表示位置 */
static GSvector2	s_TextPosition = { 0.0f, 0.0f };

/* 実行中のシェーダー番号 */
static GSuint		s_ExecuteShader = 0;

/* 退避中のビューポートサイズ */
static int s_ViewPort[4];

/*=============================================================================
*
* Purpose : グラフィックシステムの初期化．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsInitGraphics
(
	void
)
{
	GSuint		i;

	/* OpenGL拡張の初期化*/
	initialize_opengl_ext();

	/* 各種リソースの初期化 */
	for ( i = 0; i < GS_MESH_MAX; i++ )
	{
		gsDeleteMesh( i );
	}
	for ( i = 0; i < GS_SKELETON_MAX; i++ )
	{
		gsDeleteSkeleton( i );
	}
	for ( i = 0; i < GS_ANIMATION_MAX; i++ )
	{
		gsDeleteAnimation( i );
	}
	for ( i = 0; i < GS_TEXTURE_MAX; i++ )
	{
		gsDeleteTexture( i );
	}
	for ( i = 0; i < GS_OCTREE_MAX; i++ )
	{
		gsDeleteOctree( i );
	}
	for (i = 0; i < GS_SHADER_MAX; i++)
	{
		gsDeleteShader(i);
	}
	for (i = 0; i < GS_RENDER_TARGET_MAX; i++)
	{
		gsDeleteRenderTarget(i);
	}

	/* セルシェードパラメータの初期化 */
	gsVector3Init( &s_CelShadeLightPosition, 1.0f, 1.0f, 1.0f );
	s_CelShadeLineWidth = 1.5f;
	s_CelShadeTexture   = 0;
	gsColorInit( &s_CelShadeLineColor, 0.0f, 0.0f, 0.0f, 1.0f );

	/* テクスチャのカラーキーの初期化 */
	s_TexColorKeyMode = GS_TEXCOLOR_KEY_DISABLE;
	s_TexColorKeyR    = 0;
	s_TexColorKeyG    = 0;
	s_TexColorKeyB    = 0;

	/* バインド中のスケルトンの初期化 */
	s_BindSkeleton = 0;

	/* アニメーション変換行列の初期化 */
	for ( i = 0; i < GS_MATRIX_MAX; i++ )
	{
		gsMatrix4Identity( &s_AnimationMatrices[i] );
	}

	/* 姿勢変換行列の初期化 */
	for ( i = 0; i < GS_MATRIX_MAX; i++ )
	{
		gsMatrix4Identity( &s_SkeletonMatrices[i] );
	}

	/* 座標変換行列の初期化 */
	for ( i = 0; i < GS_MATRIX_MAX; i++ )
	{
		gsMatrix4Identity( &s_TransformMatrices[i] );
	}

	/* 文字フォント情報の初期化 */
	s_FontCode  = GS_FONT_NORMAL;
	s_FontPoint = 10;
	strcpy( s_FontName, "ＭＳ ゴシック" );

	/* テキストの表示位置を初期化 */
	s_TextPosition.x = 0.0f;
	s_TextPosition.y = 0.0f;

	/* 各種状態フラグの初期化 */
	s_bSkinMesh          = GS_TRUE;
	s_bCalculateSkeleton = GS_TRUE;
	s_bCelShade          = GS_FALSE;
}

/*=============================================================================
*
* Purpose : グラフィックシステムの終了処理．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsFinishGraphics
(
	void
)
{
	GSuint		i;

	/* 各種リソースの解放を行う */
	for ( i = 0; i < GS_MESH_MAX; i++ )
	{
		gsDeleteMesh( i );
	}
	for ( i = 0; i < GS_SKELETON_MAX; i++ )
	{
		gsDeleteSkeleton( i );
	}
	for ( i = 0; i < GS_ANIMATION_MAX; i++ )
	{
		gsDeleteAnimation( i );
	}
	for ( i = 0; i < GS_TEXTURE_MAX; i++ )
	{
		gsDeleteTexture( i );
	}
	for ( i = 0; i < GS_OCTREE_MAX; i++ )
	{
		gsDeleteOctree( i );
	}
	for (i = 0; i < GS_SHADER_MAX; i++)
	{
		gsDeleteShader(i);
	}
	for (i = 0; i < GS_RENDER_TARGET_MAX; i++)
	{
		gsDeleteRenderTarget(i);
	}
	for (i = 0; i < GS_UNIFORM_BLOCK_MAX; i++)
	{
		gsDeleteUniformBlock(i);
	}
}

/*=============================================================================
*
* Purpose : メッシュデータの読み込み．
*
* Return  : 読み込みに成功すれば真，失敗すれば偽を返す．
*
*============================================================================*/
extern GSboolean
gsLoadMesh
(
	GSuint			MeshID,				/* メッシュＩＤ				*/
	const char*		pszMshFileName		/* メッシュファイル名		*/
)
{
	GSboolean		Result;

	/* メッシュデータが既に存在するか？ */
	if ( s_pMeshs[ MeshID ] == NULL )
	{
		/* メッシュデータの生成 */
		s_pMeshs[ MeshID ] = gsMeshNew();
	}

	/* メッシュデータの読み込み */
	Result = gsMeshLoadFile( s_pMeshs[ MeshID ], pszMshFileName );

	/* 読み込みに失敗した場合は生成したメッシュデータを削除する */
	if ( Result == GS_FALSE )
	{
		gsDeleteMesh( MeshID );
	}

	return	Result;
}

/*=============================================================================
*
* Purpose : メッシュデータの読み込み．（シェーダー専用）
*
* Return  : 読み込みに成功すれば真，失敗すれば偽を返す．
*
*============================================================================*/
extern GSboolean
gsLoadMeshEx
(
	GSuint			MeshID,				/* メッシュＩＤ				*/
	const char*		pszMshFileName		/* メッシュファイル名		*/
)
{
	GSboolean		Result;

	/* メッシュデータが既に存在するか？ */
	if (s_pMeshs[MeshID] == NULL)
	{
		/* メッシュデータの生成 */
		s_pMeshs[MeshID] = gsMeshNew();
	}

	/* メッシュデータの読み込み */
	Result = gsMeshLoadFileEx(s_pMeshs[MeshID], pszMshFileName);

	/* 読み込みに失敗した場合は生成したメッシュデータを削除する */
	if (Result == GS_FALSE)
	{
		gsDeleteMesh(MeshID);
	}

	return	Result;
}

/*=============================================================================
*
* Purpose : スケルトンデータファイルの読み込み．
*
* Return  : 読み込みに成功すれば真，失敗すれば偽を返す．
*
*============================================================================*/
extern GSboolean
gsLoadSkeleton
(
	GSuint			SkeletonID,			/* スケルトンＩＤ			*/
	const char*		pszSklFileName		/* スケルトンファイル名		*/
)
{
	GSboolean		Result;

	/* スケルトンデータが既に存在するか？ */
	if ( s_pSkeletons[ SkeletonID ] == NULL )
	{
		/* スケルトンデータの生成 */
		s_pSkeletons[ SkeletonID ] = gsSkeletonNew();
	}

	/* スケルトンデータの読み込み */
	Result = gsSkeletonLoadFile( s_pSkeletons[ SkeletonID ], pszSklFileName );

	/* 読み込みに失敗した場合は生成したスケルトンデータを削除する */
	if ( Result == GS_FALSE )
	{
		gsDeleteSkeleton( SkeletonID );
	}

	return	Result;
}

/*=============================================================================
*
* Purpose : アニメーションデータファイルの読み込み．
*
* Return  : 読み込みに成功すれば真，失敗すれば偽を返す．
*
*============================================================================*/
extern GSboolean
gsLoadAnimation
(
	GSuint			AnimationID,		/* アニメーションＩＤ		*/
	const char*		pszAnmFileName		/* アニメーションファイル名	*/
)
{
	GSboolean		Result;

	/* アニメーションデータが既に存在するか？ */
	if ( s_pAnimations[ AnimationID ] == NULL )
	{
		/* アニメーションデータの生成 */
		s_pAnimations[ AnimationID ] = gsAnimationNew();
	}

	/* アニメーションデータの読み込み */
	Result = gsAnimationLoadFile( s_pAnimations[ AnimationID ], pszAnmFileName );

	/* 読み込みに失敗した場合は生成したアニメーションデータを削除する */
	if ( Result == GS_FALSE )
	{
		gsDeleteAnimation( AnimationID );
	}

	return	Result;
}

/*=============================================================================
*
* Purpose : テクスチャデータの読み込み．
*
* Return  : 読み込みに成功すれば真，失敗すれば偽を返す．
*
*============================================================================*/
extern GSboolean
gsLoadTexture
(
	GSuint			TextureID,			/* テクスチャＩＤ			*/
	const char*		pszTexFileName		/* テクスチャファイル名		*/
)
{
	GStexture*		pTexture;

	/* テクスチャデータの読み込み */
	pTexture = gsLoadTextureFile(pszTexFileName);
	if (pTexture == NULL) {
		return GS_FALSE;
	}

	/* 既にテクスチャがあった場合は削除  */
	if (s_pTextures[TextureID] != NULL)
	{
		gsDeleteTexture(TextureID);
	}
	/* テクスチャデータの更新 */
	s_pTextures[TextureID] = pTexture;

	return GS_TRUE;
}

/*=============================================================================
*
* Purpose : テクスチャデータの読み込み．
*
* Return  : 読み込みに成功すれば真，失敗すれば偽を返す．
*
*============================================================================*/
extern GSboolean
gsLoadTextureEx
(
	GSuint			TextureID,			/* テクスチャＩＤ			*/
	const char*		pszTexFileName,		/* テクスチャファイル名		*/
	GSenum			Wrap,				/* ラップモード				*/
	GSenum			Filter,				/* フィルタ					*/
	GSboolean		GenMipmap			/* ミップマップ自動生成		*/
) 
{
	GStexture*		pTexture;

	/* テクスチャファイルの読み込み */
	if (gsLoadTexture(TextureID, pszTexFileName) == GS_FALSE) {
		return GS_FALSE;
	}
	pTexture = s_pTextures[TextureID];

	glPushAttrib(GL_ENABLE_BIT);
	/* ミップマップの自動生成 */
	if (GenMipmap == GS_TRUE) {
		gsTextureGenerateMipmap(pTexture);
	}
	gsBindTexture(TextureID);
	/* ラップモードの設定 */
	if (Wrap == GS_TEX_CLAMP) {
		glTexParameteri(pTexture->Type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(pTexture->Type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(pTexture->Type, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}
	/* テクスチャフィルタの設定 */
	if (Filter == GS_TEX_NEAREST) {
		if (pTexture->Mipmap <= 1) {
			glTexParameteri(pTexture->Type, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(pTexture->Type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		} else {
			glTexParameteri(pTexture->Type, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
			glTexParameteri(pTexture->Type, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		}
	} else if (Filter == GS_TEX_LINEAR) {
		if (pTexture->Mipmap == 1) {
			glTexParameteri(pTexture->Type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(pTexture->Type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		} else {
			glTexParameteri(pTexture->Type, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(pTexture->Type, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		}
	}
	glBindTexture(pTexture->Type, 0);
	glPopAttrib();
	return GS_TRUE;
}

/*=============================================================================
*
* Purpose : オクツリーデータの読み込み．
*
* Return  : 読み込みに成功すれば真，失敗すれば偽を返す．
*
*============================================================================*/
extern GSboolean
gsLoadOctree
(
	GSuint			OctreeID,			/* オクツリーＩＤ			*/
	const char*		pszOctFileName		/* オクツリーファイル名		*/
)
{
	GSboolean		Result;

	/* オクツリーデータが既に存在するか？ */
	if ( s_pOctrees[ OctreeID ] == NULL )
	{
		/* オクツリーデータの生成 */
		s_pOctrees[ OctreeID ] = gsOctreeNew();
	}

	/* オクツリーデータの読み込み */
	Result = gsOctreeLoadFile( s_pOctrees[ OctreeID ], pszOctFileName );

	/* 読み込みに失敗した場合は生成したオクツリーデータを削除する */
	if ( Result == GS_FALSE )
	{
		gsDeleteOctree( OctreeID );
	}

	return	Result;
}

/*=============================================================================
*
* Purpose : オクツリーデータの読み込み．(シェーダー対応）
*
* Return  : 読み込みに成功すれば真，失敗すれば偽を返す．
*
*============================================================================*/
extern GSboolean
gsLoadOctreeEx
(
	GSuint			OctreeID,			/* オクツリーＩＤ			*/
	const char*		pszOctFileName		/* オクツリーファイル名		*/
)
{
	GSboolean		Result;

	/* オクツリーデータが既に存在するか？ */
	if (s_pOctrees[OctreeID] == NULL)
	{
		/* オクツリーデータの生成 */
		s_pOctrees[OctreeID] = gsOctreeNew();
	}

	/* オクツリーデータの読み込み */
	Result = gsOctreeLoadFileEx(s_pOctrees[OctreeID], pszOctFileName);

	/* 読み込みに失敗した場合は生成したオクツリーデータを削除する */
	if (Result == GS_FALSE)
	{
		gsDeleteOctree(OctreeID);
	}

	return	Result;
}

/*=============================================================================
*
* Purpose : テクスチャデータの透明色の指定．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsTextureColorKey
(
	GSubyte			ColorKeyR,			/* 赤成分					*/
	GSubyte			ColorKeyG,			/* 青成分					*/
	GSubyte			ColorKeyB			/* 緑成分					*/
)
{
	/* 透明色の設定をする */
	s_TexColorKeyR = ColorKeyR;
	s_TexColorKeyG = ColorKeyG;
	s_TexColorKeyB = ColorKeyB;
}

/*=============================================================================
*
* Purpose : テクスチャデータの透明色モードの指定．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsTextureColorKeyMode
(
	GSenum			ColorKeyMode		/* カラーキーモード			*/
)
{
	/* 透明色の選択方法の設定 */
	s_TexColorKeyMode = ColorKeyMode;
}

/*=============================================================================
*
* Purpose : メッシュデータの削除．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsDeleteMesh
(
	GSuint			MeshID				/* メッシュＩＤ				*/
)
{
	if ( s_pMeshs[ MeshID ] != NULL )
	{
		/* メッシュデータを消滅させる */
		gsMeshDelete( s_pMeshs[ MeshID ] );

		s_pMeshs[ MeshID ] = NULL;
	}
}

/*=============================================================================
*
* Purpose : スケルトンデータの削除．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsDeleteSkeleton
(
	GSuint			SkeletonID			/* スケルトンＩＤ			*/
)
{
	if ( s_pSkeletons[ SkeletonID ] != NULL )
	{
		/* スケルトンデータを消滅させる */
		gsSkeletonDelete( s_pSkeletons[ SkeletonID ] );

		s_pSkeletons[ SkeletonID ] = NULL;
	}
}

/*=============================================================================
*
* Purpose : アニメーションデータファイルの削除．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsDeleteAnimation
(
	GSuint			AnimationID			/* アニメーションＩＤ		*/
)
{
	if ( s_pAnimations[ AnimationID ] != NULL )
	{
		/* アニメーションデータを消滅させる */
		gsAnimationDelete( s_pAnimations[ AnimationID ] );

		s_pAnimations[ AnimationID ] = NULL;
	}
}

/*=============================================================================
*
* Purpose : テクスチャデータファイルの削除．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsDeleteTexture
(
	GSuint			TextureID			/* テクスチャＩＤ		*/
)
{
	if ( s_pTextures[ TextureID ] != NULL )
	{
		/* アニメーションデータを消滅させる */
		gsTextureDelete( s_pTextures[ TextureID ] );

		s_pTextures[ TextureID ] = NULL;
	}
}

/*=============================================================================
*
* Purpose : オクツリーデータの削除．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsDeleteOctree
(
	GSuint			OctreeID				/* オクツリーＩＤ		*/
)
{
	if ( s_pOctrees[ OctreeID ] != NULL )
	{
		/* オクツリーデータを消滅させる */
		gsOctreeDelete( s_pOctrees[ OctreeID ] );

		s_pOctrees[ OctreeID ] = NULL;
	}
}

/*=============================================================================
*
* Purpose : メッシュの描画．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsDrawMesh
(
	GSuint			MeshID				/* メッシュＩＤ				*/
)
{
	GSskeleton*		pSkeleton;
	GSmesh*			pMesh;

	/* メッシュデータを取得する */
	if ( ( pMesh = s_pMeshs[ MeshID ] ) == NULL )
	{
		return;
	}

	/* バインド中のスケルトンの取得 */
	pSkeleton = s_pSkeletons[ s_BindSkeleton ];

	/* スキニングメッシュを描画するか？ */
	if ( s_bSkinMesh == GS_TRUE && pSkeleton != NULL )
	{
		/* スケルトンの自動計算が設定されているか？ */
		if ( s_bCalculateSkeleton == GS_TRUE)
		{
			/* 姿勢変換行列の計算を行う */
			gsCalculateSkeleton( NULL, s_AnimationMatrices, s_SkeletonMatrices );
		}

		/* 最終的な座標変換行列を計算する */
		gsSkeletonCalculateTransform
		(
			pSkeleton,
			s_TransformMatrices,
			s_SkeletonMatrices
		);

		/* セルシェーディングを行うか？ */
		if ( s_bCelShade == GS_TRUE )
		{
			/* セルシェーディングを使って描画する */
			gsMeshDrawSkinCelShade
			(
				pMesh,
				s_TransformMatrices,
				s_pTextures[ s_CelShadeTexture ],
				&s_CelShadeLightPosition,
				&s_CelShadeLineColor,
				s_CelShadeLineWidth
			);
		}
		else
		{
			/* メッシュを描画する */
			gsMeshDrawSkin( pMesh, s_TransformMatrices );
		}
	}
	else
	{
		/* セルシェーディングを行うか？ */
		if ( s_bCelShade == GS_TRUE )
		{
			/* セルシェーディングを使ってメッシュを描画する */
			gsMeshDrawCelShade
			(
				pMesh,
				s_pTextures[ s_CelShadeTexture ],
				&s_CelShadeLightPosition,
				&s_CelShadeLineColor,
				s_CelShadeLineWidth
			);
		}
		else
		{
			/* メッシュを描画する */
			gsMeshDraw( pMesh );
		}
	}
}

/*=============================================================================
*
* Purpose : メッシュの描画．(OpenGL 3.0以降）
*
* Return  : なし．
*
*============================================================================*/
extern void
gsDrawMeshEx
(
	GSuint			MeshID				/* メッシュＩＤ				*/
)
{
	GSskeleton*		pSkeleton;
	GSmesh*			pMesh;

	/* メッシュデータを取得する */
	if ((pMesh = s_pMeshs[MeshID]) == NULL)
	{
		return;
	}
	/* バインド中のスケルトンの取得 */
	pSkeleton = s_pSkeletons[s_BindSkeleton];

	/* スキニングメッシュを描画するか？ */
	if (s_bSkinMesh == GS_TRUE && pSkeleton != NULL)
	{
		/* スケルトンの自動計算が設定されているか？ */
		if (s_bCalculateSkeleton == GS_TRUE)
		{
			/* 姿勢変換行列の計算を行う */
			gsCalculateSkeleton(NULL, s_AnimationMatrices, s_SkeletonMatrices);
		}
		/* 最終的な座標変換行列を計算する */
		gsSkeletonCalculateTransform
		(
			pSkeleton,
			s_TransformMatrices,
			s_SkeletonMatrices
		);
		/* ボーンの変換行列の設定 */
		gsSetShaderParamArrayMatrix4("gs_BoneMatrices", gsSkeletonGetNumBones(pSkeleton), s_TransformMatrices);
	}
	/* メッシュを描画する */
	gsMeshDrawEx(pMesh);
}

/*=============================================================================
*
* Purpose : オクツリーの描画．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsDrawOctree
(
	GSuint			OctreeID				/* オクツリーＩＤ				*/
)
{
	GSoctree*		pOctree;

	/* オクツリーデータを取得する */
	if ( ( pOctree = s_pOctrees[ OctreeID ] ) == NULL )
	{
		return;
	}

	/* オクツリーの描画 */
	gsOctreeDraw( pOctree );
}


/*=============================================================================
*
* Purpose : オクツリーの描画．(シェーダー対応）
*
* Return  : なし．
*
*============================================================================*/
extern void
gsDrawOctreeEx
(
	GSuint				OctreeID,			/* オクツリーＩＤ		*/
	const GSmatrix4*	pProjectionMatrix,	/* 透視変換行列			*/
	const GSmatrix4*	pViewMatix			/* 視野変換行列			*/
)
{
	GSoctree*		pOctree;

	/* オクツリーデータを取得する */
	if ((pOctree = s_pOctrees[OctreeID]) == NULL)
	{
		return;
	}

	/* オクツリーの描画 */
	gsOctreeDrawEx(pOctree, pProjectionMatrix, pViewMatix);
}

/*=============================================================================
*
* Purpose : テクスチャをバインドする．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsBindTexture
(
	GSuint			TextureID			/* テクスチャＩＤ			*/
)
{
	GStexture*		pTexture;

	if ( ( pTexture = s_pTextures[ TextureID ] ) == NULL )
	{
		return;
	}

	/* テクスチャをバインドする */
	gsTextureBind( s_pTextures[ TextureID ] );
}

/*=============================================================================
*
* Purpose : テクスチャをバインドする．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsBindTextureEx
(
	GSuint			TextureID,			/* テクスチャＩＤ			*/
	GSuint			Unit				/* テクスチャユニットの番号 */
)
{
	glActiveTexture(GL_TEXTURE0 + Unit);
	gsBindTexture(TextureID);
}

/*=============================================================================
*
* Purpose : テクスチャをアンバインドする．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsUnbindTexture
(
	GSuint			TextureID			/* テクスチャＩＤ			*/
)
{
	GStexture*		pTexture;

	if ((pTexture = s_pTextures[TextureID]) == NULL)
	{
		return;
	}
	gsTextureUnbind(s_pTextures[TextureID]);
}


/*=============================================================================
*
* Purpose : テクスチャをアンバインドする．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsUnbindTextureEx
(
	GSuint			TextureID,			/* テクスチャＩＤ			*/
	GSuint			UnitNo				/* テクスチャユニットの番号 */
)
{
	glActiveTexture(GL_TEXTURE0 + UnitNo);
	gsUnbindTexture(TextureID);
	glActiveTexture(GL_TEXTURE0);
}

/*=============================================================================
*
* Purpose : スケルトンをバインドする．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsBindSkeleton
(
	GSuint			SkeletonID			/* スケルトンＩＤ			*/
)
{
	/* スケルトンをバインドする */
	s_BindSkeleton = SkeletonID;
}

/*=============================================================================
*
* Purpose : アニメーションをバインドする．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsBindAnimation
(
	GSuint			AnimationID,		/* アニメーションＩＤ		*/
	GSuint			AnimationNo,		/* アニメーションナンバ		*/
	GSfloat			fTimer				/* アニメーションタイマ		*/
)
{
	gsCalculateAnimation(AnimationID, AnimationNo, fTimer, s_AnimationMatrices);
}

/*=============================================================================
*
* Purpose : アニメーションをバインドする．（ 線形補間 ）
*
* Return  : なし．
*
*============================================================================*/
extern void
gsBindAnimationLerp
(
	GSuint			AnimationID1,		/* アニメーションＩＤ		*/
	GSuint			AnimationNo1,		/* アニメーションナンバ		*/
	GSfloat			fTimer1,			/* アニメーションタイマ		*/
	GSuint			AnimationID2,		/* アニメーションＩＤ		*/
	GSuint			AnimationNo2,		/* アニメーションナンバ		*/
	GSfloat			fTimer2,			/* アニメーションタイマ		*/
	GSfloat			fLerp				/* 補間値					*/
)
{
	gsCalculateAnimationLerp(
		AnimationID1, AnimationNo1, fTimer1,
		AnimationID2, AnimationNo2, fTimer2,
		fLerp,
		s_AnimationMatrices
	);
}

/*=============================================================================
*
* Purpose : アニメーションをバインドする．（ 線形補間 ）
*
* Return  : なし．
*
*============================================================================*/
extern void
gsCalculateAnimationLerp
(
	GSuint			AnimationID1,		/* アニメーションＩＤ		*/
	GSuint			AnimationNo1,		/* アニメーションナンバ		*/
	GSfloat			fTimer1,			/* アニメーションタイマ		*/
	GSuint			AnimationID2,		/* アニメーションＩＤ		*/
	GSuint			AnimationNo2,		/* アニメーションナンバ		*/
	GSfloat			fTimer2,			/* アニメーションタイマ		*/
	GSfloat			fLerp,				/* 補間値					*/
	GSmatrix4*		pAnimationMatrices	/*-> アニメーション変換行列	*/
)
{
	GSanimation*	pAnimation1;
	GSanimation*	pAnimation2;
	GSuint			numBones;
	GSuint			BoneNo;
	GSvector3		Scale;
	GSquaternion	Rotate;
	GSvector3		Position;
	GSvector3		Scale1;
	GSquaternion	Rotate1;
	GSvector3		Position1;
	GSvector3		Scale2;
	GSquaternion	Rotate2;
	GSvector3		Position2;
	GSmatrix4		matScale;
	GSmatrix4		matRotate;

	/* ２つのアニメーションデータを取得する */
	if ( ( pAnimation1 = s_pAnimations[ AnimationID1 ] ) == NULL )
	{
		return;
	}
	if ( ( pAnimation2 = s_pAnimations[ AnimationID2 ] ) == NULL )
	{
		/* 補間先がない場合はアニメーション１のみバインド */
		gsBindAnimation( AnimationID1, AnimationNo1, fTimer1 );

		return;
	}

	/* ボーンデータ数を取得する */
	numBones = gsAnimationGetNumBones( pAnimation1, AnimationNo1 );

	/* アニメーション変換行列に設定する */
	for ( BoneNo = 0; BoneNo < numBones; BoneNo++ )
	{
		/* キーフレームデータを取得する */
		gsAnimationGetKeyFrame
		(
			pAnimation1,
			AnimationNo1, BoneNo, fTimer1,
			&Scale1, &Rotate1, &Position1
		);
		/* キーフレームデータを取得する */
		gsAnimationGetKeyFrame
		(
			pAnimation2,
			AnimationNo2, BoneNo, fTimer2,
			&Scale2, &Rotate2, &Position2
		);

		/* 各キーを線形補間する */
		gsVector3Lerp( &Scale, &Scale1, &Scale2, fLerp );
		gsQuaternionSlerp( &Rotate, &Rotate1, &Rotate2, fLerp );
		gsVector3Lerp( &Position, &Position1, &Position2, fLerp );

		/* 各キーを行列に変換する */
		gsMatrix4Scale(&matScale, Scale.x, Scale.y, Scale.z);
		gsMatrix4Quaternion(&matRotate, &Rotate);
		gsMatrix4Multiply
		(
			&pAnimationMatrices[BoneNo], &matScale, &matRotate
		);
		pAnimationMatrices[BoneNo].m[3][0] = Position.x;
		pAnimationMatrices[BoneNo].m[3][1] = Position.y;
		pAnimationMatrices[BoneNo].m[3][2] = Position.z;
	}
}

/*=============================================================================
*
* Purpose : セルシェーディング用テクスチャをバインドする．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsBindCelShadeTexture
(
	GSuint			TextureID			/* テクチャＩＤ				*/
)
{
	/* セルシェーディング用テクスチャのバインド */
	s_CelShadeTexture = TextureID;
}

/*=============================================================================
*
* Purpose : セルシェーディング用ライトの設定をする．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsLightPositionCelShade
(
	GSfloat			LightPositionX,		/* ライトｘ座標 			*/
	GSfloat			LightPositionY,		/* ライトｙ座標 			*/
	GSfloat			LightPositionZ		/* ライトｚ座標 			*/
)
{
	s_CelShadeLightPosition.x = LightPositionX;
	s_CelShadeLightPosition.y = LightPositionY;
	s_CelShadeLightPosition.z = LightPositionZ;
}

/*=============================================================================
*
* Purpose : セルシェーディングの輪郭線の太さを設定する．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsLineWidthCelShade
(
	GSfloat			LineWidth			/* 輪郭線の太さ				*/
)
{
	s_CelShadeLineWidth = LineWidth;
}

/*=============================================================================
*
* Purpose : セルシェーディングの輪郭線のカラーを設定する．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsLineColorCelShade
(
	GSfloat			LineColorR,			/* 輪郭線の色の赤成分		*/
	GSfloat			LineColorG,			/* 輪郭線の色の緑成分		*/
	GSfloat			LineColorB,			/* 輪郭線の色の青成分		*/
	GSfloat			LineColorA			/* 輪郭線のアルファ成分		*/
)
{
	s_CelShadeLineColor.r = LineColorR;
	s_CelShadeLineColor.g = LineColorG;
	s_CelShadeLineColor.b = LineColorB;
	s_CelShadeLineColor.a = LineColorA;
}
/*=============================================================================
*
* Purpose : 指定アニメーションのボーン数を返す．
*
* Return  : ボーン数を返す．
*
*============================================================================*/
extern GSuint
gsGetAnimationNumBones
(
	GSuint			AnimationID,		/* アニメーションＩＤ			*/
	GSuint			AnimationNo			/* アニメーションナンバ		*/
)
{
	GSanimation*	pAnimation;
	if ((pAnimation = s_pAnimations[AnimationID]) == NULL)
	{
		return 0;
	}
	/* ボーンデータ数を取得する */
	return gsAnimationGetNumBones(pAnimation, AnimationNo);
}

/*=============================================================================
*
* Purpose : アニメーション変換行列を計算する．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsCalculateAnimation
(
	GSuint			AnimationID,		/* アニメーションＩＤ			*/
	GSuint			AnimationNo,		/* アニメーションナンバ			*/
	GSfloat			fTimer,				/* アニメーションタイマ			*/
	GSmatrix4*		pAnimationMatrices	/*-> アニメーション変換行列		*/
)
{
	GSanimation*	pAnimation;
	GSuint			numBones;
	GSuint			BoneNo;
	GSvector3		Scale;
	GSquaternion	Rotate;
	GSvector3		Position;
	GSmatrix4		matScale;
	GSmatrix4		matRotate;

	/* アニメーションデータが存在するか？ */
	if ((pAnimation = s_pAnimations[AnimationID]) == NULL)
	{
		/* アニメーションが取得できなかった場合は全て単位行列を設定する */
		for (BoneNo = 0; BoneNo < GS_MATRIX_MAX; BoneNo++)
		{
			gsMatrix4Identity(&s_AnimationMatrices[BoneNo]);

		}
		return;
	}

	/* ボーンデータ数を取得する */
	numBones = gsAnimationGetNumBones(pAnimation, AnimationNo);

	/* アニメーション変換行列に設定する */
	for (BoneNo = 0; BoneNo < numBones; BoneNo++)
	{
		/* キーフレームデータを取得する */
		gsAnimationGetKeyFrame
		(
			pAnimation,
			AnimationNo, BoneNo, fTimer,
			&Scale, &Rotate, &Position
		);

		/* 各キーを行列に変換する */
		gsMatrix4Scale(&matScale, Scale.x, Scale.y, Scale.z);
		gsMatrix4Quaternion(&matRotate, &Rotate);
		gsMatrix4Multiply
		(
			&pAnimationMatrices[BoneNo], &matScale, &matRotate
		);
		pAnimationMatrices[BoneNo].m[3][0] = Position.x;
		pAnimationMatrices[BoneNo].m[3][1] = Position.y;
		pAnimationMatrices[BoneNo].m[3][2] = Position.z;
	}
}

/*=============================================================================
*
* Purpose : アニメーション変換行列を設定する．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsSetMatrixAnimation
(
	GSuint				numMatrices,		/* アニメーション変換行列数			*/
	const GSmatrix4*	pAnimationMatrices	/* アニメーション変換行列配列		*/
)
{
	GSuint i;
	for (i = 0; i < numMatrices; ++i) {
		s_AnimationMatrices[i] =  pAnimationMatrices[i];
	}
}

/*=============================================================================
*
* Purpose : スケルトンデータの姿勢変換行列を計算する．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsCalculateSkeleton
(
	const GSmatrix4*	pParentMatrix,		/* 親のモデリング変換行列		*/
	const GSmatrix4*	pAnimationMatrices,	/* アニメーション変換行列配列	*/
	GSmatrix4*			pSkeletonMatrices	/*-> 姿勢変換行列配列			*/
)
{
	GSskeleton*		pSkeleton;

	/* スケルトンデータを取得する*/
	if ( ( pSkeleton = s_pSkeletons[ s_BindSkeleton ] ) != NULL )
	{
		/* スケルトンデータから姿勢変換行列を計算する */
		gsSkeletonCalculateOrientation
		(
			pSkeleton,
			pSkeletonMatrices,
			pParentMatrix,
			pAnimationMatrices
		);
	}
}

/*=============================================================================
*
* Purpose : スケルトンの姿勢変換行列を設定する．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsSetMatrixSkeleton
(
	const GSmatrix4*	pSkeletonMatrices	/* 姿勢変換行列配列			*/
)
{
	GSuint			i;

	/* 変換行列の設定をする */
	for ( i = 0; i < gsGetSkeletonNumBones(s_BindSkeleton); i++ )
	{
		s_SkeletonMatrices[i] = pSkeletonMatrices[i];
	}
}

/*=============================================================================
*
* Purpose : スケルトンの姿勢変換行列を設定する．(旧形式）
*
* Return  : なし．
*
*============================================================================*/
extern void
gsSetMatrixOrientation
(
	GSuint				numMatrices,		/* 座標変換行列数			*/
	const GSmatrix4*	pOrientationMatrices	/* 姿勢変換行列配列			*/
)
{
	GSuint			i;

	/* 変換行列の設定をする */
	for (i = 0; i < numMatrices; i++)
	{
		s_SkeletonMatrices[i] = pOrientationMatrices[i];
	}
}

/*=============================================================================
*
* Purpose : スキニングメッシュ用変換行列を計算する
*
* Return  : なし．
*
*============================================================================*/
extern void
gsCalculateSkinTransform
(
	const GSmatrix4*	pSkeletonMatrices,		/* 姿勢変換行列配列				*/
	GSmatrix4*			pSkinTransformMatrices	/*-> スキニングメッシュ用配列	*/
)
{
	GSskeleton*		pSkeleton;

	/* スケルトンデータを取得する*/
	if ((pSkeleton = s_pSkeletons[s_BindSkeleton]) != NULL)
	{
		/* スケルトンデータから姿勢変換行列を計算する */
		gsSkeletonCalculateTransform
		(
			pSkeleton,
			pSkinTransformMatrices,
			pSkeletonMatrices
		);
	}
}

/*=============================================================================
*
* Purpose : スケルトンデータのボーン数を取得する．
*
* Return  : なし．
*
*============================================================================*/
extern GSuint
gsGetSkeletonNumBones
(
	GSuint		SkeletonID				/* スケルトンのＩＤ 		*/
)
{
	GSskeleton*	pSkeleton;

	if ( ( pSkeleton = s_pSkeletons[ SkeletonID ] ) == NULL )
	{
		return	0;
	}

	/* スケルトンデータからボーン数を取得する */
	return	gsSkeletonGetNumBones( pSkeleton );
}

/*=============================================================================
*
* Purpose : アニメーションの終了タイマを取得する．
*
* Return  : なし．
*
*============================================================================*/
extern GSuint
gsGetEndAnimationTime
(
	GSuint		AnimationID,			/* アニメーションのＩＤ		*/
	GSuint		AnimationNo				/* アニメーションナンバ		*/
)
{
	GSanimation*	pAnimation;

	if ( ( pAnimation = s_pAnimations[ AnimationID ] ) == NULL )
	{
		return	0;
	}

	/* アニメーションデータからアニメーションの終了タイマを取得 */
	return	gsAnimationGetEndTime( pAnimation, AnimationNo );
}

/*=============================================================================
*
* Purpose : 文字フォントのパラメータを設定する．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsTextPos
(
	GSfloat		PositoinX,		/* ｘ座標					*/
	GSfloat		PositoinY		/* ｙ座標					*/
)
{
	s_TextPosition.x = PositoinX;
	s_TextPosition.y = PositoinY;
}

/*=============================================================================
*
* Purpose : 文字フォントのパラメータを設定する．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsFontParameter
(
	GSbitfield	FontCode,		/* 文字フォントの属性		*/
	GSuint		FontPoint,		/* 文字フォントのポイント数	*/
	const char*	pszFontName		/* 文字フォント名			*/
)
{
	/* 文字フォントの属性を設定 */
	s_FontCode = FontCode;

	/* 文字フォントのポイント数を設定 */
	s_FontPoint = FontPoint;

	/* 文字フォント名の設定 */
	strcpy( s_FontName, pszFontName );
}

/*=============================================================================
*
* Purpose : テキスト描画．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsDrawText
(
	const char*	pString,			/* 表示する文字列		*/
	...								/* オプションの引数		*/
)
{
	char		String[512];		/* 表示文字列バッファ	*/
	GLint		ViewPort[4];
	va_list		ArgList;

	/* 可変長引数の使用開始 */
	va_start( ArgList, pString );

	/* 文字列のフォーマット変換を行う */
	vsprintf( String, pString, ArgList );

	/* 可変数引数の使用終了 */
	va_end( ArgList );

	/* 各種設定フラグの退避 */
	glPushAttrib( GL_ENABLE_BIT );

	/* ライティングを無効にする */
	glDisable( GL_LIGHTING );

	/* ｚバッファを無効にする */
	glDisable( GL_DEPTH_TEST );

	/* 面カリングを無効にする */
	glDisable( GL_CULL_FACE );

	/* 透視変換行列の退避 */
	glMatrixMode( GL_PROJECTION );
	glPushMatrix();

	/* ビューポートの取得 */
	glGetIntegerv( GL_VIEWPORT, ViewPort );

	/* 透視変換行列の設定 */
	glLoadIdentity();
	gluOrtho2D( 0, ViewPort[2], ViewPort[3], 0 );

	/* モデルビュー変換行列の退避 */
	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();

	/* 変換行列の初期化 */
	glLoadIdentity();

	/* テキストの位置を設定する */
	glRasterPos2f( s_TextPosition.x, s_TextPosition.y );

	/* テキストを描画する */
	gsTextDraw
	(
		s_FontCode,			/* フォントコード		*/
		s_FontPoint,		/* フォントのポイント数	*/
		s_FontName,			/* フォント名			*/
		String				/* 文字列				*/
	);

	/* モデルビュー変換行列を復帰 */
	glPopMatrix();

	/* 透視変換行列を復帰 */
	glMatrixMode( GL_PROJECTION );
	glPopMatrix();

	/* モデルビュー変換行列に設定 */
	glMatrixMode( GL_MODELVIEW );

	/* 各種設定フラグの復帰 */
	glPopAttrib();
}

/*=============================================================================
*
* Purpose : スクリーン座標からレイを計算する．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsCalculateRay
(
	GSint		ScreenX,				/* スクリーンｘ座標		*/
	GSint		ScreenY,				/* スクリーンｙ座標		*/
	GSfloat*	pRayPosX,				/*-> レイのｘ座標		*/
	GSfloat*	pRayPosY,				/*-> レイのｙ座標		*/
	GSfloat*	pRayPosZ,				/*-> レイのｙ座標		*/
	GSfloat*	pRayDirX,				/*-> レイのベクトル		*/
	GSfloat*	pRayDirY,				/*-> レイのベクトル		*/
	GSfloat*	pRayDirZ				/*-> レイのベクトル		*/
)
{
	GSvector3	Pos;
	GSvector3	Dir;
	GSvector2	Window;
	GSmatrix4	matProj;
	GSmatrix4	matView;
	GLint		ViewPort[4];

	/* ビューポートの取得 */
	glGetIntegerv( GL_VIEWPORT, ViewPort );

	/* 透視変換行列の取得 */
	glGetFloatv( GL_PROJECTION_MATRIX, (GLfloat*)&matProj );

	/* 視点変換行列の取得 */
	glGetFloatv( GL_MODELVIEW_MATRIX, (GLfloat*)&matView );

	/* マウスカーソルの座標をウィンドウ座標に変換 */
	Window.x = (GSfloat)ScreenX;
	Window.y = ViewPort[3] - (GSfloat)ScreenY;

	/* ビューポート変換の逆変換を行う */
	Pos.x = ( ( ( Window.x - ViewPort[0] ) * 2.0f / ViewPort[2] ) - 1.0f );
	Pos.y = ( ( ( Window.y - ViewPort[1] ) * 2.0f / ViewPort[3] ) - 1.0f );

	/* 透視変換の逆変換を行う */
	Pos.x /= matProj.m[0][0];
	Pos.y /= matProj.m[1][1];
	Pos.z = -1.0f;

	/* 視野変換行列の逆行列を求める */
	gsMatrix4InverseFast( &matView, &matView );

	/* 視野変換の逆変換を行う */
	gsVector3TransformNormal( &Dir, &Pos, &matView );

	/* レイの位置を設定 */
	*pRayPosX = matView.m[3][0];
	*pRayPosY = matView.m[3][1];
	*pRayPosZ = matView.m[3][2];

	/* レイの方向を正規化する */
	gsVector3Normalize( &Dir, &Dir );

	/* レイの方向を設定する */
	*pRayDirX = Dir.x;
	*pRayDirY = Dir.y;
	*pRayDirZ = Dir.z;
}

/*=============================================================================
*
* Purpose : スクリーン座標を計算する．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsCalculateScreen
(
	GSvector2*			pScreen,		/*-> スクリーン座標		*/
	const GSvector3*	pWorld			/* ワールド座標			*/
)
{
	GSmatrix4	matViewProjScreen;
	GSmatrix4	matProj;
	GSmatrix4	matView;
	GSmatrix4   matScreen;
	GLint		ViewPort[4];
	GSvector3   result;

	/* ビューポートの取得 */
	glGetIntegerv(GL_VIEWPORT, ViewPort);
	/* スクリーン座標変換行列を作成 */
	gsMatrix4Screen(&matScreen, ViewPort[2], ViewPort[3]);
	/* 透視変換行列の取得 */
	glGetFloatv(GL_PROJECTION_MATRIX, (GLfloat*)&matProj);
	/* 視点変換行列の取得 */
	glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat*)&matView);
	/* 座標変換を行う */
	gsMatrix4Multiply(&matViewProjScreen, &matView, &matProj);
	gsMatrix4Multiply(&matViewProjScreen, &matViewProjScreen, &matScreen);
	gsVector3TransformCoord(&result, pWorld, &matViewProjScreen);
	pScreen->x = result.x;
	pScreen->y = result.y;
}

/*=============================================================================
*
* Purpose : メッシュデータの取得．
*
* Return  : メッシュデータのポインタを返す．
*
*============================================================================*/
extern GSmesh*
gsGetMesh
(
	GSuint			MeshID				/* メッシュＩＤ				*/
)
{
	return	s_pMeshs[ MeshID ];
}

/*=============================================================================
*
* Purpose : スケルトンデータの取得．
*
* Return  : スケルトンデータのポインタを返す．
*
*============================================================================*/
extern GSskeleton*
gsGetSkeleton
(
	GSuint			SkeletonID			/* スケルトンＩＤ			*/
)
{
	return	s_pSkeletons[ SkeletonID ];
}

/*=============================================================================
*
* Purpose : アニメショーンデータの取得．
*
* Return  : アニメーションデータのポインタを返す．
*
*============================================================================*/
extern GSanimation*
gsGetAnimation
(
	GSuint			AnimationID			/* アニメーションＩＤ		*/
)
{
	return	s_pAnimations[ AnimationID ];
}

/*=============================================================================
*
* Purpose : オクツリーデータの取得．
*
* Return  : オクツリーデータのポインタを返す．
*
*============================================================================*/
extern GSoctree*
gsGetOctree
(
	GSuint			OctreeID			/* オクツリーＩＤ		*/
)
{
	return	s_pOctrees[ OctreeID ];
}

/*=============================================================================
*
* Purpose : テクスチャデータの取得．
*
* Return  : テクスチャデータのポインタを返す．
*
*============================================================================*/
extern GStexture*
gsGetTexture
(
	GSuint			TextureID			/* テクスチャＩＤ		*/
)
{
	return	s_pTextures[ TextureID ];
}

/*=============================================================================
*
* Purpose : 特定機能の有効化．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsEnable
(
	GSenum		Cap					/* 特定機能を示すシンボル定数 */
)
{
	switch( Cap )
	{
	case GS_SKIN_MESH:
		s_bSkinMesh = GS_TRUE;
		break;

	case GS_CALC_SKELETON:
		s_bCalculateSkeleton = GS_TRUE;
		break;

	case GS_CEL_SHADE:
		s_bCelShade = GS_TRUE;
		break;
	}
}

/*=============================================================================
*
* Purpose : 特定機能の無効化．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsDisable
(
	GSenum		Cap					/* 特定機能を示すシンボル定数 */
)
{
	switch( Cap )
	{
	case GS_SKIN_MESH:
		s_bSkinMesh = GS_FALSE;
		break;

	case GS_CALC_SKELETON:
		s_bCalculateSkeleton = GS_FALSE;
		break;

	case GS_CEL_SHADE:
		s_bCelShade = GS_FALSE;
		break;
	}
}

/*=============================================================================
*
* Purpose : シェーダーファイルの読み込み．
*
* Return  : 読み込みに成功すれば真，失敗すれば偽を返す．
*
*============================================================================*/
extern GSboolean
gsLoadShader
(
	GSuint			ShaderID,				/* シェーダーＩＤ		*/
	const char*		pszVertexShaderName,	/* 頂点シェーダー名		*/
	const char*		pszFragmentShaderName	/* ピクセルシェーダー名	*/
)
{
	return gsLoadShaderEx(ShaderID, pszVertexShaderName, pszFragmentShaderName, NULL, NULL, NULL);
}

/*=============================================================================
*
* Purpose : シェーダーファイルの読み込み．
*
* Return  : 読み込みに成功すれば真，失敗すれば偽を返す．
*
*============================================================================*/
extern GSboolean
gsLoadShaderEx
(
	GSuint			ShaderID,					/* シェーダーＩＤ			*/
	const char*		pszVertexShaderName,		/* 頂点シェーダー名			*/
	const char*		pszFragmentShaderName,		/* ピクセルシェーダー名		*/
	const char*		pszGeometryShaderName,		/* ジオメトリシェーダー名	*/
	const char*		pszTessControlShaderName,	/* ハルシェーダー名			*/
	const char*		pszTessEvaluationShaderName	/* ドメインシェーダー名		*/
)
{
	GSshaderProgram*	pShader;
	GSboolean			compled;
	GSboolean			linked;

	/* テクスチャデータの読み込み */
	pShader = gsShaderProgramNew();
	if (pShader == NULL) {
		return GS_FALSE;
	}
	/* 頂点シェーダーのコンパイル */
	if (pszVertexShaderName != NULL) {
		compled = gsShaderProgramCompleShader(pShader, GL_VERTEX_SHADER, pszVertexShaderName);
		if (compled == GS_FALSE) {
			gsShaderProgramDelete(pShader);
			return GL_FALSE;
		}
	}
	/* ピクセルシェーダーのコンパイル */
	if (pszFragmentShaderName != NULL) {
		compled = gsShaderProgramCompleShader(pShader, GL_FRAGMENT_SHADER, pszFragmentShaderName);
		if (compled == GS_FALSE) {
			gsShaderProgramDelete(pShader);
			return GL_FALSE;
		}
	}
	/* ジオメトリシェーダーのコンパイル */
	if (pszGeometryShaderName != NULL) {
		compled = gsShaderProgramCompleShader(pShader, GL_GEOMETRY_SHADER, pszGeometryShaderName);
		if (compled == GS_FALSE) {
			gsShaderProgramDelete(pShader);
			return GL_FALSE;
		}
	}
	/* ハルシェーダーのコンパイル */
	if (pszTessControlShaderName != NULL) {
		compled = gsShaderProgramCompleShader(pShader, GL_TESS_CONTROL_SHADER, pszTessControlShaderName);
		if (compled == GS_FALSE) {
			gsShaderProgramDelete(pShader);
			return GL_FALSE;
		}
	}
	/* ドメインシェーダーのコンパイル */
	if (pszTessEvaluationShaderName != NULL) {
		compled = gsShaderProgramCompleShader(pShader, GL_TESS_EVALUATION_SHADER, pszTessEvaluationShaderName);
		if (compled == GS_FALSE) {
			gsShaderProgramDelete(pShader);
			return GL_FALSE;
		}
	}
	/* シェーダーのリンク */
	linked = gsShaderProgramLinkShader(pShader);
	if (linked == GS_FALSE) {
		gsShaderProgramDelete(pShader);
		return GL_FALSE;
	}
	/* 古いシェーダーを削除 */
	gsDeleteShader(ShaderID);
	/* テクスチャデータの更新 */
	s_pShaders[ShaderID] = pShader;

	return GS_TRUE;
}

/*=============================================================================
*
* Purpose : シェーダーの削除．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsDeleteShader
(
	GSuint			ShaderID				/* シェーダーＩＤ			*/
)
{
	if (s_pShaders[ShaderID] != NULL)
	{
		glUseProgram(0);
		gsShaderProgramDelete(s_pShaders[ShaderID]);
		s_pShaders[ShaderID] = NULL;
	}
}

/*=============================================================================
*
* Purpose : シェーダーの開始．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsBeginShader
(
	GSuint			ShaderID				/* シェーダーＩＤ			*/
)
{
	s_ExecuteShader = ShaderID;
	if (s_pShaders[s_ExecuteShader] == NULL) {
		glUseProgram(0);
		return;
	}
	gsShaderProgramBegin(s_pShaders[s_ExecuteShader]);
}

/*=============================================================================
*
* Purpose : シェーダーの終了．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsEndShader
(
	void
)
{
	if (s_pShaders[s_ExecuteShader] != NULL)
	{
		gsShaderProgramEnd(s_pShaders[s_ExecuteShader]);
	}
}

/*=============================================================================
*
* Purpose : シェーダーにパラメータを設定．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsSetShaderParam1f
(
	const char*			name,			/* パラメータ名		*/
	GSfloat				param			/* パラメータ		*/
)
{
	GLint location = gsShaderProgramUniform(s_pShaders[s_ExecuteShader], name);
	glUniform1f(location, param);
}

/*=============================================================================
*
* Purpose : シェーダーにパラメータを設定．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsSetShaderParamArray1f
(
	const char*			name,			/* パラメータ名		*/
	GSuint				size,			/* 配列のサイズ		*/
	const GSfloat		param[]			/* パラメータ		*/
)
{
	GLint location = gsShaderProgramUniform(s_pShaders[s_ExecuteShader], name);
	glUniform1fv(location, size, param);
}

/*=============================================================================
*
* Purpose : シェーダーにパラメータを設定．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsSetShaderParam2f
(
	const char*			name,			/* パラメータ名		*/
	const GSvector2*	param			/* パラメータ		*/
)
{
	GLint location = gsShaderProgramUniform(s_pShaders[s_ExecuteShader], name);
	glUniform2f(location, param->x, param->y);
}

/*=============================================================================
*
* Purpose : シェーダーにパラメータを設定．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsSetShaderParamArray2f
(
	const char*			name,			/* パラメータ名		*/
	GSuint				size,			/* 配列のサイズ		*/
	const GSvector2		param[]			/* パラメータ		*/
)
{
	GLint location = gsShaderProgramUniform(s_pShaders[s_ExecuteShader], name);
	glUniform2fv(location, size, (GLfloat*)param);
}

/*=============================================================================
*
* Purpose : シェーダーにパラメータを設定．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsSetShaderParam3f
(
	const char*			name,			/* パラメータ名		*/
	const GSvector3*	param			/* パラメータ		*/
)
{
	GLint location = gsShaderProgramUniform(s_pShaders[s_ExecuteShader], name);
	glUniform3f(location, param->x, param->y, param->z);
}

/*=============================================================================
*
* Purpose : シェーダーにパラメータを設定．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsSetShaderParamArray3f
(
	const char*			name,			/* パラメータ名		*/
	GSuint				size,			/* 配列のサイズ		*/
	const GSvector3		param[]			/* パラメータ		*/
)
{
	GLint location = gsShaderProgramUniform(s_pShaders[s_ExecuteShader], name);
	glUniform3fv(location, size, (GLfloat*)param);
}

/*=============================================================================
*
* Purpose : シェーダーにパラメータを設定．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsSetShaderParam4f
(
	const char*			name,			/* パラメータ名		*/
	const GSvector4*	param			/* パラメータ		*/
)
{
	GLint location = gsShaderProgramUniform(s_pShaders[s_ExecuteShader], name);
	glUniform4f(location, param->x, param->y, param->z, param->w);
}

/*=============================================================================
*
* Purpose : シェーダーにパラメータを設定．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsSetShaderParamArray4f
(
	const char*			name,			/* パラメータ名		*/
	GSuint				size,			/* 配列のサイズ		*/
	const GSvector4		param[]			/* パラメータ		*/
)
{
	GLint location = gsShaderProgramUniform(s_pShaders[s_ExecuteShader], name);
	glUniform4fv(location, size, (GLfloat*)param);
}

/*=============================================================================
*
* Purpose : シェーダーにパラメータを設定．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsSetShaderParamMatrix4
(
	const char*			name,			/* パラメータ名		*/
	const GSmatrix4*	param			/* パラメータ		*/
)
{
	GLint location = gsShaderProgramUniform(s_pShaders[s_ExecuteShader], name);
	glUniformMatrix4fv(location, 1, GL_FALSE, (GLfloat*)param);
}

/*=============================================================================
*
* Purpose : シェーダーにパラメータを設定．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsSetShaderParamArrayMatrix4
(
	const char*			name,			/* パラメータ名		*/
	GSuint				size,			/* 配列のサイズ		*/
	const GSmatrix4		param[]			/* パラメータ		*/
)
{
	GLint location = gsShaderProgramUniform(s_pShaders[s_ExecuteShader], name);
	glUniformMatrix4fv(location, size, GL_FALSE, (GLfloat*)param);
}

/*=============================================================================
*
* Purpose : シェーダーにパラメータを設定．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsSetShaderParamTexture
(
	const char*			name,			/* パラメータ名		*/
	const GSint			param			/* パラメータ		*/
)
{
	GLint location = gsShaderProgramUniform(s_pShaders[s_ExecuteShader], name);
	glUniform1i(location, param);
}

/*=============================================================================
*
* Purpose : シェーダーにUniform Blackのインデックスを設定．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsSetShaderParamUniformBlock
(
	const char*			name,			/* パラメータ名		*/
	const GSuint		index			/* インデックス		*/
)
{
	GLint location = gsShaderProgramUniformBlock(s_pShaders[s_ExecuteShader], name);
	glUniformBlockBinding(s_pShaders[s_ExecuteShader]->program, location, index);
}

/*=============================================================================
*
* Purpose : シェーダーのUniromの位置を取得．
*
* Return  : なし．
*
*============================================================================*/
extern GLuint
gsGetShaderUnirom
(
	const char*			name			/* Unirom名			*/
)
{
	return gsShaderProgramUniform(s_pShaders[s_ExecuteShader], name);
}

/*=============================================================================
*
* Purpose : シェーダーのAttributeの位置を取得．
*
* Return  : なし．
*
*============================================================================*/
extern GLuint
gsGetShaderAttribute
(
	const char*			name			/* Attribute名		*/
)
{
	return gsShaderProgramAttribute(s_pShaders[s_ExecuteShader], name);
}

/*=============================================================================
*
* Purpose : レンダーターゲットの作成．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsCreateRenderTarget
(
	GSuint			RenderTargetID,			/* レンダーターゲットＩＤ		*/
	GSuint			width,					/* 幅							*/
	GSuint			height,					/* 高さ							*/
	GSboolean		colorBuffer,			/* カラーバッファを作成			*/
	GSboolean		depthBuffer,			/* デプスバッファを作成			*/
	GSboolean		formatFloat				/* 浮動小数点フォーマット		*/
)
{
	const GSpixelFormat* colorBufferrFormat;
	const GSpixelFormat* depthBufferrFormat;
	int numColorBuffer = (colorBuffer) ? 1 : 0;
	colorBufferrFormat = (formatFloat) ? &GS_PIXEL_FORMAT_R16G16B16A16_FLOAT: &GS_PIXEL_FORMAT_R8G8B8A8_UNORM;
	depthBufferrFormat = (formatFloat) ? &GS_PIXEL_FORMAT_D32_FLOAT_S8X24_UINT: &GS_PIXEL_FORMAT_D24_UNORM_S8_UINT;
	colorBufferrFormat = (colorBuffer) ? colorBufferrFormat : NULL;
	depthBufferrFormat = (depthBuffer) ? depthBufferrFormat : NULL;
	gsCreateRenderTargetEx(
		RenderTargetID,
		width, height,
		numColorBuffer,
		colorBufferrFormat,
		depthBufferrFormat,
		GL_LINEAR
	);
}

/*=============================================================================
*
* Purpose : レンダーターゲットの作成．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsCreateRenderTargetEx
(
	GSuint					RenderTargetID,	/* レンダーターゲットＩＤ		*/
	GSuint					width,			/* 幅							*/
	GSuint					height,			/* 高さ							*/
	GSuint					numColorBuffer,	/* カラーバッファ数				*/
	const GSpixelFormat		colorBuffer[],	/* カラーバッファフォーマット	*/
	const GSpixelFormat*	depthBuffer,	/* デプスバッファフォーマット	*/
	GLint					filter			/* テクスチャフィルタ			*/
)
{
	GSrendertarget* pRenderTagrget;
	/* レンダーターゲットの作成 */
	pRenderTagrget = gsRenderTargetNew(width, height, numColorBuffer, colorBuffer, depthBuffer, filter);

	/* 既にレンダーターゲットがあった場合は削除  */
	gsDeleteRenderTarget(RenderTargetID);

	/* テクスチャデータの更新 */
	s_pRenderTargets[RenderTargetID] = pRenderTagrget;
}

/*=============================================================================
*
* Purpose : シャドウマップ用レンダーターゲットの作成．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsCreateRenderTargetShadowMap
(
	GSuint			RenderTargetID,			/* レンダーターゲットＩＤ		*/
	GSuint			width,					/* 幅							*/
	GSuint			height,					/* 高さ							*/
	GSboolean		PCFshadow				/* PCFShadowを使うか？          */
)
{
	gsCreateRenderTarget(RenderTargetID, width, height, GS_FALSE, GS_TRUE, GS_TRUE);
	if (PCFshadow == GS_TRUE) {
		gsBindRenderTargetDepthEx(RenderTargetID, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		gsUnbindRenderTargetDepth(RenderTargetID);
	}
}

/*=============================================================================
*
* Purpose : キューブマップレンダーターゲットの作成．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsCreateRenderTargetCube
(
	GSuint			RenderTargetID,			/* レンダーターゲットＩＤ		*/
	GSuint			width,					/* 幅							*/
	GSuint			height,					/* 高さ							*/
	GSboolean		formatFloat				/* 浮動小数点フォーマット		*/
)
{
	const GSpixelFormat* colorBufferrFormat;
	const GSpixelFormat* depthBufferrFormat;
	colorBufferrFormat = (formatFloat) ? &GS_PIXEL_FORMAT_R16G16B16A16_FLOAT : &GS_PIXEL_FORMAT_R8G8B8A8_UNORM;
	depthBufferrFormat = (formatFloat) ? &GS_PIXEL_FORMAT_D32_FLOAT_S8X24_UINT : &GS_PIXEL_FORMAT_D24_UNORM_S8_UINT;
	gsCreateRenderTargetCubeEx(
		RenderTargetID,
		width, height,
		colorBufferrFormat,
		depthBufferrFormat,
		GL_LINEAR
	);
}

/*=============================================================================
*
* Purpose : キューブマップレンダーターゲットの作成．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsCreateRenderTargetCubeEx
(
	GSuint					RenderTargetID,	/* レンダーターゲットＩＤ		*/
	GSuint					width,			/* 幅							*/
	GSuint					height,			/* 高さ							*/
	const GSpixelFormat*	colorBuffer,	/* カラーバッファフォーマット	*/
	const GSpixelFormat*	depthBuffer,	/* デプスバッファフォーマット	*/
	GLint					filter			/* テクスチャフィルタ			*/
)
{
	GSrendertarget* pRenderTagrget;
	/* レンダーターゲットの作成 */
	pRenderTagrget = gsRenderTargetCubeNew(width, height, colorBuffer, depthBuffer, filter);

	/* 既にレンダーターゲットがあった場合は削除  */
	gsDeleteRenderTarget(RenderTargetID);

	/* テクスチャデータの更新 */
	s_pRenderTargets[RenderTargetID] = pRenderTagrget;
}


/*=============================================================================
*
* Purpose : レンダーターゲットの削除．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsDeleteRenderTarget
(
	GSuint					RenderTargetID	/* レンダーターゲットＩＤ		*/
)
{
	if (s_pRenderTargets[RenderTargetID] != NULL)
	{
		gsRenderTargetDelete(s_pRenderTargets[RenderTargetID]);
		s_pRenderTargets[RenderTargetID] = NULL;
	}
}

/*=============================================================================
*
* Purpose : レンダーターゲットの使用開始．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsBeginRenderTarget
(
	GSuint					RenderTargetID	/* レンダーターゲットＩＤ		*/
)
{
	if (s_pRenderTargets[RenderTargetID] == NULL)
	{
		return;
	}
	/* ビューポートの退避 */
	glGetIntegerv(GL_VIEWPORT, s_ViewPort);
	/* レンダーターゲットのサイズにビューポートを設定 */
	glViewport(0, 0, s_pRenderTargets[RenderTargetID]->Width, s_pRenderTargets[RenderTargetID]->Height);
	/* レンダーターゲットを有効にする */
	gsRenderTargetBegin(s_pRenderTargets[RenderTargetID]);
}

/*=============================================================================
*
* Purpose : レンダーターゲットの使用終了．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsEndRenderTarget
(
	void
)
{
	/* フレームバッファを無効にする */
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	/* ビューポートを元に戻す */
	glViewport(s_ViewPort[0], s_ViewPort[1], s_ViewPort[2], s_ViewPort[3]);
}

/*=============================================================================
*
* Purpose : キューブレンダーターゲットの使用開始．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsBeginRenderTargetCube
(
	GSuint					RenderTargetID,	/* レンダーターゲットＩＤ		*/
	GSenum					faceNo			/* キューブマップの面番号		*/
)
{
	if (s_pRenderTargets[RenderTargetID] == NULL)
	{
		return;
	}
	/* ビューポートの退避 */
	glGetIntegerv(GL_VIEWPORT, s_ViewPort);
	/* レンダーターゲットのサイズにビューポートを設定 */
	glViewport(0, 0, s_pRenderTargets[RenderTargetID]->Width, s_pRenderTargets[RenderTargetID]->Height);
	/* レンダーターゲットを有効にする */
	gsRenderTargetCubeBegin(s_pRenderTargets[RenderTargetID], faceNo);
}

/*=============================================================================
*
* Purpose : レンダーターゲット用のカラーバッファテクスチャをバインド．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsBindRenderTargetTexture
(
	GSuint					RenderTargetID,	/* レンダーターゲットＩＤ	*/
	GSuint					colorBuffer		/* カラーバッファの番号		*/
)
{
	if (s_pRenderTargets[RenderTargetID] == NULL)
	{
		return;
	}
	gsRenderTargetBindColorBufferTexture(s_pRenderTargets[RenderTargetID], colorBuffer);
}

/*=============================================================================
*
* Purpose : レンダーターゲット用のデプスバッファテクスチャをバインド．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsBindRenderTargetDepth
(
	GSuint					RenderTargetID	/* レンダーターゲットＩＤ	*/
)
{
	if (s_pRenderTargets[RenderTargetID] == NULL)
	{
		return;
	}
	gsRenderTargetBindDepthBufferTexture(s_pRenderTargets[RenderTargetID]);
}

/*=============================================================================
*
* Purpose : レンダーターゲット用のカラーバッファテクスチャをバインド．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsBindRenderTargetTextureEx
(
	GSuint					RenderTargetID,	/* レンダーターゲットＩＤ	*/
	GSuint					ColorBuffer,	/* カラーバッファの番号		*/
	GSuint					UnitNo			/* テクスチャの番号			*/
)
{
	glActiveTexture(GL_TEXTURE0 + UnitNo);
	gsRenderTargetBindColorBufferTexture(s_pRenderTargets[RenderTargetID], ColorBuffer);
}

/*=============================================================================
*
* Purpose : レンダーターゲット用のデプスバッファテクスチャをバインド．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsBindRenderTargetDepthEx
(
	GSuint					RenderTargetID,	/* レンダーターゲットＩＤ	*/
	GSuint					UnitNo			/* テクスチャの番号			*/
)
{
	glActiveTexture(GL_TEXTURE0 + UnitNo);
	gsRenderTargetBindDepthBufferTexture(s_pRenderTargets[RenderTargetID]);
}

/*=============================================================================
*
* Purpose : レンダーターゲット用のカラーバッファテクスチャをアンバインド．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsUnbindRenderTargetTexture
(
	GSuint					RenderTargetID,	/* レンダーターゲットＩＤ	*/
	GSuint					colorBuffer		/* カラーバッファの番号		*/
)
{
	if (s_pRenderTargets[RenderTargetID] == NULL)
	{
		return;
	}
	gsRenderTargetUnbindColorBufferTexture(s_pRenderTargets[RenderTargetID], colorBuffer);
}

/*=============================================================================
*
* Purpose : レンダーターゲット用のデプスバッファテクスチャをアンバインド．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsUnbindRenderTargetDepth
(
	GSuint					RenderTargetID	/* レンダーターゲットＩＤ	*/
)
{
	if (s_pRenderTargets[RenderTargetID] == NULL)
	{
		return;
	}
	gsRenderTargetUnbindDepthBufferTexture(s_pRenderTargets[RenderTargetID]);
}

/*=============================================================================
*
* Purpose : レンダーターゲット用のカラーバッファテクスチャをアンバインド．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsUnbindRenderTargetTextureEx
(
	GSuint					RenderTargetID,	/* レンダーターゲットＩＤ	*/
	GSuint					ColorBuffer,	/* カラーバッファの番号		*/
	GSuint					UnitNo			/* テクスチャの番号			*/
)
{
	glActiveTexture(GL_TEXTURE0 + UnitNo);
	gsRenderTargetUnbindColorBufferTexture(s_pRenderTargets[RenderTargetID], ColorBuffer);
	glActiveTexture(GL_TEXTURE0);
}

/*=============================================================================
*
* Purpose : レンダーターゲット用のデプスバッファテクスチャをアンバインド．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsUnbindRenderTargetDepthEx
(
	GSuint					RenderTargetID,	/* レンダーターゲットＩＤ	*/
	GSuint					UnitNo			/* テクスチャの番号			*/
)
{
	glActiveTexture(GL_TEXTURE0 + UnitNo);
	gsRenderTargetUnbindDepthBufferTexture(s_pRenderTargets[RenderTargetID]);
	glActiveTexture(GL_TEXTURE0);
}

/*=============================================================================
*
* Purpose : レンダーターゲットの描画．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsDrawRenderTarget
(
	GSuint					RenderTargetID	/* レンダーターゲットＩＤ	*/
)
{
	if (s_pRenderTargets[RenderTargetID] == NULL)
	{
		return;
	}
	gsRenderTargetDraw(s_pRenderTargets[RenderTargetID]);
}

/*=============================================================================
*
* Purpose : レンダーターゲットの描画．（シェーダー対応）
*
* Return  : なし．
*
*============================================================================*/
extern void
gsDrawRenderTargetEx
(
	GSuint					RenderTargetID	/* レンダーターゲットＩＤ	*/
)
{
	if (s_pRenderTargets[RenderTargetID] == NULL)
	{
		return;
	}
	gsRenderTargetDrawEx(s_pRenderTargets[RenderTargetID]);
}

/*=============================================================================
*
* Purpose : レンダーターゲットのテクスチャを取得．
*
* Return  : なし．
*
*============================================================================*/
extern GStexture*
gsGetRenderTargetTexture
(
	GSuint					RenderTargetID,	/* レンダーターゲットＩＤ		*/
	GSuint					ColorBuffer		/* カラーバッファの番号			*/
)
{
	if (s_pRenderTargets[RenderTargetID] == NULL)
	{
		return NULL;
	}
	return s_pRenderTargets[RenderTargetID]->ColorBuffers[ColorBuffer];
}

/*=============================================================================
*
* Purpose : レンダーターゲットのデプステクスチャを取得．
*
* Return  : なし．
*
*============================================================================*/
extern GStexture*
gsGetRenderTargetDepthTexture
(
	GSuint					RenderTargetID	/* レンダーターゲットＩＤ		*/
)
{
	if (s_pRenderTargets[RenderTargetID] == NULL)
	{
		return NULL;
	}
	return s_pRenderTargets[RenderTargetID]->DepthBuffer;
}

/*=============================================================================
*
* Purpose : UniformBlockの作成．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsCreateUnifomBlock
(
	GSuint			UniformBlockID,			/* Uniform Block ＩＤ		    */
	GSuint			Size,					/* サイズ						*/
	const GSvoid*	pData					/* データ						*/
)
{
	GSunifomBlock* pUniformBlock;
	/* UniformBlockの作成 */
	pUniformBlock = gsUnifomBlockNew(Size, pData);

	/* UniformBlockがあった場合は削除  */
	gsDeleteUniformBlock(UniformBlockID);

	/* UniformBlockの更新 */
	s_pUniformBlocks[UniformBlockID] = pUniformBlock;
}

/*=============================================================================
*
* Purpose : レンダーターゲットの削除．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsDeleteUniformBlock
(
	GSuint				UniformBlockID		/* Uniform Block ＩＤ		*/
)
{
	if (s_pUniformBlocks[UniformBlockID] != NULL)
	{
		gsUniformBlockDelete(s_pUniformBlocks[UniformBlockID]);
		s_pUniformBlocks[UniformBlockID] = NULL;
	}
}

/*=============================================================================
*
* Purpose : Uniform Blockのバインド．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsBindUnifomBlock
(
	GSuint				UniformBlockID,		/* Uniform Block ＩＤ		*/
	GSuint               index				/* バッファインデックス     */
)
{
	if (s_pUniformBlocks[UniformBlockID] == NULL)
	{
		return;
	}
	gsUniformBlockBind(s_pUniformBlocks[UniformBlockID], index);
}

/*=============================================================================
*
* Purpose : Uniform Blockのアンバインド．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsUnbindUnifomBlock
(
	GSuint				UniformBlockID,		/* Uniform Block ＩＤ		*/
	GSuint              index				/* バッファインデックス     */
)
{
	if (s_pUniformBlocks[UniformBlockID] == NULL)
	{
		return;
	}
	gsUniformBlockUnbind(s_pUniformBlocks[UniformBlockID], index);
}

/*=============================================================================
*
* Purpose : Uniform Blockの更新．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsUpdateUnifomBlock
(
	GSuint				UniformBlockID,		/* Uniform Block ＩＤ				*/
	const GSvoid*		pData				/* 更新するバッファデータ           */
)
{
	if (s_pUniformBlocks[UniformBlockID] == NULL)
	{
		return;
	}
	gsUpdateUnifomBlockEx(
		UniformBlockID,
		gsUniformBlockGetSize(s_pUniformBlocks[UniformBlockID]),
		0,
		pData
	);
}

/*=============================================================================
*
* Purpose : Uniform Blockの更新．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsUpdateUnifomBlockEx
(
	GSuint				UniformBlockID,		/* Uniform Block ＩＤ				*/
	GSuint				Size,				/* 更新するバッファデータのバイト数 */
	GSuint				Offset,				/* 更新する場所のオフセット         */
	const GSvoid*		pData				/* 更新するバッファデータ           */
)
{
	if (s_pUniformBlocks[UniformBlockID] == NULL)
	{
		return;
	}
	gsUniformBlockUpdate(s_pUniformBlocks[UniformBlockID], Size, Offset, pData);
}


/********** End of File ******************************************************/
