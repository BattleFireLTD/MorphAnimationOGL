#pragma once
#include "ggl.h"
class Geometry {
public:
	GLuint mVBO;
	GLuint mIBO;
	int mVertexCount;
	int mIndexCount;
	bool mbDrawWidthIBO;
	GLenum mPrimitiveType;
	Vertex*mVertices;
	unsigned int *mIndexes;
	Geometry();
	virtual void Init() {}
	void SetVertexCount(int vertex_count);
	void SetIndexCount(int index_count);
	void SetPosition(int index, float x, float y, float z, float w = 1.0f);
	void SetTexcoord(int index, float x, float y, float z=1.0f, float w = 1.0f);
	void SetNormal(int index, float x, float y, float z, float w = 0.0f);
	void SetIndex(int i, unsigned short index);
	void Submit();
	virtual void Update(float delta) {}
	virtual void BeginDraw();
	virtual void Draw();
	virtual void EndDraw();
};
