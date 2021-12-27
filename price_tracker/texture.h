#ifndef __TEXTURE
#define __TEXTURE

#include "stb_image.h"

class Texture 
{
	unsigned int textureBuffer;
	int width, height;

	unsigned char* imageBuffer;

	int textureUniformLocation;
public:
	Texture() = delete;
	Texture(const char* path, const unsigned int const* program, const char* uniformName);

	~Texture();

	void setTexture();
};

#endif