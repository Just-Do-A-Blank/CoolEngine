#include "Load.h"

json Load::Json(const char* fileAddress)
{
	std::ifstream file(fileAddress);
	try
	{
		if (file)
		{
			json j;
			file >> j;
			return j;
		}
		throw std::exception("File was not opened");
	}
	catch (const std::exception& e)
	{
		OutputDebugStringA(e.what());
		return json();
	}
}
