#define CURL_STATICLIB
#include <string>
#include <iostream>
#include <fstream>

#include "curl/curl.h"

#include "writer_function.h"

#include "parse_functions.h"

#include "website.h"

#define STR(str) *(std::string*)&str

int main()
{
	setlocale(0, "rus");

	/*std::string buffer;
	std::string error_buffer;

	CURL* curl;
	CURLcode result;
	
	curl = curl_easy_init();
	
	if (curl)
	{
		std::string aaa("https://translate.google.com/?hl=ru");

		curl_easy_setopt(curl, CURLOPT_URL, "https://translate.google.com/?hl=ru");

		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer_function);
		curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, error_buffer);

		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);

		result = curl_easy_perform(curl);

		if (result == CURLE_OK)
			for (unsigned int i = 0; i < buffer.size(); ++i) std::cout << buffer[i];
		else
			std::cout << "Îøèáêà!\n" << error_buffer << "\n";
	}

	curl_easy_cleanup(curl);*/


	website site;

	site.set_chunk_to_parse(std::basic_string<unsigned char>((const unsigned char*)"<span class=\"price-value-main\"><span class=\"js-item-price\" itemprop=\"price\" content=\"{{}}\">"));
	site.set_link(std::basic_string<unsigned char>((const unsigned char*)"https://www.avito.ru/sankt-peterburg/avtomobili/audi_80_1988_2278158637"));

	/*std::ifstream input("input.txt", std::ios::in);
	std::basic_string<unsigned char> input_data;

	if (input.is_open() == false)
	{
		std::cout << "ass\n\n";
		return 1;
	}

	input.seekg(0, std::ios::end);
	unsigned int size = input.tellg();
	input.seekg(0, std::ios::beg);

	if (size == 0)
	{
		std::cout << "ass2\n\n";
		return 1;
	}

	input_data.reserve(size);

	do
		input_data += (unsigned char)input.get();
	while (input.tellg() < size);

	input.close();


	site.set_data_to_parse_test(input_data);

	site.parse_data();*/

	site.process();

	std::cout << STR(site.get_parsed_data());


	return 0;
}