

#include "common_functions.h"

Link link_creator(std::string& str_link, const  Link& _base_link)
{
	if (str_link.find("http") != std::string::npos)
	{
		return link_creator(str_link);
	}
	Link link{};
	link.protocol = _base_link.protocol; 
	link.hostName = _base_link.hostName;
	//link.query = str_link;
	for (auto i : str_link)
	{
		if (i == '#')
		{
			return link;
		}
		else
		{
			link.query += i;
		}
	}
	return link;
}

Link link_creator(std::string& str_link)
{
	if (str_link.empty())
	{
		return Link{};
	}
	Link link{};
	bool hostName_done{};
	auto link_start{str_link.find("https")}; 
	if (link_start != std::string::npos)
	{ 
		link.protocol = ProtocolType::HTTPS;
		link_start += 8;
	} 
	else
	{
		link.protocol = ProtocolType::HTTP; 
		link_start += 7;
	}
	for (auto i{link_start}; i != str_link.size(); i++)
	{
		if (str_link[i] == '#')
		{
			if (link.query.empty())
			{
				return Link();
			}
			else
			{
				return link;
			}
		}
		if (str_link[i] != '/' && !hostName_done)
		{
			link.hostName += str_link[i];
		}
		else if (str_link[i] == '/' && !hostName_done)
		{
			link.query += str_link[i];
			hostName_done = true;
		}
		else if (hostName_done)
		{
			link.query += str_link[i];
		}
		else
		{
			std::cout << "Link creator error! Error in the link" << std::endl;
			return Link{};
		}
	}

	return link;
}
