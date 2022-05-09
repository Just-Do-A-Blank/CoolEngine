#include "PlayerGameObject.h"

PlayerGameObject::PlayerGameObject(string identifier) : CharacterGameObject(identifier)
{
	m_gameObjectType |= GameObjectType::PLAYER;
}

PlayerGameObject::~PlayerGameObject()
{

}

void PlayerGameObject::Serialize(json& jsonData)
{
	//int gUID = ;
	std::string gUID = "GUID: ";

	std::string name = "Name: ";
	//name.append();


	float position[3] = { m_transform->GetPosition().x ,m_transform->GetPosition().y ,m_transform->GetPosition().z };
	float rotation[3] = { m_transform->GetRotation().x ,m_transform->GetRotation().y ,m_transform->GetRotation().z };
	float scale[3] = { m_transform->GetScale().x ,m_transform->GetScale().y ,m_transform->GetScale().z };


	jsonData["PlayerGameObject"]["GUID"].push_back(gUID);
	jsonData["PlayerGameObject"]["Name"].push_back(name);
	jsonData["PlayerGameObject"]["Position"].push_back(position);
	jsonData["PlayerGameObject"]["Rotation"].push_back(rotation);
	jsonData["PlayerGameObject"]["Scale"].push_back(scale);
	jsonData["PlayerGameObject"]["Layers"].push_back(m_health);
	jsonData["PlayerGameObject"]["Health"].push_back(m_layer);
	jsonData["PlayerGameObject"]["Movement Speed"].push_back(m_moveSpeed);

}

void PlayerGameObject::Deserialize(json& jsonData)
{
}
