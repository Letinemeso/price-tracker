#include "website.h"

#include <iostream>

void website::download_data()
{
	retreived_data.clear();
	retreived_data.shrink_to_fit();

	if (curl_easy_perform(curl_handle) != CURLE_OK)
		retreived_data = (const unsigned char*)"error getting answer from website";
}

void website::parse_data()
{
	if ((chunk_to_parse__left.size() == 0 || chunk_to_parse__right.size() == 0 || retreived_data.size() == 0) || 
		(retreived_data.size() < chunk_to_parse__left.size() + chunk_to_parse__right.size() ) )
	{
		parsed_data = (const unsigned char*)"error getting data";
		return;
	}

	parsed_data.clear();
	parsed_data.shrink_to_fit();

	if (get_index_after_chunk(retreived_data, chunk_to_parse__left) == 0 || get_index_of_chunk(retreived_data, chunk_to_parse__right) == 0)
	{
		parsed_data = (const unsigned char*)"specified chunk wasn't found";
		return;
	}

	unsigned int offset = 0;
	while (true)
	{
		int data_to_parse_offset = get_index_after_chunk(retreived_data, chunk_to_parse__left, offset);
		if (data_to_parse_offset == -1) return;

		int end_of_data_to_parse = get_index_of_chunk(retreived_data, chunk_to_parse__right, data_to_parse_offset);
		if (end_of_data_to_parse == -1) return;

		parsed_data.reserve(end_of_data_to_parse - data_to_parse_offset + 2);

		for (unsigned int i = data_to_parse_offset; i < end_of_data_to_parse; ++i)
			parsed_data += retreived_data[i];
		parsed_data += (const unsigned char*)", ";

		offset = end_of_data_to_parse;
	}
}

void website::process()
{
	download_data();
	parse_data();
}



website::website(website&& _other)
{
	curl_handle = _other.curl_handle;
	_other.curl_handle = nullptr;

	link = std::move(_other.link);
	chunk_to_parse__right = std::move(_other.chunk_to_parse__right);
	chunk_to_parse__left = std::move(_other.chunk_to_parse__left);
	parsed_data = std::move(_other.parsed_data);
	retreived_data = std::move(_other.retreived_data);

	curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &retreived_data);
}

website::website()
{
	curl_handle = curl_easy_init();
	curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, writer_function);
	curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &retreived_data);
}

website::~website()
{
	curl_easy_cleanup(curl_handle);
}

void website::set_link(const std::basic_string<unsigned char>& _link)
{
	link = _link;
	curl_easy_setopt(curl_handle, CURLOPT_URL, link.c_str());
}

void website::set_chunk_to_parse(const std::basic_string<unsigned char>& _chtp)
{
	std::basic_string<unsigned char> plug((const unsigned char*)"*****");
	
	int brackets_offset = get_index_of_chunk(_chtp, plug, 0);
	if (brackets_offset == -1) return;
	int after_brackets_offset = get_index_after_chunk(_chtp, plug, brackets_offset);
	if (after_brackets_offset == -1) return;
	
	chunk_to_parse__left.clear();
	chunk_to_parse__left.shrink_to_fit();
	chunk_to_parse__left.reserve(brackets_offset);
	chunk_to_parse__right.clear();
	chunk_to_parse__right.shrink_to_fit();
	chunk_to_parse__right.reserve(_chtp.size() - after_brackets_offset + 1);

	for (unsigned int i = 0; i < brackets_offset; ++i)
		chunk_to_parse__left += _chtp[i];
	for (unsigned int i = after_brackets_offset; i < _chtp.size(); ++i)
		chunk_to_parse__right += _chtp[i];
}

const std::basic_string<unsigned char>& website::get_link() const
{
	return link;
}

const std::basic_string<unsigned char>& website::get_parsed_data() const
{
	return parsed_data;
}