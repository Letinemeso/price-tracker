#ifndef __SHADER
#define __SHADER

#include <iostream>
#include <fstream>

#include <glew.h>
#include <glfw3.h>
#include <mat4x4.hpp>

class shader
{
private:
	unsigned int vertex_shader = 0, fragment_shader = 0;
	unsigned int program = 0;

	bool valid = false;

private:
	bool get_source(const char* _path, char** _buffer, unsigned int* _size) noexcept;

public:
	shader() = delete;
	shader(const shader& _other) = delete;
	void operator=(const shader& _other) = delete;
	shader(shader&& _other) = delete;
	void operator=(shader&& _other) = delete;

	shader(const char* _vpath, const char* _fpath) noexcept;
	~shader();

	bool is_valid() const noexcept;
	int get_uniform_location(const char* _uniform_name) const noexcept;

	bool set_matrix(const char* _name, const glm::mat4x4& _matrix) const noexcept;

};

#endif