#include "FileIO.h"

COLLIDER_TYPE FileIO::ColliderType(std::string colliderType)
{
	if (colliderType == "BoxCollider")
	{
		return COLLIDER_TYPE::COLLIDER_TYPE_BOX;
	}
	else if (colliderType == "CircleCollider")
	{
		return COLLIDER_TYPE::COLLIDER_TYPE_CIRCLE;
	}
	else if(colliderType == "LineCollider")
	{
		return COLLIDER_TYPE::COLLIDER_TYPE_LINE;
	}
	else
	{
		OutputDebugStringA("No Matching Collider");
		return COLLIDER_TYPE::COLLIDER_TYPE_NO_COLLIDER;
	}

}

json FileIO::LoadJson(const char* fileAddress)
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

template <typename t>
t FileIO::LoadTextFile(const char* fileAddress)
{
	try
	{
		if (file)
		{
			t result;
			result >> file;
			return file;
		}
		throw std::exception("File was not opened");
	}
	catch (const std::exception& e)
	{
		OutputDebugStringA(e.what());
		return t();
	}
}

std::vector<GameObject> FileIO::LoadMultipleGameObjects(const char* fileAddress)
{
	json m_json = LoadJson(fileAddress);
	json data = m_json["ObjectData"];
	json metaData = m_json["MetaData"];

	std::vector<GameObject> objects((int)metaData.at(0)["NumberOfObject"]);

	for (size_t i = 0; i < metaData.at(0)["NumberOfObject"]; i++)
	{
		objects[i] = GameObject((std::string)data.at(i)["Name"]);
		objects[i].GetTransform()->SetPosition(XMFLOAT3(data.at(i)["Position"][0], data.at(i)["Position"][1], data.at(i)["Position"][2]));
		objects[i].GetTransform()->SetRotation(XMFLOAT3(data.at(i)["Rotation"][0], data.at(i)["Rotation"][1], data.at(i)["Rotation"][2]));
		objects[i].GetTransform()->SetScale(XMFLOAT3(data.at(i)["Scale"][0], data.at(i)["Scale"][1], data.at(i)["Scale"][2]));

		objects[i].SetMesh(QUAD_MESH_NAME);

		if (data.at(i)["VertexShaderLocation"] == "NULL")
		{
			objects[i].SetVertexShader(DEFAULT_VERTEX_SHADER_NAME);
		}
		else
		{
			objects[i].SetVertexShader(data.at(i)["VertexShaderLocation"]);
		}
		if (data.at(i)["PixelShaderLocation"] == "NULL")
		{
			objects[i].SetVertexShader(DEFAULT_PIXEL_SHADER_NAME);
		}
		else
		{
			objects[i].SetPixelShader(data.at(i)["PixelShaderLocation"]);
		}
		if (data.at(i)["Albedo"] == "NULL")
		{
			objects[i].SetAlbedo(L"Resources/Sprites/Brick.dds");
		}
		else
		{
			objects[i].SetAlbedo(data.at(i)["Albedo"]);
		}


		bool renderable = false;
		bool trigger = false;
		bool collideable = false;

		if (data.at(i)["IsRenderable"])
		{
			renderable = true;
			objects[i].SetIsRenderable(renderable);
		}
		else
		{
			objects[i].SetIsRenderable(renderable);
		}
		if (data.at(i)["IsTrigger"])
		{
			trigger = true;
			objects[i].SetIsTrigger(trigger);
		}
		else
		{
			//objects[i].SetIsTrigger(trigger);
		}
		if (data.at(i)["IsCollideable"])
		{
			//collideable = true;
			//objects[i].SetIsCollidable(collideable);
		}
		else
		{
			//objects[i].SetIsTrigger(collideable);
		}
	}
	return objects;
}

std::vector<GameObject> FileIO::LoadMultipleGameObjects(json file)
{
	json data = file["ObjectData"];
	json metaData = file["MetaData"];

	std::vector<GameObject> objects((int)metaData.at(0)["NumberOfObject"]);

	for (size_t i = 0; i < metaData.at(0)["NumberOfObject"]; i++)
	{
		objects[i] = LoadGameObject(data, i);
		int b = 0;
	}
	return objects;
}

