#include "object.h"

object::object(const glm::mat4x4& _projection_matrix, float* _coords, const glm::vec3& _center)
	: projection_matrix(_projection_matrix), coords(_coords), center(_center)
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
}