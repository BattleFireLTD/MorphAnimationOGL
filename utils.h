#pragma once
#include "ggl.h"
class Camera{
public:
	glm::mat4 mViewMatrix;
	glm::mat4 mProjectionMatrix;
};
unsigned char * LoadFileContent(const char*path, int&filesize);
GLuint CompileShader(GLenum shaderType, const char*shaderCode);
GLuint CreateProgram(GLuint vsShader, GLuint fsShader);
float GetFrameTime();
unsigned char * DecodeBMP(unsigned char*bmpFileData, int&width, int&height);
void SaveRGBToBMP(const char * path, unsigned char * pixel, int width, int height);
void SaveRGBAToBMP(const char * path, unsigned char * pixel, int width, int height);
void FlipImage32F(float*pixel_data, int width, int height, int channel_count);
GLuint CreateTexture2D(unsigned char*pixelData, int width, int height, GLenum gpu_format=GL_RGB,GLenum cpu_format=GL_RGB,GLenum date_type=GL_UNSIGNED_BYTE);
GLuint CreateTextureFromFile(const char *path);
GLuint InitTextureCube(int size, GLenum gpu_format, GLenum data_type);
GLuint CreateTextureCubeFromBMP(const char **images,bool correct_with_shader=false);
GLuint CreateBufferObject(GLenum bufferType, GLsizeiptr size, GLenum usage, void*data = nullptr);
void UpdateBufferObject(GLuint object, GLenum type, void * data, int size, int offset);
void CheckLastOpenGLError(const char *prefix, const char* file, long line, const char*operation);
#define GLAssert(x) 	{ CheckLastOpenGLError (NULL,__FILE__, __LINE__,#x); }
#define OGL_CALL(x) do { x; GLAssert(x); } while(0)