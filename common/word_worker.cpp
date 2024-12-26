#include "word_worker.h"

WordWorker::WordWorker(const std::string& _dirty_str, const std::string& lang) : dirty_str(_dirty_str)
{
	dirty_wstr = to_wstr_converter.from_bytes(dirty_str);
	correct_lang = set_language(lang);
	to_lower_case();
	clear_delimiters();
}

void WordWorker::clear_delimiters()
{
	std::wstring word{};
	int i{};

	while (i != dirty_wstr.size())
	{
		if ((characters_range.find(dirty_wstr[i]) != std::string::npos) == correct_lang)
		{
			word += dirty_wstr[i];
		}
		else
		{
			if (word.size() > 2 && word.size() < 255)
			{
				words.push_back(to_str_converter.to_bytes(word));
				word.clear();
			}
			else
			{
				word.clear();
			}
		}
		i++;
	}
	if (word.length() != 0)
	{
		words.push_back(to_str_converter.to_bytes(word));
	}
}

void WordWorker::to_lower_case()
{
	using namespace boost::locale;
	using namespace std;

	generator gen;
	locale loc = gen("");
	locale::global(loc);

	dirty_wstr = to_lower<wchar_t>(dirty_wstr);
}

bool WordWorker::set_language(const std::string& lang)
{
	ÑharactersRange cr{};

	wchar_t begin{}, end{};

	if (lang == "en")
	{
		begin = cr.latin.first;
		end = cr.latin.second;
	}
	else if (lang == "ru")
	{
		begin = cr.cyrillic.first;
		end = cr.cyrillic.second;
	}
	else
	{
		for (auto d : cr.delimiters)
		{
			auto begin{d.first};
			auto end{d.second};
			while (begin <= end)
			{
				characters_range += begin;
				begin++;
			}
		}
		return false;
	}
	while (begin <= end)
	{
		characters_range += begin;
		begin++;
	}
	return true;
}
