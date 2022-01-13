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

std::vector<GameObjectData> FileIO::LoadMultipleGameObjects(const char* fileAddress)
{
	json m_json = LoadJson(fileAddress);
	json data = m_json["ObjectData"];
	json metaData = m_json["MetaData"];

	std::vector<GameObjectData> objects(metaData.at(0)["NumberOfObject"]);

	for (size_t i = 0; i < metaData.at(0)["NumberOfObject"]; i++)
	{
		//GameObject gameObject(data.at(i)["Name"]);
		objects[i].m_Name = data.at(i)["Name"];
		objects[i].m_Position = XMFLOAT3(data.at(i)["Position"][0], data.at(i)["Position"][1], data.at(i)["Position"][2]);
		objects[i].m_Rotation = XMFLOAT3(data.at(i)["Rotation"][0], data.at(i)["Rotation"][1], data.at(i)["Rotation"][2]);
		objects[i].m_Scale = XMFLOAT3(data.at(i)["Scale"][0], data.at(i)["Scale"][1], data.at(i)["Scale"][2]);

		if (data.at(0)["IsRenderable"])
		{
			
			objects[i].m_IsRenderable = true;
		}
		else
		{
			objects[i].m_TextureLocation = data.at(i)["TextureLocation"];
			objects[i].m_IsRenderable = false;
		}
		if (data.at(0)["IsTrigger"])
		{
			objects[i].m_IsTrigger = true;
			objects[i].m_ColliderType = ColliderType(data.at(i)["ColliderType"]);
		}
		else
		{
			objects[i].m_IsTrigger = false;
		}
		if (data.at(0)["IsCollideable"])
		{
			objects[i].m_IsCollidable = true;
			objects[i].m_ColliderType = ColliderType(data.at(i)["ColliderType"]);
		}
		else
		{
			objects[i].m_IsCollidable = false;
		}
	}
	return objects;
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

GameObjectData FileIO::LoadGameObject(const char* fileAddress)
{
	json m_json = LoadJson(fileAddress);
	json data = m_json["ObjectData"];

	//GameObject gameObject(data.at(0)["Name"]);

	GameObjectData gameObjData;
	gameObjData.m_Name = data.at(0)["Name"];
	gameObjData.m_Position = XMFLOAT3(data.at(0)["Position"][0], data.at(0)["Position"][1], data.at(0)["Position"][2]);
	gameObjData.m_Rotation = XMFLOAT3(data.at(0)["Rotation"][0], data.at(0)["Rotation"][1], data.at(0)["Rotation"][2]) ;
	gameObjData.m_Scale = XMFLOAT3(data.at(0)["Scale"][0], data.at(0)["Scale"][1], data.at(0)["Scale"][2]);

	//gameObject.GetTransform()->SetPosition(XMFLOAT3(data.at(0)["Position"][0], data.at(0)["Position"][1], data.at(0)["Position"][2]));
	//gameObject.GetTransform()->SetRotation(XMFLOAT3(data.at(0)["Position"][0], data.at(0)["Position"][1], data.at(0)["Position"][2]));
	//gameObject.GetTransform()->SetScale(XMFLOAT3(data.at(0)["Position"][0], data.at(0)["Position"][1], data.at(0)["Position"][2]));

	if (data.at(0)["IsRenderable"])
	{
		gameObjData.m_TextureLocation = data.at(0)["TextureLocation"];
		gameObjData.m_IsRenderable = true;
	}
	else
	{
		gameObjData.m_IsRenderable = false;
	}
	if (data.at(0)["IsTrigger"])
	{
		gameObjData.m_IsTrigger = true;
		gameObjData.m_ColliderType = ColliderType(data.at(0)["ColliderType"]);
	}
	else
	{
		gameObjData.m_IsTrigger = false;
	}
	if (data.at(0)["IsCollideable"])
	{
		gameObjData.m_IsCollidable = true;
		gameObjData.m_ColliderType = ColliderType(data.at(0)["ColliderType"]);
	}
	else
	{
		gameObjData.m_IsCollidable = false;
	}
	return gameObjData;
}

void FileIO::LoadMap(const char* fileAddress)
{

}

void FileIO::LoadSavefile(const char* fileAddress)
{

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
	int i;
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

