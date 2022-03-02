#ifndef __TEXTURE
#define __TEXTURE

#include "stb_image.h"

#include "opengl_mgr.h"

class Texture 
{
private:
	bool valid = false;

private:
	unsigned int textureBuffer;
	int width, height;

	unsigned char* imageBuffer;

	int textureUniformLocation;
public:
	Texture() = delete;
	Texture(const char* path);

	~Texture();

	bool is_valid() const { return valid; }

	void setTexture();
};

#endif