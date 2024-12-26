#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>

#include <string>
#include <iostream>

#include "http_connection.h"
#include "../common/ini_parser.h"

void httpServer(tcp::acceptor& acceptor, tcp::socket& socket)
{
	acceptor.async_accept(socket,
		[&](beast::error_code ec)
		{
			if (!ec)
				std::make_shared<HttpConnection>(std::move(socket))->start();
			httpServer(acceptor, socket);
		});
}

int main(int argc, char* argv[])
{
#if defined(_WIN32)
	SetConsoleCP(CP_UTF8);
	SetConsoleOutputCP(CP_UTF8);
#endif

	try
	{
		IniParser ini("Properties.ini");
		ini.read_property();

		auto const address = net::ip::make_address("0.0.0.0");
		unsigned short port = ini.server.server_port;

		net::io_context ioc{1};

		tcp::acceptor acceptor{ioc, {address, port}};
		tcp::socket socket{ioc};
		httpServer(acceptor, socket);

		std::cout << "Open browser and connect to http://localhost:8080 to see the web server operating" << std::endl;

		ioc.run();
	}
	catch (std::exception const& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		std::cin.get();
		return EXIT_FAILURE;
	}
}