#include "GSrender_target.h"
#include "heap.h"
#include <opengl_ext.h>

/*=============================================================================
*
* Purpose : レンダーターゲットの生成．
*
* Return  : レンダーターゲットを返す．
*
*============================================================================*/
extern GSrendertarget*
gsRenderTargetNew
(
	GSuint					width,			/* 幅							*/
	GSuint					height,			/* 高さ							*/
	GSuint					numColorBuffer,	/* カラーバッファ数				*/
	const GSpixelFormat		colorBuffer[],	/* カラーバッファフォーマット	*/
	const GSpixelFormat*	depthBuffer,	/* デプスバッファフォーマット	*/
	GLint					filter			/* テクスチャフィルタ			*/
)
{
	GLenum drawBuffers[GS_MRT_MAX];
	GSrendertarget* renderTaget;
	renderTaget = new_object(sizeof(GSrendertarget));
	memset(renderTaget, 0, sizeof(GSrendertarget));
	renderTaget->Width = width;
	renderTaget->Height = height;
	/* フレームバッファオブジェクトの作成　*/
	glGenFramebuffers(1, &renderTaget->FrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, renderTaget->FrameBuffer);
	/* カラーバッファの作成 */
	renderTaget->NumColorBuffer = numColorBuffer;
	for (GLuint i = 0; i < renderTaget->NumColorBuffer; ++i) {
		renderTaget->ColorBuffers[i] = gsTextureNew(GL_TEXTURE_2D, width, height, 1, 1, &colorBuffer[i], NULL);
		gsTextureBind(renderTaget->ColorBuffers[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, renderTaget->ColorBuffers[i]->dwTexName, 0);
		drawBuffers[i] = GL_COLOR_ATTACHMENT0 + i;
		gsTextureUnbind(renderTaget->ColorBuffers[i]);
	}
	if (numColorBuffer == 0) {
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
	} else {
		glDrawBuffers(numColorBuffer, drawBuffers);
	}
	/* デプスバッファの作成 */
	if (depthBuffer != NULL) {
		renderTaget->DepthBuffer = gsTextureNew(GL_TEXTURE_2D, width, height, 1, 1, depthBuffer, NULL);
		gsTextureBind(renderTaget->DepthBuffer);
		GLfloat border_color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_color);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_LUMINANCE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, renderTaget->DepthBuffer->dwTexName, 0);
		gsTextureUnbind(renderTaget->DepthBuffer);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return renderTaget;
}

/*=============================================================================
*
* Purpose : キューブマップレンダーターゲットの生成．
*
* Return  : キューブマップレンダーターゲットを返す．
*
*============================================================================*/
extern GSrendertarget*
gsRenderTargetCubeNew
(
	GSuint					width,			/* 幅							*/
	GSuint					height,			/* 高さ							*/
	const GSpixelFormat*	colorBuffer,	/* カラーバッファフォーマット	*/
	const GSpixelFormat*	depthBuffer,	/* デプスバッファフォーマット	*/
	GLint					filter			/* テクスチャフィルタ			*/
)
{
	GSrendertarget* renderTaget;
	renderTaget = new_object(sizeof(GSrendertarget));
	memset(renderTaget, 0, sizeof(GSrendertarget));
	renderTaget->Width = width;
	renderTaget->Height = height;
	/* フレームバッファオブジェクトの作成　*/
	glGenFramebuffers(1, &renderTaget->FrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, renderTaget->FrameBuffer);
	/* カラーバッファの作成 */
	renderTaget->NumColorBuffer = 1;
	renderTaget->ColorBuffers[0] = gsTextureNew(GL_TEXTURE_CUBE_MAP, width, height, 1, 1, colorBuffer, NULL);
	gsTextureBind(renderTaget->ColorBuffers[0]);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, filter);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, filter);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X, renderTaget->ColorBuffers[0]->dwTexName, 0);
	gsTextureUnbind(renderTaget->ColorBuffers[0]);
	/* デプスバッファの作成 */
	if (depthBuffer != NULL) {
		renderTaget->DepthBuffer = gsTextureNew(GL_TEXTURE_2D, width, height, 1, 1, depthBuffer, NULL);
		gsTextureBind(renderTaget->DepthBuffer);
		GLfloat border_color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_color);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_LUMINANCE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, renderTaget->DepthBuffer->dwTexName, 0);
		gsTextureUnbind(renderTaget->DepthBuffer);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return renderTaget;
}


/*=============================================================================
*
* Purpose : レンダーターゲットの消滅．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsRenderTargetDelete
(
	GSrendertarget*			pRenderTarget	/* レンダーターゲット	*/
)
{
	/* カラーバッファの削除 */
	for (GLuint i = 0; i < pRenderTarget->NumColorBuffer; ++i) {
		gsTextureDelete(pRenderTarget->ColorBuffers[i]);
	}
	/* デプスバッファの削除 */
	if (pRenderTarget->DepthBuffer != NULL) {
		gsTextureDelete(pRenderTarget->DepthBuffer);
	}
	/* フレームバッファオブジェクトの削除 */
	glDeleteFramebuffers(1, &pRenderTarget->FrameBuffer);
	/*　レンダーターゲットの削除 */
	del_object(pRenderTarget);
}


