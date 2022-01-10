#pragma once

#include "json.hpp"
#include <fstream>
#include <windows.h>

using namespace nlohmann;

/// <summary>
/// This class can be used to load files into the engine
/// </summary>
class Load
{
public:

	/// <summary>
	/// Takes a file location and then returns a nlohmann::json. Throws an exception in the visual studios output channel if it has failed.
	/// </summary>
	/// <param name="fileAddress"></param>
	/// <returns></returns>
	static json Json(const char* fileAddress);

};

