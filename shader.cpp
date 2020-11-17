#include "shader.h"
#include "utils.h"
void Shader::Init(const char*vs, const char*fs) {
	int nFileSize = 0;
	const char*vsCode = (char*)LoadFileContent(vs,nFileSize);
	const char*fsCode = (char*)LoadFileContent(fs,nFileSize);
	GLuint vsShader = CompileShader(GL_VERTEX_SHADER, vsCode);
	if (vsShader==0){
		return;
	}
	GLuint fsShader = CompileShader(GL_FRAGMENT_SHADER, fsCode);
	if (fsShader == 0) {
		return;
	}
	OGL_CALL(mProgram=CreateProgram(vsShader, fsShader));
	OGL_CALL(glDeleteShader(vsShader));
	OGL_CALL(glDeleteShader(fsShader));
	if (mProgram!=0){
		OGL_CALL(mModelMatrixLocation = glGetUniformLocation(mProgram, "ModelMatrix"));
		OGL_CALL(mViewMatrixLocation = glGetUniformLocation(mProgram, "ViewMatrix"));
		OGL_CALL(mProjectionMatrixLocation = glGetUniformLocation(mProgram, "ProjectionMatrix"));
		OGL_CALL(mITModelMatrixLocation = glGetUniformLocation(mProgram, "U_NormalMatrix"));
		OGL_CALL(mPositionLocation = glGetAttribLocation(mProgram, "position"));
		OGL_CALL(mTangentLocation = glGetAttribLocation(mProgram, "tangent"));
		OGL_CALL(mTexcoordLocation = glGetAttribLocation(mProgram, "texcoord"));
		OGL_CALL(mNormalLocation = glGetAttribLocation(mProgram, "normal"));
	}
}
void Shader::BeginDraw(Camera *camera, const glm::mat4&model) {
	OGL_CALL(glUseProgram(mProgram));
	if (camera != nullptr) {
		if (mViewMatrixLocation != -1) {
			OGL_CALL(glUniformMatrix4fv(mViewMatrixLocation, 1, GL_FALSE, glm::value_ptr(camera->mViewMatrix)));
		}
		if (mProjectionMatrixLocation != -1) {
			OGL_CALL(glUniformMatrix4fv(mProjectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(camera->mProjectionMatrix)));
		}
	}
	if (mModelMatrixLocation != -1) {
		OGL_CALL(glUniformMatrix4fv(mModelMatrixLocation, 1, GL_FALSE, glm::value_ptr(model)));
	}
	if (mPositionLocation != -1) {
		OGL_CALL(glEnableVertexAttribArray(mPositionLocation));
		OGL_CALL(glVertexAttribPointer(mPositionLocation, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0));
	}
	if (mTexcoordLocation != -1) {
		OGL_CALL(glEnableVertexAttribArray(mTexcoordLocation));
		OGL_CALL(glVertexAttribPointer(mTexcoordLocation, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float) * 4)));
	}
	if (mNormalLocation != -1) {
		OGL_CALL(glEnableVertexAttribArray(mNormalLocation));
		OGL_CALL(glVertexAttribPointer(mNormalLocation, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float) * 8)));
	}
	if (mTangentLocation != -1) {
		OGL_CALL(glEnableVertexAttribArray(mTangentLocation));
		OGL_CALL(glVertexAttribPointer(mTangentLocation, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float) * 12)));
	}
}
void Shader::EndDraw() {
	OGL_CALL(glUseProgram(0));
}