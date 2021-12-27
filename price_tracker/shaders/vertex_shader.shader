#version 330 core

layout(location = 0) in vec4 vertex_in_coords;
layout(location = 1) in vec2 vertex_in_tex_coords;

out vec2 fragment_in_tex_coords;

uniform mat4x4 mtx;

void main() 
{
	fragment_in_tex_coords = vertex_in_tex_coords;
	gl_Position = mtx * vertex_in_coords;
}