#include "FileIO.h"

FileIOCache FileIO::m_Cache = FileIOCache();

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
		objects[i] = LoadGameObject(data, i);
	}
	return objects;
}

std::vector<GameObject> FileIO::LoadMultipleGameObjects(json m_json)
{
	json data = m_json["ObjectData"];
	json metaData = m_json["MetaData"];

	std::vector<GameObject> objects((int)metaData.at(0)["NumberOfObject"]);

	for (size_t i = 0; i < metaData.at(0)["NumberOfObject"]; i++)
	{
		objects[i] = LoadGameObject(data, i);
	}
	return objects;
}

std::vector<GameObject> FileIO::LoadMultipleTiles(json file)
{
	return std::vector<GameObject>();
}

std::vector<ParticleSystem> FileIO::LoadMultipleParticles(json file)
{
	json data = file["ObjectData"];
	json metaData = file["MetaData"];

	std::vector<ParticleSystem> objects((int)metaData.at(0)["NumberOfObject"]);

	for (size_t i = 0; i < metaData.at(0)["NumberOfObject"]; i++)
	{
		objects[i] = LoadParticle(data, i);
	}
	return objects;
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

void FileIO::LoadScene(const char* fileAddress, Scene* scene)
{
	json j = FileIO::LoadJson(fileAddress);

	std::vector<GameObject> gameObj = LoadMultipleGameObjects(j);
	LoadMap(j, scene);

	std::vector<ParticleSystem> particles = LoadMultipleParticles(j);

	std::string identifer;
	GameObject* gO;

	for (size_t i = 0; i < gameObj.size(); ++i)
	{
		identifer = gameObj[i].GetIdentifier();
		scene->CreateGameObject(identifer);
		gO = scene->GetGameObjectUsingIdentifier(identifer);
		*gO = gameObj[i];
	}

	for (size_t i = 0; i < particles.size(); ++i)
	{
		identifer = particles[i].;
		scene->CreateGameObject(identifer);
		gO = scene->GetGameObjectUsingIdentifier(identifer);
		*gO = gameObj[i];
	}

	//for (size_t i = 0; i < tileObj.size(); ++i)
	//{
	//	identifer = tileObj[i].GetIdentifier();
	//	scene->CreateGameObject(identifer);
	//	gO = scene->GetGameObjectUsingIdentifier(identifer);
	//	*gO = tileObj[i];
	//}

	//for (size_t i = 0; i < mapObj.size(); ++i)
	//{
	//	identifer = mapObj[i].GetIdentifier();
	//	scene->CreateGameObject(identifer);
	//	gO = scene->GetGameObjectUsingIdentifier(identifer);
	//	*gO = mapObj[i];
	//}
}

void FileIO::LoadMap(json file, Scene* scene)
{
	json j = file["TileMap"];

	for (size_t i = 0; i < file["MetaData"].at(0)["NumberOfTileZones"]; ++i)
	{
		XMFLOAT3 position(j.at(i)["Position"][0], j.at(i)["Position"][1], j.at(i)["Position"][2]);
		TileMap tileMap(j.at(i)["Width"], j.at(i)["Height"], j.at(i)["TileName"], position);

		tileMap.SetMesh(QUAD_MESH_NAME);
		if (j.at(i)["VertexShaderLocation"] == "NULL")
		{
			tileMap.SetVertexShader(DEFAULT_VERTEX_SHADER_NAME);
		}
		else
		{
			wstring location = ToWstring((std::string)j.at(i)["VertexShaderLocation"]);
			tileMap.SetVertexShader(location);
		}
		if (j.at(i)["PixelShaderLocation"] == "NULL")
		{
			tileMap.SetPixelShader(DEFAULT_PIXEL_SHADER_NAME);
		}
		else
		{
			wstring location = ToWstring((std::string)j.at(i)["PixelShaderLocation"]);
			tileMap.SetPixelShader(location);
		}
		if (j.at(i)["Albedo"] == "NULL")
		{
			tileMap.SetAlbedo(L"Resources/Sprites/Brick.dds");
		}
		else
		{
			wstring location = ToWstring((std::string)j.at(i)["AlbedoLocation"]);
			tileMap.SetAlbedo(location);
		}

		bool renderable = false;
		bool trigger = false;
		bool collideable = false;

		if (j.at(i)["IsRenderable"])
		{
			renderable = true;
			tileMap.SetIsRenderable(renderable);
		}
		else
		{
			tileMap.SetIsRenderable(renderable);
		}
		if (j.at(i)["IsTrigger"])
		{
			trigger = true;
			tileMap.SetIsTrigger(trigger);
		}
		else
		{
			tileMap.SetIsTrigger(trigger);
		}
		if (j.at(i)["IsCollideable"])
		{
			collideable = true;
			tileMap.SetIsCollidable(collideable);
			if (j.at(i)["ColliderType"] == "CircleCollider")
			{
				Circle* c = new Circle();
				tileMap.SetShape(c);
				c->m_radius = j.at(i)["CircleRadius"];
				c->m_transform = tileMap.GetTransform();
			}
			else
			{
				///NEED TO CHECK ASSIGNING THE POINTER TO THE GAME OBJECTS TRANSFORM IS OK
				Box* b = new Box();
				tileMap.SetShape(b);
				tileMap.GetShape()->m_transform = tileMap.GetTransform();
			}
		}
		else
		{
			tileMap.SetIsTrigger(collideable);
		}

		std::string name = tileMap.GetIdentifier();
		scene->CreateGameObject(name);
		GameObject* gameObject = scene->GetGameObjectUsingIdentifier(name);
		*gameObject = tileMap;
	}

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
	return LoadGameObject(data, objectCount);
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
		m_Cache.m_VertexCache[gameObject.GetVertexShader()] = location;
	}
	if (data.at(objectCount)["PixelShaderLocation"] == "NULL")
	{
		gameObject.SetPixelShader(DEFAULT_PIXEL_SHADER_NAME);
	}
	else
	{
		wstring location = ToWstring((std::string)data.at(objectCount)["PixelShaderLocation"]);
		gameObject.SetPixelShader(location);
		m_Cache.m_PixelCache[gameObject.GetPixelShader()] = location;
	}
	if (data.at(objectCount)["AlbedoLocation"] == "NULL")
	{
		gameObject.SetAlbedo(L"Resources/Sprites/Brick.dds");
	}
	else
	{
		wstring location = ToWstring((std::string)data.at(objectCount)["AlbedoLocation"]);
		gameObject.SetAlbedo(location);
		m_Cache.m_AlebdoCache[gameObject.GetAlbedoSRV()] = location;
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
		if (data.at(objectCount)["ColliderType"] == "CircleCollider")
		{
			Circle* c = new Circle();
			gameObject.SetShape(c);
			c->m_radius = data.at(objectCount)["CircleRadius"];
			c->m_transform = gameObject.GetTransform();
		}
		else
		{
			///NEED TO CHECK ASSIGNING THE POINTER TO THE GAME OBJECTS TRANSFORM IS OK
			Box* b =  new Box();
			gameObject.SetShape(b);
			gameObject.GetShape()->m_transform = gameObject.GetTransform();
		}
	}
	else
	{
		gameObject.SetIsCollidable(collideable);
	}
	return gameObject;
}


