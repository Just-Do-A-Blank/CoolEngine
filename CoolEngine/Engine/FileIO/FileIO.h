#pragma once

#include "json.hpp"
#include "Engine/GameObjects/GameObject.h"
#include "Engine/Physics/ParticleManager.h"
#include "Engine/Managers/GraphicsManager.h"
#include <fstream>
#include <iostream>
#include <windows.h>
#include <codecvt>
#include <locale>

///All wstring handling is based off of this https://json.nlohmann.me/home/faq/#wide-string-handling & https://stackoverflow.com/questions/2573834/c-convert-string-or-char-to-wstring-or-wchar-t


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
/// Flags for functions where saving multiple data types at once is important
/// </summary>
enum JSON_VARIABLE_TYPE
{
	JSON_VARIABLE_TYPE_INT,
	JSON_VARIABLE_TYPE_CHAR,
	JSON_VARIABLE_TYPE_STRING,
	JSON_VARIABLE_TYPE_FLOAT,
	JSON_VARIABLE_TYPE_DOUBLE,
	JSON_VARIABLE_TYPE_WCHAR_T,
	JSON_VARIABLE_TYPE_XMFLOAT2,
	JSON_VARIABLE_TYPE_XMFLOAT3,
	JSON_VARIABLE_TYPE_XMFLOAT4
};

enum ENGINE_TYPE
{
	ENGINE_TYPE_GAME_OBJECT
};

class LoadedObjectContainer
{
public:
	ENGINE_TYPE m_ObjectType;
private:
	void* m_Data;
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
		m_TextureLocation = "";
		m_Name = "";
		m_IsRenderable = false;
		m_IsCollidable = false;
		m_IsTrigger = false;
		m_ColliderType = COLLIDER_TYPE_NO_COLLIDER;
	};

	XMFLOAT3 m_Position, m_Rotation, m_Scale;
	bool m_IsRenderable, m_IsCollidable, m_IsTrigger;
	std::string m_TextureLocation, m_Name;
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
	/// Returns a string of the current .exe location
	/// </summary>
	/// <returns></returns>
	inline std::string ExeLocation(bool removeExeName);

	/// <summary>
	/// Takes a string pointer and appends the .exe location to the front of the string
	/// </summary>
	/// <param name="fileAddress"></param>
	/// <returns>const char pointer with file address</returns>
	static std::string AttachDefaultFileLocation(std::string* fileAddress);

	/// <summary>
	/// Takes a string and appends the .exe location to the front of the string
	/// </summary>
	/// <param name="fileAddress"></param>
	/// <returns>const char pointer with file address</returns>
	static std::string AttachDefaultFileLocation(std::string fileAddress);

	/// <summary>
	/// Takes a const char pointer and appends the .exe location to the front of the string
	/// </summary>
	/// <param name="fileAddress"></param>
	/// <returns>const char pointer with file address</returns>
	static std::string AttachDefaultFileLocation(const char* fileAddress);

	/// <summary>
	/// Loads a Game Object from a JSON file.
	/// </summary>
	/// <param name="fileAddress"></param>
	/// <param name="manager"></param>
	/// <returns></returns>
	static GameObject LoadGameObject(const char* fileAddress, int objectCount);

	/// <summary>
	/// Loads a Game Object from a JSON file. This function also calls the load texture function
	/// </summary>
	/// <param name="fileAddress"></param>
	/// <param name="manager"></param>
	/// <returns></returns>
	static GameObject LoadGameObject(const char* fileAddress, ID3D11Device* device, int objectCount);

	/// <summary>
	/// Loads multiple game objects from a singular JSON file
	/// </summary>
	/// <param name="fileAddress"></param>
	/// <returns>std::vector<GameObject></returns>
	static std::vector<GameObject> LoadMultipleGameObjects(const char* fileAddress);


	/// <summary>
	/// Loads a map from a JSON file
	/// </summary>
	/// <param name="file"></param>
	/// <param name="tiles"></param>
	/// <returns></returns>
	static std::vector<GameObject> LoadMap(json file, std::vector<GameObject> tiles);

	/// <summary>
	/// Loads game objects, tiles and a map from a JSON file. 
	/// </summary>
	static std::vector<std::vector<GameObject>>  LoadScene(const char* fileAddress);

	/// <summary>
	/// Loads a Save File from a JSON file. Needs a file address to do so. Code in this function is temorary until the structure of the object that needs loading is made clear
	/// </summary>
	/// <param name="fileAddress"></param>
	/// <returns></returns>
	static void LoadSavefile(const char* fileAddress);

	/// <summary>
	/// Takes a file location and then returns a nlohmann::json. Throws an exception in the visual studios output channel if it has failed to open the file.
	/// </summary>
	/// <param name="char* fileAddress"></param>
	/// <returns>returns the output of the file in the expected type, if there is an exception it returns the default value of the variable</returns>
	static json LoadJson(const char* fileAddress);

	/// <summary>
	/// Takes a file location and then returns a specified type.Throws an exception in the visual studios output channel if it has failed to open the file.
	/// </summary>
	/// <param name="char* fileAddress"></param>
	/// <returns>returns the output of the file in the expected type, if there is an exception it returns the default value of the variable</returns>
	template <typename t>
	static t LoadTextFile(const char* fileAddress);

	static ParticleManager LoadParticle(const char* fileLocation, int particleNumber);


	static ParticleManager LoadParticle(json j, int particleNumber);



	/// <summary>
	/// A function that saves data related to a Game Object and saves it to a file. Ensure to specify the file type in the string
	/// </summary>
	/// <param name="gameObjData"></param>
	/// <param name="fileLocation"></param>
	static void SaveGameObject(GameObjectData gameObjData, const char* fileLocation);

	/// <summary>
	/// This function allows for the saving of a custom json file.
	/// </summary>
	/// <param name="fileLocation"></param>
	/// <param name="varNames"></param>
	/// <param name="types"></param>
	/// <param name="data"></param>
	/// <returns></returns>
	static bool SaveObjectInJson(const char* fileLocation, std::vector<std::string> varNames, std::vector<JSON_VARIABLE_TYPE> types, std::vector<void*> data);

private:

	/// <summary>
	/// Determins what collider type a object has from a string
	/// </summary>
	/// <param name="colliderType"></param>
	/// <returns>COLLIDER_TYPE</returns>
	static COLLIDER_TYPE ColliderType(std::string colliderType);

	/// <summary>
/// Loads multiple game objects from a singular JSON file
/// </summary>
/// <param name="fileAddress"></param>
/// <returns></returns>
	static std::vector<GameObject> LoadMultipleGameObjects(json file);

	/// <summary>
/// Loads multiple game objects from a singular JSON file
/// </summary>
/// <param name="fileAddress"></param>
/// <returns>GameObject<GameObject></returns>
	static GameObject LoadGameObject(json file, int objectCount);

	/// <summary>
	/// Loads Multiple tiles from a JSON file
	/// </summary>
	/// <param name="file"></param>
	/// <returns></returns>
	static std::vector<GameObject> LoadMultipleTiles(json file);

	/// <summary>
	/// Loads Multiple tiles from a JSON file
	/// </summary>
	/// <param name="file"></param>
	/// <returns></returns>
	static std::vector<ParticleManager> LoadMultipleParticles(json file);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="wide_string"></param>
	/// <returns></returns>
	static std::wstring ToWstring(std::string& wide_string);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="wide_string"></param>
	/// <returns></returns>
	static std::string FileIO::ToString(std::wstring& wide_string);
};
