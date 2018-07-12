#ifndef _GS_GRAPHICS_UTIL_H_
#define _GS_GRAPHICS_UTIL_H_

#include <gslib.h>

#ifdef __cplusplus
extern "C" {
#endif

// ２Ｄスプライトの描画
extern void gsDrawSprite2D(
	GSuint				textureID,			// テクスチャＩＤ
	const GSvector2*	translation,		// 平行移動量
	const GSrect*		sourceRect,			// テクチャ内の位置
	const GSvector2*	center,				// スプライトの中心位置
	const GScolor*		color,				// カラー
	const GSvector2*	scaling,			// スケール値
	GSfloat				rotation			// 回転角度
);

// ３Ｄスプライトの描画
extern void gsDrawSprite3D(
	GSuint				textureID,			// テクスチャＩＤ
	const GSvector3*	translation,		// 平行移動量
	const GSrect*		spriteRect,			// スプライトの矩形
	const GSrect*		sourceRect,			// テクチャ内の位置
	const GScolor*		color,				// カラー
	const GSvector2*	scaling,			// スケール値
	GSfloat				rotation			// 回転角度
);

// ３Ｄスプライトの描画 (シェーダー用）
extern void gsDrawSprite3DEx(
	GSuint				textureID,			// テクスチャＩＤ
	const GSrect*		spriteRect,			// スプライトの矩形
	const GSrect*		sourceRect			// テクチャ内の位置
);

// スカイボックスの描画
extern void gsDrawSkyBox(
	GSuint meshID							// メッシュID
);

// スカイボックスの描画 (シェーダー用）
extern void gsDrawSkyBoxEx(
	GSuint cube_map							// テクスチャＩＤ（キューブマップ）
);

// スカイボックスの描画
extern void gsDrawSkyBoxCubeMap(
	GSuint cube_map							// テクスチャＩＤ（キューブマップ）
);

// キューブマップに描画
extern void gsDrawCubeMap(
	GSuint cube_map,						// レンダーターゲットのID（キューブマップ）
	const GSvector3* pViewPosition,			// 視点の座標
	void(*draw_scene)(void*),				// コールバック関数
	void* user								// ユーザーポインタ
);

// キューブマップに描画
extern void gsDrawCubeMapEx(
	GSuint cube_map,						// レンダーターゲットのID（キューブマップ）
	const GSvector3* pViewPosition,			// 視点の座標
											// コールバック関数
	void(*draw_scene)(const GSmatrix4* projection, const GSmatrix4* view, void* user),
	void* user								// ユーザーポインタ
);


#ifdef __cplusplus
}
#endif

#endif