/*=============================================================================
*
* Purpose : レンダーターゲットの使用開始．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsRenderTargetBegin
(
	GSrendertarget*			pRenderTarge	/* レンダーターゲット	*/
)
{
	glBindFramebuffer(GL_FRAMEBUFFER, pRenderTarge->FrameBuffer);
}

/*=============================================================================
*
* Purpose : レンダーターゲットの使用終了．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsRenderTargetEnd
(
	GSrendertarget*			pRenderTarget	/* レンダーターゲット	*/
)
{
	(void)pRenderTarget;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

/*=============================================================================
*
* Purpose : キューブマップレンダーターゲットの使用開始．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsRenderTargetCubeBegin
(
	GSrendertarget*			pRenderTarge,	/* レンダーターゲット	  */
	GSuint					faceNo			/* キューブマップの面番号 */
)
{
	glBindFramebuffer(GL_FRAMEBUFFER, pRenderTarge->FrameBuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + faceNo, pRenderTarge->ColorBuffers[0]->dwTexName, 0);
}

/*=============================================================================
*
* Purpose : レンダーターゲット用のカラーバッファテクスチャをバインド．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsRenderTargetBindColorBufferTexture
(
	GSrendertarget*			pRenderTarget,	/* レンダーターゲット	*/
	GSuint					no				/* カラーバッファの番号	*/
)
{
	if (pRenderTarget->ColorBuffers[no] != NULL) {
		gsTextureBind(pRenderTarget->ColorBuffers[no]);
	}
}

/*=============================================================================
*
* Purpose : レンダーターゲット用のデプスバッファテクスチャをバインド．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsRenderTargetBindDepthBufferTexture
(
	GSrendertarget*			pRenderTarget	/* レンダーターゲット	*/
)
{
	if (pRenderTarget->DepthBuffer != NULL) {
		gsTextureBind(pRenderTarget->DepthBuffer);
	}
}

/*=============================================================================
*
* Purpose : レンダーターゲット用のカラーバッファテクスチャをアンバインド．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsRenderTargetUnbindColorBufferTexture
(
	GSrendertarget*			pRenderTarget,	/* レンダーターゲット	*/
	GSuint					no				/* カラーバッファの番号	*/
)
{
	if (pRenderTarget->ColorBuffers[no] != NULL) {
		gsTextureUnbind(pRenderTarget->ColorBuffers[no]);
	}
}

/*=============================================================================
*
* Purpose : レンダーターゲット用のデプスバッファテクスチャをアンバインド．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsRenderTargetUnbindDepthBufferTexture
(
	GSrendertarget*			pRenderTarget	/* レンダーターゲット	*/
)
{
	if (pRenderTarget->DepthBuffer != NULL) {
		gsTextureUnbind(pRenderTarget->DepthBuffer);
	}
}

/*=============================================================================
*
* Purpose : レンダーターゲットの描画
*
* Return  : なし．
*
*============================================================================*/
extern void
gsRenderTargetDraw
(
	GSrendertarget*			pRenderTarget	/* レンダーターゲット	*/
)
{
	(void)pRenderTarget;
	/* ビューポート全体に描画 */
	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f);
		glVertex2f(-1.0f, 1.0f);
		glTexCoord2f(0.0f, 0.0f);
		glVertex2f(-1.0f, -1.0f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex2f(1.0f, -1.0f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex2f(1.0f, 1.0f);
	glEnd();
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopAttrib();
}

/*=============================================================================
*
* Purpose : レンダーターゲットの描画
*
* Return  : なし．
*
*============================================================================*/
extern void
gsRenderTargetDrawEx
(
	GSrendertarget*			pRenderTarget	/* レンダーターゲット	*/
)
{
	const static GSvector2 vertices[4] = {
		{-1.0f, 1.0f}, {-1.0f, -1.0f}, {1.0f, -1.0f}, {1.0f, 1.0f}
	};
	const static GSvector2 texcoords[4] = {
		{ 0.0f, 1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}
	};
	(void)pRenderTarget;
	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_DEPTH_TEST);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(8);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, vertices);
	glVertexAttribPointer(8, 2, GL_FLOAT, GL_FALSE, 0, texcoords);
	glDrawArrays(GL_QUADS, 0, 4);
	glDisableVertexAttribArray(8);
	glDisableVertexAttribArray(0);
	glPopAttrib();
}

/*=============================================================================
*
* Purpose : レンダーターゲットの幅を取得
*
* Return  : レンダーターゲットの幅を返す．
*
*============================================================================*/
extern GSuint
gsRenderTargetGetWidth
(
	GSrendertarget*			pRenderTarget	/* レンダーターゲット	*/
)
{
	return pRenderTarget->Width;
}

/*=============================================================================
*
* Purpose : レンダーターゲットの高さを取得
*
* Return  : レンダーターゲットの高さを返す．
*
*============================================================================*/
extern GSuint
gsRenderTargetGetHeight
(
	GSrendertarget*			pRenderTarget	/* レンダーターゲット	*/
)
{
	return pRenderTarget->Height;
}

/********** End of File ******************************************************/