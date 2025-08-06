#pragma once

#include <string>
#include <codecvt>
#include <vector>
#include <boost/locale.hpp>

struct CharactersRange
{
	std::pair<wchar_t, wchar_t> latin{L'\x061', L'\x07A'};
	std::pair<wchar_t, wchar_t> cyrillic{L'\\x430', L'\\x44F'};

	std::vector<std::pair<wchar_t, wchar_t>> delimiters{{L'\x020', L'\x02F'}, 
														{L'\x03A', L'\x040'},
														{L'\x05B', L'\x060'},
														{L'\x07B', L'\x07E'},
														{L'\x0A0', L'\x0FF'}};
};

class WordWorker
{
public:
	WordWorker(const std::string& _dirty_str, const std::string& lang);
	std::vector<std::string> get_clear_words() { return words; }

protected:
	void clear_delimiters();
	void to_lower_case();
	bool set_language(const std::string& lang);

private:
	std::string dirty_str{};
	std::wstring dirty_wstr{};
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> to_wstr_converter;
	std::wstring_convert<std::codecvt_utf8<wchar_t>> to_str_converter;
	std::vector<std::string> words;

	bool correct_lang{};
	std::wstring characters_range{};
};