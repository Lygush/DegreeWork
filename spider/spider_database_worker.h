#pragma once

#include <map>
#include <mutex>
#include "../common/database_worker.h"
#include "../common/ini_parser.h"
#include "link.h"

class SpiderDatabaseWorker : public DatabaseWorker
{
public:
	SpiderDatabaseWorker(IniParser& ini);

	bool url_already_exists(const Link& link);
	void add_url_to_database(const Link& link, const std::map<std::string, int>& words_list);

protected:
	std::mutex URLMutex;
};