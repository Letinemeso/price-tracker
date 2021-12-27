#include "object.h"

object::object(const shader& _shdr, const glm::mat4x4& _projection_matrix, float* _coords, unsigned int _coords_count, const glm::vec3& _center)
	: projection_matrix(_projection_matrix), coords(_coords), center(_center), coords_count(_coords_count)
{
	move_matrix =
	{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
	rotate_matrix = move_matrix;
	scale_matrix = move_matrix;

	vertices_count = coords_count / 4;

	glGenVertexArrays(1, &vertex_array);
	glBindVertexArray(vertex_array);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	//coords buffer
	glGenBuffers(1, &coords_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, coords_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * coords_count + 1, coords, GL_STATIC_DRAW);
	glVertexAttribPointer(0, vertices_count, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);

	float temp_texture_coords[8] =
	{
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f
	};

	//texture coords buffer
	glGenBuffers(1, &texture_coords_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, texture_coords_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 8 + 1, temp_texture_coords, GL_STATIC_DRAW);
	glVertexAttribPointer(1, vertices_count, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

}

object::~object() noexcept
{
	delete texture;
	delete[] coords;

	glDeleteBuffers(1, &coords_buffer);
	glDeleteBuffers(1, &texture_coords_buffer);
	glDeleteVertexArrays(1, &vertex_array);
}



void object::set_texture(const char* _path) noexcept
{
	texture = new Texture(_path, )
}











void object::draw() const noexcept
{

}