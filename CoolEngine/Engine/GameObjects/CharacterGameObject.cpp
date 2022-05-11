#include "CharacterGameObject.h"
#include "Engine/Managers/GameManager.h"

CharacterGameObject::CharacterGameObject() : TriggerableGameObject()
{
}

CharacterGameObject::CharacterGameObject(string identifier, CoolUUID uuid) : TriggerableGameObject(identifier, uuid)
{
	m_gameObjectType |= GameObjectType::CHARACTER;
}

CharacterGameObject::CharacterGameObject(json data, int index) : RenderableCollidableGameObject(data, index)
{
	m_gameObjectType |= GameObjectType::CHARACTER;
	m_transform->SetPosition(XMFLOAT3(data[index]["Position"][0], data[index]["Position"][1], data[index]["Position"][2]));
	m_transform->SetRotation(XMFLOAT3(data[index]["Rotation"][0], data[index]["Rotation"][1], data[index]["Rotation"][2]));
	m_transform->SetScale(XMFLOAT3(data[index]["Scale"][0], data[index]["Scale"][1], data[index]["Scale"][2]));
	m_layer = data[index]["Layers"];
	m_health = data[index]["Health"];
	m_moveSpeed = data[index]["Movement Speed"];
	
}

CharacterGameObject::~CharacterGameObject()
{

}

void CharacterGameObject::Update()
{

}

void CharacterGameObject::Serialize(json& jsonData)
{

	//int gUID = ;
	std::string gUID = "GUID: ";

	std::string name = "Name: ";
	//name.append();


	float position[3] = { m_transform->GetPosition().x ,m_transform->GetPosition().y ,m_transform->GetPosition().z };
	float rotation[3] = { m_transform->GetRotation().x ,m_transform->GetRotation().y ,m_transform->GetRotation().z };
	float scale[3] = { m_transform->GetScale().x ,m_transform->GetScale().y ,m_transform->GetScale().z };

	GameObject::Serialize(jsonData);
	jsonData[std::to_string((int)m_gameObjectType)]["Position"].push_back(position);
	jsonData[std::to_string((int)m_gameObjectType)]["Rotation"].push_back(rotation);
	jsonData[std::to_string((int)m_gameObjectType)]["Scale"].push_back(scale);
	jsonData[std::to_string((int)m_gameObjectType)]["Layers"].push_back(m_layer);
	jsonData[std::to_string((int)m_gameObjectType)]["Health"].push_back(m_health);
	jsonData[std::to_string((int)m_gameObjectType)]["Movement Speed"].push_back(m_moveSpeed);
}

void CharacterGameObject::Deserialize(json& jsonData)
{
}