void FileIO::LoadSavefile(const char* fileAddress)
{

}

//ParticleManager FileIO::LoadParticle(const char* fileLocation, int particleNumber)
//{
//	json m_json = LoadJson(fileLocation);
//	json data = m_json["ParticleData"];
//
//
//	XMFLOAT3 postion(m_json.at(particleNumber)["Positon"][0], m_json.at(particleNumber)["Positon"][1], m_json.at(particleNumber)["Positon"][2]);
//	XMFLOAT3 rotation(m_json.at(particleNumber)["Rotation"][0], m_json.at(particleNumber)["Rotation"][1], m_json.at(particleNumber)["Rotation"][2]);
//	XMFLOAT3 scale(m_json.at(particleNumber)["Scale"][0], m_json.at(particleNumber)["Scale"][1], m_json.at(particleNumber)["Scale"][2]);
//
//	ParticleManager particleManager;
//
//	Transform transform;
//	transform.Initialize(postion, rotation, scale);
//
//	particleManager.AddSystem(transform, m_json.at(particleNumber)["Life"], m_json.at(particleNumber)["SystemType"], ToWstring((std::string)m_json.at(particleNumber)["AlbedoLocation"]));
//	return particleManager;
//}
//
//ParticleSystem FileIO::LoadParticle(json j, int particleNumber)
//{
//	json m_json = j["ParticleData"];
//
//	XMFLOAT3 postion(m_json.at(particleNumber)["Positon"][0], m_json.at(particleNumber)["Positon"][1], m_json.at(particleNumber)["Positon"][2]);
//	XMFLOAT3 rotation(m_json.at(particleNumber)["Rotation"][0], m_json.at(particleNumber)["Rotation"][1], m_json.at(particleNumber)["Rotation"][2]);
//	XMFLOAT3 scale(m_json.at(particleNumber)["Scale"][0], m_json.at(particleNumber)["Scale"][1], m_json.at(particleNumber)["Scale"][2]);
//
//	ParticleManager particleManager;
//
//	Transform transform;
//	transform.Initialize(postion, rotation, scale);
//
//	particleManager.AddSystem(transform, m_json.at(particleNumber)["Life"], m_json.at(particleNumber)["SystemType"], ToWstring((std::string)m_json.at(particleNumber)["AlbedoLocation"]));
//	return particleManager;
//}

