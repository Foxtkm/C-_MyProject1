#include "GSgraphics_util.h"

// ２Ｄスプライトの描画
extern void gsDrawSprite2D(
	GSuint				textureID,			// テクスチャＩＤ
	const GSvector2*	translation,		// 平行移動量
	const GSrect*		sourceRect,			// テクチャ内の位置
	const GSvector2*	center,				// スプライトの中心位置
	const GScolor*		color,				// カラー
	const GSvector2*	scaling,			// スケール値
	GSfloat				rotation			// 回転角度
) {
	GScolor currentColor;
	GLsizei	texWidth;
	GLsizei	texHeight;
	GLfloat	width;
	GLfloat	height;
	GSrect texCoord = { 0.0f, 0.0f, 1.0f, 1.0f };

	// 各種レンダリングステートの退避
	glPushAttrib( GL_ENABLE_BIT );
	// ライティングを無効にする
	glDisable( GL_LIGHTING );
	// ｚバッファを無効にする
	glDisable( GL_DEPTH_TEST );
	// 面カリングを無効にする
	glDisable( GL_CULL_FACE );

	// カレントカラーを取得する
	glGetFloatv( GL_CURRENT_COLOR, (float*)&currentColor);

	// 透視変換行列の退避
	glMatrixMode( GL_PROJECTION );
	glPushMatrix();
	// ビューポートの取得
	int viewport[4];
	glGetIntegerv( GL_VIEWPORT, viewport );
	// 正射影の変換行列を設定
	glLoadIdentity();
	gluOrtho2D( 0.0f, (float)viewport[2], (float)viewport[3], 0.0f );

	// モデルビュー変換行列の退避
	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();
	// 変換行列の初期化
	glLoadIdentity();

	// 平行移動量の設定を行う
	if ( translation != 0 ) {
		glTranslatef( translation->x, translation->y, 0.0f );
	}
	// 回転角度の設定を行う
	glRotatef(rotation, 0.0f, 0.0f, 1.0f);
	// 拡大縮小を行う
	if ( scaling != 0 ) {
		glScalef( scaling->x, scaling->y, 0.0f );
	}
	// 中心位置の補正を行う
	if ( center != 0 ) {
		glTranslatef( -center->x, -center->y, 0 );
	}
	// 頂点カラーの設定
	if ( color != 0 ) {
		glColor4f( color->r, color->g, color->b, color->a );
	}
	// テクスチャをバインドする
	gsBindTexture( textureID );

	// バインド中のテクスチャの幅と高さを取得
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH,  &texWidth);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &texHeight);
	width = (GLfloat)texWidth;
	height = (GLfloat)texHeight;
	if ( sourceRect != 0 ) {
		// テクスチャ座標の計算を行う
		texCoord.left   = sourceRect->left   / width;
		texCoord.top    = sourceRect->top    / height;
		texCoord.right  = sourceRect->right  / width;
		texCoord.bottom = sourceRect->bottom / height;
		// 四角形の幅と高さを求める
		width  = ABS(sourceRect->right  - sourceRect->left);
		height = ABS(sourceRect->bottom - sourceRect->top);
	}
	// 四角形の描画
	glBegin( GL_QUADS );
		glTexCoord2f(texCoord.left, texCoord.top);
		glVertex2f(0.0f, 0.0f);
		glTexCoord2f(texCoord.left, texCoord.bottom);
		glVertex2f(0.0f, height);
		glTexCoord2f(texCoord.right, texCoord.bottom);
		glVertex2f(width, height);
		glTexCoord2f(texCoord.right, texCoord.top);
		glVertex2f(width, 0.0f);
	glEnd();
	// モデルビュー変換行列を復帰
	glPopMatrix();
	// 透視変換行列を復帰
	glMatrixMode( GL_PROJECTION );
	glPopMatrix();
	// モデルビュー変換行列に設定
	glMatrixMode( GL_MODELVIEW );
	// カレントカラーを復帰する
	glColor4fv( (float*)&currentColor );
	// レンダリングステートの復帰
	glPopAttrib();
}

