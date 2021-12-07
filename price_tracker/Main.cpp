#define CURL_STATICLIB
#include <string>
#include <iostream>
#include "curl/curl.h"

#include "writer_function.h"

#include "parse_functions.h"

#include "website.h"

int main()
{
	setlocale(0, "rus");

	//std::string buffer;
	/*std::basic_string<unsigned char> buffer;

	CURL* curl;
	CURLcode result;
	
	curl = curl_easy_init();
	
	if (curl)
	{
		std::string aaa("google.com");

		curl_easy_setopt(curl, CURLOPT_URL, aaa);

		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer_function);

		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);

		result = curl_easy_perform(curl);

		if (result == CURLE_OK)
			for (unsigned int i = 0; i < buffer.size(); ++i) std::cout << buffer[i];
		else
			std::cout << "Îøèáêà!\n";
	}

	curl_easy_cleanup(curl);*/

	/*std::string data("https://www.youtube.com/watch?v=TTAHPehElzg");
	std::string chunk("watch");

	std::cout << get_index_of_chunk(*(std::basic_string<unsigned char>*) & data, *(std::basic_string<unsigned char>*) & chunk) << "\n"
		<< get_index_after_chunk(*(std::basic_string<unsigned char>*) & data, *(std::basic_string<unsigned char>*) & chunk);*/

	
	website site;

	site.set_chunk_to_parse(std::basic_string<unsigned char>((const unsigned char*)"qwert{{}}op"));


	return 0;
}