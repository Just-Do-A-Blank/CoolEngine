#pragma once

#include "json.hpp"
#include <fstream>
#include <iostream>
#include <windows.h>

using namespace nlohmann;

enum OBJECT_TYPE
{
	OBJECT_TYPE_NULL,
	OBJECT_TYPE_GAME_OBJECT,
	OBJECT_TYPE_SAVE_FILE,
	OBJECT_TYPE_MAP_FILE
};

/// <summary>
/// 
/// </summary>
struct GameObjectData
{
	GameObjectData() { m_Name = "null"; m_Position[0] = 0;  m_Position[1] = 0; TextureData = nullptr; };
	std::string m_Name;
	float m_Position[2];	
	void* TextureData;
};

struct SaveFileData
{
	SaveFileData() {};
};

struct MapFileData
{
	MapFileData() {};
};

/// <summary>
/// This class can be used to load files into the engine
/// </summary>
class Load
{
public:

	/// <summary>
	/// Loads a Game Object from a JSON file. Needs a file address to do so. Code in this function is temorary until the structure of the object that needs loading is made clear
	/// </summary>
	/// <param name="fileAddress"></param>
	/// <returns></returns>
	static GameObjectData GameObject(const char* fileAddress);

	/// <summary>
	/// Loads the map from a JSON file. Needs a file address to do so. Code in this function is temorary until the structure of the object that needs loading is made clear
	/// </summary>
	/// <param name="fileAddress"></param>
	/// <returns></returns>
	static MapFileData Map(const char* fileAddress);

	/// <summary>
	/// Loads a Save File from a JSON file. Needs a file address to do so. Code in this function is temorary until the structure of the object that needs loading is made clear
	/// </summary>
	/// <param name="fileAddress"></param>
	/// <returns></returns>
	static SaveFileData Savefile(const char* fileAddress);

	static std::vector<void*> FromFile(const char* fileAddress);

private:
	/// <summary>
	/// Takes a file location and then returns a nlohmann::json. Throws an exception in the visual studios output channel if it has failed.
	/// </summary>
	/// <param name="fileAddress"></param>
	/// <returns></returns>
	static json Json(const char* fileAddress);
};
