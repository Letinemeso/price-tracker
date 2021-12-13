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
	delay = std::stoi(STR(input_delay));

	unsigned int op_offset = get_index_after_chunk(raw_input, op_tag, d_offset);
	if (op_offset == -1) return;
	++op_offset;
	while (raw_input[op_offset] != '}')
	{
		output_path += raw_input[op_offset];
		++op_offset;
	}

	unsigned int offset = 0;

	while (true)
	{
		unsigned int ws_offset = get_index_of_chunk(raw_input, ws_tag, offset);
		if (ws_offset == -1) return;
		unsigned int l_offset = get_index_after_chunk(raw_input, l_tag, ws_offset);
		if (l_offset == -1) return;
		unsigned int c_offset = get_index_after_chunk(raw_input, c_tag, ws_offset);
		if (c_offset == -1) return;

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

		is_active.push_back(false);
		is_realy_active.push_back(false);
		process_completed.push_back(true);
	}
}



manager::manager() 
{ 
	raw_input.clear();
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

	while(all_threads_stopped() == false)
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
}

std::mutex output_mutex, completed_set_mutex;
void thread_function
(
	std::vector<website>* _websites, std::vector<bool>* _is_active, std::vector<bool>* _is_realy_active, 
	std::vector<bool>* _completed, unsigned int _index, unsigned int _delay, const std::basic_string<unsigned char>* _output_path, bool* _fp
)
{
	_is_realy_active->at(_index) = true;

	while ((*_is_active)[_index] != false)
	{
		completed_set_mutex.lock();
		_completed->at(_index) = false;
		completed_set_mutex.unlock();

		for (unsigned int i = 0; i < _completed->size(); ++i)
		{
			if (_completed->at(i) == true)
			{
				i = 0;
				std::this_thread::sleep_for(std::chrono::milliseconds(20));
			}
		}

		_websites->at(_index).process();
		
		output_mutex.lock();
		if(*_output_path == std_tag)
			std::cout << "link:  " << STR(((*_websites)[_index]).get_link()) << "\ndata:  " << STR(((*_websites)[_index]).get_parsed_data()) << "\n";
		else
		{
			if (*_fp == false)
			{
				*_fp = true;
				std::ofstream file((const char*)(_output_path->c_str()), std::ios::trunc);
				if (file.is_open() == true)
				{
					file << "link, parsed_data\n,,,\n";
					file << STR((*_websites)[_index].get_link()) << ", " << STR(((*_websites)[_index]).get_parsed_data()) << "\n";
					file.close();
				}
			}
			else
			{
				std::ofstream file((const char*)(_output_path->c_str()), std::ios::app);
				if (file.is_open() == true)
				{
					file << STR((*_websites)[_index].get_link()) << ", " << STR(((*_websites)[_index]).get_parsed_data()) << "\n";
					file.close();
				}
			}
			
		}
		output_mutex.unlock();



		completed_set_mutex.lock();
		_completed->at(_index) = true;
		completed_set_mutex.unlock();

		for (unsigned int i = 0; i < _completed->size(); ++i)
		{
			if (_completed->at(i) == false)
			{
				i = 0;
				std::this_thread::sleep_for(std::chrono::milliseconds(20));
			}
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(_delay));
	}

	_is_realy_active->at(_index) = false;
}

bool manager::all_threads_stopped()
{
	for (unsigned int i = 0; i < is_realy_active.size(); ++i)
		if (is_realy_active.at(i) == true) return false;
	return true;
}

void manager::start_parsing()
{
	for (unsigned int i = 0; i < websites.size(); ++i)
	{
		is_active[i] = true;
		std::thread thr(&thread_function, &websites, &is_active, &is_realy_active, &process_completed, i, delay, &output_path, &first_printed);
		thr.detach();
	}
}

void manager::stop_parsing()
{
	for (unsigned int i = 0; i < websites.size(); ++i)
		is_active[i] = false;
}