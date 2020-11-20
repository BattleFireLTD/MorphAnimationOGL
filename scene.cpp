#include "scene.h"
#include "ggl.h"
#include "utils.h"
#include "shader.h"
#include "Material.h"
#include "model.h"
#include "SceneNode.h"
#include "framebufferobject.h"
#include "fullscreenquad.h"
#include "TextureCubeMap.h"
#pragma comment(lib,"glu32.lib")
static int sCanvasWidth = 0, sCanvasHeight = 0;
static SceneNode*sRootNode1 = nullptr, *sRootNode2 = nullptr;
Camera sMainCamera;
Shader *s_cube;
Material *m_cube;
Model *model_cube;
void AddSceneNode(SceneNode**root, SceneNode*node) {
	if (*root==nullptr){
		*root = node;
	}
	else {
		//while ((*root)->mNext!=nullptr){
		//	(*root) = (*root)->Next<SceneNode>();
		//}
		(*root)->Append(node);
	}
}
void AddSceneNodeToRoot1(SceneNode*node) {
	if (sRootNode1 == nullptr) {
		sRootNode1 = node;
	}
	else {
		SceneNode*last_node = sRootNode1;
		while (last_node->mNext != nullptr) {
			last_node = last_node->Next<SceneNode>();
		}
		last_node->Append(node);
	}
}
void AddSceneNodeToRoot2(SceneNode*node) {
	AddSceneNode(&sRootNode2, node);
}
void InitGeometries() {
	model_cube = new Model;
	model_cube->SetModelPath("Res/Model/Cube.raw");
}
void InitShaders() {
	s_cube = new Shader;
	s_cube->Init("Res/Shader/Cube.vs", "Res/Shader/Cube.fs");
}
void InitMaterials() {
	m_cube = new Material(s_cube);
}
glm::vec3 InitWave(float u, float v, float t) {
	return glm::vec3(u, sinf(PI * (u + v + t)), v);
}
glm::vec3 InitMultiWave(float u, float v, float t) {
	float y = sinf(PI * (u + 0.5f * t));
	y += 0.5f * sinf(2.0f * PI * (v + t));
	y += sinf(PI * (u + v + 0.25f * t));
	y *= 1.0f / 2.5f;
	return glm::vec3(u, y, v);
}
glm::vec3 InitRipple(float u, float v, float t) {
	float d = sqrtf(u * u + v * v);
	float y = sinf(PI * (4.0f * d - t));
	y /= 1.0f + 10.0f * d;
	return glm::vec3(u, y, v);
}
glm::vec3 InitSphere(float u, float v, float t) {
	float r = 0.9f + 0.1f * sinf(PI * (6.0f * u + 4.0f * v + t));
	float s = r * cosf(0.5f * PI * v);
	return glm::vec3(s * sinf(PI * u), r * sinf(PI * 0.5f * v), s * cosf(PI * u));
}
glm::vec3 InitTorus(float u, float v, float t) {
	float r1 = 0.7f + 0.1f * sinf(PI * (6.0f * u + 0.5f * t));
	float r2 = 0.15f + 0.05f * sinf(PI * (8.0f * u + 4.0f * v + 2.0f * t));
	float s = r1 + r2 * cosf(PI * v);
	return glm::vec3(s * sinf(PI * u), r2 * sinf(PI * v), s * cosf(PI * u));
}
void InitDrawCommand() {
	int size = 40;
	float step = 2.0f / float(size);
	glm::mat4 scale = glm::scale(step, step, step);
	for (int z = 0; z < size; ++z){
		for (int x = 0; x < size; ++x) {
			float u = (x + 0.5f)*step - 1.0f;
			float v = (z + 0.5f)*step - 1.0f;
			SceneNode *node = new SceneNode;
			node->mGeometry = model_cube;
			node->mMaterial = m_cube;
			node->mScaleMatrix = scale;
			node->mUV = glm::vec2(u,v);
			AddSceneNodeToRoot1(node);
		}
	}
}
void Init() {
	InitGeometries();
	InitShaders();
	InitMaterials();
	InitDrawCommand();
}
void SetViewPortSize(float width, float height) {
	sCanvasWidth = int(width);
	sCanvasHeight = int(height);
	sMainCamera.mProjectionMatrix = glm::perspective(45.0f, width / height, 0.1f, 1000.0f);
	sMainCamera.mViewMatrix = glm::lookAt(glm::vec3(0.0f, 2.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}
glm::vec3 GetPosition(int geometry_index, float u, float v, float t) {
	switch (geometry_index){
	case 0:
		return InitWave(u,v , t);
	case 1:
		return InitMultiWave(u, v, t);
	case 2:
		return InitRipple(u, v, t);
	case 3:
		return InitSphere(u, v, t);
	case 4:
		return InitTorus(u, v, t);
	}
}
void Draw() {
	float frameTime = GetFrameTime();
	static float timeSinceStart = 0.0f;
	static float animationTime = 0.0f;
	static int current_geometry = 0;
	static int next_geometry = 0;
	static float transition_time = 0.0f;
	OGL_CALL(glClearColor(0.1f, 0.4f, 0.6f, 1.0f));
	OGL_CALL(glViewport(0, 0, sCanvasWidth, sCanvasHeight));
	OGL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	if (sRootNode1 != nullptr) {
		SceneNode*node = sRootNode1;
		while (node != nullptr) {
			glm::vec3 current_pos = GetPosition(current_geometry, node->mUV.x, node->mUV.y, timeSinceStart);
			glm::vec3 next_pos = GetPosition(next_geometry, node->mUV.x, node->mUV.y, timeSinceStart);
			glm::vec3 pos = glm::lerp(current_pos, next_pos, glm::clamp(transition_time/0.5f,0.0f,1.0f));
			node->mModelMatrix = glm::translate(pos)*node->mScaleMatrix;
			node->Render(&sMainCamera);
			node = node->Next<SceneNode>();
		}
	}
	animationTime += frameTime;
	transition_time += frameTime;
	timeSinceStart += frameTime;
	if (animationTime>2.0f){
		animationTime -= 2.0f;
		current_geometry = (current_geometry + 1) % 5;
		next_geometry = (current_geometry + 1) % 5;
		transition_time = 0.0f;
	}
}
void OnKeyDown(int key) {
	switch (key) {
	case 'W':
		//model.mPBRMaterial.x += 0.1f;
		break;
	case 'S':
		//model.mPBRMaterial.x -= 0.1f;
		break;
	case 'D':
		//model.mPBRMaterial.y -= 0.1f;
		break;
	case 'A':
		//model.mPBRMaterial.y += 0.1f;
		break;
	}
	//model.mShader->SetVec4("U_Material", model.mPBRMaterial.x, model.mPBRMaterial.y, 0.0f, 0.0f);
	//printf("roughness: %f metallic: %f\n", model.mPBRMaterial.x, model.mPBRMaterial.y);
}
void OnKeyUp(int key) {

}