std::vector<GameObject> FileIO::LoadMultipleTiles(json file)
{
	json data = file["TilesData"];
	json metaData = file["MetaData"];

	std::vector<GameObject> objects((int)metaData.at(0)["NumberOfTiles"]);

	for (size_t i = 0; i < metaData.at(0)["NumberOfTiles"]; i++)
	{
		objects[i] = GameObject((std::string)data.at(i)["Name"]);

		bool renderable = false;
		bool trigger = false;
		bool collideable = false;

		if (data.at(i)["IsRenderable"])
		{
			renderable = true;
			//objects[i].SetIsRenderable(renderable);
		}
		else
		{
			//objects[i].SetIsRenderable(renderable);
		}
		if (data.at(i)["IsTrigger"])
		{
			trigger = true;
			//objects[i].SetIsTrigger(trigger);
		}
		else
		{
			//objects[i].SetIsTrigger(trigger);
		}
		if (data.at(i)["IsCollideable"])
		{
			collideable = true;
			//objects[i].SetIsCollidable(collideable);
		}
		else
		{
			//objects[i].SetIsTrigger(collideable);
		}
	}
	return objects;
}

std::vector<ParticleManager> FileIO::LoadMultipleParticles(json file)
{

	return std::vector<ParticleManager>();
}

std::string FileIO::ToString(std::wstring& wideString)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
	return conv.to_bytes(wideString);
}

std::wstring FileIO::ToWstring(std::string& string)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	std::wstring wide = converter.from_bytes(string);
	return wide;
}

std::vector<std::vector<GameObject>> FileIO::LoadScene(const char* fileAddress)
{
	json j = FileIO::LoadJson(fileAddress);

	std::vector<std::vector<GameObject>> fileData(3);
	fileData[0] = std::vector<GameObject>((int)j["MetaData"].at(0)["NumberOfObject"]);
	fileData[1] = std::vector<GameObject>((int)j["MetaData"].at(0)["NumberOfTiles"]);
	fileData[2] = std::vector<GameObject>((int)j["MetaData"].at(0)["NumberOfTileZones"]);

	std::vector<GameObject> gameObj = LoadMultipleGameObjects(j);
	std::vector<GameObject> tileObj = LoadMultipleTiles(j);
	std::vector<GameObject> mapObj = LoadMap(j, tileObj);

	fileData[0] = gameObj;
	fileData[1] = tileObj;
	fileData[2] = mapObj;

	return fileData;
}

std::vector<GameObject> FileIO::LoadMap(json file, std::vector<GameObject> tiles)
{
	json j = file["TileMap"];
	int mapSize = j.at(0)["Area"][0] * (int)j.at(0)["Area"][1];
	std::vector<GameObject> mapStorage = std::vector<GameObject>(mapSize);
	for (size_t i = 1; i < file["MetaData"].at(0)["NumberOfTileZones"]; ++i)
	{
		GameObject obj("");
		for (size_t count = 0; count < tiles.size(); ++count)
		{
			if (tiles[count].GetIdentifier() == j.at(i)["TileName"])
			{
				obj = tiles[count];
			}
		}

		for (size_t yAxis = j.at(i)["AreaY"][0]; yAxis < j.at(i)["AreaY"][1]; ++yAxis)
		{
			for (size_t xAxis = j.at(i)["AreaX"][0]; xAxis < j.at(i)["AreaX"][1]; ++xAxis)
			{
				obj.GetTransform()->SetPosition(XMFLOAT3(xAxis, yAxis, 0.f));
				mapStorage[xAxis + yAxis] = obj;
			}
		}
	}


	return mapStorage;
}

inline std::string FileIO::ExeLocation(bool removeExeName)
{
	char exePath[MAX_PATH];
	//based on fatih sennik's answer on https://stackoverflow.com/questions/1023306/finding-current-executables-path-without-proc-self-exe/1024937#1024937
	DWORD result = GetModuleFileNameA(NULL, exePath, MAX_PATH);
	std::string finalOutput(exePath);
	if (removeExeName)
	{
		int exeNameStart = finalOutput.find_last_of('\\');
		finalOutput.erase(exeNameStart);
	}
	return finalOutput;
}

