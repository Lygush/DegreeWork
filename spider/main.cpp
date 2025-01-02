#if defined(_WIN32)
#include <Windows.h>
#endif

#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <functional>

#include "../common/ini_parser.h"
#include "../common/common_functions.h"
#include "http_utils.h"
#include "html_parser.h"
#include "spider_database_worker.h"

std::mutex mtx;
std::condition_variable cv;
std::queue<std::function<void()>> tasks;
bool exitThreadPool = false;
const int max_redirect{5};

IniParser ini("Properties.ini");

void threadPoolWorker()
{
	std::unique_lock<std::mutex> lock(mtx);
	while (!exitThreadPool || !tasks.empty())
	{
		if (tasks.empty())
		{
			cv.wait(lock);
		}
		else
		{
			auto task = tasks.front();
			tasks.pop();
			lock.unlock();
			task();
			lock.lock();
		}
	}
}
void parseLink(const Link& link, int depth, SpiderDatabaseWorker& db_worker)
{
	try
	{

		std::this_thread::sleep_for(std::chrono::milliseconds(500));

		std::string html = getHtmlContent(link, max_redirect);

		if (html.size() == 0)
		{
			std::cout << "Failed to get HTML Content: " << link.hostName << link.query << std::endl;
			return;
		}

		HtmlParser html_parser(link, html);
		html_parser.set_properties(ini.spider);

		db_worker.add_url_to_database(link, html_parser.indexer());

		if (depth > 0)
		{
			std::vector<Link> links = {html_parser.get_sub_links()};

			std::lock_guard<std::mutex> lock(mtx);

			size_t count = links.size();

			for (auto& subLink : links)
			{
				if (!db_worker.url_already_exists(subLink))
				{
					tasks.push([subLink, depth, &db_worker]() { parseLink(subLink, depth - 1, db_worker); });
				}
			}
			cv.notify_one();
		}

		std::cout << tasks.size() << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}

int main()
{
#if defined(_WIN32)
	SetConsoleOutputCP(CP_UTF8);
#endif // (_WIN32)

	try
	{
		HtmlParser html_parser;
		ini.read_property();
		html_parser.set_properties(ini.spider);
		Link start_link{link_creator(ini.spider.start_page)};

		SpiderDatabaseWorker db_worker(ini);

		int numThreads = std::thread::hardware_concurrency();
		std::vector<std::thread> threadPool;

		for (int i = 0; i < numThreads; ++i)
		{
			threadPool.emplace_back(threadPoolWorker);
		}

		{
			std::lock_guard<std::mutex> lock(mtx);
			tasks.push([start_link, &db_worker]() { parseLink(start_link, ini.spider.depth, db_worker); });
			cv.notify_one();
		}

		std::this_thread::sleep_for(std::chrono::seconds(2));

		{
			std::lock_guard<std::mutex> lock(mtx);
			exitThreadPool = true;
			cv.notify_all();
		}

		for (auto& t : threadPool)
		{
			t.join();
		}
		std::cin.get();
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
		std::cout << "\n\n\nPress 'ENTER' to exit.";
		std::cin.get();
	}
	return 0;
}
