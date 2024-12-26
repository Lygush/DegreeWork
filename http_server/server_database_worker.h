#pragma once

#include <vector>
#include <string>

#include <pqxx/pqxx>

#include "../common/database_worker.h"
#include "../common/ini_parser.h"
#include "../common/word_worker.h"

class ServerDataBaseWorker : public DatabaseWorker
{
public:
	ServerDataBaseWorker(IniParser& ini);

	std::vector<std::string> search_pages(const std::string& query);

protected:
	std::string create_req(const std::vector<std::string>& request_words);
	std::string create_totalsum_req(const std::vector<std::string>& request_words);
	std::string create_join_req(const std::vector<std::string>& request_words);
};