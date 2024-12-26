#include "database_worker.h"

DatabaseWorker::DatabaseWorker(IniParser& ini)
	: c("host=" + ini.data_base.host + 
		" port=" + ini.data_base.port + 
		" dbname=" + ini.data_base.dbname + 
		" user=" + ini.data_base.user +
		" password=" + ini.data_base.password)
{
	create_tables();
}

void DatabaseWorker::create_tables()
{
	pqxx::work t{c};

	t.exec("CREATE TABLE IF NOT EXISTS words(id VARCHAR(255) NOT NULL PRIMARY KEY); ");
	t.exec("CREATE TABLE IF NOT EXISTS url(id SERIAL PRIMARY KEY, protocol INTEGER NOT NULL, host TEXT NOT NULL, query TEXT NOT NULL); ");
	t.exec("CREATE TABLE IF NOT EXISTS words_url("
		   "id SERIAL PRIMARY KEY, "
		   "word_id VARCHAR(255) NOT NULL REFERENCES words(id), "
		   "url_id INTEGER NOT NULL REFERENCES url(id), "
		   "count INTEGER NOT NULL); ");
	t.commit();
}
