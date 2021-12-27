#version 330 core

in vec3 vertex_out_color;

out vec4 result_color;

void main()
{
	result_color = vec4(vertex_out_color, 1.0f);
}