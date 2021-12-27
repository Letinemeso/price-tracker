#include <glew.h>
#include <glfw3.h>

#include "shader.h"
#include "object.h"

#ifndef STR
#define STR(str) *(std::string*)&str
#endif

int main()
{
	/*setlocale(0, "rus");

	manager mgr("data.config");
	if (!mgr.is_valid())
	{
		std::cout << "some errors in config\n";
		while (true) std::this_thread::sleep_for(std::chrono::milliseconds(25));
		return -1;
	}
	std::cout << "config data was loaded\n\navailible commands:\n"
		<< "\tstart - start parsing specified data\n"
		<< "\tstop - stop parsing\n"
		<< "\texit - stop parsing and close the program\n\n";


	std::string input;
	while (true)
	{
		std::cin >> input;
		std::cout << "\n";
		if (input == "exit") break;
		else if (input == "start") mgr.start_parsing();
		else if (input == "stop") mgr.stop_parsing();
		else std::cout << "specified command does not exist\n\n";
	}

	mgr.stop_parsing();*/

	glfwInit();
	GLFWwindow* window = glfwCreateWindow(800, 600, "price-tracker", 0, 0);
	glfwMakeContextCurrent(window);
	glewInit();



	shader sdr("shaders/vertex_shader.shader", "shaders/fragment_shader.shader");
	if (!sdr.is_valid()) return -1;



	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);


		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);

	return 0;
}