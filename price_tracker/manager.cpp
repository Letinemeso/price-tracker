#include "manager.h"

#ifndef STR
#define STR(str) *(std::string*)&str
#endif

std::basic_string<unsigned char> d_tag((const unsigned char*)"delay");
std::basic_string<unsigned char> ws_tag((const unsigned char*)"website");
std::basic_string<unsigned char> l_tag((const unsigned char*)"link");
std::basic_string<unsigned char> c_tag((const unsigned char*)"chunk");
std::basic_string<unsigned char> op_tag((const unsigned char*)"output");

std::basic_string<unsigned char> std_tag((const unsigned char*)"COUT");

std::basic_string<unsigned char> header((const unsigned char*)"\"дата\";\"ссылка\";\"данные\"\n");



void manager::parse_data()
{
	#ifdef ASSERT(error)
	#undef ASSERT(error)
	#endif
	#define ASSERT(error) { std::cout << error << "\n"; return; }

	//getting delay data (delay{1000})
	unsigned int d_offset = get_index_after_chunk(raw_input, d_tag);
	if (d_offset == -1) ASSERT("field \"delay\" wasn't found");

	d_offset = get_next_sym_index(raw_input, '{', d_offset);
	if (d_offset == -1) ASSERT("field \"delay\" is missing '{'");
	++d_offset;
	unsigned int d_end = get_next_sym_index(raw_input, '}', d_offset);
	if (d_end == -1) ASSERT("field \"delay\" is missing '}'");

	std::basic_string<unsigned char> input_delay;

	for (unsigned int i = d_offset; i < d_end; ++i)
	{
		if (is_digit(raw_input[i]) == false) ASSERT("field \"delay\" contains invalid data");
		input_delay += raw_input[i];
	}
	if (input_delay.size() == 0) ASSERT("field \"delay\" contains no data");
	delay = std::stoi(STR(input_delay));
	std::cout << "delay set to " << delay << "\n\n";

	//getting output data (output{COUT} / output{file.txt})
	unsigned int op_offset = get_index_after_chunk(raw_input, op_tag);
	if (op_offset == -1) ASSERT("field \"output\" wasn't found");

	op_offset = get_next_sym_index(raw_input, '{', op_offset);
	if (op_offset == -1) ASSERT("field \"output\" is missing '{'");
	++op_offset;
	unsigned int op_end = get_next_sym_index(raw_input, '}', op_offset);
	if (op_end == -1) ASSERT("field \"output\" is missing '}'");

	for (unsigned int i = op_offset; i < op_end; ++i)
		output_path += raw_input[i];

	if (output_path.size() == 0) ASSERT("field \"output\" contains no data");
	std::cout << "output path set to " << STR(output_path) << "\n";

	//checking if file contains previously parsed data
	if (output_path != std_tag)
	{
		std::ifstream test((const char*)output_path.c_str(), std::ios::in);
		if (test.is_open())
		{
			std::basic_string<unsigned char> test_input;
			while (test.peek() != -1)
				test_input += test.get();
			test.close();

			if (get_index_after_chunk(test_input, header) == -1)
			{
				std::ofstream header_output_file((const char*)output_path.c_str(), std::ios::trunc);
				header_output_file << STR(header);
				header_output_file.close();

				std::cout << "file " << STR(output_path) << " didn't match program's output format and was truncated\n\n";
			}
			else std::cout << "previously parsed data found. next data will append to it\n\n";
		}
		else
		{
			std::ofstream header_output_file((const char*)output_path.c_str(), std::ios::trunc);
			header_output_file << STR(header);
			header_output_file.close();
			std::cout << "file " << STR(output_path) << " could not be openned. created new file\n\n";
		}
	}

	//getting websites' data (website{link{smth.com}chunk{ass}})
	unsigned int offset = 0;
	while (offset != -1)
	{
		unsigned int ws_offset = get_index_of_chunk(raw_input, ws_tag, offset);
		if (ws_offset == -1) break;
		unsigned int l_offset = get_index_after_chunk(raw_input, l_tag, ws_offset);
		if (l_offset == -1) break;
		unsigned int c_offset = get_index_after_chunk(raw_input, c_tag, ws_offset);
		if (c_offset == -1) break;

		l_offset = get_next_sym_index(raw_input, '{', l_offset);
		c_offset = get_next_sym_index(raw_input, '{', c_offset);

		unsigned int l_end = get_next_sym_index(raw_input, '}', l_offset + 1);
		if (l_end == -1) break;
		unsigned int c_end = get_next_sym_index(raw_input, '}', c_offset + 1);
		if (c_end == -1) break;

		++l_offset;
		++c_offset;

		std::basic_string<unsigned char> link;
		for(unsigned int i=l_offset; i<l_end; ++i)
			link += raw_input[i];

		std::basic_string<unsigned char> chunk;
		for (unsigned int i = c_offset; i < c_end; ++i)
			chunk += raw_input[i];

		offset = get_next_sym_index(raw_input, '}', (l_end > c_end ? l_end : c_end) + 1);

		websites.push_back(website());

		websites[websites.size() - 1].set_link(link);
		websites[websites.size() - 1].set_chunk_to_parse(chunk);

		std::cout << "found website's data:\n"
			<< "\tlink:  " << STR(link) << "\n"
			<< "\tchunk to find and parse:  " << STR(chunk) << "\n\n";
	}

	if (websites.size() > 0) valid = true;
	else ASSERT("no websites' data could be parsed properly");

	#undef ASSERT(error)
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
	if (!valid) return;

	stop_parsing();

	while (is_active == true) std::this_thread::sleep_for(std::chrono::milliseconds(25));
}

std::mutex write_mutex;
void thread_function(manager* _mgr, unsigned int _index, unsigned int* _threads_active)
{
	_mgr->websites[_index].process();

	write_mutex.lock();
	_mgr->output_buffer.append((const unsigned char*)"\"");
	_mgr->output_buffer.append((const unsigned char*)get_date().c_str());
	_mgr->output_buffer.append((const unsigned char*)"\";\"");
	_mgr->output_buffer.append(_mgr->websites[_index].get_link());
	_mgr->output_buffer.append((const unsigned char*)"\";\"");
	_mgr->output_buffer.append(_mgr->websites[_index].get_parsed_data());
	_mgr->output_buffer.append((const unsigned char*)"\"\n");
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

		if (_mgr->output_path == std_tag) std::cout << STR(_mgr->output_buffer) << "\n";
		else
		{
			std::ofstream file((const char*)(_mgr->output_path.c_str()), std::ios::app);
			file.clear();
			file << STR(_mgr->output_buffer) << "\n";
			//file << '\"' << get_date() << "\";" << STR(_mgr->output_buffer) << "\n";
			file.close();
		}

		std::this_thread::sleep_for(std::chrono::seconds(_mgr->delay));
	}

	_mgr->is_active = false;
}

void manager::start_parsing()
{
	if (!valid) return;

	if (need_to_process == false)
	{
		need_to_process = true;
		std::thread control_thread(&thread_control_function, this);
		control_thread.detach();
	}
}

void manager::stop_parsing()
{
	if (!valid) return;

	need_to_process = false;
}