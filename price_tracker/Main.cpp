#define CURL_STATICLIB
#include <string>
#include <iostream>
#include <fstream>

#include "curl/curl.h"

#include "website.h"

#include "manager.h"

#ifndef STR
#define STR(str) *(std::string*)&str
#endif

int main()
{
	setlocale(0, "rus");

	manager mgr("data.config");
	if (!mgr.is_valid())
	{
		std::cout << "EL PROBLEMO: some errors in config\n";
		while (true) std::this_thread::sleep_for(std::chrono::milliseconds(25));
		return -1;
	}

	

	//std::this_thread::sleep_for(std::chrono::seconds(5));

	std::string input;
	while (true)
	{
		std::cin >> input;
		std::cout << "\n";
		if (input == "exit") break;
		else if(input == "start") mgr.start_parsing();
		else if(input == "stop") mgr.stop_parsing();
	}

	mgr.stop_parsing();

	return 0;
}