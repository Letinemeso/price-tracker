#include "opengl_mgr.h"

opengl_mgr::~opengl_mgr()
{
	delete m_shader;
}



bool opengl_mgr::create_shader(const char* _v_path, const char* _f_path)
{
	m_shader = new shader(_v_path, _f_path);

	if (m_shader->is_valid() == false)
	{
		delete m_shader;
		m_shader = nullptr;
		return false;
	}

	return true;
}

void opengl_mgr::set_texture_uniform_name(const char* _tex_unif_name)
{
	texture_uniform_name = _tex_unif_name;
}



const shader* opengl_mgr::get_shader() const
{
	return m_shader;
}

const char* opengl_mgr::get_texture_uniform_name() const
{
	return texture_uniform_name.c_str();
}