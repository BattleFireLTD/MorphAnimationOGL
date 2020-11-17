#pragma once
#include "utils.h"
class Shader;
class Camera;
struct TextureProperty{
    GLint mLocation;
    GLuint mTexture;
    TextureProperty(){
        mTexture=0;
    }
};
struct Vec4Property{
    GLint mLocation;
    float v[4];
    Vec4Property(){
        memset(v,0,sizeof(float)*4);
    }
};
class Material{
public:
    Shader *mShader;
    std::unordered_map<std::string,TextureProperty*> mTextures,mTextureCubes;
    std::unordered_map<std::string,Vec4Property*> mVec4s;
    GLenum mBlendSrcFunc,mBlendDstFunc;
    bool mbEnableBlend;
    bool mbEnableDepthTest;
	bool mbEnableCullFace;
	float mLightPos[16];
	float mLightColor[16];
	float mLightIntensity[4];
	GLint mLightPosLocation, mLightColorLocation, mLightIntensityLocation;
    Material(Shader*shader);
	void SetTexture(const char * property_name, GLuint texture);
	void SetTextureCube(const char * property_name, GLuint texture);
	void SetVec4(const char * property_name, const float*v);
	void SetVec4(const char * property_name, float x, float y, float z, float w);
	void SetLight(int index,float x, float y, float z, float w, float r, float g, float b, float a, float i);
    void BeginDraw(Camera*camera, const glm::mat4&m);
    void EndDraw();
	Material*Clone();
};
