#pragma once

#include <string>

static std::string className(const std::string& prettyFunction)
{
	size_t colons = prettyFunction.find("::");
	if (colons == std::string::npos)
		return "::";
	size_t begin = prettyFunction.substr(0, colons).rfind(" ") + 1;
	size_t end = colons - begin;

	return prettyFunction.substr(begin, end);
}

#define __CLASS_NAME__ className(__FUNCSIG__)

#if _DEBUG 
#define LOG(message)cout << message << " - " << __CLASS_NAME__ << ", " << __LINE__ <<endl;
#else
#define LOG(message)
#endif
