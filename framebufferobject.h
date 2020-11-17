#pragma once
#include "ggl.h"
struct BufferInfo {
	GLuint mBufferObject;
	GLenum mGPUFormat;
	GLenum mCPUFormat;
	GLenum mDataType;
	GLenum mAttachmentPoint;
	BufferInfo(GLuint object,GLenum gpu_format,GLenum cpu_format,GLenum data_type,GLenum attchment_point) {
		mBufferObject = object;
		mGPUFormat = gpu_format;
		mCPUFormat = cpu_format;
		mDataType = data_type;
		mAttachmentPoint = attchment_point;
	}
};
class FrameBufferObject{
public:
	GLuint mFrameBufferObject;
	GLint mPrevFrameBuffer;
	std::map<std::string, BufferInfo*> mBuffers;
	std::vector<GLenum> mDrawBuffers;
	int mWidth, mHeight;
public:
	FrameBufferObject();
	void AttachColorBuffer(const char*bufferName,GLenum attachment,int width,int height,
		GLenum gpu_format=GL_RGBA,GLenum cpu_format=GL_RGBA,GLenum data_type=GL_UNSIGNED_BYTE);
	void AttachDepthBuffer(const char*bufferName, int width, int height);
	void Finish();
	void Bind();
	void Unbind();
	GLuint GetBuffer(const char*bufferName);
	GLenum ReadBuffer(const char * buffer_name,void ** data, int&width, int&height);
};