#ifndef __PARSE_FUNCTIONS
#define __PARSE_FUNCTIONS

#include <string>

static bool chunks_are_equal(const std::basic_string<unsigned char>& _data, const std::basic_string<unsigned char>& _chunk, unsigned int _offset = 0)
{
	if (_data.size() == 0 || _chunk.size() == 0) return false;
	if (_data.size() < _chunk.size()) return false;
	if (_offset >= _data.size()) return false;
	if (_offset >= _chunk.size())
		if (_data.size() - _offset < _chunk.size()) return false;

	for (unsigned int i = 0; i < _chunk.size(); ++i)
		if (_data[i + _offset] != _chunk[i]) return false;

	return true;
}

static int get_index_after_chunk(const std::basic_string<unsigned char>& _data, const std::basic_string<unsigned char>& _chunk, unsigned int _offset = 0)
{
	for (unsigned int i = _offset; i < _data.size(); ++i)
		if (chunks_are_equal(_data, _chunk, i)) return i + _chunk.size();
	return -1;
}

static int get_index_of_chunk(const std::basic_string<unsigned char>& _data, const std::basic_string<unsigned char>& _chunk, unsigned int _offset = 0)
{
	for (unsigned int i = _offset; i < _data.size(); ++i)
		if (chunks_are_equal(_data, _chunk, i)) return i;
	return -1;
}

static int get_next_sym_index(const std::basic_string<unsigned char>& _data, unsigned char _sym, unsigned int _offset)
{
	for (unsigned int i = _offset; i < _data.size(); ++i)
		if (_data[i] == _sym) return i;
	return -1;
}

static bool is_digit(unsigned char _char)
{
	unsigned int zero = '0';
	for (unsigned int i = 0; i < 10; ++i)
		if (_char == zero + i) return true;
	return false;
}

#endif