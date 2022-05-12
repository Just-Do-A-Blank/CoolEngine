#include "CharacterGameObject.h"
#include "Engine/Managers/GameManager.h"

CharacterGameObject::CharacterGameObject() : TriggerableGameObject()
{
}

CharacterGameObject::CharacterGameObject(string identifier, CoolUUID uuid) : TriggerableGameObject(identifier, uuid)
{
	m_gameObjectType |= GameObjectType::CHARACTER;
}

CharacterGameObject::CharacterGameObject(json data, int index) : TriggerableGameObject(data, index)
{
	json j = data["Position"];
	json j2 = data["Rotation"];
	json j3 = data["Scale"];
	json j4 = data["Layer"];
	json j5 = data["Movement Speed"];
	json j6 = data["Health"];

	m_transform->SetPosition(XMFLOAT3(j[index][0], j[index][1], j[index][2]));
	m_transform->SetRotation(XMFLOAT3(j2[index][0], j2[index][1], j2[index][2]));
	m_transform->SetScale(XMFLOAT3(j3[index][0], j3[index][1], j3[index][2]));
	m_layer = j4[index];
	m_moveSpeed = j5[index];
	m_health = j6[index];
}

CharacterGameObject::~CharacterGameObject()
{

}

void CharacterGameObject::Update()
{

}

void CharacterGameObject::Serialize(json& jsonData)
{



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
