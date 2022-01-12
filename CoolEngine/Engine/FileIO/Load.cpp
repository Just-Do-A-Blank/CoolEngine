#include "Load.h"

std::vector<void*> Load::FromFile(const char* fileAddress)
{
	return std::vector<void*>();
}

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


GameObjectData Load::GameObject(const char* fileAddress)
{
	json m_json = Load::Json(fileAddress);
	json data = m_json["MetaData"];
	GameObjectData god = GameObjectData();
	god.m_ObjectType = OBJECT_TYPE_GAME_OBJECT;
	god.m_Name = data.at(0)["Name"];
	god.m_Position[0] = data.at(0)["Position"][0];
	god.m_Position[1] = data.at(0)["Position"][1];
	std::string i = data.at(0)["TextureName"];
	return god;
}

MapFileData Load::Map(const char* fileAddress)
{
	json m_json = Load::Json(fileAddress);
	json data = m_json["MetaData"];
	MapFileData mfd = MapFileData();
	return mfd;
}

SaveFileData Load::Savefile(const char* fileAddress)
{
	json m_json = Load::Json(fileAddress);
	SaveFileData sfd = SaveFileData();
	return sfd;
}
