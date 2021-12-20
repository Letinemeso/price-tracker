#define CURL_STATICLIB
#include <string>
#include <iostream>
#include <fstream>

#include "manager.h"

#include <chrono>

#ifndef STR
#define STR(str) *(std::string*)&str
#endif

int main()
{
	setlocale(0, "rus");

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

	mgr.stop_parsing();

	return 0;
}