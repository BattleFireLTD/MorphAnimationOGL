#include "framebufferobject.h"
#include "utils.h"
FrameBufferObject::FrameBufferObject(){
	glGenFramebuffers(1, &mFrameBufferObject);
	mWidth = 0;
	mHeight = 0;
}
void FrameBufferObject::AttachColorBuffer(const char*bufferName, GLenum attachment, int width, int height, GLenum gpu_format, GLenum cpu_format, GLenum data_type){
	GLuint colorBuffer;
	glBindFramebuffer(GL_FRAMEBUFFER, mFrameBufferObject);
	glGenTextures(1, &colorBuffer);
	glBindTexture(GL_TEXTURE_2D, colorBuffer);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, gpu_format, width, height, 0, cpu_format, data_type, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, colorBuffer, 0);
	BufferInfo*bi = new BufferInfo(colorBuffer, gpu_format, cpu_format, data_type, attachment);
	mDrawBuffers.push_back(attachment);
	mBuffers.insert(std::pair<std::string,BufferInfo*>(bufferName, bi));
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	mWidth = width;
	mHeight = height;
}
void FrameBufferObject::AttachDepthBuffer(const char*bufferName, int width, int height){
	GLuint depthMap;
	glBindFramebuffer(GL_FRAMEBUFFER, mFrameBufferObject);
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	BufferInfo*bi = new BufferInfo(depthMap, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT, GL_DEPTH_ATTACHMENT);
	mBuffers.insert(std::pair<std::string, BufferInfo*>(bufferName, bi));
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	mWidth = width;
	mHeight = height;
}
void FrameBufferObject::Finish(){
	int nCount = (int)mDrawBuffers.size();
	if (nCount>0){
		GLenum *buffers = new GLenum[nCount];
		int i = 0;
		while (i<nCount){
			buffers[i] = mDrawBuffers[i];
            i++;
		}
		glBindFramebuffer(GL_FRAMEBUFFER, mFrameBufferObject);
		glDrawBuffers(nCount, buffers);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}
void FrameBufferObject::Bind(){
	OGL_CALL(glGetIntegerv(GL_FRAMEBUFFER_BINDING, &mPrevFrameBuffer));
	OGL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, mFrameBufferObject));
	OGL_CALL(glViewport(0,0,mWidth, mHeight));
	OGL_CALL(glClearColor(0.0f, 0.0f, 0.0f, 0.0f));
	OGL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}
void FrameBufferObject::Unbind(){
	OGL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, mPrevFrameBuffer));
}
GLuint FrameBufferObject::GetBuffer(const char*bufferName){
	auto iter = mBuffers.find(bufferName);
	if (iter!=mBuffers.end()){
		return iter->second->mBufferObject;
	}
	return 0;
}
GLenum FrameBufferObject::ReadBuffer(const char * buffer_name, void ** data, int&width, int&height) {
	auto iter = mBuffers.find(buffer_name);
	if (iter != mBuffers.end()) {
		width = mWidth;
		height = mHeight;
		OGL_CALL(glGetIntegerv(GL_FRAMEBUFFER_BINDING, &mPrevFrameBuffer));
		OGL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, mFrameBufferObject));
		OGL_CALL(glReadBuffer(iter->second->mAttachmentPoint));
		if (iter->second->mDataType==GL_UNSIGNED_BYTE){
			if (iter->second->mGPUFormat == GL_RGBA) {
				*data = new unsigned char[width*height*4];
				OGL_CALL(glReadPixels(0, 0, width, height, iter->second->mGPUFormat, iter->second->mDataType, *data));
			}
		}else if (iter->second->mDataType==GL_FLOAT){
			if (iter->second->mGPUFormat == GL_RGBA16F) {
				*data = new unsigned short[width*height * 4];
				OGL_CALL(glReadPixels(0, 0, width, height, iter->second->mGPUFormat, iter->second->mDataType, *data));
			}
			else if (iter->second->mGPUFormat == GL_RGBA32F) {
				*data = new float[width*height * 4];
				OGL_CALL(glReadPixels(0, 0, width, height, iter->second->mGPUFormat, iter->second->mDataType, *data));
			}
		}
		Unbind();
	}
	return GL_NONE;
}
