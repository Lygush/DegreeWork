#include "spider_database_worker.h"

SpiderDatabaseWorker::SpiderDatabaseWorker(IniParser& _ini) : DatabaseWorker(_ini)
{
	c.prepare("insert_url", "INSERT INTO url (protocol, host, query) VALUES ($1, $2, $3) RETURNING id;");
	c.prepare("insert_word", "INSERT INTO words (id) VALUES ($1) ON CONFLICT (id) DO NOTHING;");
	c.prepare("insert_word_url", "INSERT INTO words_url (word_id, url_id, count) VALUES ($1, $2, $3) RETURNING id;");
	c.prepare("url_exists", "SELECT COUNT(*) FROM url where protocol=$1 and host=$2 and query=$3");
}

bool SpiderDatabaseWorker::url_already_exists(const Link& link)
{
	pqxx::work t{c};
	pqxx::result r = t.exec_prepared("url_exists", static_cast<int>(link.protocol), link.hostName, link.query);
	t.commit();

	if (!r.empty())
	{
		int id = r[0][0].as<int>();

		if (id == 0)
		{
			return false;
		}
		else
		{
			return true;
		}
	}

	return false;
}

void SpiderDatabaseWorker::add_url_to_database(const Link& link, const std::map<std::string, int>& words_list)
{
	std::lock_guard<std::mutex> lock(URLMutex);

	bool urlExists = url_already_exists(link);
	if (urlExists)
	{
		return;
	}

	pqxx::work url_trx{c};

	pqxx::result r = url_trx.exec_prepared("insert_url", static_cast<int>(link.protocol), link.hostName, link.query);

	url_trx.commit();

	if (!r.empty())
	{
		int id = r[0][0].as<int>();

		pqxx::work words_trx{c};

		for (auto w : words_list)
		{
			words_trx.exec_prepared("insert_word", w.first);
			words_trx.exec_prepared("insert_word_url", w.first, id, w.second);
		}

		words_trx.commit();
	}
}
