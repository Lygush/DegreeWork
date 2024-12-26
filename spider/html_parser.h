#pragma once

#include <string>
#include <vector>
#include <regex>
#include <map>

#include "link.h"
#include "../common/ini_parser.h"
#include "../common/common_functions.h"
#include "../common/word_worker.h"

struct RegularExps
{
	const std::regex ref_reg{"<a href=\"(.*?)\"",
		std::regex_constants::optimize};

	std::regex light_reg{"<meta name=\"(title|description|keywords)\" content=\"(.*?)\"",
		std::regex_constants::optimize};

	std::regex middle_reg{"<meta name=\"(title|description|keywords)\" content=\"(.*?)\"|"
						  "(<title>|<h\\d>)(.*?)(<\/title>|<\/h\\d>)",
		std::regex_constants::optimize};

	std::regex deep_reg{"<meta name=\"(title|description|keywords)\" content=\"(.*?)\"|"
						"(<title>|<p>|<h\\d>)(.*?)(<\/title>|<\/p>|<\/h\\d>)",
		std::regex_constants::optimize};
};

class HtmlParser
{
public:
	HtmlParser(){};
	HtmlParser(const Link& _base_link, std::string& _html);
	HtmlParser(const Link& _base_link);

	void set_properties(SpiderProperties& _prop);

	std::vector<Link> get_sub_links();

	std::map<std::string, int> indexer();

protected:
	void add_to_words_list(std::vector<std::string>& clear_words, int& weight);
	void set_working_reg();

private:
	SpiderProperties properties{};

	RegularExps reg_exps{};
	std::regex& working_reg{reg_exps.light_reg};
	std::string working_html{};
	std::string base_html{};
	std::smatch match{};

	std::map<std::string, int> words_list{};

	std::vector<Link> sub_links{};
	Link base_link{};
};