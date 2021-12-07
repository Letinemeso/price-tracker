#ifndef __WRITER_FUNCTION
#define __WRITER_FUNCTION

#include <string>

static int writer_function(char* data, size_t size, size_t nmemb, std::string& buffer)
{
	buffer.append(data, size * nmemb);

	return size * nmemb;
}

#endif