#include "TextureCubeMap.h"
#include "utils.h"
#include "Geometry.h"
TextureCubeMap::TextureCubeMap() {
	mEnvironment = nullptr;
	mTextureCubeFBOResolution = 512;
	mCaptureFBOResolution = 256;
}
void TextureCubeMap::InitTextureCubeFBO(int resolution, bool gen_mipmap, GLenum gpu_format, GLenum data_type) {
	mTextureCubeFBOResolution = resolution;
	OGL_CALL(glGenFramebuffers(1, &mCaptureFBO));
	OGL_CALL(glGenRenderbuffers(1, &mCaptureRBO));
	OGL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, mCaptureFBO));
	OGL_CALL(glBindRenderbuffer(GL_RENDERBUFFER, mCaptureRBO));
	OGL_CALL(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, mTextureCubeFBOResolution, mTextureCubeFBOResolution));
	OGL_CALL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mCaptureRBO));
	OGL_CALL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mCaptureRBO));
	OGL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	OGL_CALL(glBindRenderbuffer(GL_RENDERBUFFER, 0));
	mEnvironmentMap = InitTextureCube(mTextureCubeFBOResolution, gpu_format, data_type);
	if (gen_mipmap) {
		OGL_CALL(glBindTexture(GL_TEXTURE_CUBE_MAP, mEnvironmentMap));
		OGL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
		OGL_CALL(glGenerateMipmap(GL_TEXTURE_CUBE_MAP));
		OGL_CALL(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
	}
}
void TextureCubeMap::InitCaptureFBO(int resolution,GLuint * textures, Material*linear_material, Material*sRGB_material) {
	memcpy(mEnvironmentTextures, textures, sizeof(GLuint)*6);
	mCaptureFBOResolution = resolution;
	mEnvironment = new FrameBufferObject[6];
	for (int i = 0; i < 6; ++i) {
		mEnvironment[i].AttachColorBuffer("color", GL_COLOR_ATTACHMENT0, mCaptureFBOResolution, mCaptureFBOResolution);
		mEnvironment[i].AttachDepthBuffer("depth", mCaptureFBOResolution, mCaptureFBOResolution);
		mEnvironment[i].Finish();
	}
	Material*render_material = linear_material == nullptr ? sRGB_material : linear_material;
	GLuint environment_color_buffers[] = {
		mEnvironment[0].GetBuffer("color"),
		mEnvironment[1].GetBuffer("color"),
		mEnvironment[2].GetBuffer("color"),
		mEnvironment[3].GetBuffer("color"),
		mEnvironment[4].GetBuffer("color"),
		mEnvironment[5].GetBuffer("color")
	};
	InitDebugGeometries(mTextureCubeMapProcessed, render_material, environment_color_buffers, 0.0f);
	InitDebugGeometries(mTextureCubeMapOriginal, sRGB_material,textures,-1.0f);
}
void TextureCubeMap::InitDebugGeometries(SceneNode*nodes, Material*material, GLuint *textures, float offset) {
	Geometry * quad = new Geometry;
	quad->SetVertexCount(4);
	quad->SetPosition(0, 0.75f + offset, -0.125f, 0.0f);
	quad->SetPosition(1, 1.0f + offset, -0.125f, 0.0f);
	quad->SetPosition(2, 0.75f + offset, 0.125f, 0.0f);
	quad->SetPosition(3, 1.0f + offset, 0.125f, 0.0f);
	quad->SetTexcoord(0, 0.0f, 0.0f);
	quad->SetTexcoord(1, 1.0f, 0.0f);
	quad->SetTexcoord(2, 0.0f, 1.0f);
	quad->SetTexcoord(3, 1.0f, 1.0f);
	quad->Submit();
	quad->mPrimitiveType = GL_TRIANGLE_STRIP;
	nodes[0].mGeometry = quad;
	nodes[0].mMaterial = material->Clone();
	nodes[0].mMaterial->SetTexture("U_Texture", textures[0]);

	quad = new Geometry;
	quad->SetVertexCount(4);
	quad->SetPosition(0, 0.25f + offset, -0.125f, 0.0f);
	quad->SetPosition(1, 0.5f + offset, -0.125f, 0.0f);
	quad->SetPosition(2, 0.25f + offset, 0.125f, 0.0f);
	quad->SetPosition(3, 0.5f + offset, 0.125f, 0.0f);
	quad->SetTexcoord(0, 0.0f, 0.0f);
	quad->SetTexcoord(1, 1.0f, 0.0f);
	quad->SetTexcoord(2, 0.0f, 1.0f);
	quad->SetTexcoord(3, 1.0f, 1.0f);
	quad->Submit();
	quad->mPrimitiveType = GL_TRIANGLE_STRIP;
	nodes[1].mGeometry = quad;
	nodes[1].mMaterial = material->Clone();
	nodes[1].mMaterial->SetTexture("U_Texture", textures[1]);

	quad = new Geometry;
	quad->SetVertexCount(4);
	quad->SetPosition(0, 0.5f + offset, 0.125f, 0.0f);
	quad->SetPosition(1, 0.75f + offset, 0.125f, 0.0f);
	quad->SetPosition(2, 0.5f + offset, 0.375f, 0.0f);
	quad->SetPosition(3, 0.75f + offset, 0.375f, 0.0f);
	quad->SetTexcoord(0, 0.0f, 0.0f);
	quad->SetTexcoord(1, 1.0f, 0.0f);
	quad->SetTexcoord(2, 0.0f, 1.0f);
	quad->SetTexcoord(3, 1.0f, 1.0f);
	quad->Submit();
	quad->mPrimitiveType = GL_TRIANGLE_STRIP;
	nodes[2].mGeometry = quad;
	nodes[2].mMaterial = material->Clone();
	nodes[2].mMaterial->SetTexture("U_Texture", textures[2]);

	quad = new Geometry;
	quad->SetVertexCount(4);
	quad->SetPosition(0, 0.5f + offset, -0.375f, 0.0f);
	quad->SetPosition(1, 0.75f + offset, -0.375f, 0.0f);
	quad->SetPosition(2, 0.5f + offset, -0.125f, 0.0f);
	quad->SetPosition(3, 0.75f + offset, -0.125f, 0.0f);
	quad->SetTexcoord(0, 0.0f, 0.0f);
	quad->SetTexcoord(1, 1.0f, 0.0f);
	quad->SetTexcoord(2, 0.0f, 1.0f);
	quad->SetTexcoord(3, 1.0f, 1.0f);
	quad->Submit();
	quad->mPrimitiveType = GL_TRIANGLE_STRIP;
	nodes[3].mGeometry = quad;
	nodes[3].mMaterial = material->Clone();
	nodes[3].mMaterial->SetTexture("U_Texture", textures[3]);

	quad = new Geometry;
	quad->SetVertexCount(4);
	quad->SetPosition(0, 0.5f + offset, -0.15f, 0.0f);
	quad->SetPosition(1, 0.75f + offset, -0.15f, 0.0f);
	quad->SetPosition(2, 0.5f + offset, 0.15f, 0.0f);
	quad->SetPosition(3, 0.75f + offset, 0.15f, 0.0f);
	quad->SetTexcoord(0, 0.0f, 0.0f);
	quad->SetTexcoord(1, 1.0f, 0.0f);
	quad->SetTexcoord(2, 0.0f, 1.0f);
	quad->SetTexcoord(3, 1.0f, 1.0f);
	quad->Submit();
	quad->mPrimitiveType = GL_TRIANGLE_STRIP;
	nodes[4].mGeometry = quad;
	nodes[4].mMaterial = material->Clone();
	nodes[4].mMaterial->SetTexture("U_Texture", textures[4]);

	quad = new Geometry;
	quad->SetVertexCount(4);
	quad->SetPosition(0, 0.0f + offset, -0.125f, 0.0f);
	quad->SetPosition(1, 0.25f + offset, -0.125f, 0.0f);
	quad->SetPosition(2, 0.0f + offset, 0.125f, 0.0f);
	quad->SetPosition(3, 0.25f + offset, 0.125f, 0.0f);
	quad->SetTexcoord(0, 0.0f, 0.0f);
	quad->SetTexcoord(1, 1.0f, 0.0f);
	quad->SetTexcoord(2, 0.0f, 1.0f);
	quad->SetTexcoord(3, 1.0f, 1.0f);
	quad->Submit();
	quad->mPrimitiveType = GL_TRIANGLE_STRIP;
	nodes[5].mGeometry = quad;
	nodes[5].mMaterial = material->Clone();
	nodes[5].mMaterial->SetTexture("U_Texture", textures[5]);
}
void TextureCubeMap::Bind() {
	OGL_CALL(glGetIntegerv(GL_FRAMEBUFFER_BINDING, &mPrevFrameBuffer));
	OGL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, mCaptureFBO));
	OGL_CALL(glViewport(0, 0, mTextureCubeFBOResolution, mTextureCubeFBOResolution));
	OGL_CALL(glClearColor(0.0f, 0.0f, 0.0f, 0.0f));
}
void TextureCubeMap::BeginRenderFace(int i, int mipmap_level) {
	switch (i)
	{
	case 0:
		OGL_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X, mEnvironmentMap, mipmap_level));
		break;
	case 1:
		OGL_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, mEnvironmentMap, mipmap_level));
		break;
	case 2:
		OGL_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, mEnvironmentMap, mipmap_level));
		break;
	case 3:
		OGL_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, mEnvironmentMap, mipmap_level));
		break;
	case 4:
		OGL_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, mEnvironmentMap, mipmap_level));
		break;
	case 5:
		OGL_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, mEnvironmentMap, mipmap_level));
		break;
	}
	OGL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}
