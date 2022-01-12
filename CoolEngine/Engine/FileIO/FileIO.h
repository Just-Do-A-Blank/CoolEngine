#pragma once

#include "json.hpp"
#include "Engine/Managers/GameManager.h"
#include <fstream>
#include <iostream>
#include <windows.h>

using namespace nlohmann;

/// <summary>
/// Flags that can be used to determin an objects collider type
/// </summary>
enum COLLIDER_TYPE
{
	COLLIDER_TYPE_BOX,
	COLLIDER_TYPE_CIRCLE,
	COLLIDER_TYPE_LINE,
	COLLIDER_TYPE_NO_COLLIDER
};

/// <summary>
/// A struct that contains data related to a game object
/// </summary>
struct GameObjectData
{
	GameObjectData() 
	{ 
		m_Position = XMFLOAT3();
		m_Rotation = XMFLOAT3();
		m_Scale = XMFLOAT3();
		m_TextureData = nullptr; 
		m_IsRenderable = false;
		m_IsCollidable = false;
		m_IsTrigger = false;
		m_ColliderType = COLLIDER_TYPE_NO_COLLIDER;
	};

	XMFLOAT3 m_Position, m_Rotation, m_Scale;
	bool m_IsRenderable, m_IsCollidable, m_IsTrigger;
	ID3D11Resource* m_TextureData;
	COLLIDER_TYPE m_ColliderType;
};

/// <summary>
/// A struct that contains data related to save data
/// </summary>
struct SaveFileData
{
	SaveFileData() {};
};

/// <summary>
/// A struct that contains data related to a map file
/// </summary>
struct MapFileData
{
	MapFileData() {};
};

/// <summary>
/// This class can be used to load & save files in the engine
/// </summary>
class FileIO
{
public:

	/// <summary>
	/// Loads a Game Object from a JSON file.
	/// </summary>
	/// <param name="fileAddress"></param>
	/// <param name="manager"></param>
	/// <returns></returns>
	static GameObjectData LoadGameObject(const char* fileAddress);

	/// <summary>
	/// Loads multiple game objects from a singular JSON file
	/// </summary>
	/// <param name="fileAddress"></param>
	/// <returns>std::vector<GameObjectData></returns>
	static std::vector<GameObjectData> LoadMultipleGameObjects(const char* fileAddress);

	/// <summary>
	/// Loads the map from a JSON file. Needs a file address to do so. Code in this function is temorary until the structure of the object that needs loading is made clear
	/// </summary>
	/// <param name="fileAddress"></param>
	/// <returns></returns>
	static void LoadMap(const char* fileAddress);

	/// <summary>
	/// Loads a Save File from a JSON file. Needs a file address to do so. Code in this function is temorary until the structure of the object that needs loading is made clear
	/// </summary>
	/// <param name="fileAddress"></param>
	/// <returns></returns>
	static void LoadSavefile(const char* fileAddress);

	/// <summary>
	/// A function that saves data related to a Game Object and saves it to a file. Ensure to specify the file type in the string
	/// </summary>
	/// <param name="gameObjData"></param>
	/// <param name="fileLocation"></param>
	static void SaveGameObject(GameObjectData gameObjData, const char* fileLocation);

private:
	/// <summary>
	/// Takes a file location and then returns a nlohmann::json. Throws an exception in the visual studios output channel if it has failed.
	/// </summary>
	/// <param name="char* fileAddress"></param>
	/// <returns></returns>
	static json LoadJson(const char* fileAddress);

	/// <summary>
	/// Determins what collider type a object has from a string
	/// </summary>
	/// <param name="colliderType"></param>
	/// <returns>COLLIDER_TYPE</returns>
	static COLLIDER_TYPE ColliderType(std::string colliderType);
};
