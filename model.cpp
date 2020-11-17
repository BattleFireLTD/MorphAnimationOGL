#include "model.h"
#include "utils.h"
void Model::SetModelPath(const char*path) {
	FILE *file = fopen(path, "rb");
	if (file != NULL) {
		int vertice_count;
		fread(&vertice_count, 1, sizeof(int), file);
		SetVertexCount(vertice_count);
		fread(mVertices, 1, sizeof(Vertex)*vertice_count, file);
		fread(&mIndexCount, 1, sizeof(int), file);
		SetIndexCount(mIndexCount);
		fread(mIndexes, 1, sizeof(unsigned int)*mIndexCount, file);
		fclose(file);
	}
	Submit();
}