#pragma once

#include "Engine/GameObjects/GameObject.h"
#include "Engine/Physics/ParticleManager.h"
#include "Engine/Physics/Circle.h"
#include "Engine/Physics/Box.h"
#include "Engine/Managers/GraphicsManager.h"
#include "Engine/Managers/GameManager.h"
#include "Engine/Managers/FontManager.h"
#include "Engine/Scene/Scene.h"
#include "Engine/Includes/json.hpp"
#include "Engine/TileMap/TileMap/TileMap.h"
#include "Engine/Graphics/SpriteAnimation.h"
#include "Engine/GameUI/ImageComponent.h"
#include "Engine/GameUI/TextComponent.h"

#include <fstream>
#include <iostream>
#include <windows.h>
#include <codecvt>
#include <locale>

///All wstring handling is based off of this https://json.nlohmann.me/home/faq/#wide-string-handling & https://stackoverflow.com/questions/2573834/c-convert-string-or-char-to-wstring-or-wchar-t

using namespace nlohmann;

/// <summary>
/// This class can be used to load & save files in the engine, it is a simplified version focusing on loading one object at a time.
/// </summary>
class SimpleFileIO
{
	friend GameObject;
public:

	/// <summary>
	/// This function loads a scene, functions by calling a deserilze function on each manager in the engine and takes the data from a specified json
	/// </summary>
	/// <param name="location"></param>
	static void LoadScene(std::string location);

	/// <summary>
	///   This function saves a scene, functions by calling a serialize function on each manager in the engine which calls a serialize function on each object and inserts this data into a new json file.
	/// </summary>
	/// <param name="location"></param>
	static void SaveScene(std::string location);

private:

	/// <summary>
	/// Converts a string to wstring
	/// </summary>
	/// <param name="wide_string"></param>
	/// <returns></returns>
	static std::wstring ToWstring(std::string& wide_string);

	/// <summary>
	/// Converts a wstring to string
	/// </summary>
	/// <param name="wide_string"></param>
	/// <returns></returns>
	static std::string ToString(std::wstring& wide_string);

};