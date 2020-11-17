#include "Geometry.h"
#include "utils.h"
Geometry::Geometry() {
	mbDrawWidthIBO = false;
	mPrimitiveType = GL_TRIANGLES;
}
void Geometry::SetIndexCount(int index_count) {
	mbDrawWidthIBO = true;
	mIndexCount = index_count;
	mIndexes = new unsigned int[index_count];
	mIBO = CreateBufferObject(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*index_count, GL_DYNAMIC_DRAW, nullptr);
}
void Geometry::SetVertexCount(int vertex_count) {
	mVertexCount = vertex_count;
	mVertices = new Vertex[vertex_count];
	mVBO = CreateBufferObject(GL_ARRAY_BUFFER, sizeof(Vertex)*vertex_count, GL_DYNAMIC_DRAW, nullptr);
}
void Geometry::SetIndex(int i, unsigned short index) {
	mIndexes[i] = index;
}
void Geometry::SetPosition(int index, float x, float y, float z, float w) {
	mVertices[index].mPosition[0] = x;
	mVertices[index].mPosition[1] = y;
	mVertices[index].mPosition[2] = z;
	mVertices[index].mPosition[3] = w;
}
void Geometry::SetTexcoord(int index, float x, float y, float z, float w) {
	mVertices[index].mTexcoord[0] = x;
	mVertices[index].mTexcoord[1] = y;
	mVertices[index].mTexcoord[2] = z;
	mVertices[index].mTexcoord[3] = w;
}
void Geometry::SetNormal(int index, float x, float y, float z, float w) {
	mVertices[index].mNormal[0] = x;
	mVertices[index].mNormal[1] = y;
	mVertices[index].mNormal[2] = z;
	mVertices[index].mNormal[3] = w;
}
void Geometry::Submit() {
	UpdateBufferObject(mVBO, GL_ARRAY_BUFFER, mVertices, sizeof(Vertex)*mVertexCount, 0);
	if (mbDrawWidthIBO) {
		UpdateBufferObject(mIBO, GL_ELEMENT_ARRAY_BUFFER, mIndexes, sizeof(unsigned int)*mIndexCount, 0);
	}
}
void Geometry::BeginDraw() {
	OGL_CALL(glBindBuffer(GL_ARRAY_BUFFER,mVBO));
	if (mbDrawWidthIBO) {
		OGL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO));
	}
}
void Geometry::Draw() {
	if (mbDrawWidthIBO) {
		OGL_CALL(glDrawElements(mPrimitiveType, mIndexCount, GL_UNSIGNED_INT, 0));
	}
	else {
		OGL_CALL(glDrawArrays(mPrimitiveType, 0, mVertexCount));
	}
}
void Geometry::EndDraw() {
	OGL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
	if (mbDrawWidthIBO) {
		OGL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}
}