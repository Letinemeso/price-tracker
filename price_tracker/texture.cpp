#include <glew.h>
#include "texture.h"
#include <fstream>
#include <iostream>

bool checkExistance(const char* path) 
{
	std::ifstream file = std::ifstream(path, std::ios::in);

	if (file.is_open()) 
	{
		file.close();
		return true;
	}
	else return false;
}

Texture::Texture(const char* path) 
{
	//TODO: сделать очищение текстуры перед загрузкой новой

	valid = false;

	if (checkExistance(path) == true) 
	{
		stbi_set_flip_vertically_on_load(1);
		imageBuffer = stbi_load(path, &width, &height, 0, 4);

		glGenTextures(1, &textureBuffer);

		glBindTexture(GL_TEXTURE_2D, textureBuffer);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageBuffer);

		stbi_image_free(imageBuffer);

		textureUniformLocation = mgr.get_shader()->get_uniform_location(mgr.get_texture_uniform_name());

		if (textureUniformLocation == -1)
			std::cout << "uniform sampler \"" << mgr.get_texture_uniform_name() << "\" not found!\n";
		else
			valid = true;
	}
	else std::cout << "Texture not found!\n";
}

Texture::~Texture() 
{
	glDeleteTextures(1, &textureBuffer);
}

void Texture::setTexture()
{
	glBindTexture(GL_TEXTURE_2D, textureBuffer);
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(textureUniformLocation, 0);
}