std::string FileIO::AttachDefaultFileLocation(std::string* fileAddress)
{
	char exePath[MAX_PATH];
	//based on fatih sennik's answer on https://stackoverflow.com/questions/1023306/finding-current-executables-path-without-proc-self-exe/1024937#1024937
	DWORD result = GetModuleFileNameA(NULL, exePath, MAX_PATH);
	std::string finalOutput(exePath);
	int exeNameStart = finalOutput.find_last_of('\\');
	finalOutput.erase(exeNameStart);
	finalOutput.append(*fileAddress);
	return finalOutput.c_str();
}

std::string FileIO::AttachDefaultFileLocation(std::string fileAddress)
{
	char exePath[MAX_PATH];
	//based on fatih sennik's answer on https://stackoverflow.com/questions/1023306/finding-current-executables-path-without-proc-self-exe/1024937#1024937
	DWORD result = GetModuleFileNameA(NULL, exePath, MAX_PATH);
	std::string finalOutput(exePath);
	int exeNameStart = finalOutput.find_last_of('\\');
	finalOutput.erase(exeNameStart);
	finalOutput.append(fileAddress);
	return finalOutput.c_str();
}

std::string FileIO::AttachDefaultFileLocation(const char* fileAddress)
{
	std::string fileAdd(fileAddress);
	char exePath[MAX_PATH];
	//based on fatih sennik's answer on https://stackoverflow.com/questions/1023306/finding-current-executables-path-without-proc-self-exe/1024937#1024937
	DWORD result = GetModuleFileNameA(NULL, exePath, MAX_PATH);
	std::string finalOutput(exePath);
	int exeNameStart =finalOutput.find_last_of('\\');
	finalOutput.erase(exeNameStart);
	finalOutput.append(fileAdd);
	return finalOutput.c_str();
}

GameObject FileIO::LoadGameObject(const char* fileAddress, int objectCount)
{
	json m_json = LoadJson(fileAddress);
	json data = m_json["ObjectData"];

	GameObject gameObject((std::string)data.at(objectCount)["Name"]);

	gameObject.GetTransform()->SetPosition(XMFLOAT3(data.at(objectCount)["Position"][0], data.at(objectCount)["Position"][1], data.at(objectCount)["Position"][2]));
	gameObject.GetTransform()->SetRotation(XMFLOAT3(data.at(objectCount)["Rotation"][0], data.at(objectCount)["Rotation"][1], data.at(objectCount)["Rotation"][2]));
	gameObject.GetTransform()->SetScale(XMFLOAT3(data.at(objectCount)["Scale"][0], data.at(objectCount)["Scale"][1], data.at(objectCount)["Scale"][2]));


	gameObject.SetMesh(QUAD_MESH_NAME);
	if (data.at(objectCount)["VertexShaderLocation"] == "NULL")
	{
		gameObject.SetVertexShader(DEFAULT_VERTEX_SHADER_NAME);
	}
	else
	{
		wstring location = ToWstring((std::string)data.at(objectCount)["VertexShaderLocation"]);
		gameObject.SetVertexShader(location);
	}
	if (data.at(objectCount)["PixelShaderLocation"] == "NULL")
	{
		gameObject.SetPixelShader(DEFAULT_PIXEL_SHADER_NAME);
	}
	else
	{
		wstring location = ToWstring((std::string)data.at(objectCount)["PixelShaderLocation"]);
		gameObject.SetPixelShader(location);
	}
	if (data.at(objectCount)["Albedo"] == "NULL")
	{
		gameObject.SetAlbedo(L"Resources/Sprites/Brick.dds");
	}
	else
	{
		wstring location = ToWstring((std::string)data.at(objectCount)["AlbedoLocation"]);
		gameObject.SetAlbedo(location);
	}

	bool renderable = false;
	bool trigger = false;
	bool collideable = false;

	if (data.at(objectCount)["IsRenderable"])
	{
		renderable = true;
		gameObject.SetIsRenderable(renderable);
	}
	else
	{
		gameObject.SetIsRenderable(renderable);
	}
	if (data.at(objectCount)["IsTrigger"])
	{
		trigger = true;
		gameObject.SetIsTrigger(trigger);
	}
	else
	{
		gameObject.SetIsTrigger(trigger);
	}
	if (data.at(objectCount)["IsCollideable"])
	{
		collideable = true;
		gameObject.SetIsCollidable(collideable);
	}
	else
	{
		gameObject.SetIsTrigger(collideable);
	}
	return gameObject;
}

