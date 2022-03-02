#ifndef __OPENGL_MGR
#define __OPENGL_MGR

#include "shader.h"



class opengl_mgr
{
private:
	shader* m_shader = nullptr;

private:
	std::string texture_uniform_name;

public:
	opengl_mgr() {}
	~opengl_mgr();

	bool create_shader(const char* _v_path, const char* _f_path);
	void set_texture_uniform_name(const char* _tex_unif_name);

	const shader* get_shader() const;

	const char* get_texture_uniform_name() const;

};

static opengl_mgr mgr;



#endif