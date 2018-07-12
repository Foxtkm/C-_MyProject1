/******************************************************************************
* File Name : GSshader.c                         Ver : 1.00  Date : 2016-03-22
*
* Description :
*
*       シェーダー．
*
* Author : Moriharu Ohzu.
*
******************************************************************************/
#include "GSshader.h"
#include "heap.h"
#include <stdio.h>
#include <opengl_ext.h>
#include <assert.h>

/* GLSLシェーダー構造体 */
typedef struct
{
	GLenum Type;		/* シェーダータイプ */
	GLuint Shader;		/* シェーダー		*/
} GSshader;

static GSshader* gsShaderNew(GLenum type);
static void gsShaderDelete(GSshader* shader);
static GLint gsShaderComplie(GSshader* shader, const char* fileName);

/* シェーダープログラムの生成 */
GSshaderProgram* gsShaderProgramNew(void) {
	GSshaderProgram* program;
	program = new_object(sizeof(GSshaderProgram));
	program->program = glCreateProgram();
	return program;
}

/* シェーダープログラムの削除 */
void gsShaderProgramDelete(GSshaderProgram* program) {
	glDeleteProgram(program->program);
	del_object(program);
}

/* シェーダーのコンパイル */
GSboolean gsShaderProgramCompleShader(GSshaderProgram* program, GLenum type, const char* fileName) {
	// シェーダーの作成
	GSshader* shader;
	GLint compled;
	shader = gsShaderNew(type);
	compled = gsShaderComplie(shader, fileName);
	if (compled == GL_FALSE) {
		gsShaderDelete(shader);
		return GS_FALSE;
	}
	// シェーダーをアタッチ
	glAttachShader(program->program, shader->Shader);
	gsShaderDelete(shader);
	return GS_TRUE;
}

/* シェーダーのリンク */
GSboolean gsShaderProgramLinkShader(GSshaderProgram* program) {
	// シェーダーのリンク
	glLinkProgram(program->program);
	// リンクエラーの確認
	GLint linked = 0;
	glGetProgramiv(program->program, GL_LINK_STATUS, &linked);
	if (linked == GL_FALSE) {
		// コンパイルエラーを出力
		GLint length = 0;
		GLchar* log = NULL;
		glGetProgramiv(program->program, GL_INFO_LOG_LENGTH, &length);
		log = new_object(length);
		glGetProgramInfoLog(program->program, length, 0, log);
		OutputDebugStringA("glsl link error\n");
		OutputDebugStringA(log);
		del_object(log);
		assert(!"glsl link error");
		return GS_FALSE;
	}
	return GS_TRUE;
}

/* プログラムの開始 */
void gsShaderProgramBegin(GSshaderProgram* program) {
	glUseProgram(program->program);
}

/* プログラムの終了 */
void gsShaderProgramEnd(GSshaderProgram* program) {
	(void)program;
	glUseProgram(0);
}

/* シェーダーパラメータの取得 */
GLint gsShaderProgramUniform(GSshaderProgram* program, const char* name) {
	return glGetUniformLocation(program->program, name);
}

/* UniformBlockインデックスの取得 */
GLint gsShaderProgramUniformBlock(GSshaderProgram* program, const char* name) {
	return glGetUniformBlockIndex(program->program, name);
}

/* アトリビュートの取得 */
GLint gsShaderProgramAttribute(GSshaderProgram* program, const char* name) {
	return glGetAttribLocation(program->program, name);
}

/* アトリビュートの設定 */
void gsShaderBindAttribute(GSshaderProgram* program, GLuint index, const char* name) {
	glBindAttribLocation(program->program, index, name);
}

/* フラグメントデータの設定 */
void gsShaderBindFragData(GSshaderProgram* program, GLuint color, const char* name) {
	glBindFragDataLocation(program->program, color, name);
}

// シェーダーファイルの読み込み
static GLint gsShaderLoadFile(const char* fileName, GLchar** program, GLint* length) {
	FILE*	fp;
	if ((fp = fopen(fileName, "rb")) == NULL) {
		OutputDebugStringA("シェーダーファイルが開けません!\n");
		OutputDebugStringA(fileName);
		OutputDebugStringA("\n");
		assert(!"file open error");
		return GL_FALSE;
	}
	fseek(fp,0, SEEK_END);
	*length = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	*program = new_object(*length);
	fread(*program, sizeof(char), *length, fp);
	fclose(fp);
	return GL_TRUE;
}

// シェーダーの生成
static GSshader* gsShaderNew(GLenum type) {
	GSshader* shader;
	shader = new_object(sizeof(GSshader));
	shader->Type = type;
	shader->Shader = glCreateShader(type);
	return shader;
}

// シェーダーの削除
static void gsShaderDelete(GSshader* shader) {
	glDeleteShader(shader->Shader);
	del_object(shader);
}

// シェーダーのコンパイル
static GLint gsShaderComplie(GSshader* shader, const char* fileName) {
	GLchar* program;
	GLint	length;
	GLint	compiled;
	GLint	error;
	/* シェーダーのコンパイル */
	error = gsShaderLoadFile(fileName, &program, &length);
	if (error == GL_FALSE) return GL_FALSE;
	glShaderSource(shader->Shader, 1, &program, &length);
	glCompileShader(shader->Shader);
	del_object(program);
	/* コンパイルエラーのチェック */
	glGetShaderiv(shader->Shader, GL_COMPILE_STATUS, &compiled);
	if (compiled == GL_FALSE) {
		// コンパイルエラーを出力
		GLchar* log = NULL;
		glGetShaderiv(shader->Shader, GL_INFO_LOG_LENGTH, &length);
		log = new_object(length);
		glGetShaderInfoLog(shader->Shader, length, 0, log);
		glDeleteShader(shader->Shader);
		shader->Shader = 0;
		OutputDebugStringA("glsl compile error\n");
		OutputDebugStringA(fileName);
		OutputDebugStringA("\n");
		OutputDebugStringA(log);
		del_object(log);
		assert(!"glsl compile error");
		return GL_FALSE;
	}
	return GL_TRUE;
}

/********** End of File ******************************************************/
