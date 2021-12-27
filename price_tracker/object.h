#ifndef __OBJECT
#define __OBJECT

#include <glew.h>
#include <glfw3.h>
#include <mat4x4.hpp>



class object
{
private:
	unsigned int attrib_array = 0;
	unsigned int vertex_buffer, color_buffer;
	float* coords = nullptr;

	glm::vec3 center;

	const glm::mat4x4& projection_matrix;
	glm::mat4x4 move_matrix, rotate_matrix, scale_matrix;

public:
	object(const glm::mat4x4& _projection_matrix,  float* _coords, const glm::vec3& _center);

	void move_by(glm::vec3 _how_much) noexcept;
	void move_by(const glm::vec3& _how_much) noexcept;
	void move_to(glm::vec3 _new_pos) noexcept;
	void move_to(const glm::vec3& _new_pos) noexcept;

	void set_scale(float _scale_x, float _scale_y, float _scale_z) noexcept;

	void set_rotation_params(glm::vec3 _axis, float _angle) noexcept;
	void set_rotation_params(const glm::vec3& _axis, float _angle) noexcept;

	glm::vec3 get_center_pos() const noexcept;

};









#endif