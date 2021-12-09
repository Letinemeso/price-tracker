#ifndef __CONFIG_PARSER
#define __CONFIG_PARSER

#ifndef STR
#define STR(str) *(std::string*)&str
#endif

#include <string>
#include <vector>
#include <thread>
#include <chrono>

#include "website.h"
#include "parse_functions.h"

class manager
{
private:
	std::vector<website> websites;

	std::vector<std::thread*> threads;
	std::vector<bool> is_active;

	std::basic_string<unsigned char> path;
	std::basic_string<unsigned char> raw_input;

private:
	void parse_data();
	void thread_function(std::vector<website>::iterator _site, std::vector<bool>::iterator _should_be_active);

public:
	manager();
	manager(const char* _path);
	void operator=(const char* _path);
	
	void set_delay(float _delay);
	void start_parse();

};

#endif