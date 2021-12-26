#ifndef __OBJECT
#define __OBJECT

#include <glew.h>
#include <glfw3.h>
#include <mat4x4.hpp>

class object
{
private:
	glm::mat4x4 move_matrix, rotate_matrix, scale_matrix;
	float* coords = nullptr;

public:
	object();


};









#endif