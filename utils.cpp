#include "utils.h"
#include "stbi/stb_image_aug.h"
GLuint CompileShader(GLenum shaderType, const char*shaderCode) {
	GLuint shader = 0;
	OGL_CALL(shader = glCreateShader(shaderType));
	OGL_CALL(glShaderSource(shader, 1, &shaderCode, nullptr));
	OGL_CALL(glCompileShader(shader));
	GLint compileResult = GL_TRUE;
	OGL_CALL(glGetShaderiv(shader, GL_COMPILE_STATUS, &compileResult));
	if (compileResult == GL_FALSE) {
		char szLog[10240] = { 0 };
		GLsizei logLen = 0;
		OGL_CALL(glGetShaderInfoLog(shader, 10240, &logLen, szLog));
		printf("Compile Shader fail error log : %s \nshader code :\n%s\n", szLog, shaderCode);
		OGL_CALL(glDeleteShader(shader));
		shader = 0;
	}
	return shader;
}
GLuint CreateProgram(GLuint vsShader, GLuint fsShader) {
	GLuint program = 0;
	OGL_CALL(program=glCreateProgram());
	OGL_CALL(glAttachShader(program, vsShader));
	OGL_CALL(glAttachShader(program, fsShader));
	OGL_CALL(glLinkProgram(program));
	OGL_CALL(glDetachShader(program, vsShader));
	OGL_CALL(glDetachShader(program, fsShader));
	GLint nResult;
	OGL_CALL(glGetProgramiv(program, GL_LINK_STATUS, &nResult));
	if (nResult == GL_FALSE) {
		char log[10240] = { 0 };
		GLsizei writed = 0;
		OGL_CALL(glGetProgramInfoLog(program, 10240, &writed, log));
		printf("create gpu program fail,link error : %s\n", log);
		OGL_CALL(glDeleteProgram(program));
		program = 0;
	}
	return program;
}
void SwapRGBPixel(unsigned char * pixel, int src0, int src1) {
	int src0_data_offset = src0 * 3;
	int src1_data_offset = src1 * 3;
	unsigned char src1_pixel_r = pixel[src1_data_offset];
	unsigned char src1_pixel_g = pixel[src1_data_offset + 1];
	unsigned char src1_pixel_b = pixel[src1_data_offset + 2];
	pixel[src1_data_offset] = pixel[src0_data_offset];
	pixel[src1_data_offset + 1] = pixel[src0_data_offset + 1];
	pixel[src1_data_offset + 2] = pixel[src0_data_offset + 2];
	pixel[src0_data_offset] = src1_pixel_r;
	pixel[src0_data_offset + 1] = src1_pixel_g;
	pixel[src0_data_offset + 2] = src1_pixel_b;
}
void SwapRGB32FPixel(float * pixel, int src0, int src1) {
	int src0_data_offset = src0 * 3;
	int src1_data_offset = src1 * 3;
	float src1_pixel_r = pixel[src1_data_offset];
	float src1_pixel_g = pixel[src1_data_offset + 1];
	float src1_pixel_b = pixel[src1_data_offset + 2];
	pixel[src1_data_offset] = pixel[src0_data_offset];
	pixel[src1_data_offset + 1] = pixel[src0_data_offset + 1];
	pixel[src1_data_offset + 2] = pixel[src0_data_offset + 2];
	pixel[src0_data_offset] = src1_pixel_r;
	pixel[src0_data_offset + 1] = src1_pixel_g;
	pixel[src0_data_offset + 2] = src1_pixel_b;
}
void SwapRGBAPixel(unsigned char * pixel, int src0, int src1) {
	int src0_data_offset = src0 * 4;
	int src1_data_offset = src1 * 4;
	unsigned char src1_pixel_r = pixel[src1_data_offset];
	unsigned char src1_pixel_g = pixel[src1_data_offset + 1];
	unsigned char src1_pixel_b = pixel[src1_data_offset + 2];
	unsigned char src1_pixel_a = pixel[src1_data_offset + 3];
	pixel[src1_data_offset] = pixel[src0_data_offset];
	pixel[src1_data_offset + 1] = pixel[src0_data_offset + 1];
	pixel[src1_data_offset + 2] = pixel[src0_data_offset + 2];
	pixel[src1_data_offset + 3] = pixel[src0_data_offset + 3];
	pixel[src0_data_offset] = src1_pixel_r;
	pixel[src0_data_offset + 1] = src1_pixel_g;
	pixel[src0_data_offset + 2] = src1_pixel_b;
	pixel[src0_data_offset + 3] = src1_pixel_a;
}
void FlipImage(unsigned char*pixel_data, int width, int height, int channel_count) {
	int half_height = height / 2;
	for (int y = 0; y < half_height; ++y) {
		for (int x = 0; x < width; ++x) {
			int src_pixel_index = y * width + x;
			int dst_pixel_index = (height - y - 1)*width + x;
			if (channel_count == 3) {
				SwapRGBPixel(pixel_data, src_pixel_index, dst_pixel_index);
			}
			else if (channel_count == 4) {
				SwapRGBAPixel(pixel_data, src_pixel_index, dst_pixel_index);
			}
		}
	}
}
void FlipImage32F(float*pixel_data, int width, int height, int channel_count) {
	int half_height = height / 2;
	for (int y = 0; y < half_height; ++y) {
		for (int x = 0; x < width; ++x) {
			int src_pixel_index = y * width + x;
			int dst_pixel_index = (height - y - 1)*width + x;
			if (channel_count == 3) {
				SwapRGB32FPixel(pixel_data, src_pixel_index, dst_pixel_index);
			}
		}
	}
}
void FlipImageX(unsigned char*pixel_data, int width, int height, int channel_count) {
	int half_width = width / 2;
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < half_width; ++x) {
			int src_pixel_index = y * width + x;
			int dst_pixel_index = y * width + width -1 - x;
			if (channel_count == 3) {
				SwapRGBPixel(pixel_data, src_pixel_index, dst_pixel_index);
			}
			else if (channel_count == 4) {
				SwapRGBAPixel(pixel_data, src_pixel_index, dst_pixel_index);
			}
		}
	}
}
void SwapRB(unsigned char * pixel, int pixel_data_offset) {
	unsigned char temp = pixel[pixel_data_offset];
	pixel[pixel_data_offset] = pixel[pixel_data_offset + 2];
	pixel[pixel_data_offset + 2] = temp;
}
unsigned char* DecodeBMP(unsigned char*bmpFileData, int&width, int&height) {
	if (0x4D42 == *((unsigned short*)bmpFileData)) {
		int pixelDataOffset = *((int*)(bmpFileData + 10));
		width = *((int*)(bmpFileData + 18));
		height = *((int*)(bmpFileData + 22));
		unsigned char*pixelData = bmpFileData + pixelDataOffset;
		for (int i = 0; i < width*height;++i) {
			SwapRB(pixelData, i*3);
		}
		return pixelData;
	}
	return nullptr;
}
void RGBAImageToRGBImage(unsigned char * rgba_pixel, unsigned char * rgb_pixel, int width, int height) {
	for (int i = 0; i < width*height; ++i) {
		rgb_pixel[i * 3] = rgba_pixel[i * 4];
		rgb_pixel[i * 3 + 1] = rgba_pixel[i * 4 + 1];
		rgb_pixel[i * 3 + 2] = rgba_pixel[i * 4 + 2];
	}
}
void SaveRGBToBMP(const char * path,unsigned char * pixel,int width, int height){
	FILE*pFile = fopen(path, "wb");
	if (pFile){
		BITMAPFILEHEADER bfh;
		memset(&bfh, 0, sizeof(BITMAPFILEHEADER));
		bfh.bfType = 0x4D42;
		bfh.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + width * height * 3;
		bfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
		fwrite(&bfh, sizeof(BITMAPFILEHEADER), 1, pFile);

		BITMAPINFOHEADER bih;
		memset(&bih, 0, sizeof(BITMAPINFOHEADER));
		bih.biWidth = width;
		bih.biHeight = height;
		bih.biBitCount = 24;
		bih.biSize = sizeof(BITMAPINFOHEADER);
		fwrite(&bih, sizeof(BITMAPINFOHEADER), 1, pFile);
		//rgb -> bgr
		for (int i = 0; i < width*height; ++i) {
			SwapRB(pixel, i*3);
		}
		fwrite(pixel, 1, width*height * 3, pFile);
		fclose(pFile);
	}
}
void SaveRGBAToBMP(const char * path, unsigned char * pixel, int width, int height) {
	unsigned char * rgb_pixel = new unsigned char[width*height*3];
	RGBAImageToRGBImage(pixel, rgb_pixel, width, height);
	SaveRGBToBMP(path, rgb_pixel, width, height);
	delete[]rgb_pixel;
}
GLuint CreateTexture2D(unsigned char*pixelData, int width, int height, GLenum gpu_format, GLenum cpu_format, GLenum date_type) {
	GLuint texture;
	OGL_CALL(glGenTextures(1, &texture));
	OGL_CALL(glBindTexture(GL_TEXTURE_2D, texture));
	OGL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	OGL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	OGL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	OGL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	OGL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, gpu_format, width, height, 0, cpu_format, date_type, pixelData));
	OGL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
	return texture;
}
GLuint CreateTextureFromFile(const char *path) {
	int file_size = 0;
	unsigned char* filecontent = LoadFileContent(path, file_size);
	int image_width, image_height, channel_count;
	unsigned char * pixel = nullptr;
	if (strcmp(path + (strlen(path) - 4), ".png") == 0) {
		pixel = stbi_png_load_from_memory(filecontent, file_size, &image_width, &image_height, &channel_count, 0);
	}
	else if (strcmp(path + (strlen(path) - 4), ".bmp") == 0) {
		pixel = stbi_bmp_load_from_memory(filecontent, file_size, &image_width, &image_height, &channel_count, 0);
	}
	else if (strcmp(path + (strlen(path) - 4), ".jpg") == 0) {
		pixel = stbi_jpeg_load_from_memory(filecontent, file_size, &image_width, &image_height, &channel_count, 0);
	}
	else {
		delete[] filecontent;
		return 0;
	}
	FlipImage(pixel, image_width, image_height, channel_count);
	GLenum pixel_format = channel_count == 3 ? GL_RGB : GL_RGBA;
	GLuint texture = CreateTexture2D(pixel, image_width, image_height, pixel_format, pixel_format);
	delete[] pixel;
	delete[] filecontent;
	return texture;
}
GLuint InitTextureCube(int size, GLenum gpu_format, GLenum data_type) {
	GLuint texture;
	OGL_CALL(glGenTextures(1, &texture));
	OGL_CALL(glBindTexture(GL_TEXTURE_CUBE_MAP, texture));
	for (unsigned int i = 0; i < 6; ++i){
		OGL_CALL(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, gpu_format, size, size, 0, GL_RGB, data_type, nullptr));
	}
	OGL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	OGL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	OGL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
	OGL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
	OGL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	OGL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	OGL_CALL(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
	return texture;
}
GLuint CreateTextureCubeFromBMP(const char **images, bool correct_with_shader) {
	GLuint texture;
	int nFileSize = 0, width = 0, height = 0;
	OGL_CALL(glGenTextures(1, &texture));
	OGL_CALL(glBindTexture(GL_TEXTURE_CUBE_MAP, texture));
	OGL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	OGL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	OGL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	OGL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	OGL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
	for (int i = 0; i < 6; ++i) {
		unsigned char *bmpFileContent = LoadFileContent(images[i], nFileSize);
		unsigned char *pixelData = DecodeBMP(bmpFileContent, width, height);
		if (correct_with_shader == false) {
			FlipImage(pixelData, width, height, 3);
		}
		OGL_CALL(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixelData));
		delete[] bmpFileContent;
	}
	OGL_CALL(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
	return texture;
}
GLuint CreateBufferObject(GLenum bufferType, GLsizeiptr size, GLenum usage, void*data /* = nullptr */) {
	GLuint object;
	OGL_CALL(glGenBuffers(1, &object));
	OGL_CALL(glBindBuffer(bufferType, object));
	OGL_CALL(glBufferData(bufferType, size, data, usage));
	OGL_CALL(glBindBuffer(bufferType, 0));
	return object;
}
void UpdateBufferObject(GLuint object, GLenum type, void * data, int size, int offset) {
	OGL_CALL(glBindBuffer(type, object));
	OGL_CALL(glBufferSubData(type, offset, size, data));//cpu -> gpu
	OGL_CALL(glBindBuffer(type, 0));
}
void CheckLastOpenGLError(const char *prefix, const char* file, long line, const char*operation){
	GLenum glerr;
	while ((glerr = glGetError()) != GL_NO_ERROR){
		switch (glerr){
		case GL_INVALID_VALUE:
			printf("%s GL_INVALID_VALUE\n", operation);
			break;
		default:
			printf("gl error  0x%x\n", (int)glerr);
			break;
		}
		std::string str = file;
		const int kMaxErrors = 10;
		int counter = 0;
		int pos = str.find_last_of('\\');
		printf("%s:%ld :", str.substr(pos + 1, str.length() - pos).c_str(), line);
		if (prefix){
			std::string errorString = prefix;
			errorString += ": ";
			const char* gluMsg = reinterpret_cast<const char*>(gluErrorString(glerr));
			if (gluMsg){
				printf("prefix error  %s\n", gluMsg);

			}else{
				printf("prefix error : unkown error 0x%x\n", glerr);
			}
		}else{
			const char* gluMsg = reinterpret_cast<const char*>(gluErrorString(glerr));
			if (gluMsg){
				printf("%s %s\n", operation, gluMsg);
			}else{
				printf("%s 0x%x\n", operation, glerr);
			}
		}
		++counter;
		if (counter > kMaxErrors){
			printf("GL: error count exceeds %i, stop reporting errors\n", kMaxErrors);
			return;
		}
	}
}