#ifndef __WEBSITE_DATA
#define __WEBSITE_DATA

#ifndef CURL_STATICLIB
#define CURL_STATICLIB
#endif

#include "curl/curl.h"
#include "writer_function.h"
#include "parse_functions.h"

#include <string>

class website
{
private:
	CURL* curl_handle = nullptr;

	std::basic_string<unsigned char> link, chunk_to_parse__left, chunk_to_parse__right, parsed_data;
	std::basic_string<unsigned char> retreived_data;

private:
	void download_data();
	void parse_data();

public:
	website();
	website(website&& _site);
	website(const website& _site) = delete;
	~website();

	void set_link(const std::basic_string<unsigned char>& _link);
	void set_chunk_to_parse(const std::basic_string<unsigned char>& _chtp);
	
	const std::basic_string<unsigned char>& get_link() const;
	const std::basic_string<unsigned char>& get_parsed_data() const;

	void process();

};

#endif