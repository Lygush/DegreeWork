#include "server_database_worker.h"

ServerDataBaseWorker::ServerDataBaseWorker(IniParser& ini) : DatabaseWorker(ini) {}

std::vector<std::string> ServerDataBaseWorker::search_pages(const std::string& query)
{
	std::vector<std::string> result{};
	if (query.empty())
	{
		return result;
	}

	WordWorker ww{query, std::string{"not_defined"}};
	auto clear_words_lower_case{ww.get_clear_words()};

	pqxx::work wk{c};
	pqxx::result r;

	r = wk.exec(create_req(clear_words_lower_case));

	for (const auto& row : r)
	{
		std::string url = (row["protocol"].as<int>() == 0) ? "https://" : "http://";

		url += row["host"].as<std::string>();
		url += row["query"].as<std::string>();
		result.push_back(url);
	}

	return result;
}

std::string ServerDataBaseWorker::create_req(const std::vector<std::string>& request_words)
{
	std::string req{};

	req += "SELECT u.protocol, u.host, u.query, " + create_totalsum_req(request_words) + "AS totalsum ";
	req += "FROM url u ";
	req += create_join_req(request_words);
	req += "GROUP BY(u.protocol, u.host, u.query) ";
	req += "ORDER BY totalsum DESC LIMIT 10";

	return req;
}

std::string ServerDataBaseWorker::create_totalsum_req(const std::vector<std::string>& request_words)
{
	std::string reslt_str{};

	int counter{};
	while (counter <= request_words.size() - 1)
	{
		std::string wu_name{"wu" + std::to_string(counter)};
		reslt_str += "sum(" + wu_name + ".count) ";
		if (counter != request_words.size() - 1)
		{
			reslt_str += ("+ ");
		}
		counter++;
	}
	return reslt_str;
}

std::string ServerDataBaseWorker::create_join_req(const std::vector<std::string>& request_words)
{
	std::string result_str{};

	int counter{};
	for (auto word : request_words)
	{
		std::string wu_name{"wu" + std::to_string(counter)};
		std::string w_name{"w" + std::to_string(counter)};
		result_str += "JOIN words_url " + wu_name + " ON u.id = " + wu_name + ".url_id ";
		result_str += "JOIN words " + w_name + " ON " + wu_name + ".word_id = " + w_name + ".id AND " + w_name + ".id = '" + word + "' ";
		counter++;
	}
	return result_str;
}
