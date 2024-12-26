#include "ini_parser.h"

IniParser::IniParser(char* _path)
{
	path = _path;
}

void IniParser::read_property()
{
	file.open(path);
	if (!file.is_open())
	{
		throw std::exception("File not open!");
	}
	else
	{
		std::string s{};
		while (getline(file, s))
		{
			variable_name.clear();
			str_analyser(s);
		}
	}
	file.close();
}

void IniParser::str_analyser(std::string str)
{
	if (str.size() == 0)
	{
		return;
	}

	for (const auto i : str)
	{
		if (i != 32)
		{
			if (i == '#')
			{
				return;
			}
			if (i == '[')
			{
				section = true;
				section_name.clear();
			}
			else if (i == ']')
			{
				section = false;
				return;
			}
			else if (i == '=' && !var_value)
			{
				var_value = true;
				variable_value.clear();
			}
			else
			{
				if (section)
				{
					const char low_i = tolower(i);
					section_name += low_i;
				}
				else if (var_value)
				{
					variable_value += i;
				}
				else
				{
					const char low_i = tolower(i);
					variable_name += low_i;
				}
			}
		}
	}
	var_value = false;
	if (section)
	{
		throw std::exception(create_error_message(ErrorType::SECTION).c_str());
	}
	set_property();
}

void IniParser::set_property()
{
	if (variable_value.length() == 0)
	{
		throw std::exception(create_error_message(ErrorType::ZERO_VARIABLE).c_str());
	}
	if (section_name == "data_base")
	{
		if (variable_name == "host")
		{
			data_base.host = variable_value;
		}
		else if (variable_name == "port")
		{
			data_base.port = variable_value;
		}
		else if (variable_name == "dbname")
		{
			data_base.dbname = variable_value;
		}
		else if (variable_name == "user")
		{
			data_base.user = variable_value;
		}
		else if (variable_name == "password")
		{
			data_base.password = variable_value;
		}
		else
		{
			throw std::exception(create_error_message(ErrorType::VARIABLE).c_str());
		}
	}
	else if (section_name == "spider")
	{
		if (variable_name == "start_page")
		{
			spider.start_page = variable_value;
		}
		else if (variable_name == "depth")
		{
			spider.depth = stoi(variable_value);
		}
		else if (variable_name == "search_strength")
		{
			spider.search_strength = define_search_strength(variable_value);
		}
		else
		{
			throw std::exception(create_error_message(ErrorType::VARIABLE).c_str());
		}
	}
	else if (section_name == "http_server")
	{
		if (variable_name == "server_port")
		{
			server.server_port = stoi(variable_value);
		}
		else
		{
			throw std::exception(create_error_message(ErrorType::VARIABLE).c_str());
		}
	}
	else
	{
		{
			throw std::exception(create_error_message(ErrorType::SECTION).c_str());
		}
	}
}

std::string IniParser::create_error_message(ErrorType type) const
{
	switch (type)
	{
	case ErrorType::VARIABLE:
		return std::string{"Ini_parser error!\n"
						   "This VARIABLE does not exist or variable NAME is incorrect.\n"
						   "Section name - [" +
						   section_name +
						   "]\n"
						   "Variable name - '" +
						   variable_name + "'"};
		break;
	case ErrorType::ZERO_VARIABLE:
		return std::string{"Ini_parser error!\n"
						   "Missing VARIABLE VALUE\n"
						   "Section name = [" +
						   section_name +
						   "]\n"
						   "Variable name - '" +
						   variable_name + "' =" + variable_value};
		break;
	case ErrorType::SECTION:
		return std::string{"Ini_parser error!\n"
						   "The SECTION name is incorrect or SYNTAX error.\n"
						   "Section name = [" +
						   section_name + "]"};
		break;
	default:
		break;
	}
}

SearchStrength IniParser::define_search_strength(std::string& property)
{
	int depth{stoi(property)};
	switch (depth)
	{
	case 0:
		return SearchStrength::LIGHT;
		break;
	case 1:
		return SearchStrength::MIDDLE;
		break;
	case 2:
		return SearchStrength::DEEP;
	default:
		break;
	}
	return SearchStrength::LIGHT;
}
