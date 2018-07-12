/******************************************************************************
* File Name : GSshader.c                         Ver : 1.00  Date : 2016-03-22
*
* Description :
*
*       シェーダー ヘッダファイル．
*
* Author : Moriharu Ohzu.
*
******************************************************************************/
#ifndef	_GS_SHADER_H_
#define	_GS_SHADER_H_

#include	"GStype.h"

/* GLSLプログラム */
typedef struct
{
	GLuint program;		/* GLSLプログラム　*/
} GSshaderProgram;

#ifdef __cplusplus
extern "C" {
#endif

/* シェーダープログラムの生成 */
GSshaderProgram* gsShaderProgramNew(void);

/* シェーダープログラムの削除 */
void gsShaderProgramDelete(GSshaderProgram* program);

/* シェーダーのコンパイル */
GSboolean gsShaderProgramCompleShader(GSshaderProgram* program, GLenum type, const char* fileName);

/* シェーダーのリンク */
GSboolean gsShaderProgramLinkShader(GSshaderProgram* program);

/* プログラムの開始 */
void gsShaderProgramBegin(GSshaderProgram* program);

/* プログラムの終了 */
void gsShaderProgramEnd(GSshaderProgram* program);

/* シェーダーパラメータの取得 */
GLint gsShaderProgramUniform(GSshaderProgram* program, const char* name);

/* UniformBlockインデックスの取得 */
GLint gsShaderProgramUniformBlock(GSshaderProgram* program, const char* name);

/* アトリビュートの取得 */
GLint gsShaderProgramAttribute(GSshaderProgram* program, const char* name);

/* アトリビュートの設定 */
void gsShaderBindAttribute(GSshaderProgram* program, GLuint index, const char* name);

/* フラグメントデータの設定 */
void gsShaderBindFragData(GSshaderProgram* program, GLuint color, const char* name);


#ifdef __cplusplus
}
#endif

#endif

/********** End of File ******************************************************/
