#pragma once 
#include <vector>
#include <string>
#include "link.h"

std::string getHtmlContent(const Link& link, int max_redirects);
