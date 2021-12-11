#ifndef __CONFIG_PARSER
#define __CONFIG_PARSER

#ifndef STR
#define STR(str) *(std::string*)&str
#endif

#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <mutex>
#include <fstream>

#include "website.h"
#include "parse_functions.h"

class manager
{
private:
	std::vector<website> websites;

	//std::vector<std::thread*> threads;
	int delay = 1000;
	std::vector<bool> is_active;
	std::vector<bool> is_realy_active;

	std::basic_string<unsigned char> raw_input;

private:
	void parse_data();
	bool all_threads_stopped();

public:
	manager();
	manager(const char* _path);
	void operator=(const char* _path);
	~manager();
	
	void set_delay(int _delay) { delay = _delay; }
	void start_parsing();
	void stop_parsing();

};

#endif