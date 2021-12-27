#version 330 core

layout(location = 0) in vec4 coords;
layout(location = 1) in vec3 vertex_in_color;

uniform mat4 mtrx;

out vec3 vertex_out_color;

void main()
{
	vertex_out_color = vertex_in_color;
	gl_Position = mtrx * coords;
}