GameObject FileIO::LoadGameObject(json file, int objectCount)
{
	json data = file;

	GameObject gameObject((std::string)data.at(objectCount)["Name"]);

	gameObject.GetTransform()->SetPosition(XMFLOAT3(data.at(objectCount)["Position"][0], data.at(objectCount)["Position"][1], data.at(objectCount)["Position"][2]));
	gameObject.GetTransform()->SetRotation(XMFLOAT3(data.at(objectCount)["Rotation"][0], data.at(objectCount)["Rotation"][1], data.at(objectCount)["Rotation"][2]));
	gameObject.GetTransform()->SetScale(XMFLOAT3(data.at(objectCount)["Scale"][0], data.at(objectCount)["Scale"][1], data.at(objectCount)["Scale"][2]));


	gameObject.SetMesh(QUAD_MESH_NAME);
	if (data.at(objectCount)["VertexShaderLocation"] == "NULL")
	{
		gameObject.SetVertexShader(DEFAULT_VERTEX_SHADER_NAME);
	}
	else
	{
		wstring location = ToWstring((std::string)data.at(objectCount)["VertexShaderLocation"]);
		gameObject.SetVertexShader(location);
	}
	if (data.at(objectCount)["PixelShaderLocation"] == "NULL")
	{
		gameObject.SetPixelShader(DEFAULT_PIXEL_SHADER_NAME);
	}
	else
	{
		wstring location = ToWstring((std::string)data.at(objectCount)["PixelShaderLocation"]);
		gameObject.SetPixelShader(location);
	}
	if (data.at(objectCount)["Albedo"] == "NULL")
	{
		gameObject.SetAlbedo(L"Resources/Sprites/Brick.dds");
	}
	else
	{
		wstring location = ToWstring((std::string)data.at(objectCount)["AlbedoLocation"]);
		gameObject.SetAlbedo(location);
	}

	bool renderable = false;
	bool trigger = false;
	bool collideable = false;

	if (data.at(objectCount)["IsRenderable"])
	{
		renderable = true;
		gameObject.SetIsRenderable(renderable);
	}
	else
	{
		gameObject.SetIsRenderable(renderable);
	}
	if (data.at(objectCount)["IsTrigger"])
	{
		trigger = true;
		gameObject.SetIsTrigger(trigger);
	}
	else
	{
		gameObject.SetIsTrigger(trigger);
	}
	if (data.at(objectCount)["IsCollideable"])
	{
		collideable = true;
		gameObject.SetIsCollidable(collideable);
	}
	else
	{
		gameObject.SetIsTrigger(collideable);
	}
	return gameObject;
}

