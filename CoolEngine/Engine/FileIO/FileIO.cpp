#include "FileIO.h"

#include "Engine/Managers/AudioManager.h"
#include "Engine/Managers/FontManager.h"
#include "Engine/Managers/GraphicsManager.h"

void SimpleFileIO::LoadScene(std::string location)
{
	ifstream fileIn(location);
	json dataIn;
	fileIn >> dataIn;

	AudioManager::GetInstance()->Deserialize(dataIn);
	GraphicsManager::GetInstance()->Deserialize(dataIn);
	FontManager::GetInstance()->Deserialize(dataIn);
	GameManager::GetInstance()->Deserialize(dataIn);
}

void SimpleFileIO::SaveScene(std::string location)
{
	location.append(".json");

	std::string path = GameManager::GetInstance()->GetWorkingDirectory() + "\\" + location;

	ofstream fileOut(path);

	json outData;


	AudioManager::GetInstance()->Serialize(outData);
	GraphicsManager::GetInstance()->Serialize(outData);
	FontManager::GetInstance()->Serialize(outData);
	GameManager::GetInstance()->Serialize(outData);

	fileOut << outData;

	fileOut.close();
}

std::string SimpleFileIO::ToString(std::wstring& wideString)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
	return conv.to_bytes(wideString);
}

std::wstring SimpleFileIO::ToWstring(std::string& string)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	std::wstring wide = converter.from_bytes(string);
	return wide;
}

