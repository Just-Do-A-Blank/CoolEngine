#pragma once
#include <fstream>
#include "json.hpp"
#include "Load.h"

using namespace nlohmann;


struct GameObjectSaveData
{
	GameObjectSaveData() {m_Name = "null"; m_Position[0] = 0;  m_Position[1] = 0; m_TextureLocation = "null"; };
	GameObjectSaveData(std::string name, float* positionArr, std::string texLocation) { m_Name = name; m_Position[0] = *positionArr; ++positionArr;  m_Position[1] = *positionArr; m_TextureLocation = texLocation; };
	std::string m_Name;
	float m_Position[2];
	std::string m_TextureLocation;
};

class Save
{
public:
	static void SaveFile();
	static void Map();
	static void GameObject(GameObjectSaveData god);
};