GameObject FileIO::LoadGameObject(const char* fileAddress, ID3D11Device* device, int objectCount)
{
	json m_json = LoadJson(fileAddress);
	json data = m_json["ObjectData"];

	GameObject gameObject((std::string)data.at(objectCount)["Name"]);

	gameObject.GetTransform()->SetPosition(XMFLOAT3(data.at(objectCount)["Position"][0], data.at(objectCount)["Position"][1], data.at(objectCount)["Position"][2]));
	gameObject.GetTransform()->SetRotation(XMFLOAT3(data.at(objectCount)["Rotation"][0], data.at(objectCount)["Rotation"][1], data.at(objectCount)["Rotation"][2]));
	gameObject.GetTransform()->SetScale(XMFLOAT3(data.at(objectCount)["Scale"][0], data.at(objectCount)["Scale"][1], data.at(objectCount)["Scale"][2]));

	gameObject.SetMesh(QUAD_MESH_NAME);
	if (data.at(objectCount)["VertexShaderLocation"] == "NULL")
	{
		gameObject.SetVertexShader(DEFAULT_VERTEX_SHADER_NAME);
	}
	else
	{
		wstring location = ToWstring((std::string)data.at(objectCount)["VertexShaderLocation"]);
		gameObject.SetVertexShader(location);
	}
	if (data.at(objectCount)["PixelShaderLocation"] == "NULL")
	{
		gameObject.SetPixelShader(DEFAULT_PIXEL_SHADER_NAME);
	}
	else
	{
		wstring location = ToWstring((std::string)data.at(objectCount)["PixelShaderLocation"]);
		gameObject.SetPixelShader(location);
	}
	if (data.at(objectCount)["Albedo"] == "NULL")
	{
		gameObject.SetAlbedo(L"Resources/Sprites/Brick.dds");
	}
	else
	{
		gameObject.SetAlbedo(data.at(objectCount)["Albedo"]);
	}

	bool renderable = false;
	bool trigger = false;
	bool collideable = false;

	if (data.at(objectCount)["IsRenderable"])
	{
		GraphicsManager::GetInstance()->LoadTextureFromFile(data.at(objectCount)["TextureLocation"], device);
	    renderable = true;
		gameObject.SetIsRenderable(renderable);
	}
	else
	{
		gameObject.SetIsRenderable(renderable);
	}
	if (data.at(objectCount)["IsTrigger"])
	{
		trigger = true;
		gameObject.SetIsTrigger(trigger);
	}
	else
	{
		gameObject.SetIsTrigger(trigger);
	}
	if (data.at(objectCount)["IsCollideable"])
	{
		collideable = true;
		gameObject.SetIsCollidable(collideable);
	}
	else
	{
		gameObject.SetIsTrigger(collideable);
	}
	return gameObject;
}

void FileIO::LoadSavefile(const char* fileAddress)
{

}

ParticleManager FileIO::LoadParticle(const char* fileLocation, int particleNumber)
{
	json m_json = LoadJson(fileLocation);
	json data = m_json["ParticleData"];


	XMFLOAT3 postion(m_json.at(particleNumber)["Positon"][0], m_json.at(particleNumber)["Positon"][1], m_json.at(particleNumber)["Positon"][2]);
	XMFLOAT3 rotation(m_json.at(particleNumber)["Rotation"][0], m_json.at(particleNumber)["Rotation"][1], m_json.at(particleNumber)["Rotation"][2]);
	XMFLOAT3 scale(m_json.at(particleNumber)["Scale"][0], m_json.at(particleNumber)["Scale"][1], m_json.at(particleNumber)["Scale"][2]);

	ParticleManager particleManager;

	Transform transform;
	transform.Initialize(postion, rotation, scale);

	particleManager.AddSystem(transform, m_json.at(particleNumber)["Life"], m_json.at(particleNumber)["SystemType"], ToWstring((std::string)m_json.at(particleNumber)["AlbedoLocation"]));
	return particleManager;
}

ParticleManager FileIO::LoadParticle(json j, int particleNumber)
{
	json m_json = j["ParticleData"];

	XMFLOAT3 postion(m_json.at(particleNumber)["Positon"][0], m_json.at(particleNumber)["Positon"][1], m_json.at(particleNumber)["Positon"][2]);
	XMFLOAT3 rotation(m_json.at(particleNumber)["Rotation"][0], m_json.at(particleNumber)["Rotation"][1], m_json.at(particleNumber)["Rotation"][2]);
	XMFLOAT3 scale(m_json.at(particleNumber)["Scale"][0], m_json.at(particleNumber)["Scale"][1], m_json.at(particleNumber)["Scale"][2]);

	ParticleManager particleManager;

	Transform transform;
	transform.Initialize(postion, rotation, scale);

	particleManager.AddSystem(transform, m_json.at(particleNumber)["Life"], m_json.at(particleNumber)["SystemType"], ToWstring((std::string)m_json.at(particleNumber)["AlbedoLocation"]));
	return particleManager;
}

//////////////////////////SAVE FUNCTIONS ////////////////////////////////////////