void TextureCubeMap::EndRenderFace(int i) {
	//glReadBuffer(GL_COLOR_ATTACHMENT0);
	//float * pixel_data = new float[mResolution*mResolution * 4];
	//glReadPixels(0, 0, mResolution, mResolution, GL_RGB32F, GL_FLOAT, pixel_data);
	//FlipImage32F(pixel_data, mResolution, mResolution, 3);
	//glBindTexture(GL_TEXTURE_CUBE_MAP, mEnvironmentMap);
	//glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB32F, mResolution, mResolution, 0, GL_RGB32F, GL_FLOAT, pixel_data);
	//delete[] pixel_data;
	//glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}
void TextureCubeMap::Unbind() {
	OGL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, mPrevFrameBuffer));
}
void TextureCubeMap::DebugTextureCube() {
	for (int i=0;i<6;++i){
		mTextureCubeMapOriginal[i].Render(nullptr);
		mTextureCubeMapProcessed[i].Render(nullptr);
	}
}
void TextureCubeMap::DebugTextureCubeFace(int i) {
	mOriginalFace.mMaterial->SetTexture("U_Texture", mEnvironmentTextures[i]);
	mProcessedFace.mMaterial->SetTexture("U_Texture", mEnvironment[i].GetBuffer("color"));
	mOriginalFace.Render(nullptr);
	mProcessedFace.Render(nullptr);
}