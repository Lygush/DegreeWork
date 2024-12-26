#pragma once

#include <pqxx/pqxx>
#include "ini_parser.h"

class DatabaseWorker
{
public:
	DatabaseWorker(IniParser& ini);

	void create_tables();

protected:
	pqxx::connection c;
};