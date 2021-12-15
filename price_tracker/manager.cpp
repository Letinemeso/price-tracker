#include "manager.h"

#include <iostream>
#include <fstream>

#ifndef STR
#define STR(str) *(std::string*)&str
#endif

std::basic_string<unsigned char> d_tag((const unsigned char*)"delay");
std::basic_string<unsigned char> ws_tag((const unsigned char*)"website");
std::basic_string<unsigned char> l_tag((const unsigned char*)"link");
std::basic_string<unsigned char> c_tag((const unsigned char*)"chunk");
std::basic_string<unsigned char> op_tag((const unsigned char*)"output");

std::basic_string<unsigned char> std_tag((const unsigned char*)"COUT");


void manager::parse_data()
{
	unsigned int d_offset = get_index_after_chunk(raw_input, d_tag);
	if (d_offset == -1) return;
	++d_offset;
	std::basic_string<unsigned char> input_delay;
	while (raw_input[d_offset] != '}')
	{
		if (is_digit(raw_input[d_offset]) == false) return;
		input_delay += raw_input[d_offset];
		++d_offset;
	}
	if (input_delay.size() == 0) return;
	delay = std::stoi(STR(input_delay));

	unsigned int op_offset = get_index_after_chunk(raw_input, op_tag, d_offset);
	if (op_offset == -1) return;
	++op_offset;
	while (raw_input[op_offset] != '}')
	{
		output_path += raw_input[op_offset];
		++op_offset;
	}
	if (output_path.size() == 0) return;

	unsigned int offset = 0;

	while (true)
	{
		unsigned int ws_offset = get_index_of_chunk(raw_input, ws_tag, offset);
		if (ws_offset == -1) break;
		unsigned int l_offset = get_index_after_chunk(raw_input, l_tag, ws_offset);
		if (l_offset == -1) break;
		unsigned int c_offset = get_index_after_chunk(raw_input, c_tag, ws_offset);
		if (c_offset == -1) break;

		++l_offset;
		++c_offset;

		std::basic_string<unsigned char> link;
		while (raw_input[l_offset] != '}')
		{
			link += raw_input[l_offset];
			++l_offset;
		}
		std::basic_string<unsigned char> chunk;
		while (raw_input[c_offset] != '}')
		{
			chunk += raw_input[c_offset];
			++c_offset;
		}

		offset = c_offset;

		websites.push_back(website());

		websites[websites.size() - 1].set_link(link);
		websites[websites.size() - 1].set_chunk_to_parse(chunk);
	}

	if (websites.size() > 0) valid = true;
}



manager::manager(const char* _path)
{
	raw_input.clear();
	raw_input.shrink_to_fit();

	std::ifstream file(_path, std::ios::in);
	if (file.is_open() == false) return;

	file.seekg(0, std::ios::end);
	raw_input.reserve(file.tellg());
	file.seekg(0, std::ios::beg);

	while (file.peek() != -1)
	{
		unsigned char inp = file.get();
		raw_input += inp;
	}

	file.close();

	parse_data();
}

manager::~manager()
{
	stop_parsing();

	while (is_active == true) std::this_thread::sleep_for(std::chrono::milliseconds(25));
}

std::mutex write_mutex;
void thread_function(manager* _mgr, unsigned int _index, unsigned int* _threads_active)
{
	_mgr->websites[_index].process();

	write_mutex.lock();
	_mgr->output_buffer.append(_mgr->websites[_index].get_link());
	_mgr->output_buffer.append((const unsigned char*)", ");
	_mgr->output_buffer.append(_mgr->websites[_index].get_parsed_data());
	_mgr->output_buffer.append((const unsigned char*)",\n");
	write_mutex.unlock();

	--(*_threads_active);
}

void thread_control_function(manager* _mgr)
{
	_mgr->is_active = true;

	while (_mgr->need_to_process == true)
	{
		_mgr->output_buffer.clear();
		_mgr->output_buffer.shrink_to_fit();

		unsigned int threads_active = _mgr->websites.size();

		for (unsigned int i = 0; i < _mgr->websites.size(); ++i)
		{
			std::thread thr(&thread_function, _mgr, i, &threads_active);
			thr.detach();
		}

		while (threads_active != 0) std::this_thread::sleep_for(std::chrono::milliseconds(25));

		if (_mgr->output_path == std_tag)
			std::cout << STR(_mgr->output_buffer) << "\n";
		else
		{
			std::ofstream file((const char*)(_mgr->output_path.c_str()), std::ios::trunc);
			file.clear();
			file << "link, data\n,,,\n" << STR(_mgr->output_buffer) << "\n";
			file.close();
		}
	}

	_mgr->is_active = false;
}

void manager::start_parsing()
{
	need_to_process = true;
	std::thread control_thread(&thread_control_function, this);
	control_thread.detach();
}

void manager::stop_parsing()
{
	need_to_process = false;
}