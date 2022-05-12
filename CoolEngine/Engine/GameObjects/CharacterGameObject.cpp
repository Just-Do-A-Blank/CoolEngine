#include "CharacterGameObject.h"
#include "Engine/Managers/GameManager.h"

CharacterGameObject::CharacterGameObject() : TriggerableGameObject()
{
}

CharacterGameObject::CharacterGameObject(string identifier, CoolUUID uuid) : TriggerableGameObject(identifier, uuid)
{
	m_gameObjectType |= GameObjectType::CHARACTER;
}

CharacterGameObject::CharacterGameObject(json data, CoolUUID index) : TriggerableGameObject(data, index)
{
	json j = data["Position"];
	json j2 = data["Rotation"];
	json j3 = data["Scale"];
	json j4 = data["Layer"];
	json j5 = data["Movement Speed"];
	json j6 = data["Health"];

	m_transform->SetPosition(XMFLOAT3(j[0], j[1], j[2]));
	m_transform->SetRotation(XMFLOAT3(j2[0], j2[1], j2[2]));
	m_transform->SetScale(XMFLOAT3(j3[0], j3[1], j3[2]));
	m_layer = j4;
	m_moveSpeed = j5;
	m_health = j6;
}

CharacterGameObject::~CharacterGameObject()
{

}

void CharacterGameObject::Update()
{

}

void CharacterGameObject::Serialize(json& jsonData)
{
	GameObject::Serialize(jsonData);
	TriggerableGameObject::Serialize(jsonData);
	jsonData["Health"] = m_health;
	jsonData["Movement Speed"] = m_moveSpeed;
}

void CharacterGameObject::Deserialize(json& jsonData)
{
}
