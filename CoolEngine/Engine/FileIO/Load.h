#pragma once

#include "json.hpp"
#include <fstream>
#include <windows.h>

using namespace nlohmann;

class Load
{
public:
	static json Json(const char* fileAddress);

};

