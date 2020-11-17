#pragma once
#include "ggl.h"
#include "framebufferobject.h"
#include "Material.h"
#include "Geometry.h"
#include "SceneNode.h"
class TextureCubeMap {
public:
	GLuint mCaptureFBO, mCaptureRBO;
	GLuint mEnvironmentMap;//processed cube map
	GLint mPrevFrameBuffer;
	int mTextureCubeFBOResolution, mCaptureFBOResolution;
	FrameBufferObject * mEnvironment;//rtt
	GLuint mEnvironmentTextures[6];
	SceneNode mTextureCubeMapOriginal[6], mTextureCubeMapProcessed[6],mOriginalFace,mProcessedFace;
public:
	TextureCubeMap();
	void InitTextureCubeFBO(int resolution = 512,bool gen_mipmap=false,GLenum gpu_format=GL_RGB16F,GLenum data_type=GL_FLOAT);
	void InitCaptureFBO(int resolution, GLuint * textures, Material*linear_material, Material*sRGB_material);
	void InitDebugGeometries(SceneNode*nodes,Material*material,GLuint *textures,float offset);
	void Bind();
	void Unbind();
	void BeginRenderFace(int i, int mipmap_level=0);
	void EndRenderFace(int i);
	void DebugTextureCube();
	void DebugTextureCubeFace(int i);
};