#define CURL_STATICLIB
#include <string>
#include <iostream>
#include <fstream>

#include "curl/curl.h"

#include "website.h"

#ifndef STR
#define STR(str) *(std::string*)&str
#endif

int main()
{
	setlocale(0, "rus");



	website site;

	site.set_chunk_to_parse(std::basic_string<unsigned char>((const unsigned char*)
		"a title='13 {{}} - «Царь Горы» онлайн'"
		));	
	site.set_link(std::basic_string<unsigned char>((const unsigned char*)"https://kingofthehill.fox-fan.tv/"));


	site.process();

	std::cout << STR(site.get_parsed_data());


	return 0;
}