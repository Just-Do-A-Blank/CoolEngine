#include "SaveFile.h"
void Save::SaveFile()
{

}

void Save::Map()
{
	json m_JsonOutput;
	std::ofstream outFile;
	outFile.open("TestFile.json");
	outFile << m_JsonOutput;
	outFile.close();
}

void Save::GameObject(GameObjectSaveData gosd)
{
	std::ofstream outFile;
	outFile.open("TestFile.json");
	json m_JsonOutput
	{
		{"Metadata", 
			{
				{"Name", gosd.m_Name},
				{"Position", {gosd.m_Position[0], gosd.m_Position[1]}},
				{"TextureName", gosd.m_TextureLocation}
			}
		}
	};
	outFile << m_JsonOutput;
	outFile.close();
}