// ３Ｄスプライトの描画
extern void gsDrawSprite3D(
	GSuint				textureID,			// テクスチャＩＤ
	const GSvector3*	translation,		// 平行移動量
	const GSrect*		spriteRect,			// スプライトの矩形
	const GSrect*		sourceRect,			// テクチャ内の位置
	const GScolor*		color,				// カラー
	const GSvector2*	scaling,			// スケール値
	GSfloat				rotation			// 回転角度
) {
	GScolor currentColor;
	GSmatrix4 matView;
	GLsizei	texWidth;
	GLsizei texHeight;
	GSrect texCoord = { 0.0f, 0.0f, 1.0f, 1.0f };

	// 各種レンダリングモードの退避
	glPushAttrib(GL_ENABLE_BIT);
	// 面カリングを無効にする
	glDisable(GL_CULL_FACE);
	// カレントカラーを取得する
	glGetFloatv(GL_CURRENT_COLOR, (GLfloat*)&currentColor);

	// モデルビュー変換行列の退避
	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();
	// 平行移動量の設定を行う
	if ( translation != 0 ) {
		glTranslatef(translation->x, translation->y, translation->z);
	}
	// 視野変換行列を取得する
	glGetFloatv(GL_MODELVIEW_MATRIX, (float*)&matView);
	// ビルボード処理用の変換行列を作成
	matView.m[3][0] = 0;
	matView.m[3][1] = 0;
	matView.m[3][2] = 0;
	gsMatrix4Transpose(&matView, &matView);
	// ビルボード処理用の変換行列を設定する
	glMultMatrixf((float*)&matView);
	// 回転角度の設定を行う */
	glRotatef(rotation, 0.0f, 0.0f, 1.0f);
	// 拡大縮小を行う
	if ( scaling != 0 ) {
		glScalef(scaling->x, scaling->y, 1.0f);
	}
	// 頂点カラーの設定
	if ( color != 0 ) {
		glColor4f(color->r, color->g, color->b, color->a);
	}
	// テクスチャをバインドする
	gsBindTexture(textureID);
	// バインド中のテクスチャの幅と高さを取得
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH,  &texWidth );
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &texHeight);
	if ( sourceRect != 0 ) {
		// テクスチャ座標の計算を行う
		texCoord.left   = sourceRect->left   / texWidth;
		texCoord.top    = sourceRect->top    / texHeight;
		texCoord.right  = sourceRect->right  / texWidth;
		texCoord.bottom = sourceRect->bottom / texHeight;
	}
	// 四角形の描画 
	glBegin( GL_QUADS );
		glNormal3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f(texCoord.left, texCoord.top);
		glVertex2f(spriteRect->left, spriteRect->top);
		glTexCoord2f(texCoord.left, texCoord.bottom);
		glVertex2f(spriteRect->left, spriteRect->bottom);
		glTexCoord2f(texCoord.right, texCoord.bottom);
		glVertex2f(spriteRect->right, spriteRect->bottom);
		glTexCoord2f(texCoord.right, texCoord.top);
		glVertex2f(spriteRect->right, spriteRect->top);
	glEnd();

	// モデルビュー変換行列を復帰
	glPopMatrix();
	// カレントカラーを復帰する
	glColor4fv((GLfloat*)&currentColor);
	// レンダリングモードの復帰
	glPopAttrib();
}

// ３Ｄスプライトの描画
extern void gsDrawSprite3DEx(
	GSuint				textureID,			// テクスチャＩＤ
	const GSrect*		spriteRect,			// スプライトの矩形
	const GSrect*		sourceRect			// テクチャ内の位置
) {
	GLsizei	  texWidth;
	GLsizei   texHeight;
	GSrect    texCoord = { 0.0f, 0.0f, 1.0f, 1.0f };
	struct {
		GSvector2 position;
		GSvector2 texcoord;
	} vertices[4];
	// 各種レンダリングモードの退避
	glPushAttrib(GL_ENABLE_BIT);
	// 面カリングを無効にする
	glDisable(GL_CULL_FACE);
	// テクスチャをバインドする
	gsBindTexture(textureID);
	// バインド中のテクスチャの幅と高さを取得
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH,  &texWidth);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &texHeight);
	if (sourceRect != 0) {
		// テクスチャ座標の計算を行う
		texCoord.left   = sourceRect->left   / texWidth;
		texCoord.top    = sourceRect->top    / texHeight;
		texCoord.right  = sourceRect->right  / texWidth;
		texCoord.bottom = sourceRect->bottom / texHeight;
	}
	// 頂点バッファの設定
	vertices[0].position.x = spriteRect->left;
	vertices[0].position.y = spriteRect->top;
	vertices[0].texcoord.x = texCoord.left;
	vertices[0].texcoord.y = texCoord.top;
	vertices[1].position.x = spriteRect->left;
	vertices[1].position.y = spriteRect->bottom;
	vertices[1].texcoord.x = texCoord.left;
	vertices[1].texcoord.y = texCoord.bottom;
	vertices[2].position.x = spriteRect->right;
	vertices[2].position.y = spriteRect->top;
	vertices[2].texcoord.x = texCoord.right;
	vertices[2].texcoord.y = texCoord.top;
	vertices[3].position.x = spriteRect->right;
	vertices[3].position.y = spriteRect->bottom;
	vertices[3].texcoord.x = texCoord.right;
	vertices[3].texcoord.y = texCoord.bottom;
	// 四角形の描画 
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(8);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]), &vertices[0].position);
	glVertexAttribPointer(8, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]), &vertices[0].texcoord);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glDisableVertexAttribArray(8);
	glDisableVertexAttribArray(0);
	// レンダリングモードの復帰
	glPopAttrib();
}

