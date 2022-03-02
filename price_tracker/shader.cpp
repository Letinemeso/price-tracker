#include "shader.h"

#ifdef SHADER_ASSERT(cond, msg)
#undef SHADER_ASSERT(cond, msg)
#endif
#define SHADER_ASSERT(cond, msg) if(!cond) { std::cout << msg << '\n'; return; }



bool shader::get_source(const char* _path, char** _buffer, unsigned int* _size) noexcept
{
	delete[] *_buffer;
	*_buffer = nullptr;
	*_size = 0;

	std::ifstream source(_path, std::ios::in);
	if (!source.is_open()) return false;

	source.seekg(0, std::ios::end);
	*_size = source.tellg();
	if (*_size == 0) return false;
	source.seekg(0, std::ios::beg);

	*_buffer = new char[*_size + 1];
	(*_buffer)[*_size] = 0;

	for (unsigned int i = 0; i < *_size; ++i)
		(*_buffer)[i] = source.get();
}



shader::shader(const char* _vpath, const char* _fpath) noexcept
{
	char* buffer = nullptr;
	unsigned int buffer_size = 0;
	int result;

	SHADER_ASSERT(get_source(_vpath, &buffer, &buffer_size), "error openning vertex shader source file!");

	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &buffer, 0);
	glCompileShader(vertex_shader);

	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &result);
	SHADER_ASSERT(result == GL_TRUE, "error compiling vertex shader!");

	SHADER_ASSERT(get_source(_fpath, &buffer, &buffer_size), "error openning fragment shader source file!");

	fragment_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(fragment_shader, 1, &buffer, 0);
	glCompileShader(fragment_shader);

	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &result);
	SHADER_ASSERT(result == GL_TRUE, "error compiling fragment shader!");

	program = glCreateProgram();

	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);

	glLinkProgram(program);
	glUseProgram(program);

	delete[] buffer;
	buffer_size = 0;

	valid = true;
}

shader::~shader()
{
	glDeleteProgram(program);
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
}



bool shader::is_valid() const noexcept
{
	return valid;
}

int shader::get_uniform_location(const char* _uniform_name) const noexcept
{
	return glGetUniformLocation(program, _uniform_name);
}



bool shader::set_matrix(const char* _name, const glm::mat4x4& _matrix) const noexcept
{
	int location = glGetUniformLocation(program, _name);
	if (location < 0) return false;

	glUniformMatrix4fv(location, 1, GL_FALSE, &_matrix[0][0]);

	return true;
}



#undef SHADER_ASSERT(cond, msg)