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
			json jsonData;
			file >> jsonData;
			return jsonData;
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

std::vector<GameObject*> FileIO::LoadMultipleGameObjects(const char* fileAddress)
{
	json jsonData = LoadJson(fileAddress);
	json objData = jsonData["ObjectData"];
	json metaData = jsonData["MetaData"];

	std::vector<GameObject*> objects((int)metaData.at(0)["NumberOfObject"]);

	for (size_t i = 0; i < metaData.at(0)["NumberOfObject"]; i++)
	{
		objects[i] = LoadGameObject(objData, i);
	}
	return objects;
}

std::vector<GameObject*> FileIO::LoadMultipleGameObjects(json m_json)
{
	json jsonData = m_json["ObjectData"];
	json metaData = m_json["MetaData"];

	std::vector<GameObject*> objects((int)metaData.at(0)["NumberOfObject"]);

	for (size_t i = 0; i < metaData.at(0)["NumberOfObject"]; i++)
	{
		objects[i] = LoadGameObject(jsonData, i);
	}
	return objects;
}

void FileIO::LoadMultipleParticles(json file)
{
	json particleSystemData = file["ParticleSystemData"];
	json metaData = file["MetaData"];
	json particleData = file["ParticleData"];

	for (size_t i = 0; i < metaData.at(0)["NumberOfParticlesSystems"]; i++)
	{
		Transform t;
		t.SetPosition(XMFLOAT3(particleSystemData.at(i)["Position"][0], particleSystemData.at(i)["Position"][1], particleSystemData.at(i)["Position"][2]));
		t.SetRotation(XMFLOAT3(particleSystemData.at(i)["Rotation"][0], particleSystemData.at(i)["Rotation"][1], particleSystemData.at(i)["Rotation"][2]));
		t.SetScale(XMFLOAT3(particleSystemData.at(i)["Scale"][0], particleSystemData.at(i)["Scale"][1], particleSystemData.at(i)["Scale"][2]));

		float life = particleSystemData.at(i)["Life"];
		string st = (std::string)particleSystemData.at(i)["AlbedoLocation"];
		float interval = particleSystemData.at(i)["Interval"];
		int count = particleSystemData.at(i)["Count"];

		XMFLOAT2 velocity(particleSystemData.at(i)["Velocity"][0], particleSystemData.at(i)["Velocity"][1]);
		XMFLOAT2 acceleration(particleSystemData.at(i)["Acceleration"][0], particleSystemData.at(i)["Acceleration"][1]);
		float particleLife  = particleSystemData.at(i)["ParticleLife"];
		float randPos = particleSystemData.at(i)["RandPosition"];
		float randVel = particleSystemData.at(i)["RandVelocity"];
		float randAcc = particleSystemData.at(i)["RandAcceleration"];
		float randLife = particleSystemData.at(i)["RandLife"];
		int layer = particleSystemData.at(i)["Layer"];

		ParticleManager::GetInstance()->AddSystem(t, life, ToWstring(st), velocity, acceleration, particleLife, interval, count, randPos, randVel, randAcc, randLife, layer);
	}
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

void FileIO::LoadScene(const char* fileAddress, GameManager* pScene, ParticleManager* pManager)
{
	json j = FileIO::LoadJson(fileAddress);

	LoadMap(j, pScene);
	std::vector<GameObject*> gameObj = LoadMultipleGameObjects(j);

	std::string identifer;

	for (size_t i = 0; i < gameObj.size(); ++i)
	{
		identifer = gameObj[i]->GetIdentifier();

		if (gameObj[i]->ContainsType(GameObjectType::COLLIDABLE) && gameObj[i]->ContainsType(GameObjectType::RENDERABLE))
		{
			RenderableCollidableGameObject* rCGO;
			pScene->CreateGameObject<RenderableCollidableGameObject>(identifer);
			rCGO = pScene->GetGameObjectUsingIdentifier<RenderableCollidableGameObject>(identifer);
			*rCGO = *dynamic_cast<RenderableCollidableGameObject*>(gameObj[i]);
		}
		else if (gameObj[i]->ContainsType(GameObjectType::COLLIDABLE))
		{
			CollidableGameObject* cGO;
			pScene->CreateGameObject<CollidableGameObject>(identifer);
			cGO = pScene->GetGameObjectUsingIdentifier<CollidableGameObject>(identifer);
			*cGO = *dynamic_cast<CollidableGameObject*>(gameObj[i]);
		}
		else if (gameObj[i]->ContainsType(GameObjectType::RENDERABLE))
		{
			RenderableGameObject* rGO;
			pScene->CreateGameObject<RenderableGameObject>(identifer);
			rGO = pScene->GetGameObjectUsingIdentifier<RenderableGameObject>(identifer);
			*rGO = *dynamic_cast<RenderableGameObject*>(gameObj[i]);
		}
	}

	for (GameObject* g : gameObj)
	{

		if (g->ContainsType(GameObjectType::COLLIDABLE) && g->ContainsType(GameObjectType::RENDERABLE))
		{
			RenderableCollidableGameObject* gameObj = dynamic_cast<RenderableCollidableGameObject*>(g);
			delete gameObj;
		}
		else if(g->ContainsType(GameObjectType::COLLIDABLE))
		{
			CollidableGameObject* gameObj = dynamic_cast<CollidableGameObject*>(g);
			delete gameObj;
		}
		else if (g->ContainsType(GameObjectType::RENDERABLE))
		{
			RenderableGameObject* gameObj = dynamic_cast<RenderableGameObject*>(g);
			delete gameObj;
		}
	}

	if (pManager == nullptr) 
	{
		return;
	}

	LoadMultipleParticles(j);

	//std::vector<ParticleSystem*> particles = LoadMultipleParticles(j);

	//for (size_t i = 0; i < particles.size(); ++i)
	//{
	//	m_Cache.m_ParticleSystems.push_back(particles[i]);
	//	pManager->AddSystem(particles[i]);
	//}
}

void FileIO::LoadMap(json file, GameManager* scene)
{
	json tileFile = file["TileMap"];

	for (size_t i = 0; i < file["MetaData"].at(0)["NumberOfTileZones"]; ++i)
	{
		XMFLOAT3 position(tileFile.at(i)["Position"][0], tileFile.at(i)["Position"][1], tileFile.at(i)["Position"][2]);
		XMFLOAT3 scale(tileFile.at(i)["Scale"][0], tileFile.at(i)["Scale"][1], tileFile.at(i)["Scale"][2]);
		wstring location = ToWstring((std::string)tileFile.at(i)["TileData"]);
		TileMap* tileMap = new TileMap(location, position, tileFile.at(i)["Name"]);
		std::string name = tileMap->GetIdentifier();
		scene->CreateGameObject<GameObject>(name);
		GameObject* gameObject = scene->GetGameObjectUsingIdentifier<GameObject>(name);
		gameObject = tileMap;
		m_Cache.m_Map.push_back(tileMap);
		m_Cache.m_MapData.push_back(location);
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

GameObject* FileIO::LoadGameObject(const char* fileAddress, int objectCount)
{
	json m_json = LoadJson(fileAddress);
	json data = m_json["ObjectData"];
	return LoadGameObject(data, objectCount);
}

GameObject* FileIO::LoadGameObject(json file, int objectCount)
{
	json data = file;

	GameObject gameObject((std::string)data.at(objectCount)["Name"]);
	gameObject.GetTransform()->SetPosition(XMFLOAT3(data.at(objectCount)["Position"][0], data.at(objectCount)["Position"][1], data.at(objectCount)["Position"][2]));
	gameObject.GetTransform()->SetRotation(XMFLOAT3(data.at(objectCount)["Rotation"][0], data.at(objectCount)["Rotation"][1], data.at(objectCount)["Rotation"][2]));
	gameObject.GetTransform()->SetScale(XMFLOAT3(data.at(objectCount)["Scale"][0], data.at(objectCount)["Scale"][1], data.at(objectCount)["Scale"][2]));

	if(data.at(objectCount)["IsRenderable"] && data.at(objectCount)["IsCollideable"])
	{ 
		RenderableCollidableGameObject* renderColliderObject = new RenderableCollidableGameObject();

		*renderColliderObject->GetTransform() = *gameObject.GetTransform();

		renderColliderObject->m_identifier = (std::string)data.at(objectCount)["Name"];

		if (data.at(objectCount)["ColliderType"] == "CircleCollider")
		{
			Circle* pCircle = new Circle();
			renderColliderObject->SetShape(pCircle);
			pCircle->SetIsCollidable(true);
			pCircle->SetIsTrigger(data.at(objectCount)["IsTrigger"]);
			pCircle->m_radius = data.at(objectCount)["CircleRadius"];
			pCircle->m_transform = renderColliderObject->GetTransform();
		}
		else
		{
			Box* pbox = new Box(gameObject.GetTransform());
			renderColliderObject->SetShape(pbox);
			pbox->SetIsCollidable(true);
			pbox->SetIsTrigger(data.at(objectCount)["IsTrigger"]);
			pbox->m_transform = renderColliderObject->GetTransform();
		}

		renderColliderObject->SetMesh(QUAD_MESH_NAME);
		bool isRenderable = data.at(objectCount)["IsRenderable"];
		renderColliderObject->SetIsRenderable(isRenderable);
		if (data.at(objectCount)["VertexShaderLocation"] == "NULL")
		{
			LOG("NO VERTEX SHADER WAS SPECIFIED")
		}
		else
		{
			wstring location = ToWstring((std::string)data.at(objectCount)["VertexShaderLocation"]);
			renderColliderObject->SetVertexShader(location);
			m_Cache.m_VertexCache[renderColliderObject->GetVertexShader()] = location;
		}
		if (data.at(objectCount)["PixelShaderLocation"] == "NULL")
		{
			LOG("NO PIXEL SHADER WAS SPECIFIED")
		}
		else
		{
			wstring location = ToWstring((std::string)data.at(objectCount)["PixelShaderLocation"]);
			renderColliderObject->SetPixelShader(location);
			m_Cache.m_PixelCache[renderColliderObject->GetPixelShader()] = location;
		}
		if (data.at(objectCount)["AlbedoLocation"] == "NULL")
		{
			renderColliderObject->SetAlbedo(L"Resources/Sprites/Brick.dds");
		}
		else
		{
			wstring location = ToWstring((std::string)data.at(objectCount)["AlbedoLocation"]);
			renderColliderObject->SetAlbedo(location);
			m_Cache.m_AlebdoCache[renderColliderObject->GetAlbedoSRV()] = location;
		}

		std::vector<Frame> framesOfAnimation(data.at(objectCount)["NumberOfAnimations"]);

		if (framesOfAnimation.size() > 0)
		{
			for (size_t i = 0; i < framesOfAnimation.size(); ++i)
			{
				wstring animationFile = ToWstring((std::string)data.at(objectCount)["AnimationFile"][i]);
				GraphicsManager::GetInstance()->LoadAnimationFromFile(animationFile);
				std::string name = data.at(objectCount)["AnimationFile"][i];
				SpriteAnimation* sAnimation = &GraphicsManager::GetInstance()->GetAnimation(ToWstring(name));
				m_Cache.m_AnimationCache[sAnimation->GetFrames()] = name;
				renderColliderObject->AddAnimation(name, *sAnimation);
			}
		}
		return renderColliderObject;
	}
	if (data.at(objectCount)["IsRenderable"])
	{

		bool renderable = data.at(objectCount)["IsRenderable"];
		gameObject.ContainsType(GameObjectType::RENDERABLE);
		RenderableGameObject* renderObject = new RenderableGameObject((std::string)data.at(objectCount)["Name"]);
		*renderObject->GetTransform() = *gameObject.GetTransform();
		renderObject->SetMesh(QUAD_MESH_NAME);
		renderObject->SetIsRenderable(renderable);
		if (data.at(objectCount)["VertexShaderLocation"] == "NULL")
		{
			LOG("NO VERTEX SHADER WAS SPECIFIED")
		}
		else
		{
			wstring location = ToWstring((std::string)data.at(objectCount)["VertexShaderLocation"]);
			renderObject->SetVertexShader(location);
			m_Cache.m_VertexCache[renderObject->GetVertexShader()] = location;
		}
		if (data.at(objectCount)["PixelShaderLocation"] == "NULL")
		{
			LOG("NO PIXEL SHADER WAS SPECIFIED")
		}
		else
		{
			wstring location = ToWstring((std::string)data.at(objectCount)["PixelShaderLocation"]);
			renderObject->SetPixelShader(location);
			m_Cache.m_PixelCache[renderObject->GetPixelShader()] = location;
		}
		if (data.at(objectCount)["AlbedoLocation"] == "NULL")
		{
			renderObject->SetAlbedo(L"Resources/Sprites/Brick.dds");
		}
		else
		{
			wstring location = ToWstring((std::string)data.at(objectCount)["AlbedoLocation"]);
			renderObject->SetAlbedo(location);
			m_Cache.m_AlebdoCache[renderObject->GetAlbedoSRV()] = location;
		}

		std::vector<Frame> framesOfAnimation(data.at(objectCount)["NumberOfAnimations"]);

		if (framesOfAnimation.size() > 0)
		{
			for (size_t i = 0; i < framesOfAnimation.size(); ++i)
			{
				wstring animationFile = ToWstring((std::string)data.at(objectCount)["AnimationFile"][i]);
				GraphicsManager::GetInstance()->LoadAnimationFromFile(animationFile);
				std::string name = data.at(objectCount)["AnimationFile"][i];
				SpriteAnimation* sAnimation = &GraphicsManager::GetInstance()->GetAnimation(ToWstring(name));
				m_Cache.m_AnimationCache[sAnimation->GetFrames()] = name;
				renderObject->AddAnimation(name, *sAnimation);
			}
		}
		return renderObject;
	}

	bool trigger = false;
	bool collideable = false;

	if (data.at(objectCount)["IsCollideable"])
	{
		CollidableGameObject* colliderObject = new CollidableGameObject((std::string)data.at(objectCount)["Name"]);
		*colliderObject->GetTransform() = *gameObject.GetTransform();

		if (data.at(objectCount)["IsTrigger"])
		{
			trigger = true;
		}
		if (data.at(objectCount)["ColliderType"] == "CircleCollider")
		{
			Circle* pCircle = new Circle();
			colliderObject->SetShape(pCircle);
			pCircle->SetIsCollidable(true);
			pCircle->SetIsTrigger(trigger);
			pCircle->m_radius = data.at(objectCount)["CircleRadius"];
			pCircle->m_transform = gameObject.GetTransform();
		}
		else
		{
			Box* pbox = new Box(gameObject.GetTransform());
			colliderObject->SetShape(pbox);
			pbox->SetIsCollidable(true);
			pbox->SetIsTrigger(trigger);
			pbox->m_transform = gameObject.GetTransform();
		}
		return colliderObject;
	}
}

ParticleData FileIO::LoadParticle(json particleData, int particleNumber)
{

	Transform t = Transform();


	t.SetPosition(XMFLOAT3(particleData.at(particleNumber)["Position"][0], particleData.at(particleNumber)["Position"][1], particleData.at(particleNumber)["Position"][2]));
	t.SetRotation(XMFLOAT3(particleData.at(particleNumber)["Rotation"][0], particleData.at(particleNumber)["Rotation"][1], particleData.at(particleNumber)["Rotation"][2]));
	t.SetScale(XMFLOAT3(particleData.at(particleNumber)["Scale"][0], particleData.at(particleNumber)["Scale"][1], particleData.at(particleNumber)["Scale"][2]));
	XMFLOAT2 vel = XMFLOAT2(particleData.at(particleNumber)["Velocity"][0], particleData.at(particleNumber)["Velocity"][1]);
	XMFLOAT2 accl = XMFLOAT2(particleData.at(particleNumber)["Acceleration"][0], particleData.at(particleNumber)["Acceleration"][1]);

	ParticleData p = ParticleData();
	p.m_Transform = t;
	p.m_Acceleration = accl;
	p.m_Velocity = vel;
	p.m_Life = particleData.at(particleNumber)["Life"];

	return p;
}

std::unordered_map<std::string, void*> FileIO::LoadCustomJsonData(const char* fileAddress, std::vector<std::string> variableNames, std::vector<JSON_VARIABLE_TYPE> variableTypes)
{
	json dataToLoad = LoadJson(fileAddress);
	std::unordered_map<std::string, void*> results;
	std::string s;
 	for (size_t i = 0; i < variableNames.size(); i++)
	{
		switch (variableTypes[i])
		{
		default:
			LOG("No data type was specified : FileIO LoadCustomJsonData")
				break;
		case JSON_VARIABLE_TYPE::JSON_VARIABLE_TYPE_CHAR:
			//results[variableNames[i]] = new char*(std::string(dataToLoad.at(0)[variableNames[i]]);
			break;
		case JSON_VARIABLE_TYPE::JSON_VARIABLE_TYPE_STRING:
			results[variableNames[i]] = new std::string(dataToLoad.at(0)[variableNames[i]]);
			break;
		case JSON_VARIABLE_TYPE::JSON_VARIABLE_TYPE_DOUBLE:
			results[variableNames[i]] = &dataToLoad.at(0)[variableNames[i]];
			break;
		case JSON_VARIABLE_TYPE::JSON_VARIABLE_TYPE_FLOAT:
			memcpy(results[variableNames[i]], &dataToLoad.at(0)[variableNames[i]], sizeof(float));
			break;
		case JSON_VARIABLE_TYPE::JSON_VARIABLE_TYPE_INT:
			memcpy(results[variableNames[i]], &dataToLoad.at(0)[variableNames[i]], sizeof(int));
			break;
		case JSON_VARIABLE_TYPE::JSON_VARIABLE_TYPE_WCHAR_T:
			memcpy(results[variableNames[i]], &dataToLoad.at(0)[variableNames[i]], sizeof(wstring));
			break;
		case JSON_VARIABLE_TYPE::JSON_VARIABLE_TYPE_XMFLOAT2:
			memcpy(results[variableNames[i]], &dataToLoad.at(0)[variableNames[i]], sizeof(XMFLOAT2));
			break;
		case JSON_VARIABLE_TYPE::JSON_VARIABLE_TYPE_XMFLOAT3:
			memcpy(results[variableNames[i]], &dataToLoad.at(0)[variableNames[i]], sizeof(XMFLOAT3));
			break;
		case JSON_VARIABLE_TYPE::JSON_VARIABLE_TYPE_XMFLOAT4:
			memcpy(results[variableNames[i]], &dataToLoad.at(0)[variableNames[i]], sizeof(XMFLOAT4));
			break;
		case JSON_VARIABLE_TYPE::JSON_VARIABLE_TYPE_GAME_OBJECTS:
			GameObject* gO = LoadGameObject(dataToLoad.at(0)[variableNames[i]], 0);
			memcpy(gO, & dataToLoad[variableNames[i]], sizeof(GameObject));
			break;
		}
	}




	return results;
}

//////////////////////////SAVE FUNCTIONS ////////////////////////////////////////

bool FileIO::SaveObjectInJson(const char* fileLocation, std::vector<std::string> varNames, std::vector<JSON_VARIABLE_TYPE> types, std::vector<void*> data)
{
	try
	{
		std::ofstream outFile;
		outFile.open(fileLocation);
		json jsonOutput;
		jsonOutput.push_back({});
		for (size_t i = 0; i < varNames.size(); i++)
		{
			switch (types[i])
			{
			default:
				jsonOutput.at(0).push_back({varNames[i], *(int*)data[i]});
				break;
			case JSON_VARIABLE_TYPE::JSON_VARIABLE_TYPE_CHAR:
				jsonOutput.at(0).push_back({ varNames[i], *(char*)data[i] });
				break;
			case JSON_VARIABLE_TYPE::JSON_VARIABLE_TYPE_STRING:
				jsonOutput.at(0).push_back({ varNames[i], *(std::string*)data[i] });
				break;
			case JSON_VARIABLE_TYPE::JSON_VARIABLE_TYPE_DOUBLE:
				jsonOutput.at(0).push_back({ varNames[i], *(double*)data[i] });
				break;
			case JSON_VARIABLE_TYPE::JSON_VARIABLE_TYPE_FLOAT:
				jsonOutput.at(0).push_back({ varNames[i], *(float*)data[i] });
				break;
			case JSON_VARIABLE_TYPE::JSON_VARIABLE_TYPE_INT:
				jsonOutput.at(0).push_back({ varNames[i], *(int*)data[i] });
				break;
			case JSON_VARIABLE_TYPE::JSON_VARIABLE_TYPE_WCHAR_T:
				jsonOutput.at(0).push_back({ varNames[i], *(wchar_t*)data[i] });
				break;
			case JSON_VARIABLE_TYPE::JSON_VARIABLE_TYPE_XMFLOAT2:
				XMFLOAT2 vec2 = *(XMFLOAT2*)data[i];
				jsonOutput.at(0).push_back({ varNames[i],  { vec2.x, vec2.y} });
				break;
			case JSON_VARIABLE_TYPE::JSON_VARIABLE_TYPE_XMFLOAT3:
				XMFLOAT3 vec3 = *(XMFLOAT3*)data[i];
				jsonOutput.at(0).push_back({ varNames[i],   { vec3.x, vec3.y, vec3.z } });
				break;
			case JSON_VARIABLE_TYPE::JSON_VARIABLE_TYPE_XMFLOAT4:
				XMFLOAT4 vec4 = *(XMFLOAT4*)data[i];
				jsonOutput.at(0).push_back({ varNames[i],  { vec4.x, vec4.y, vec4.z, vec4.w } });
			case JSON_VARIABLE_TYPE::JSON_VARIABLE_TYPE_GAME_OBJECTS:
				jsonOutput.at(0).push_back("ObjectData");
				jsonOutput["ObjectData"].at(0).push_back({});
				jsonOutput["ObjectData"].at(0).at(i) = PackJson((GameObject*)data[i], 0);
				break;
			}
		}
		outFile << jsonOutput;
		outFile.close();
		return true;
	}
	catch (const std::exception&)
	{
		return false;
	}
}

void FileIO::LoadUI(const char* fileLocation, UIManager* pUManager, ID3D11Device* device)
{
	json data = LoadJson(fileLocation);
	json metaData = data["MetaData"];
	json uiData = data["UIData"];

	for (size_t i = 0; i < metaData.at(0)["NumberOfUIelements"]; i++)
	{
		if (uiData.at(i)["Canvas"])
		{
			std::string name = (std::string)uiData.at(i)["Name"]; 
			XMFLOAT3 pos =  XMFLOAT3(uiData.at(i)["Position"][0], uiData.at(i)["Position"][1], uiData.at(i)["Position"][2]);
			XMFLOAT3 rot = XMFLOAT3(uiData.at(i)["Rotation"][0], uiData.at(i)["Rotation"][1], uiData.at(i)["Rotation"][2]);
			XMFLOAT3 sca = XMFLOAT3(uiData.at(i)["Scale"][0], uiData.at(i)["Scale"][1], uiData.at(i)["Scale"][2]);
			pUManager->CreateCanvas(name, pos, sca, rot);
			pUManager->SelectUIObjectUsingIdentifier(name);
		}
		else if (uiData.at(i)["Image"])
		{
			std::string name = (std::string)uiData.at(i)["Name"];
			XMFLOAT3 pos = XMFLOAT3(uiData.at(i)["Position"][0], uiData.at(i)["Position"][1], uiData.at(i)["Position"][2]);
			XMFLOAT3 rot = XMFLOAT3(uiData.at(i)["Rotation"][0], uiData.at(i)["Rotation"][1], uiData.at(i)["Rotation"][2]);
			XMFLOAT3 sca = XMFLOAT3(uiData.at(i)["Scale"][0], uiData.at(i)["Scale"][1], uiData.at(i)["Scale"][2]);

			ImageComponent* iC = pUManager->CreateUIComponent<ImageComponent>(name, pos, sca, rot);
			wstring location = ToWstring((std::string)uiData.at(i)["ImageLocation"]);
			GraphicsManager::GetInstance()->LoadTextureFromFile(location);
			m_Cache.m_TextureCache[GraphicsManager::GetInstance()->GetShaderResourceView(location)] = location;
			iC->Init(location);
		}
		else if (uiData.at(i)["Text"])
		{
			std::string name = (std::string)uiData.at(i)["Name"];
			XMFLOAT3 pos = XMFLOAT3(uiData.at(i)["Position"][0], uiData.at(i)["Position"][1], uiData.at(i)["Position"][2]);
			XMFLOAT3 rot = XMFLOAT3(uiData.at(i)["Rotation"][0], uiData.at(i)["Rotation"][1], uiData.at(i)["Rotation"][2]);
			XMFLOAT3 sca = XMFLOAT3(uiData.at(i)["Scale"][0], uiData.at(i)["Scale"][1], uiData.at(i)["Scale"][2]);


			XMVECTORF32 colour;
			colour.f[0] = uiData.at(i)["Colour"][0];
			colour.f[1] = uiData.at(i)["Colour"][1];
			colour.f[2] = uiData.at(i)["Colour"][2];

			TextComponent* tC = pUManager->CreateUIComponent<TextComponent>(name, pos, sca, rot);
			std::string text = uiData.at(i)["TextData"];
			std::string fontName = uiData.at(i)["FontName"];
			int fontSize = uiData.at(i)["FontSize"];
			tC->Init(text, fontName, fontSize, colour, device);
		}
		else
		{
#ifdef DEBUG
			LOG ("No UI Element type was specified")
#endif // DEBUG

		}
	}
}

void FileIO::SaveUI(const char* fileLocation, UIManager* pUManager)
{
	std::ofstream outFile;
	outFile.open(fileLocation);
	json jsonOutput{};

	jsonOutput["MetaData"] = {};
	jsonOutput["MetaData"].push_back({});
	jsonOutput["MetaData"].at(0).push_back({ "NumberOfUIelements", 0});
	jsonOutput["UIData"] = {};

	std::vector<GameUIComponent*> components = pUManager->GetAllUIComponents();
	std::vector<UICanvas*> canvases;

	for (size_t i = 0; i < components.size(); i++)
	{
		if (dynamic_cast<UICanvas*>(components[i]) != NULL)
		{
			canvases.push_back(dynamic_cast<UICanvas*>(components[i]));
		}
	}

	int UICount = 0;
	for (UICanvas* canvas : canvases)
	{
		jsonOutput["UIData"].push_back({});
		XMFLOAT3 pos = canvas->GetTransform()->GetPosition();
		float position[3]{ pos.x, pos.y, pos.z };
		XMFLOAT3 rot = canvas->GetTransform()->GetRotation();
		float rotation[3]{ rot.x, rot.y, rot.z };
		XMFLOAT3 scal = canvas->GetTransform()->GetScale();
		float scale[3]{ scal.x, scal.y, scal.z };

		jsonOutput["UIData"].at(UICount).push_back({ "Name", canvas->GetIdentifier() });
		jsonOutput["UIData"].at(UICount).push_back({ "Position", position });
		jsonOutput["UIData"].at(UICount).push_back({ "Rotation" , rotation });
		jsonOutput["UIData"].at(UICount).push_back({ "Scale" , scale });
		jsonOutput["UIData"].at(UICount).push_back({ "Canvas" , true });
		jsonOutput["UIData"].at(UICount).push_back({ "Image" , false });
		jsonOutput["UIData"].at(UICount).push_back({ "Text" , false });
		++UICount;
		for (size_t i = 0; i < components.size(); i++)
		{
			TextComponent* textCom = dynamic_cast<TextComponent*>(components[i]);
			ImageComponent* imageCom = dynamic_cast<ImageComponent*>(components[i]);
			if (textCom)
			{
				jsonOutput["UIData"].push_back({});
				jsonOutput["UIData"].at(UICount).push_back({ "Name", textCom->GetIdentifier() });

				pos = textCom->GetTransform()->GetPosition();
				float tPosition[3]{ pos.x, pos.y, pos.z };
				XMFLOAT3 rot = textCom->GetTransform()->GetRotation();
				float tRotation[3]{ rot.x, rot.y, rot.z };
				XMFLOAT3 scal = textCom->GetTransform()->GetScale();
				float tScale[3]{ scal.x, scal.y, scal.z };

				jsonOutput["UIData"].at(UICount).push_back({ "Position", tPosition });
				jsonOutput["UIData"].at(UICount).push_back({ "Rotation" , tRotation });
				jsonOutput["UIData"].at(UICount).push_back({ "Scale" , tScale });
				jsonOutput["UIData"].at(UICount).push_back({ "Canvas" , false });
				jsonOutput["UIData"].at(UICount).push_back({ "Image" , false });
				jsonOutput["UIData"].at(UICount).push_back({ "Text" , true });
				jsonOutput["UIData"].at(UICount).push_back({ "TextData" , textCom->m_text });
				jsonOutput["UIData"].at(UICount).push_back({ "FontName" , textCom->m_fontName });
				jsonOutput["UIData"].at(UICount).push_back({ "FontSize" , 16});
				jsonOutput["UIData"].at(UICount).push_back({ "FontDDS" ,  ToString(FontManager::GetInstance()->GetFontTextureFilePath(textCom->m_fontName)) });
				jsonOutput["UIData"].at(UICount).push_back({ "FontXML" ,ToString(m_Cache.m_XMLCache[textCom->m_ptexture]) });
				float colour[3]{ textCom->m_colour.f[0], textCom->m_colour.f[1] , textCom->m_colour.f[2] };
				jsonOutput["UIData"].at(UICount).push_back({ "Colour" , 	colour });
				++UICount;
			}
			else if (imageCom)
			{
				jsonOutput["UIData"].push_back({});
				jsonOutput["UIData"].at(UICount).push_back({ "Name", imageCom->GetIdentifier() });

				pos = imageCom->GetTransform()->GetPosition();
				float iPosition[3]{ pos.x, pos.y, pos.z };
				XMFLOAT3 rot = imageCom->GetTransform()->GetRotation();
				float iRotation[3]{ rot.x, rot.y, rot.z };
				XMFLOAT3 scal = imageCom->GetTransform()->GetScale();
				float iScale[3]{ scal.x, scal.y, scal.z };

				jsonOutput["UIData"].at(UICount).push_back({ "Position", iPosition });
				jsonOutput["UIData"].at(UICount).push_back({ "Rotation" , iRotation });
				jsonOutput["UIData"].at(UICount).push_back({ "Scale" , iScale });
				jsonOutput["UIData"].at(UICount).push_back({ "Canvas" , false });
				jsonOutput["UIData"].at(UICount).push_back({ "Image" , true });
				jsonOutput["UIData"].at(UICount).push_back({ "Text" , false });
				jsonOutput["UIData"].at(UICount).push_back({ "ImageLocation" , ToString(GrabTextureName(imageCom->m_ptexture)) });
				++UICount;
			}
		}


	}
	jsonOutput["MetaData"].at(0)["NumberOfUIelements"] = UICount;
	outFile << jsonOutput;
	outFile.close();
}

void FileIO::SaveGameObject(const char* fileLocation ,GameObject* gameObject)
{
	std::ofstream outFile;
	outFile.open(fileLocation);
	json jsonOutput{};
	jsonOutput = PackJson(gameObject, 0);
	outFile << jsonOutput;
	outFile.close();
}

void FileIO::SaveScene(const char* fileLocation, GameManager* scene)
{
	bool saveObject = true;
	int tileCount = 0;
	int particleCount = 0;
	int particleSystemCount = 0;
	TileMap* tileMap = nullptr;
	ParticleSystem* particleSys = nullptr;
	Tile* tile = nullptr;
	Particle* particle = nullptr;

	vector<GameObject*> gameObjects = scene->GetAllGameObjects();
	std::ofstream outFile;
	outFile.open(fileLocation);
	json jsonOutput{};
	jsonOutput["MetaData"] = {};
	jsonOutput["MetaData"].push_back({});
	jsonOutput["TileMap"] = {};
	jsonOutput["ObjectData"] = {};
	jsonOutput["ParticleData"] = {};
	jsonOutput["ParticleSystemData"] = {};

	int count = 0;

	for (int it = 0; it < gameObjects.size(); ++it)
	{
		saveObject = true;
		GameObject* gameObjectToStore = gameObjects[it];
	
		particleSys = dynamic_cast<ParticleSystem*>(gameObjectToStore);
		tileMap = dynamic_cast<TileMap*>(gameObjectToStore);
		tile = dynamic_cast<Tile*>(gameObjectToStore);
		particle = dynamic_cast<Particle*>(gameObjectToStore);

		if (tile != NULL)
		{
			saveObject = false;
		}

		if (saveObject)
		{
			if (!tileMap && !particleSys && !particle)
			{
				if (gameObjectToStore->ContainsType(GameObjectType::PLAYER))
				{
					XMFLOAT3 pos = gameObjectToStore->GetTransform()->GetPosition();
					float position[3]{ pos.x, pos.y, pos.z };
					XMFLOAT3 rot = gameObjectToStore->GetTransform()->GetRotation();
					float rotation[3]{ rot.x, rot.y, rot.z };
					XMFLOAT3 scal = gameObjectToStore->GetTransform()->GetScale();
					float scale[3]{ scal.x, scal.y, scal.z };
					jsonOutput["ObjectData"].push_back({});
					jsonOutput["ObjectData"].at(count).push_back({ "Name", gameObjectToStore->GetIdentifier() });

					jsonOutput["ObjectData"].at(count).push_back({ "Position", position });
					jsonOutput["ObjectData"].at(count).push_back({ "Rotation" , rotation });
					jsonOutput["ObjectData"].at(count).push_back({ "Scale" , scale });

					PlayerGameObject* playerGameObj = dynamic_cast<PlayerGameObject*>(gameObjectToStore);

					std::string vSL = ToString(GrabVertexShaderName(playerGameObj->GetVertexShader()));
					std::string pSL = ToString(GrabPixelShaderName(playerGameObj->GetPixelShader()));
					std::string aSRV = ToString(GrabAlbedoName(playerGameObj->GetAlbedoSRV()));
					std::vector<std::string> shaders(3);
					shaders[0] = vSL;
					shaders[1] = pSL;
					shaders[2] = aSRV;

					jsonOutput["ObjectData"].at(count).push_back({ "VertexShaderLocation" , shaders[0] });
					jsonOutput["ObjectData"].at(count).push_back({ "PixelShaderLocation" , shaders[1] });
					jsonOutput["ObjectData"].at(count).push_back({ "AlbedoLocation" , shaders[2] });

					jsonOutput["ObjectData"].at(count).push_back({ "IsRenderable" , playerGameObj->IsRenderable() });


					jsonOutput["ObjectData"].at(count).push_back({ "NumberOfAnimations", playerGameObj->GetAnimations()->size() });

					std::vector<std::string> spriteAnimation(playerGameObj->GetAnimations()->size());
					int animationCount = 0;
					for (unordered_map<string, SpriteAnimation>::iterator it2 = playerGameObj->GetAnimations()->begin(); it2 != playerGameObj->GetAnimations()->end(); ++it2)
					{
						spriteAnimation[animationCount] = m_Cache.m_AnimationCache.at(it2->second.GetFrames());
						++animationCount;
					}
					jsonOutput["ObjectData"].at(count).push_back({ "AnimationFile",	spriteAnimation });

					Box* testBox = dynamic_cast<Box*>(playerGameObj->GetShape());
					if (testBox == NULL)
					{
						Circle* c = dynamic_cast<Circle*>(playerGameObj->GetShape());
						if (c != NULL)
						{
							jsonOutput["ObjectData"].at(count).push_back({ "ColliderType" , "CircleCollider" });
							jsonOutput["ObjectData"].at(count).push_back({ "CircleRadius" , c->m_radius });
						}
					}
					else
					{
						jsonOutput["ObjectData"].at(count).push_back({ "ColliderType" , "BoxCollider" });
					}
					if (playerGameObj->GetShape() != nullptr)
					{
						jsonOutput["ObjectData"].at(count).push_back({ "IsCollideable" , playerGameObj->GetShape()->IsCollidable() });
						jsonOutput["ObjectData"].at(count).push_back({ "IsTrigger" , playerGameObj->GetShape()->IsTrigger() });
					}
					else
					{
						jsonOutput["ObjectData"].at(count).push_back({ "IsCollideable" , false });
						jsonOutput["ObjectData"].at(count).push_back({ "IsTrigger" , false });
					}
					jsonOutput["ObjectData"].at(count).push_back({ "MovementSpeed", playerGameObj->GetMoveSpeed() });
					++count;
				}
				else if (gameObjectToStore->ContainsType(GameObjectType::RENDERABLE) && gameObjectToStore->ContainsType(GameObjectType::COLLIDABLE))
				{
					XMFLOAT3 pos = gameObjectToStore->GetTransform()->GetPosition();
					float position[3]{ pos.x, pos.y, pos.z };
					XMFLOAT3 rot = gameObjectToStore->GetTransform()->GetRotation();
					float rotation[3]{ rot.x, rot.y, rot.z };
					XMFLOAT3 scal = gameObjectToStore->GetTransform()->GetScale();
					float scale[3]{ scal.x, scal.y, scal.z };

					jsonOutput["ObjectData"].push_back({});

					jsonOutput["ObjectData"].at(count).push_back({ "Name", gameObjectToStore->GetIdentifier() });

					jsonOutput["ObjectData"].at(count).push_back({ "Position", position });
					jsonOutput["ObjectData"].at(count).push_back({ "Rotation" , rotation });
					jsonOutput["ObjectData"].at(count).push_back({ "Scale" , scale });

					RenderableCollidableGameObject* renderCollideObject = dynamic_cast<RenderableCollidableGameObject*>(gameObjectToStore);
					std::string vSL = ToString(GrabVertexShaderName(renderCollideObject->GetVertexShader()));
					std::string pSL = ToString(GrabPixelShaderName(renderCollideObject->GetPixelShader()));
					std::string aSRV = ToString(GrabAlbedoName(renderCollideObject->GetAlbedoSRV()));

					std::vector<std::string> shaders(3);
					shaders[0] = vSL;
					shaders[1] = pSL;
					shaders[2] = aSRV;

					jsonOutput["ObjectData"].at(count).push_back({ "VertexShaderLocation" , shaders[0] });
					jsonOutput["ObjectData"].at(count).push_back({ "PixelShaderLocation" , shaders[1] });
					jsonOutput["ObjectData"].at(count).push_back({ "AlbedoLocation" , shaders[2] });

					jsonOutput["ObjectData"].at(count).push_back({ "IsRenderable" , renderCollideObject->IsRenderable() });


					jsonOutput["ObjectData"].at(count).push_back({ "NumberOfAnimations", renderCollideObject->GetAnimations()->size() });

					std::vector<std::string> spriteAnimation(renderCollideObject->GetAnimations()->size());
					int animationCount = 0;
					for (unordered_map<string, SpriteAnimation>::iterator it2 = renderCollideObject->GetAnimations()->begin(); it2 != renderCollideObject->GetAnimations()->end(); ++it2)
					{
						spriteAnimation[animationCount] = m_Cache.m_AnimationCache.at(it2->second.GetFrames());
						++animationCount;
					}
					jsonOutput["ObjectData"].at(count).push_back({ "AnimationFile",	spriteAnimation });

					Box* testBox = dynamic_cast<Box*>(renderCollideObject->GetShape());
					if (testBox == NULL)
					{
						Circle* c = dynamic_cast<Circle*>(renderCollideObject->GetShape());
						if (c != NULL)
						{
							jsonOutput["ObjectData"].at(count).push_back({ "ColliderType" , "CircleCollider" });
							jsonOutput["ObjectData"].at(count).push_back({ "CircleRadius" , c->m_radius });
						}
					}
					else
					{
						jsonOutput["ObjectData"].at(count).push_back({ "ColliderType" , "BoxCollider" });
					}
					if (renderCollideObject->GetShape() != nullptr)
					{
						jsonOutput["ObjectData"].at(count).push_back({ "IsCollideable" , renderCollideObject->GetShape()->IsCollidable() });
						jsonOutput["ObjectData"].at(count).push_back({ "IsTrigger" , renderCollideObject->GetShape()->IsTrigger() });
					}
					else
					{
						jsonOutput["ObjectData"].at(count).push_back({ "IsCollideable" , false });
						jsonOutput["ObjectData"].at(count).push_back({ "IsTrigger" , false });
					}
					++count;
				}
				else if (gameObjectToStore->ContainsType(GameObjectType::RENDERABLE))
				{
					XMFLOAT3 pos = gameObjectToStore->GetTransform()->GetPosition();
					float position[3]{ pos.x, pos.y, pos.z };
					XMFLOAT3 rot = gameObjectToStore->GetTransform()->GetRotation();
					float rotation[3]{ rot.x, rot.y, rot.z };
					XMFLOAT3 scal = gameObjectToStore->GetTransform()->GetScale();
					float scale[3]{ scal.x, scal.y, scal.z };

					jsonOutput["ObjectData"].push_back({});

					jsonOutput["ObjectData"].at(count).push_back({ "Name", gameObjectToStore->GetIdentifier() });

					jsonOutput["ObjectData"].at(count).push_back({ "IsCollideable", false });

					RenderableGameObject* renderObject = dynamic_cast<RenderableGameObject*>(gameObjectToStore);
					std::string vSL = ToString(GrabVertexShaderName(renderObject->GetVertexShader()));
					std::string pSL = ToString(GrabPixelShaderName(renderObject->GetPixelShader()));
					std::string aSRV = ToString(GrabAlbedoName(renderObject->GetAlbedoSRV()));

					std::vector<std::string> shaders(3);
					shaders[0] = vSL;
					shaders[1] = pSL;
					shaders[2] = aSRV;

					jsonOutput["ObjectData"].at(count).push_back({ "VertexShaderLocation" , shaders[0] });
					jsonOutput["ObjectData"].at(count).push_back({ "PixelShaderLocation" , shaders[1] });
					jsonOutput["ObjectData"].at(count).push_back({ "AlbedoLocation" , shaders[2] });

					jsonOutput["ObjectData"].at(count).push_back({ "IsRenderable" , renderObject->IsRenderable() });


					jsonOutput["ObjectData"].at(count).push_back({ "NumberOfAnimations", renderObject->GetAnimations()->size() });

					std::vector<std::string> spriteAnimation(renderObject->GetAnimations()->size());
					int animationCount = 0;
					for (unordered_map<string, SpriteAnimation>::iterator it2 = renderObject->GetAnimations()->begin(); it2 != renderObject->GetAnimations()->end(); ++it2)
					{
						spriteAnimation[animationCount] = m_Cache.m_AnimationCache.at(it2->second.GetFrames());
						++animationCount;
					}
					jsonOutput["ObjectData"].at(count).push_back({ "AnimationFile",	spriteAnimation });
					++count;
				}
				else if (gameObjectToStore->ContainsType(GameObjectType::COLLIDABLE))
				{

					XMFLOAT3 pos = gameObjectToStore->GetTransform()->GetPosition();
					float position[3]{ pos.x, pos.y, pos.z };
					XMFLOAT3 rot = gameObjectToStore->GetTransform()->GetRotation();
					float rotation[3]{ rot.x, rot.y, rot.z };
					XMFLOAT3 scal = gameObjectToStore->GetTransform()->GetScale();
					float scale[3]{ scal.x, scal.y, scal.z };

					jsonOutput["ObjectData"].push_back({});
					jsonOutput["ObjectData"].at(count).push_back({ "Name", gameObjectToStore->GetIdentifier() });

					jsonOutput["ObjectData"].at(count).push_back({ "IsRenderable", false});

					CollidableGameObject* collideObject = dynamic_cast<CollidableGameObject*>(gameObjectToStore);
					Box* testBox = dynamic_cast<Box*>(collideObject->GetShape());
					if (testBox == NULL)
					{
						Circle* c = dynamic_cast<Circle*>(collideObject->GetShape());
						if (c != NULL)
						{
							jsonOutput["ObjectData"].at(count).push_back({ "ColliderType" , "CircleCollider" });
							jsonOutput["ObjectData"].at(count).push_back({ "CircleRadius" , c->m_radius });
						}
					}
					else
					{
						jsonOutput["ObjectData"].at(count).push_back({ "ColliderType" , "BoxCollider" });
					}
					if (collideObject->GetShape() != nullptr)
					{
						jsonOutput["ObjectData"].at(count).push_back({ "IsCollideable" , collideObject->GetShape()->IsCollidable() });
						jsonOutput["ObjectData"].at(count).push_back({ "IsTrigger" , collideObject->GetShape()->IsTrigger() });
					}
					else
					{
						jsonOutput["ObjectData"].at(count).push_back({ "IsCollideable" , false });
						jsonOutput["ObjectData"].at(count).push_back({ "IsTrigger" , false });
					}
					++count;
				}
			}
			else if (particleSys)
			{
				if (gameObjectToStore->ContainsType(GameObjectType::RENDERABLE))
				{
					RenderableGameObject* renderObject = dynamic_cast<RenderableGameObject*>(gameObjectToStore);
					std::string vSL = ToString(GrabVertexShaderName(renderObject->GetVertexShader()));
					std::string pSL = ToString(GrabPixelShaderName(renderObject->GetPixelShader()));
					std::string aSRV = ToString(GrabAlbedoName(renderObject->GetAlbedoSRV()));

					std::vector<std::string> shaders(3);
					shaders[0] = vSL;
					shaders[1] = pSL;
					shaders[2] = aSRV;

					jsonOutput["ParticleSystemData"].at(particleSystemCount).push_back({ "VertexShaderLocation" , shaders[0] });
					jsonOutput["ParticleSystemData"].at(particleSystemCount).push_back({ "PixelShaderLocation" , shaders[1] });
					jsonOutput["ParticleSystemData"].at(particleSystemCount).push_back({ "AlbedoLocation" , shaders[2] });
				}

				

				XMFLOAT3 pos = gameObjectToStore->GetTransform()->GetPosition();
				float position[3]{ pos.x, pos.y, pos.z };
				XMFLOAT3 rot = gameObjectToStore->GetTransform()->GetRotation();
				float rotation[3]{ rot.x, rot.y, rot.z };
				XMFLOAT3 scal = gameObjectToStore->GetTransform()->GetScale();
				float scale[3]{ scal.x, scal.y, scal.z };

				XMFLOAT2 acceleration = particleSys->GetAccel();
				XMFLOAT2 veclocity = particleSys->GetVelocity();

				float particleAcceleration[2]{ acceleration.x, acceleration.y};
				float particleVelocity[2]{ veclocity.x, veclocity.y};

				//Need height and width of tile map
				jsonOutput["ParticleSystemData"].push_back({});
				jsonOutput["ParticleSystemData"].at(particleSystemCount).push_back({ "Name", particleSys->GetIdentifier()});
				jsonOutput["ParticleSystemData"].at(particleSystemCount).push_back({ "Life", particleSys->GetLife() });
				jsonOutput["ParticleSystemData"].at(particleSystemCount).push_back({ "Position", position });
				jsonOutput["ParticleSystemData"].at(particleSystemCount).push_back({ "Rotation", rotation });
				jsonOutput["ParticleSystemData"].at(particleSystemCount).push_back({ "Scale", scale });
				jsonOutput["ParticleSystemData"].at(particleSystemCount).push_back({ "Count" ,  particleSys->GetSpawnNumber()});
				jsonOutput["ParticleSystemData"].at(particleSystemCount).push_back({ "Interval" ,  particleSys->GetSpawnInterval()});
				jsonOutput["ParticleSystemData"].at(particleSystemCount).push_back({ "RandVelocity" ,  particleSys->GetRandomAccel() });
				jsonOutput["ParticleSystemData"].at(particleSystemCount).push_back({ "Velocity" ,  particleVelocity});
				jsonOutput["ParticleSystemData"].at(particleSystemCount).push_back({ "RandAcceleration" ,  particleSys->GetRandomAccel() });
				jsonOutput["ParticleSystemData"].at(particleSystemCount).push_back({ "Acceleration" ,  particleAcceleration });
				jsonOutput["ParticleSystemData"].at(particleSystemCount).push_back({ "RandLife" ,  particleSys->GetRandomLife() });
				jsonOutput["ParticleSystemData"].at(particleSystemCount).push_back({ "ParticleLife" ,  particleSys->GetParticleLife()});
				jsonOutput["ParticleSystemData"].at(particleSystemCount).push_back({ "RandPosition" ,  particleSys->GetRandomPos() });
				jsonOutput["ParticleSystemData"].at(particleSystemCount).push_back({ "AlbedoLocation" ,  ToString(GrabAlbedoName(particleSys->m_pTexture)) });
				jsonOutput["ParticleSystemData"].at(particleSystemCount).push_back({ "Layer" ,  particleSys->GetLayer() });



				++particleSystemCount;
			}
			else
			{

				std::string vSL = ToString(GrabVertexShaderName(tileMap->GetVertexShader()));
				std::string pSL = ToString(GrabPixelShaderName(tileMap->GetPixelShader()));
				std::string aSRV = ToString(GrabAlbedoName(tileMap->GetAlbedoSRV()));

				std::vector<std::string> shaders(3);
				shaders[0] = vSL;
				shaders[1] = pSL;
				shaders[2] = aSRV;

				XMFLOAT3 pos = tileMap->GetTransform()->GetPosition();
				XMFLOAT3 sca = tileMap->GetTransform()->GetScale();
				float position[3]{ pos.x, pos.y, pos.z };
				float scale[3]{ sca.x, sca.y, sca.z };

				//Need height and width of tile map
				jsonOutput["TileMap"].push_back({});
				jsonOutput["TileMap"].at(tileCount).push_back({ "Name", tileMap->GetIdentifier() });
				//jsonOutput["TileMap"].at(count).push_back({ "Height",  });	

				if (gameObjectToStore->ContainsType(GameObjectType::COLLIDABLE))
				{
					CollidableGameObject* collideObject = dynamic_cast<CollidableGameObject*>(tileMap);
					Box* testBox = dynamic_cast<Box*>(collideObject->GetShape());
					if (testBox == NULL)
					{
						Circle* c = dynamic_cast<Circle*>(collideObject->GetShape());
						if (c != NULL)
						{
							jsonOutput["TileMap"].at(count).push_back({ "ColliderType" , "CircleCollider" });
							jsonOutput["TileMap"].at(count).push_back({ "CircleRadius" , c->m_radius });
						}
					}
					else
					{
						jsonOutput["TileMap"].at(count).push_back({ "ColliderType" , "BoxCollider" });
					}
					if (collideObject->GetShape() != nullptr)
					{
						jsonOutput["TileMap"].at(count).push_back({ "IsCollideable" , collideObject->GetShape()->IsCollidable() });
						jsonOutput["TileMap"].at(count).push_back({ "IsTrigger" , collideObject->GetShape()->IsTrigger() });
					}
					else
					{
						jsonOutput["TileMap"].at(count).push_back({ "IsCollideable" , false });
						jsonOutput["TileMap"].at(count).push_back({ "IsTrigger" , false });
					}
				}
				jsonOutput["TileMap"].at(tileCount).push_back({ "Position", position });
				jsonOutput["TileMap"].at(tileCount).push_back({ "Scale", scale });
				jsonOutput["TileMap"].at(tileCount).push_back({ "VertexShaderLocation" , shaders[0] });
				jsonOutput["TileMap"].at(tileCount).push_back({ "PixelShaderLocation" , shaders[1] });
				jsonOutput["TileMap"].at(tileCount).push_back({ "AlbedoLocation" , shaders[2] });
				jsonOutput["TileMap"].at(tileCount).push_back({ "AnimationCount", 0 });
				jsonOutput["TileMap"].at(tileCount).push_back({ "TileData", ToString(m_Cache.m_MapData[tileCount]) });
				++tileCount;
			}
		}
		
	}

	jsonOutput["MetaData"].at(0).push_back({ "NumberOfObject", count });
	jsonOutput["MetaData"].at(0).push_back({ "NumberOfTileZones", tileCount });
	jsonOutput["MetaData"].at(0).push_back({ "NumberOfParticlesSystems", particleSystemCount });
	jsonOutput["MetaData"].at(0).push_back({ "NumberOfParticles", particleCount });


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

json FileIO::PackJson(GameObject* dataToPack, int count)
{
	json dataDestination;
	TileMap* tileMap = nullptr;
	ParticleSystem* particleSys = nullptr;
	Tile* tile = nullptr;
	Particle* particle = nullptr;
	particleSys = dynamic_cast<ParticleSystem*>(dataToPack);
	tileMap = dynamic_cast<TileMap*>(dataToPack);
	tile = dynamic_cast<Tile*>(dataToPack);
	particle = dynamic_cast<Particle*>(dataToPack);
	const std::string identifier = dataToPack->GetIdentifier();

	if (dataToPack->ContainsType(GameObjectType::PLAYER))
	{
		XMFLOAT3 pos = dataToPack->GetTransform()->GetPosition();
		float position[3]{ pos.x, pos.y, pos.z };
		XMFLOAT3 rot = dataToPack->GetTransform()->GetRotation();
		float rotation[3]{ rot.x, rot.y, rot.z };
		XMFLOAT3 scal = dataToPack->GetTransform()->GetScale();
		float scale[3]{ scal.x, scal.y, scal.z };
		dataDestination[identifier] = {};
		dataDestination[identifier].push_back({});
		dataDestination[identifier].at(count).push_back({ "Name", dataToPack->GetIdentifier() });

		dataDestination[identifier].at(count).push_back({ "Position", position });
		dataDestination[identifier].at(count).push_back({ "Rotation" , rotation });
		dataDestination[identifier].at(count).push_back({ "Scale" , scale });

		PlayerGameObject* playerGameObj = dynamic_cast<PlayerGameObject*>(dataToPack);

		std::string vSL = ToString(GrabVertexShaderName(playerGameObj->GetVertexShader()));
		std::string pSL = ToString(GrabPixelShaderName(playerGameObj->GetPixelShader()));
		std::string aSRV = ToString(GrabAlbedoName(playerGameObj->GetAlbedoSRV()));
		std::vector<std::string> shaders(3);
		shaders[0] = vSL;
		shaders[1] = pSL;
		shaders[2] = aSRV;

		dataDestination[identifier].at(count).push_back({ "VertexShaderLocation" , shaders[0] });
		dataDestination[identifier].at(count).push_back({ "PixelShaderLocation" , shaders[1] });
		dataDestination[identifier].at(count).push_back({ "AlbedoLocation" , shaders[2] });

		dataDestination[identifier].at(count).push_back({ "IsRenderable" , playerGameObj->IsRenderable() });


		dataDestination[identifier].at(count).push_back({ "NumberOfAnimations", playerGameObj->GetAnimations()->size() });

		std::vector<std::string> spriteAnimation(playerGameObj->GetAnimations()->size());
		int animationCount = 0;
		for (unordered_map<string, SpriteAnimation>::iterator it2 = playerGameObj->GetAnimations()->begin(); it2 != playerGameObj->GetAnimations()->end(); ++it2)
		{
			spriteAnimation[animationCount] = m_Cache.m_AnimationCache.at(it2->second.GetFrames());
			++animationCount;
		}
		dataDestination[identifier].at(count).push_back({ "AnimationFile",	spriteAnimation });

		Box* testBox = dynamic_cast<Box*>(playerGameObj->GetShape());
		if (testBox == NULL)
		{
			Circle* c = dynamic_cast<Circle*>(playerGameObj->GetShape());
			if (c != NULL)
			{
				dataDestination[identifier].at(count).push_back({ "ColliderType" , "CircleCollider" });
				dataDestination[identifier].at(count).push_back({ "CircleRadius" , c->m_radius });
			}
		}
		else
		{
			dataDestination[identifier].at(count).push_back({ "ColliderType" , "BoxCollider" });
		}
		if (playerGameObj->GetShape() != nullptr)
		{
			dataDestination[identifier].at(count).push_back({ "IsCollideable" , playerGameObj->GetShape()->IsCollidable() });
			dataDestination[identifier].at(count).push_back({ "IsTrigger" , playerGameObj->GetShape()->IsTrigger() });
		}
		else
		{
			dataDestination[identifier].at(count).push_back({ "IsCollideable" , false });
			dataDestination[identifier].at(count).push_back({ "IsTrigger" , false });
		}
		dataDestination[identifier].at(count).push_back({"MovementSpeed", playerGameObj->GetMoveSpeed()});
	}
	else if (dataToPack->ContainsType(GameObjectType::RENDERABLE) && dataToPack->ContainsType(GameObjectType::COLLIDABLE))
	{
		XMFLOAT3 pos = dataToPack->GetTransform()->GetPosition();
		float position[3]{ pos.x, pos.y, pos.z };
		XMFLOAT3 rot = dataToPack->GetTransform()->GetRotation();
		float rotation[3]{ rot.x, rot.y, rot.z };
		XMFLOAT3 scal = dataToPack->GetTransform()->GetScale();
		float scale[3]{ scal.x, scal.y, scal.z };

		dataDestination[identifier] = {};

		dataDestination[identifier].at(count).push_back({ "Name", dataToPack->GetIdentifier() });

		dataDestination[identifier].at(count).push_back({ "Position", position });
		dataDestination[identifier].at(count).push_back({ "Rotation" , rotation });
		dataDestination[identifier].at(count).push_back({ "Scale" , scale });

		RenderableCollidableGameObject* renderCollideObject = dynamic_cast<RenderableCollidableGameObject*>(dataToPack);
		std::string vSL = ToString(GrabVertexShaderName(renderCollideObject->GetVertexShader()));
		std::string pSL = ToString(GrabPixelShaderName(renderCollideObject->GetPixelShader()));
		std::string aSRV = ToString(GrabAlbedoName(renderCollideObject->GetAlbedoSRV()));

		std::vector<std::string> shaders(3);
		shaders[0] = vSL;
		shaders[1] = pSL;
		shaders[2] = aSRV;

		dataDestination[identifier].at(count).push_back({ "VertexShaderLocation" , shaders[0] });
		dataDestination[identifier].at(count).push_back({ "PixelShaderLocation" , shaders[1] });
		dataDestination[identifier].at(count).push_back({ "AlbedoLocation" , shaders[2] });

		dataDestination[identifier].at(count).push_back({ "IsRenderable" , renderCollideObject->IsRenderable() });


		dataDestination[identifier].at(count).push_back({ "NumberOfAnimations", renderCollideObject->GetAnimations()->size() });

		std::vector<std::string> spriteAnimation(renderCollideObject->GetAnimations()->size());
		int animationCount = 0;
		for (unordered_map<string, SpriteAnimation>::iterator it2 = renderCollideObject->GetAnimations()->begin(); it2 != renderCollideObject->GetAnimations()->end(); ++it2)
		{
			spriteAnimation[animationCount] = m_Cache.m_AnimationCache.at(it2->second.GetFrames());
			++animationCount;
		}
		dataDestination[identifier].at(count).push_back({ "AnimationFile",	spriteAnimation });

		Box* testBox = dynamic_cast<Box*>(renderCollideObject->GetShape());
		if (testBox == NULL)
		{
			Circle* c = dynamic_cast<Circle*>(renderCollideObject->GetShape());
			if (c != NULL)
			{
				dataDestination[identifier].at(count).push_back({ "ColliderType" , "CircleCollider" });
				dataDestination[identifier].at(count).push_back({ "CircleRadius" , c->m_radius });
			}
		}
		else
		{
			dataDestination[identifier].at(count).push_back({ "ColliderType" , "BoxCollider" });
		}
		if (renderCollideObject->GetShape() != nullptr)
		{
			dataDestination[identifier].at(count).push_back({ "IsCollideable" , renderCollideObject->GetShape()->IsCollidable() });
			dataDestination[identifier].at(count).push_back({ "IsTrigger" , renderCollideObject->GetShape()->IsTrigger() });
		}
		else
		{
			dataDestination[identifier].at(count).push_back({ "IsCollideable" , false });
			dataDestination[identifier].at(count).push_back({ "IsTrigger" , false });
		}
	}
	else if (dataToPack->ContainsType(GameObjectType::RENDERABLE))
	{
		XMFLOAT3 pos = dataToPack->GetTransform()->GetPosition();
		float position[3]{ pos.x, pos.y, pos.z };
		XMFLOAT3 rot = dataToPack->GetTransform()->GetRotation();
		float rotation[3]{ rot.x, rot.y, rot.z };
		XMFLOAT3 scal = dataToPack->GetTransform()->GetScale();
		float scale[3]{ scal.x, scal.y, scal.z };

		dataDestination[identifier].push_back({});

		dataDestination[identifier].at(count).push_back({ "Name", dataToPack->GetIdentifier() });

		dataDestination[identifier].at(count).push_back({ "IsCollideable", false });

		RenderableGameObject* renderObject = dynamic_cast<RenderableGameObject*>(dataToPack);
		std::string vSL = ToString(GrabVertexShaderName(renderObject->GetVertexShader()));
		std::string pSL = ToString(GrabPixelShaderName(renderObject->GetPixelShader()));
		std::string aSRV = ToString(GrabAlbedoName(renderObject->GetAlbedoSRV()));

		std::vector<std::string> shaders(3);
		shaders[0] = vSL;
		shaders[1] = pSL;
		shaders[2] = aSRV;

		dataDestination[identifier].at(count).push_back({ "VertexShaderLocation" , shaders[0] });
		dataDestination[identifier].at(count).push_back({ "PixelShaderLocation" , shaders[1] });
		dataDestination[identifier].at(count).push_back({ "AlbedoLocation" , shaders[2] });

		dataDestination[identifier].at(count).push_back({ "IsRenderable" , renderObject->IsRenderable() });


		dataDestination[identifier].at(count).push_back({ "NumberOfAnimations", renderObject->GetAnimations()->size() });

		std::vector<std::string> spriteAnimation(renderObject->GetAnimations()->size());
		int animationCount = 0;
		for (unordered_map<string, SpriteAnimation>::iterator it2 = renderObject->GetAnimations()->begin(); it2 != renderObject->GetAnimations()->end(); ++it2)
		{
			spriteAnimation[animationCount] = m_Cache.m_AnimationCache.at(it2->second.GetFrames());
			++animationCount;
		}
		dataDestination[identifier].at(count).push_back({ "AnimationFile",	spriteAnimation });
	}
	else if (dataToPack->ContainsType(GameObjectType::COLLIDABLE))
	{

		XMFLOAT3 pos = dataToPack->GetTransform()->GetPosition();
		float position[3]{ pos.x, pos.y, pos.z };
		XMFLOAT3 rot = dataToPack->GetTransform()->GetRotation();
		float rotation[3]{ rot.x, rot.y, rot.z };
		XMFLOAT3 scal = dataToPack->GetTransform()->GetScale();
		float scale[3]{ scal.x, scal.y, scal.z };

		dataDestination[identifier].push_back({});
		dataDestination[identifier].at(count).push_back({ "Name", dataToPack->GetIdentifier() });

		dataDestination[identifier].at(count).push_back({ "IsRenderable", false });

		CollidableGameObject* collideObject = dynamic_cast<CollidableGameObject*>(dataToPack);
		Box* testBox = dynamic_cast<Box*>(collideObject->GetShape());
		if (testBox == NULL)
		{
			Circle* c = dynamic_cast<Circle*>(collideObject->GetShape());
			if (c != NULL)
			{
				dataDestination[identifier].at(count).push_back({ "ColliderType" , "CircleCollider" });
				dataDestination[identifier].at(count).push_back({ "CircleRadius" , c->m_radius });
			}
		}
		else
		{
			dataDestination[identifier].at(count).push_back({ "ColliderType" , "BoxCollider" });
		}
		if (collideObject->GetShape() != nullptr)
		{
			dataDestination[identifier].at(count).push_back({ "IsCollideable" , collideObject->GetShape()->IsCollidable() });
			dataDestination[identifier].at(count).push_back({ "IsTrigger" , collideObject->GetShape()->IsTrigger() });
		}
		else
		{
			dataDestination[identifier].at(count).push_back({ "IsCollideable" , false });
			dataDestination[identifier].at(count).push_back({ "IsTrigger" , false });
		}
	}
	else if (particle)
	{
		XMFLOAT3 pos = dataToPack->GetTransform()->GetPosition();
		float position[3]{ pos.x, pos.y, pos.z };
		position[0] = pos.x;
		position[1] = pos.y;
		position[2] = pos.z;
		XMFLOAT3 rot = dataToPack->GetTransform()->GetRotation();
		float rotation[3]{ rot.x, rot.y, rot.z };
		rotation[0] = rot.x;
		rotation[1] = rot.y;
		rotation[2] = rot.z;
		XMFLOAT3 scal = dataToPack->GetTransform()->GetScale();
		float scale[3]{ scal.x, scal.y, scal.z };
		scale[0] = scal.x;
		scale[1] = scal.y;
		scale[2] = scal.z;

		XMFLOAT2 vel = particle->GetVelocity();
		float velocity[2]{ vel.x, vel.y };

		XMFLOAT2 accel = particle->GetAccel();
		float acceleration[2]{ accel.x, accel.y };

		dataDestination[identifier].push_back({});
		dataDestination[identifier].at(count).push_back({ "Life", particle->GetLife() });
		dataDestination[identifier].at(count).push_back({ "Position", position });
		dataDestination[identifier].at(count).push_back({ "Rotation", rotation });
		dataDestination[identifier].at(count).push_back({ "Scale", scale });
		dataDestination[identifier].at(count).push_back({ "Velocity" , velocity });
		dataDestination[identifier].at(count).push_back({ "Acceleration" , acceleration });
	}
	else if (particleSys)
	{
		if (dataToPack->ContainsType(GameObjectType::RENDERABLE))
		{
			RenderableGameObject* renderObject = dynamic_cast<RenderableGameObject*>(dataToPack);
			std::string vSL = ToString(GrabVertexShaderName(renderObject->GetVertexShader()));
			std::string pSL = ToString(GrabPixelShaderName(renderObject->GetPixelShader()));
			std::string aSRV = ToString(GrabAlbedoName(renderObject->GetAlbedoSRV()));

			std::vector<std::string> shaders(3);
			shaders[0] = vSL;
			shaders[1] = pSL;
			shaders[2] = aSRV;

			dataDestination[identifier].at(count).push_back({ "VertexShaderLocation" , shaders[0] });
			dataDestination[identifier].at(count).push_back({ "PixelShaderLocation" , shaders[1] });
			dataDestination[identifier].at(count).push_back({ "AlbedoLocation" , shaders[2] });
		}
			XMFLOAT3 pos = dataToPack->GetTransform()->GetPosition();
			float position[3]{ pos.x, pos.y, pos.z };
			XMFLOAT3 rot = dataToPack->GetTransform()->GetRotation();
			float rotation[3]{ rot.x, rot.y, rot.z };
			XMFLOAT3 scal = dataToPack->GetTransform()->GetScale();
			float scale[3]{ scal.x, scal.y, scal.z };

			//Need height and width of tile map
			dataDestination[identifier].push_back({});
			dataDestination[identifier].at(count).push_back({ "Name", particleSys->GetIdentifier() });
			dataDestination[identifier].at(count).push_back({ "Life", particleSys->GetLife() });
			dataDestination[identifier].at(count).push_back({ "Position", position });
			dataDestination[identifier].at(count).push_back({ "Rotation", rotation });
			dataDestination[identifier].at(count).push_back({ "Scale", scale });
			dataDestination[identifier].at(count).push_back({ "Count" ,  particleSys->GetSpawnInterval()});


	}
	else
	{

		std::string vSL = ToString(GrabVertexShaderName(tileMap->GetVertexShader()));
		std::string pSL = ToString(GrabPixelShaderName(tileMap->GetPixelShader()));
		std::string aSRV = ToString(GrabAlbedoName(tileMap->GetAlbedoSRV()));

		std::vector<std::string> shaders(3);
		shaders[0] = vSL;
		shaders[1] = pSL;
		shaders[2] = aSRV;

		XMFLOAT3 pos = tileMap->GetTransform()->GetPosition();
		XMFLOAT3 sca = tileMap->GetTransform()->GetScale();
		float position[3]{ pos.x, pos.y, pos.z };
		float scale[3]{ sca.x, sca.y, sca.z };

		//Need height and width of tile map
		dataDestination[identifier].push_back({});
		dataDestination[identifier].at(count).push_back({ "Name", tileMap->GetIdentifier() });
		//dataToPack["TileMap"].at(count).push_back({ "Height",  });	

		if (dataToPack->ContainsType(GameObjectType::COLLIDABLE))
		{
			CollidableGameObject* collideObject = dynamic_cast<CollidableGameObject*>(tileMap);
			Box* testBox = dynamic_cast<Box*>(collideObject->GetShape());
			if (testBox == NULL)
			{
				Circle* c = dynamic_cast<Circle*>(collideObject->GetShape());
				if (c != NULL)
				{
					dataDestination["TileMap"].at(count).push_back({ "ColliderType" , "CircleCollider" });
					dataDestination["TileMap"].at(count).push_back({ "CircleRadius" , c->m_radius });
				}
			}
			else
			{
				dataDestination["TileMap"].at(count).push_back({ "ColliderType" , "BoxCollider" });
			}
			if (collideObject->GetShape() != nullptr)
			{
				dataDestination["TileMap"].at(count).push_back({ "IsCollideable" , collideObject->GetShape()->IsCollidable() });
				dataDestination["TileMap"].at(count).push_back({ "IsTrigger" , collideObject->GetShape()->IsTrigger() });
			}
			else
			{
				dataDestination["TileMap"].at(count).push_back({ "IsCollideable" , false });
				dataDestination["TileMap"].at(count).push_back({ "IsTrigger" , false });
			}
		}
		dataDestination[identifier].at(count).push_back({ "Position", position });
		dataDestination[identifier].at(count).push_back({ "Scale", scale });
		dataDestination[identifier].at(count).push_back({ "VertexShaderLocation" , shaders[0] });
		dataDestination[identifier].at(count).push_back({ "PixelShaderLocation" , shaders[1] });
		dataDestination[identifier].at(count).push_back({ "AlbedoLocation" , shaders[2] });
		dataDestination[identifier].at(count).push_back({ "AnimationCount", 0 });
		dataDestination[identifier].at(count).push_back({ "TileData", "RandomData" });
	}
	return dataDestination;
}