// スカイボックスの描画
extern void gsDrawSkyBox(GSuint meshID) {
	GSmatrix4 matView;
	// 各種レンダリングモードの退避
	glPushAttrib(GL_ENABLE_BIT);
	// ライティングを無効にする
	glDisable(GL_LIGHTING);
	// ｚバッファを無効にする
	glDisable(GL_DEPTH_TEST);
	// 面カリングを無効にする
	glDisable(GL_CULL_FACE);
	// フォグを無効にする
	glDisable(GL_FOG);
	// 視野変換行列を退避する
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	// スカイボックス用の変換行列を設定する */
	glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat*)&matView);
	matView.m[3][0] = 0;
	matView.m[3][1] = 0;
	matView.m[3][2] = 0;
	glLoadMatrixf((GLfloat*)&matView);
	// スカイボックスを描画する
	gsDrawMesh( meshID );
	// 視野変換行列を復帰する
	glPopMatrix();
	// 各種レンダリングモードの復帰
	glPopAttrib();
}

// スカイボックスの描画
extern void gsDrawSkyBoxEx(GSuint cube_map) {
	// インデックスバッファ
	static const GLushort indices[] = {
		0, 3, 1, 1, 3, 2,	// 前
		4, 5, 7, 5, 6, 7,	// 後
		0, 1, 4, 1, 5, 4,	// 左
		7, 6, 3, 6, 2, 3,	// 右
		0, 4, 3, 4,	7, 3,	// 上
		5, 1, 6, 1, 2, 6	// 下
	};
	// 頂点データ
	static const float vertices[] = {
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f
	};
	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glActiveTexture(GL_TEXTURE0);
	gsBindTexture(cube_map);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vertices);
	glDrawElements(GL_TRIANGLES, 6 * 6, GL_UNSIGNED_SHORT, indices);
	glDisableVertexAttribArray(0);
	gsUnbindTexture(cube_map);
	glPopAttrib();
}

// スカイボックスの描画
extern void gsDrawSkyBoxCubeMap(GSuint cube_map) {
	// インデックスバッファ
	static const GLushort indices[] = {
		0, 3, 1, 1, 3, 2,	// 前
		4, 5, 7, 5, 6, 7,	// 後
		0, 1, 4, 1, 5, 4,	// 左
		7, 6, 3, 6, 2, 3,	// 右
		0, 4, 3, 4,	7, 3,	// 上
		5, 1, 6, 1, 2, 6	// 下
	};
	// 頂点データ
	static const float vertices[] = {
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f
	};
	GSmatrix4 matView;
	glPushAttrib(GL_ENABLE_BIT);
	// ライティングを無効にする
	glDisable(GL_LIGHTING);
	// ｚバッファを無効にする
	glDisable(GL_DEPTH_TEST);
	// 面カリングを無効にする
	glDisable(GL_CULL_FACE);
	// フォグを無効にする
	glDisable(GL_FOG);
	// 視野変換行列を退避する
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	// スカイボックス用の変換行列を設定する */
	glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat*)&matView);
	matView.m[3][0] = 0;
	matView.m[3][1] = 0;
	matView.m[3][2] = 0;
	glLoadMatrixf((GLfloat*)&matView);
	// キューブマップをバインド
	gsBindTexture(cube_map);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(3, GL_FLOAT, 0, vertices);
	glDrawElements(GL_TRIANGLES, 6 * 6, GL_UNSIGNED_SHORT, indices);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	// キューブマップをアンバインド
	gsUnbindTexture(cube_map);
	// 視野変換行列を復帰する
	glPopMatrix();
	// 各種レンダリングモードの復帰
	glPopAttrib();
}

