#pragma once

#include <string>
#include <iostream>

#include "../spider/link.h"

Link link_creator(std::string& str_link, Link& _base_link);
Link link_creator(std::string& str_link);
