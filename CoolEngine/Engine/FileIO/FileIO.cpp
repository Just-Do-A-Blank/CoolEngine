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

std::vector<GameObject> FileIO::LoadMultipleGameObjects(const char* fileAddress)
{
	json jsonData = LoadJson(fileAddress);
	json objData = jsonData["ObjectData"];
	json metaData = jsonData["MetaData"];

	std::vector<GameObject> objects((int)metaData.at(0)["NumberOfObject"]);

	for (size_t i = 0; i < metaData.at(0)["NumberOfObject"]; i++)
	{
		objects[i] = LoadGameObject(objData, i);
	}
	return objects;
}

std::vector<GameObject> FileIO::LoadMultipleGameObjects(json m_json)
{
	json jsonData = m_json["ObjectData"];
	json metaData = m_json["MetaData"];

	std::vector<GameObject> objects((int)metaData.at(0)["NumberOfObject"]);

	for (size_t i = 0; i < metaData.at(0)["NumberOfObject"]; i++)
	{
		objects[i] = LoadGameObject(jsonData, i);
	}
	return objects;
}

std::vector<ParticleSystem*> FileIO::LoadMultipleParticles(json file)
{
	json particleSystemData = file["ParticleSystemData"];
	json metaData = file["MetaData"];
	json particleData = file["ParticleData"];

	std::vector<ParticleSystem*> objects((int)metaData.at(0)["NumberOfParticlesSystems"]);

	for (size_t i = 0; i < metaData.at(0)["NumberOfParticlesSystems"]; i++)
	{
		
		ParticleData p; // = LoadParticle(particleData, i);
		objects[i] = new ParticleSystem(particleSystemData.at(i)["Name"]);


		Transform t;
		t.SetPosition(XMFLOAT3(particleSystemData.at(i)["Position"][0], particleSystemData.at(i)["Position"][1], particleSystemData.at(i)["Position"][2]));
		t.SetRotation(XMFLOAT3(particleSystemData.at(i)["Rotation"][0], particleSystemData.at(i)["Rotation"][1], particleSystemData.at(i)["Rotation"][2]));
		t.SetScale(XMFLOAT3(particleSystemData.at(i)["Scale"][0], particleSystemData.at(i)["Scale"][1], particleSystemData.at(i)["Scale"][2]));

		objects[i]->Initialise(t , particleSystemData.at(i)["Life"], particleSystemData.at(i)["SystemType"], nullptr);
		objects[i]->AddParticle(p.m_Transform, p.m_Velocity, p.m_Acceleration,p.m_Life);
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

void FileIO::LoadScene(const char* fileAddress, GameManager* scene, ParticleManager* pManager)
{
	json j = FileIO::LoadJson(fileAddress);

	LoadMap(j, scene);
	std::vector<GameObject> gameObj = LoadMultipleGameObjects(j);

	std::string identifer;
	GameObject* gO;

	for (size_t i = 0; i < gameObj.size(); ++i)
	{
		identifer = gameObj[i].GetIdentifier();
		scene->CreateGameObject<GameObject>(identifer);
		gO = scene->GetGameObjectUsingIdentifier<GameObject>(identifer);
		*gO = gameObj[i];
	}

	if (pManager == nullptr)
	{
		return;
	}

	std::vector<ParticleSystem*> particles = LoadMultipleParticles(j);

	for (size_t i = 0; i < particles.size(); ++i)
	{
		m_Cache.m_ParticleSystems.push_back(particles[i]);
		pManager->AddSystem(particles[i]);
	}
}

void FileIO::LoadMap(json file, GameManager* scene)
{
	json tileFile = file["TileMap"];

	for (size_t i = 0; i < file["MetaData"].at(0)["NumberOfTileZones"]; ++i)
	{
		XMFLOAT3 position(tileFile.at(i)["Position"][0], tileFile.at(i)["Position"][1], tileFile.at(i)["Position"][2]);
		XMFLOAT3 scale(tileFile.at(i)["Scale"][0], tileFile.at(i)["Scale"][1], tileFile.at(i)["Scale"][2]);
		wstring location = ToWstring((std::string)tileFile.at(i)["TileData"]);
		TileMap* tileMap = new TileMap(location, position, scale, tileFile.at(i)["Name"]);
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
		LOG("NO VERTEX SHADER WAS SPECIFIED")
	}
	else
	{
		wstring location = ToWstring((std::string)data.at(objectCount)["VertexShaderLocation"]);
		gameObject.SetVertexShader(location);
		m_Cache.m_VertexCache[gameObject.GetVertexShader()] = location;
	}
	if (data.at(objectCount)["PixelShaderLocation"] == "NULL")
	{
		LOG("NO PIXEL SHADER WAS SPECIFIED")
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
	if (data.at(objectCount)["IsCollideable"])
	{
		collideable = true;
		if (data.at(objectCount)["IsTrigger"])
		{
			trigger = true;
		}

		if (data.at(objectCount)["ColliderType"] == "CircleCollider")
		{
			Circle* pCircle = new Circle();
			gameObject.SetShape(pCircle);
			pCircle->SetIsCollidable(true);
			pCircle->SetIsTrigger(trigger);
			pCircle->m_radius = data.at(objectCount)["CircleRadius"];
			pCircle->m_transform = gameObject.GetTransform();
		}
		else
		{
			Box* pbox = new Box(gameObject.GetTransform());
			gameObject.SetShape(pbox);
			pbox->SetIsCollidable(true);
			pbox->SetIsTrigger(trigger);
			pbox->m_transform = gameObject.GetTransform();
		}
	}

	std::vector<Frame> framesOfAnimation(data.at(objectCount)["NumberOfAnimations"]);

	for (size_t i = 0; i < framesOfAnimation.size(); ++i)
	{
		wstring animationFile = ToWstring((std::string)data.at(objectCount)["AnimationFile"][i]);
		GraphicsManager::GetInstance()->LoadAnimationFromFile(animationFile);
		std::string name = data.at(objectCount)["AnimationFile"][i] ;
		SpriteAnimation* sAnimation = &GraphicsManager::GetInstance()->GetAnimation(ToWstring(name));
		m_Cache.m_AnimationCache[sAnimation->GetFrames()] = name;
		gameObject.AddAnimation(name, *sAnimation);
	}
	return gameObject;
}

void FileIO::LoadSavefile(const char* fileAddress)
{
	///TODO
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

	unordered_map<std::string, GameObject*> gameObjects = scene->GetAllGameObjects();
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

	for (unordered_map<string, GameObject*>::iterator it = gameObjects.begin(); it != gameObjects.end(); ++it)
	{
		saveObject = true;
		GameObject* gameObjectToStore = it->second;
	
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

				jsonOutput["ObjectData"].at(count).push_back({ "Name", gameObjectToStore->GetIdentifier() });

				Box* testBox = dynamic_cast<Box*>(gameObjectToStore->GetShape());
				if (testBox == NULL)
				{
					Circle* c = dynamic_cast<Circle*>(gameObjectToStore->GetShape());
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
				jsonOutput["ObjectData"].at(count).push_back({ "IsRenderable" , gameObjectToStore->IsRenderable() });
				if (gameObjectToStore->GetShape() != nullptr)
				{
					jsonOutput["ObjectData"].at(count).push_back({ "IsCollideable" , gameObjectToStore->GetShape()->IsCollidable() });
					jsonOutput["ObjectData"].at(count).push_back({ "IsTrigger" , gameObjectToStore->GetShape()->IsTrigger() });
				}
				else
				{
					jsonOutput["ObjectData"].at(count).push_back({ "IsCollideable" , false });
					jsonOutput["ObjectData"].at(count).push_back({ "IsTrigger" , false });
				}
				jsonOutput["ObjectData"].at(count).push_back({ "Position", position });
				jsonOutput["ObjectData"].at(count).push_back({ "Rotation" , rotation });
				jsonOutput["ObjectData"].at(count).push_back({ "Scale" , scale });
				jsonOutput["ObjectData"].at(count).push_back({ "VertexShaderLocation" , shaders[0] });
				jsonOutput["ObjectData"].at(count).push_back({ "PixelShaderLocation" , shaders[1] });
				jsonOutput["ObjectData"].at(count).push_back({ "AlbedoLocation" , shaders[2] });
				jsonOutput["ObjectData"].at(count).push_back({ "NumberOfAnimations", gameObjectToStore->GetAnimations()->size() });

				std::vector<std::string> spriteAnimation(gameObjectToStore->GetAnimations()->size());
				int animationCount = 0;
				for (unordered_map<string, SpriteAnimation>::iterator it2 = gameObjectToStore->GetAnimations()->begin(); it2 != gameObjectToStore->GetAnimations()->end(); ++it2)
				{
					spriteAnimation[animationCount] = m_Cache.m_AnimationCache.at(it2->second.GetFrames());
					++animationCount;
				}
				jsonOutput["ObjectData"].at(count).push_back({ "AnimationFile",	spriteAnimation });
				++count;
			}
			else if (particle)
			{
				XMFLOAT3 pos = gameObjectToStore->GetTransform()->GetPosition();
				float position[3]{ pos.x, pos.y, pos.z };
				position[0] = pos.x;
				position[1] = pos.y;
				position[2] = pos.z;
				XMFLOAT3 rot = gameObjectToStore->GetTransform()->GetRotation();
				float rotation[3]{ rot.x, rot.y, rot.z };
				rotation[0] = rot.x;
				rotation[1] = rot.y;
				rotation[2] = rot.z;
				XMFLOAT3 scal = gameObjectToStore->GetTransform()->GetScale();
				float scale[3]{ scal.x, scal.y, scal.z };
				scale[0] = scal.x;
				scale[1] = scal.y;
				scale[2] = scal.z;

				XMFLOAT2 vel = particle->GetVelocity();
				float velocity[2]{ vel.x, vel.y };

				XMFLOAT2 accel = particle->GetAccel();
				float acceleration[2]{ accel.x, accel.y };

				jsonOutput["ParticleData"].push_back({});
				jsonOutput["ParticleData"].at(particleCount).push_back({ "Life", particle->GetLife() });
				jsonOutput["ParticleData"].at(particleCount).push_back({ "Position", position });
				jsonOutput["ParticleData"].at(particleCount).push_back({ "Rotation", rotation });
				jsonOutput["ParticleData"].at(particleCount).push_back({ "Scale", scale });
				jsonOutput["ParticleData"].at(particleCount).push_back({ "Velocity" , velocity });
				jsonOutput["ParticleData"].at(particleCount).push_back({ "Acceleration" , acceleration });
				++particleCount;
			}
			else if (particleSys)
			{
				std::string vSL = ToString(GrabVertexShaderName(particleSys->GetVertexShader()));
				std::string pSL = ToString(GrabPixelShaderName(particleSys->GetPixelShader()));
				std::string aSRV = ToString(GrabAlbedoName(particleSys->GetAlbedoSRV()));

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

				//Need height and width of tile map
				jsonOutput["ParticleSystemData"].push_back({});
				jsonOutput["ParticleSystemData"].at(particleSystemCount).push_back({ "Name", particleSys->GetIdentifier()});
				jsonOutput["ParticleSystemData"].at(particleSystemCount).push_back({ "Life", particleSys->GetLife() });
				jsonOutput["ParticleSystemData"].at(particleSystemCount).push_back({ "Position", position });
				jsonOutput["ParticleSystemData"].at(particleSystemCount).push_back({ "Rotation", rotation });
				jsonOutput["ParticleSystemData"].at(particleSystemCount).push_back({ "Scale", scale });
				jsonOutput["ParticleSystemData"].at(particleSystemCount).push_back({ "AlbedoLocation" , shaders[2] });
				jsonOutput["ParticleSystemData"].at(particleSystemCount).push_back({ "SystemType" ,  particleSys->GetType()});

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
				Box* testBox = dynamic_cast<Box*>(gameObjectToStore->GetShape());
				if (testBox == NULL)
				{
					jsonOutput["TileMap"].at(tileCount).push_back({ "ColliderType" , "CircleCollider" });
				}
				else
				{
					jsonOutput["TileMap"].at(tileCount).push_back({ "ColliderType" , "BoxCollider" });
				}
				jsonOutput["TileMap"].at(tileCount).push_back({ "IsRenderable" , tileMap->IsRenderable() });

				if (tileMap->GetShape() != nullptr)
				{
					jsonOutput["TileMap"].at(tileCount).push_back({ "IsCollideable" , tileMap->GetShape()->IsCollidable() });
					jsonOutput["TileMap"].at(tileCount).push_back({ "IsTrigger" , tileMap->GetShape()->IsTrigger() });
				}
				else
				{
					jsonOutput["ObjectData"].at(tileCount).push_back({ "IsCollideable" , false });
					jsonOutput["ObjectData"].at(tileCount).push_back({ "IsTrigger" , false });
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