// キューブマップに描画
extern void gsDrawCubeMap(GSuint cube_map, const GSvector3* pViewPosition, void(*draw_scene)(void*), void* user) {
	static const struct {
		GSenum		face;
		GSvector3	at;
		GSvector3	up;
	} cube_map_faces[] = {
		{ GS_CUBE_MAP_POSITIVE_X,{  1.0f,  0.0f,  0.0f },{ 0.0f, 1.0f,  0.0f } },
		{ GS_CUBE_MAP_NEGATIVE_X,{ -1.0f,  0.0f,  0.0f },{ 0.0f, 1.0f,  0.0f } },
		{ GS_CUBE_MAP_POSITIVE_Y,{  0.0f,  1.0f,  0.0f },{ 0.0f, 0.0f, -1.0f } },
		{ GS_CUBE_MAP_NEGATIVE_Y,{  0.0f, -1.0f,  0.0f },{ 0.0f, 0.0f,  1.0f } },
		{ GS_CUBE_MAP_POSITIVE_Z,{  0.0f,  0.0f,  1.0f },{ 0.0f, 1.0f,  0.0f } },
		{ GS_CUBE_MAP_NEGATIVE_Z,{  0.0f,  0.0f, -1.0f },{ 0.0f, 1.0f,  0.0f } }
	};
	int i;
	/* 透視射影の変換行列を設定する */
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glScalef(-1.0f, -1.0f, 1.0f);
	gluPerspective(90.0f, 1.0f, 0.1f, 10000.0f);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	/* キューブマップにレンダリング */
	for (i = 0; i < 6; ++i) {
		GSvector3 view_position = { 0.0f, 0.0f, 0.0f };
		GSvector3 at;
		if (pViewPosition != NULL) view_position = *pViewPosition;
		gsVector3Add(&at, &view_position, &cube_map_faces[i].at);
		gsBeginRenderTargetCube(cube_map, cube_map_faces[i].face);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(
			view_position.x, view_position.y, view_position.z,
			at.x, at.y, at.z,
			cube_map_faces[i].up.x, cube_map_faces[i].up.y, cube_map_faces[i].up.z
		);
		draw_scene(user);
		gsEndRenderTarget();
	}
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

// キューブマップに描画
extern void gsDrawCubeMapEx(GSuint cube_map, const GSvector3* pViewPosition, void(*draw_scene)(const GSmatrix4* projection, const GSmatrix4* view, void* user), void* user) {
	static const struct {
		GSenum		face;
		GSvector3	at;
		GSvector3	up;
	} cube_map_faces[] = {
		{ GS_CUBE_MAP_POSITIVE_X,{ 1.0f,  0.0f,  0.0f },{ 0.0f, 1.0f,  0.0f } },
		{ GS_CUBE_MAP_NEGATIVE_X,{ -1.0f, 0.0f,  0.0f },{ 0.0f, 1.0f,  0.0f } },
		{ GS_CUBE_MAP_POSITIVE_Y,{ 0.0f,  1.0f,  0.0f },{ 0.0f, 0.0f, -1.0f } },
		{ GS_CUBE_MAP_NEGATIVE_Y,{ 0.0f, -1.0f,  0.0f },{ 0.0f, 0.0f,  1.0f } },
		{ GS_CUBE_MAP_POSITIVE_Z,{ 0.0f,  0.0f,  1.0f },{ 0.0f, 1.0f,  0.0f } },
		{ GS_CUBE_MAP_NEGATIVE_Z,{ 0.0f,  0.0f, -1.0f },{ 0.0f, 1.0f,  0.0f } }
	};
	int i;
	/* 透視射影の変換行列を作成する */
	GSmatrix4 projection;
	GSmatrix4 view;
	GSmatrix4 scale;
	gsMatrix4Scale(&scale, -1.0f, -1.0f, 1.0f);
	gsMatrix4PerspectiveRH(&projection, 90.0f, 1.0f, 0.1f, 10000.0f);
	gsMatrix4Multiply(&projection, &projection, &scale);
	/* キューブマップにレンダリング */
	for (i = 0; i < 6; ++i) {
		GSvector3 view_position = { 0.0f, 0.0f, 0.0f };
		GSvector3 at;
		if (pViewPosition != NULL) view_position = *pViewPosition;
		gsVector3Add(&at, &view_position, &cube_map_faces[i].at);
		gsBeginRenderTargetCube(cube_map, cube_map_faces[i].face);
		gsMatrix4LookAtRH(&view, &view_position, &at, &cube_map_faces[i].up);
		draw_scene(&projection, &view, user);
		gsEndRenderTarget();
	}
}

// end of flie
