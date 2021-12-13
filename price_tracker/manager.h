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

	int delay = 3000;
	std::vector<bool> is_active;
	std::vector<bool> is_realy_active;
	std::vector<bool> process_completed;

	bool first_printed = false;

	std::basic_string<unsigned char> raw_input;

	std::basic_string<unsigned char> output_path;

private:
	void parse_data();

	bool all_threads_stopped();

public:
	manager();
	manager(const char* _path);
	manager(const manager& _other) = delete;
	manager(manager&& _other) = delete;
	~manager();
	
	void set_delay(int _delay) { delay = _delay; }
	void start_parsing();
	void stop_parsing();

};

#endif