void FileIO::SaveGameObject(GameObjectData gameObjData, const char* fileLocation)
{

	std::ofstream outFile;
	outFile.open(fileLocation);

	std::string colliderType;

	switch (gameObjData.m_ColliderType)
	{
	default:
		colliderType = "NoCollider";
		break;
	case COLLIDER_TYPE::COLLIDER_TYPE_BOX:
		colliderType = "BoxCollider";
		break;
	case COLLIDER_TYPE::COLLIDER_TYPE_CIRCLE:
		colliderType = "CircleCollider";
		break;
	case COLLIDER_TYPE::COLLIDER_TYPE_LINE:
		colliderType = "LineCollider";
		break;
	}
	float position[3] = {gameObjData.m_Position.x, gameObjData.m_Position.y , gameObjData.m_Position.z };
	float rotation[3] = {gameObjData.m_Rotation.x, gameObjData.m_Rotation.y , gameObjData.m_Rotation.z };
	float scale[3] = {gameObjData.m_Scale.x, gameObjData.m_Scale.y , gameObjData.m_Scale.z };

	json m_JsonOutput
	{
		{"Metadata",
			{
				{"Name", gameObjData.m_Name },
				{"ColliderType", colliderType},
				{"IsCollideable" , gameObjData.m_IsCollidable},
				{"IsRenderable", gameObjData.m_IsRenderable},
				{"IsTrigger", gameObjData.m_IsTrigger},
				{"Position" , position},
				{"Rotation", rotation } ,
				{"Scale" , scale} ,
				{"TextureLocation", "ATexLocation"}
			}
		}
	};
	outFile << m_JsonOutput;
	outFile.close();
}

bool FileIO::SaveObjectInJson(const char* fileLocation, std::vector<std::string> varNames, std::vector<JSON_VARIABLE_TYPE> types, std::vector<void*> data)
{
	try
	{
		std::ofstream outFile;
		outFile.open(fileLocation);
		json m_JsonOutput;
		for (size_t i = 0; i < varNames.size(); i++)
		{
			switch (types[i])
			{
			default:
				m_JsonOutput.push_back({ varNames[i], *(int*)data[i] });
				break;
			case JSON_VARIABLE_TYPE::JSON_VARIABLE_TYPE_CHAR:
				m_JsonOutput.push_back({ varNames[i], *(char*)data[i] });
				break;
			case JSON_VARIABLE_TYPE::JSON_VARIABLE_TYPE_STRING:
				m_JsonOutput.push_back({ varNames[i], *(std::string*)data[i] });
				break;
			case JSON_VARIABLE_TYPE::JSON_VARIABLE_TYPE_DOUBLE:
				m_JsonOutput.push_back({ varNames[i], *(double*)data[i] });
				break;
			case JSON_VARIABLE_TYPE::JSON_VARIABLE_TYPE_FLOAT:
				m_JsonOutput.push_back({ varNames[i], *(float*)data[i] });
				break;
			case JSON_VARIABLE_TYPE::JSON_VARIABLE_TYPE_INT:
				m_JsonOutput.push_back({ varNames[i], *(int*)data[i] });
				break;
			case JSON_VARIABLE_TYPE::JSON_VARIABLE_TYPE_WCHAR_T:
				m_JsonOutput.push_back({ varNames[i], *(wchar_t*)data[i] });
				break;
			case JSON_VARIABLE_TYPE::JSON_VARIABLE_TYPE_XMFLOAT2:
				XMFLOAT2 vec2 = *(XMFLOAT2*)data[i];
				m_JsonOutput.push_back({ varNames[i],  { vec2.x, vec2.y} });
				break;
			case JSON_VARIABLE_TYPE::JSON_VARIABLE_TYPE_XMFLOAT3:
				XMFLOAT3 vec3 = *(XMFLOAT3*)data[i];
				m_JsonOutput.push_back({ varNames[i],   { vec3.x, vec3.y, vec3.z } });
				break;
			case JSON_VARIABLE_TYPE::JSON_VARIABLE_TYPE_XMFLOAT4:
				XMFLOAT4 vec4 = *(XMFLOAT4*)data[i];
				m_JsonOutput.push_back({ varNames[i],  { vec4.x, vec4.y, vec4.z, vec4.w } });
				break;
			}
		}
		outFile << m_JsonOutput;
		outFile.close();
		return true;
	}
	catch (const std::exception&)
	{
		return false;
	}
}

