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

std::vector<GameObjectData> FileIO::LoadMultipleGameObjects(const char* fileAddress)
{
	json m_json = LoadJson(fileAddress);
	json data = m_json["ObjectData"];
	json metaData = m_json["MetaData"];

	std::vector<GameObjectData> objects(metaData.at(0)["NumberOfObject"]);

	for (size_t i = 0; i < metaData.at(0)["NumberOfObject"]; i++)
	{
		objects[i].m_Position = XMFLOAT3(data.at(0)["Position"][0], data.at(0)["Position"][1], data.at(0)["Position"][2]);
		objects[i].m_Rotation = XMFLOAT3(data.at(0)["Rotation"][0], data.at(0)["Rotation"][1], data.at(0)["Rotation"][2]);
		objects[i].m_Scale = XMFLOAT3(data.at(0)["Scale"][0], data.at(0)["Scale"][1], data.at(0)["Scale"][2]);

		if (data.at(0)["IsRenderable"])
		{
			objects[i].m_TextureData = nullptr;
			objects[i].m_IsRenderable = true;
		}
		else
		{
			objects[i].m_TextureData = nullptr;
			objects[i].m_IsRenderable = false;
		}
		if (data.at(0)["IsTrigger"])
		{
			objects[i].m_IsTrigger = true;
			objects[i].m_ColliderType = ColliderType(data.at(0)["ColliderType"]);
		}
		else
		{
			objects[i].m_IsTrigger = false;
		}
		if (data.at(0)["IsCollideable"])
		{
			objects[i].m_IsCollidable = true;
			objects[i].m_ColliderType = ColliderType(data.at(0)["ColliderType"]);
		}
		else
		{
			objects[i].m_IsCollidable = false;
		}
	}
}

GameObjectData FileIO::LoadGameObject(const char* fileAddress)
{
	json m_json = LoadJson(fileAddress);
	json data = m_json["ObjectData"];

	GameObjectData gameObjData;
	gameObjData.m_Position = XMFLOAT3(data.at(0)["Position"][0], data.at(0)["Position"][1], data.at(0)["Position"][2]);
	gameObjData.m_Rotation = XMFLOAT3(data.at(0)["Rotation"][0], data.at(0)["Rotation"][1], data.at(0)["Rotation"][2]) ;
	gameObjData.m_Scale = XMFLOAT3(data.at(0)["Scale"][0], data.at(0)["Scale"][1], data.at(0)["Scale"][2]);

	if (data.at(0)["IsRenderable"])
	{
		gameObjData.m_TextureData = nullptr;
		gameObjData.m_IsRenderable = true;
	}
	else
	{
		gameObjData.m_TextureData = nullptr;
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
