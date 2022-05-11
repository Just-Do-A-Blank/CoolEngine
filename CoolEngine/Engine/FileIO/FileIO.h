#pragma once

#include "Engine/GameObjects/GameObject.h"
#include "Engine/Physics/ParticleManager.h"
#include "Engine/Physics/Circle.h"
#include "Engine/Physics/Box.h"
#include "Engine/Managers/GraphicsManager.h"
#include "Engine/Managers/GameManager.h"
#include "Engine/Managers/UIManager.h"
#include "Engine/Managers/FontManager.h"
#include "Engine/Scene/Scene.h"
#include  "Engine/Includes/json.hpp"
#include "Engine/TileMap/TileMap/TileMap.h"
#include "Engine/FileIO/FileIOCache.h"
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
/// A storage container for data related to the particle class type
/// </summary>
struct ParticleSystemData
{
	Transform m_Transform;
	wstring m_Albedo;
	int m_SystemType;
	float m_Life;
};

/// <summary>
/// A storage container for data related to the particle class type
/// </summary>
struct ParticleData
{
	Transform m_Transform;
	XMFLOAT2 m_Velocity;
	XMFLOAT2 m_Acceleration;
	ID3D11ShaderResourceView* m_Texture;
	float m_Life;
	float m_RandomPosition;
	float m_RandomVelocity;
	float m_RandomAcceleration;
	float m_RandomLife;
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
	JSON_VARIABLE_TYPE_XMFLOAT4,
	JSON_VARIABLE_TYPE_GAME_OBJECTS
};



static class JsonSerializer
{
public:

	std::string Serialize(json data);

	//std::string Serialize(std::string objecrName);

	json Deserialize(std::string obj);


private:

	void ParseString();

};



/// <summary>
/// This class can be used to load & save files in the engine, it is a simplified version focusing on loading one object at a time.
/// </summary>
class SimpleFileIO
{
	friend GameObject;
public:

	static void LoadScene(std::string location);

	static void SaveScene(std::string location);


private:


	static void LoadGameObject(const char* fileAddress);

	static void LoadTile(const char* fileAddress);

	static void LoadParticle(const char* fileAddress);

	static void LoadMap(const char* fileAddress);

	static json LoadJsonFile(const char* fileAddress);


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


/// <summary>
/// This class can be used to load & save files in the engine
/// </summary>
class FileIO
{
	friend GameObject;
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
	static GameObject* LoadGameObject(const char* fileAddress, int objectCount);

	/// <summary>
	/// Loads multiple Game Object from a JSON file.
	/// </summary>
	/// <param name="fileAddress"></param>
	/// <param name="manager"></param>
	/// <returns></returns>
	static std::vector<GameObject*> FileIO::LoadMultipleGameObjects(const char* fileAddress);

	/// <summary>
	/// Loads a map from a JSON file
	/// </summary>
	/// <param name="file"></param>
	/// <param name="tiles"></param>
	/// <returns></returns>
	static void LoadMap(json file, GameManager* scene);

	/// <summary>
	/// Loads game objects, tiles and a map from a JSON file. 
	/// <param name="fileAddress"></param>
	/// <param name="scene"></param>
	/// <param name="pManager"></param>
	/// </summary>
	static void LoadScene(const char* fileAddress, GameManager* pScene, ParticleManager* pManager);

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

	/// <summary>
	/// loads a set of data related to a particle from a json file
	/// </summary>
	/// <param name="j"></param>
	/// <param name="particleNumber"></param>
	/// <returns></returns>
	static ParticleData FileIO::LoadParticle(json j, int particleNumber);

	static std::unordered_map<std::string, void*> LoadCustomJsonData(const char* fileAddress, std::vector<std::string> variableNames, std::vector<JSON_VARIABLE_TYPE> variableTypes);

	/// <summary>
	/// This function allows for the saving of a custom json file.
	/// </summary>
	/// <param name="fileLocation"></param>
	/// <param name="varNames"></param>
	/// <param name="types"></param>
	/// <param name="data"></param>
	/// <returns></returns>
	static bool SaveObjectInJson(const char* fileLocation, std::vector<std::string> varNames, std::vector<JSON_VARIABLE_TYPE> types, std::vector<void*> data);

	static void LoadUI(const char* fileLocation, UIManager* pUManager, ID3D11Device* device);


	static void SaveUI(const char* fileLocation, UIManager* pUManager);

	/// <summary>
	/// To DO
	/// </summary>
	static void SaveGameObject(const char* fileLocation, GameObject* gameObject);

	/// <summary>
	/// Saves the current active scene in a json file.
	/// </summary>
	/// <param name="fileLocation"></param>
	/// <param name="scene"></param>
	static void SaveScene(const char* fileLocation, GameManager* scene);


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
/// <returns>GameObject<GameObject></returns>
	static GameObject* LoadGameObject(json file, int objectCount);

	/// <summary>
	/// Loads multiple game objects from a single json file
	/// </summary>
	/// <param name="j"></param>
	/// <returns></returns>
	static std::vector<GameObject*> FileIO::LoadMultipleGameObjects(json j);

	/// <summary>
	/// Loads Multiple tiles from a JSON file
	/// </summary>
	/// <param name="file"></param>
	/// <returns></returns>
	static void LoadMultipleParticles(json file);

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
	static std::string FileIO::ToString(std::wstring& wide_string);

	/// <summary>
	/// Accesses the FileIO's cache to return the location of a vertex shader. Note this only functions if the vertex shader was loaded using the class.
	/// </summary>
	/// <param name="shader"></param>
	/// <returns></returns>
	static wstring GrabVertexShaderName(ID3D11VertexShader* shader);

	/// <summary>
	/// Accesses the FileIO's cache to return the location of a vertex shader. Note this only functions if the vertex shader was loaded using the class.
	/// </summary>
	/// <param name="shader"></param>
	/// <returns></returns>
	static wstring GrabPixelShaderName(ID3D11PixelShader* shader);

	/// <summary>
	/// Accesses the FileIO's cache to return the location of a pixel shader. Note this only functions if the pixel shader was loaded using the class.
	/// </summary>
	/// <param name="shader"></param>
	/// <returns></returns>

	/// <summary>
	/// Accesses the FileIO's cache to return the location of a texture. Note this only functions if the texture was loaded using the class.
	/// </summary>
	/// <param name="shader"></param>
	/// <returns></returns>
	static wstring GrabTextureName(ID3D11ShaderResourceView* texture);

	/// <summary>
	/// Accesses the FileIO's cache to return the location of a albedo texture. Note this only functions if the albedo texture was loaded using the class.
	/// </summary>
	/// <param name="shader"></param>
	/// <returns></returns>
	static wstring GrabAlbedoName(ID3D11ShaderResourceView* albedo);

	static json PackJson(GameObject* dataToPack, int count);
	/// <summary>
	/// Local cache that contains data on the location textures, shaders and more.
	/// </summary>
	static FileIOCache m_Cache;
};