//////////////////////////SAVE FUNCTIONS ////////////////////////////////////////

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

void FileIO::SaveScene(const char* fileLocation, Scene* scene)
{
	unordered_map<std::string, GameObject*> gameObjects = scene->GetAllGameObjects();
	std::ofstream outFile;
	outFile.open(fileLocation);
	json jsonOutput{};
	jsonOutput["MetaData"] = {};
	jsonOutput["TilesData"] = {};
	jsonOutput["TileMap"] = {};
	jsonOutput["ObjectData"] = {};
	jsonOutput["ParticleData"] = {};
	
	jsonOutput["MetaData"].push_back({ "NumberOfObject", gameObjects.size() });

	
	int count = 0;

	for (unordered_map<string, GameObject*>::iterator it = gameObjects.begin(); it != gameObjects.end(); ++it)
	{
		GameObject* gameObjectToStore = it->second;


		std::string vSL = ToString(GrabVertexShaderName(gameObjectToStore->GetVertexShader()));
		std::string pSL = ToString(GrabPixelShaderName(gameObjectToStore->GetPixelShader()));
		std::string aSRV = ToString(GrabAlbedoName(gameObjectToStore->GetAlbedoSRV()));
		
		std::vector<std::string> shaders(3);
		shaders[0] = vSL;
		shaders[1] = pSL;
		shaders[2] = aSRV;

		XMFLOAT3 pos = gameObjectToStore->GetTransform()->GetPosition();
		float position[3]{ pos.x, pos.y, pos.z };
		XMFLOAT3 rot = gameObjectToStore->GetTransform()->GetRotation();
		float rotation[3]{ rot.x, rot.y, rot.z };
		XMFLOAT3 scal = gameObjectToStore->GetTransform()->GetScale();
		float scale[3]{ scal.x, scal.y, scal.z };

		jsonOutput["ObjectData"].push_back({});
		jsonOutput["ObjectData"].at(count).push_back({ "Name", gameObjectToStore->GetIdentifier()});
		jsonOutput["ObjectData"].at(count).push_back({ "ColliderType" ,  typeid(gameObjectToStore->GetShape()).name() });
		jsonOutput["ObjectData"].at(count).push_back({ "IsCollideable", gameObjectToStore->IsCollidable()});
		jsonOutput["ObjectData"].at(count).push_back({ "IsRenderable" , gameObjectToStore->IsRenderable()});
		jsonOutput["ObjectData"].at(count).push_back({ "IsTrigger" , gameObjectToStore->IsTrigger()});
		jsonOutput["ObjectData"].at(count).push_back({ "Position", position });
		jsonOutput["ObjectData"].at(count).push_back({ "Rotation" , rotation });
		jsonOutput["ObjectData"].at(count).push_back({ "Scale" , scale });
		jsonOutput["ObjectData"].at(count).push_back({ "VertexShaderLocation" , shaders[0] });
		jsonOutput["ObjectData"].at(count).push_back({ "PixelShaderLocation" , shaders[1] });
		jsonOutput["ObjectData"].at(count).push_back({ "AlbedoLocation" , shaders[2] });
		jsonOutput["ObjectData"].at(count).push_back({ "AnimationCount", 0});

		++count;
	}



	outFile << jsonOutput;
	outFile.close();
}

wstring FileIO::GrabVertexShaderName(ID3D11VertexShader* shader)
{
	return m_Cache.m_VertexCache[shader];
}

wstring FileIO::GrabPixelShaderName(ID3D11PixelShader* shader)
{
	return m_Cache.m_PixelCache[shader];
}

wstring FileIO::GrabTextureName(ID3D11ShaderResourceView* texture)
{
	return m_Cache.m_TextureCache[texture];
}

wstring FileIO::GrabAlbedoName(ID3D11ShaderResourceView* albedo)
{
	return m_Cache.m_AlebdoCache[albedo];
}

