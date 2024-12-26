#include "html_parser.h"

HtmlParser::HtmlParser(const Link& _base_link, std::string& _html)
{
	this->base_html = _html;
	this->working_html = _html;
	this->base_link = _base_link;
}

HtmlParser::HtmlParser(const Link& _base_link)
{
	this->base_link = _base_link;
}

void HtmlParser::set_properties(SpiderProperties& _prop)
{
	this->properties = _prop;
	set_working_reg();
}

std::vector<Link> HtmlParser::get_sub_links()
{
	auto words_begin = std::sregex_iterator(base_html.begin(), base_html.end(), reg_exps.ref_reg);
	auto words_end = std::sregex_iterator();

	for (std::sregex_iterator i = words_begin; i != words_end; ++i)
	{
		match = *i;
		if (match[1].matched)
		{
			sub_links.push_back(link_creator(match[1].str(), base_link));
		}
	}
	return sub_links;
}

std::map<std::string, int> HtmlParser::indexer()
{
	if (properties.search_strength == SearchStrength::LIGHT)
	{
		auto head{base_html.find("</head>")};
		if (head != std::string::npos)
		{
			working_html.erase(head, working_html.size());
		}
	}
	auto language_pos{working_html.find("lang=\"")};
	std::string lang{};
	if (language_pos != std::string::npos)
	{
		lang = working_html.substr(language_pos + 6, 2);
	}

	int weight{1};

	auto words_begin = std::sregex_iterator(working_html.begin(), working_html.end(), working_reg);
	auto words_end = std::sregex_iterator();

	for (std::sregex_iterator i = words_begin; i != words_end; ++i)
	{
		match = *i;

		if (match[1].matched)
		{
			if (match[1].str() == "keywords")
			{
				weight = 4;
			}
			else if (match[1].str() == "title")
			{
				weight = 3;
			}
			else if (match[1].str() == "description")
			{
				weight = 2;
			}
		}
		if (match[2].matched)
		{
			WordWorker dirty_str(match[2].str(), lang);
			add_to_words_list(dirty_str.get_clear_words(), weight);
		}

		if (match[4].matched)
		{
			weight = 1;
			WordWorker dirty_str(match[4].str(), lang);
			add_to_words_list(dirty_str.get_clear_words(), weight);
		}
		weight = 1;
	}
	return words_list;
}

void HtmlParser::add_to_words_list(std::vector<std::string>& clear_words, int& _weight)
{

	for (const auto key : clear_words)
	{
		auto iter = words_list.find(key);
		if (iter != words_list.end())
		{
			words_list.at(key) += _weight;
		}
		else
		{
			words_list[key] = _weight;
		}
	}
}

void HtmlParser::set_working_reg()
{
	if (properties.search_strength == SearchStrength::LIGHT)
	{
		working_reg = reg_exps.light_reg;
	}
	else if (properties.search_strength == SearchStrength::MIDDLE)
	{
		working_reg = reg_exps.middle_reg;
	}
	else if (properties.search_strength == SearchStrength::DEEP)
	{
		working_reg = reg_exps.deep_reg;
	}
	else
	{
		working_reg = reg_exps.light_reg;
	}
}
