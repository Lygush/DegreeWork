#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>

enum class ErrorType
{
	VARIABLE = 0,
	ZERO_VARIABLE = 1,
	SECTION = 2
};

enum class SearchStrength
{
	LIGHT = 0,
	MIDDLE = 1,
	DEEP = 2
};

struct DataBaseProperties
{
	std::string host{};
	std::string port{};
	std::string dbname{};
	std::string user{};
	std::string password{};
};

struct SpiderProperties
{
	std::string start_page{};
	int depth{};
	SearchStrength search_strength{};
};

struct ServerProperties
{
	int server_port{};
};

class IniParser
{
public:
	IniParser(char* _path);
	void read_property();

protected:
	void str_analyser(std::string str);
	void set_property();
	std::string create_error_message(ErrorType type) const;
	SearchStrength define_search_strength(std::string& property);

public:
	DataBaseProperties data_base{};
	SpiderProperties spider{};
	ServerProperties server{};

private:
	char* path{};
	std::ifstream file{};

	std::string section_name{};
	std::string variable_name{};
	std::string variable_value{};

	bool section{false};
	bool var_value{false};
};