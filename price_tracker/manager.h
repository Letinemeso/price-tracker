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
	std::basic_string<unsigned char> raw_input;
	std::vector<website> websites;

	int delay;

	std::basic_string<unsigned char> output_buffer;

	std::basic_string<unsigned char> output_path;
	bool need_to_write_header = false;

	bool need_to_process = false;
	bool is_active = false;

	bool valid = false;

private:
	void parse_data();

	friend void thread_control_function(manager* _mgr);
	friend void thread_function(manager* _mgr, unsigned int _index, unsigned int* _threads_active);

public:
	manager(const char* _path);
	manager(const manager& _other) = delete;
	manager(manager&& _other) = delete;
	~manager();
	
	void set_delay(int _delay) { delay = _delay; }
	void start_parsing();
	void stop_parsing();

	bool is_valid() { return valid; }
};

#endif