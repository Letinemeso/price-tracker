#version 330 core

in vec2 fragment_in_tex_coords;

uniform sampler2D input_texture;

out vec4 result_texture;

void main() 
{
	result_texture = vec4(texture(input_texture, fragment_in_tex_coords));
}