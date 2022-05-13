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
	m_gameObjectType |= GameObjectType::CHARACTER;
	json j5 = data["Movement Speed"];
	json j6 = data["Health"];

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
