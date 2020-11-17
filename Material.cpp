#include "Material.h"
#include "shader.h"
Material::Material(Shader*shader) {
    mbEnableBlend=false;
    mbEnableDepthTest=true;
	mbEnableCullFace = true;
    mBlendSrcFunc=GL_SRC_ALPHA;
    mBlendDstFunc=GL_ONE_MINUS_SRC_ALPHA;
	mShader = shader;
	mLightPosLocation = glGetUniformLocation(mShader->mProgram, "U_LightPos");
	mLightColorLocation = glGetUniformLocation(mShader->mProgram, "U_LightColor");
	mLightIntensityLocation = glGetUniformLocation(mShader->mProgram, "U_LightIntensity");
}
void Material::SetTexture(const char *property_name, GLuint texture) {
    auto iter=mTextures.find(property_name);
    TextureProperty*tp= nullptr;
    if(iter==mTextures.end()){
		GLint location = -1;
		OGL_CALL(location=glGetUniformLocation(mShader->mProgram, property_name));
        if(location!=-1){
            tp=new TextureProperty;
            tp->mLocation=location;
            tp->mTexture=texture;
            mTextures.insert(std::pair<std::string,TextureProperty*>(property_name,tp));
        }
    }else{
        tp=iter->second;
        tp->mTexture=texture;
    }
}
void Material::SetTextureCube(const char *property_name, GLuint texture) {
	auto iter = mTextureCubes.find(property_name);
	TextureProperty*tp = nullptr;
	if (iter == mTextureCubes.end()) {
		GLint location = -1;
		OGL_CALL(location = glGetUniformLocation(mShader->mProgram, property_name));
		if (location != -1) {
			tp = new TextureProperty;
			tp->mLocation = location;
			mTextureCubes.insert(std::pair<std::string, TextureProperty*>(property_name, tp));
		}
	}
	else {
		tp = iter->second;
	}
	if (tp != nullptr) {
		tp->mTexture = texture;
	}
}
void Material::SetVec4(const char *property_name,const float *v){
	SetVec4(property_name, v[0], v[1], v[2], v[3]);
}
void Material::SetLight(int index, float x, float y, float z, float w, float r, float g, float b, float a, float i) {
	mLightPos[index * 4] = x;
	mLightPos[index * 4+1] = y;
	mLightPos[index * 4+2] = z;
	mLightPos[index * 4 + 3] = w;
	mLightColor[index * 4] = r;
	mLightColor[index * 4 + 1] = g;
	mLightColor[index * 4 + 2] = b;
	mLightColor[index * 4 + 3] = a;
	mLightIntensity[index] = i;
}
void Material::SetVec4(const char *property_name, float x, float y, float z, float w) {
	auto iter = mVec4s.find(property_name);
	Vec4Property*vp = nullptr;
	if (iter == mVec4s.end()) {
		GLint location = -1;
		OGL_CALL(location = glGetUniformLocation(mShader->mProgram, property_name));
		if (location != -1) {
			vp = new Vec4Property;
			vp->mLocation = location;
			mVec4s.insert(std::pair<std::string, Vec4Property*>(property_name, vp));
		}
	}
	else {
		vp = iter->second;
	}
	if (vp != nullptr) {
		vp->v[0] = x;
		vp->v[1] = y;
		vp->v[2] = z;
		vp->v[3] = w;
	}
}
void Material::BeginDraw(Camera*camera,const glm::mat4 &m) {
    if(mbEnableDepthTest){
		OGL_CALL(glEnable(GL_DEPTH_TEST));
    }else{
		OGL_CALL(glDisable(GL_DEPTH_TEST));
    }
    if(mbEnableBlend){
		OGL_CALL(glEnable(GL_BLEND));
		OGL_CALL(glBlendFunc(mBlendSrcFunc,mBlendDstFunc));
    }else{
		OGL_CALL(glDisable(GL_BLEND));
    }
	if (mbEnableCullFace){
		OGL_CALL(glEnable(GL_CULL_FACE));
	}
	else {
		OGL_CALL(glDisable(GL_CULL_FACE));
	}
    mShader->BeginDraw(camera,m);
    if(mShader->mITModelMatrixLocation!=-1){
        glm::mat4 normal_matrix=glm::inverseTranspose(m);
        OGL_CALL(glUniformMatrix4fv(mShader->mITModelMatrixLocation,1,GL_FALSE,glm::value_ptr(normal_matrix)));
    }
    int texture_unit_index=0;
    for (auto iter = mTextures.begin(); iter!=mTextures.end() ; ++iter) {
        OGL_CALL(glActiveTexture(GL_TEXTURE0+texture_unit_index));
        OGL_CALL(glBindTexture(GL_TEXTURE_2D,iter->second->mTexture));
        OGL_CALL(glUniform1i(iter->second->mLocation,texture_unit_index++));
	}
	for (auto iter = mTextureCubes.begin(); iter != mTextureCubes.end(); ++iter) {
		OGL_CALL(glActiveTexture(GL_TEXTURE0 + texture_unit_index));
		OGL_CALL(glBindTexture(GL_TEXTURE_CUBE_MAP, iter->second->mTexture));
		OGL_CALL(glUniform1i(iter->second->mLocation, texture_unit_index++));
	}
    for (auto iter = mVec4s.begin(); iter!=mVec4s.end() ; ++iter) {
        Vec4Property*vp=iter->second;
		OGL_CALL(glUniform4fv(vp->mLocation,1,vp->v));
    }
	if (mLightPosLocation!=-1){
		OGL_CALL(glUniform4fv(mLightPosLocation,4,mLightPos));
	}
	if (mLightColorLocation != -1) {
		OGL_CALL(glUniform4fv(mLightColorLocation, 4, mLightColor));
	}
	if (mLightIntensityLocation != -1) {
		OGL_CALL(glUniform4fv(mLightIntensityLocation, 1, mLightIntensity));
	}
}
void Material::EndDraw() {
    mShader->EndDraw();
}
Material*Material::Clone() {
	Material*ret = new Material(mShader);
	ret->mBlendSrcFunc = mBlendSrcFunc;
	ret->mBlendDstFunc = mBlendDstFunc;
	ret->mbEnableBlend = mbEnableBlend;
	ret->mbEnableDepthTest = mbEnableDepthTest;
	ret->mbEnableCullFace = mbEnableCullFace;
	for (auto iter = mTextures.begin(); iter != mTextures.end(); ++iter) {
		ret->SetTexture(iter->first.c_str(), iter->second->mTexture);
	}
	for (auto iter = mTextureCubes.begin(); iter != mTextureCubes.end(); ++iter) {
		ret->SetTextureCube(iter->first.c_str(), iter->second->mTexture);
	}
	for (auto iter = mVec4s.begin(); iter != mVec4s.end(); ++iter) {
		ret->SetVec4(iter->first.c_str(), iter->second->v);
	}
	return ret;
}