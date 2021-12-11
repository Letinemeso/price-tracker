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
	//curl_global_init(CURL_GLOBAL_ALL);

	setlocale(0, "rus");

	/*website site;

	site.set_chunk_to_parse(std::basic_string<unsigned char>((const unsigned char*)
		"\"description\":\"{{}}\",\"name\":"
		));	
	site.set_link(std::basic_string<unsigned char>((const unsigned char*)"https://auto.ru/sankt-peterburg/cars/land_rover/all/"));


	site.process();

	for (unsigned int i = 0; i < site.get_parsed_data().size(); ++i)
		std::cout << (unsigned char)(site.get_parsed_data()[i]);*/


	manager mgr("file.txt");
	mgr.set_delay(10000);

	mgr.start_parsing();

	std::this_thread::sleep_for(std::chrono::seconds(10));

	mgr.stop_parsing();

	//curl_global_cleanup();

	return 0;
}