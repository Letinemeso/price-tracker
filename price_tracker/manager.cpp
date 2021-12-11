#include "manager.h"
#ifndef STR
#define STR(str) *(std::string*)&str
#endif
#include <iostream>

void manager::parse_data()
{
	std::basic_string<unsigned char> ws_tag((unsigned char*)"website");
	std::basic_string<unsigned char> l_tag((unsigned char*)"link");
	std::basic_string<unsigned char> c_tag((unsigned char*)"chunk");

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
		while (raw_input[l_offset] != '*')
		{
			link += raw_input[l_offset];
			++l_offset;
		}
		std::basic_string<unsigned char> chunk;
		while (raw_input[c_offset] != '*')
		{
			chunk += raw_input[c_offset];
			++c_offset;
		}

		offset = c_offset;

		websites.push_back(website());
		std::vector<website>::iterator wit = websites.end();
		--wit;
		wit->set_link(link);
		wit->set_chunk_to_parse(chunk);

		is_active.push_back(false);
		is_realy_active.push_back(false);
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
		raw_input += file.get();

	file.close();

	parse_data();
}

manager::~manager()
{
	stop_parsing();
	//std::this_thread::sleep_for(std::chrono::milliseconds(100));

	while(all_threads_stopped() == false)
		std::this_thread::sleep_for(std::chrono::milliseconds(10));

	//websites.clear();
}

std::mutex output_mutex;
void thread_function(std::vector<website>* _websites, std::vector<bool>* _is_active, std::vector<bool>* _is_realy_active, unsigned int _index, unsigned int _delay)
{
	_is_realy_active->at(_index) = true;

	while ((*_is_active)[_index] != false)
	{
		_websites->at(_index).process();

		output_mutex.lock();
		std::cout << "link:  " << STR(((*_websites)[_index]).get_link()) << "\ndata:  " << STR(((*_websites)[_index]).get_parsed_data()) << "\n\n";
		output_mutex.unlock();

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
		std::thread thr(&thread_function, &websites, &is_active, &is_realy_active, i, delay);
		thr.detach();
	}
}

void manager::stop_parsing()
{
	for (unsigned int i = 0; i < websites.size(); ++i)
		is_active[i] = false;
}