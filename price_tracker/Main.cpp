#define CURL_STATICLIB
#include <string>
#include <iostream>
#include "curl/curl.h"

static std::string buffer;

static int writer(char* data, size_t size, size_t nmemb, std::string* buffer)
{
	int result = 0;

	if (buffer != NULL)
	{
	
		buffer->append(data, size * nmemb);
		
		result = size * nmemb;
	}

	return result;
}

int main()
{
	CURL* curl;
	CURLcode result;
	
	curl = curl_easy_init();
	
	if (curl)
	{
		curl_easy_setopt(curl, CURLOPT_URL, "https://translate.google.com/?hl=ru");

		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);

		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);

		result = curl_easy_perform(curl);

		if (result == CURLE_OK)

			std::cout << "ass\n\n" << buffer << "\n\n\n" << buffer.size();
		else
			std::cout << "Îøèáêà!\n";
	}

	curl_easy_cleanup(curl);
	return 0;
}