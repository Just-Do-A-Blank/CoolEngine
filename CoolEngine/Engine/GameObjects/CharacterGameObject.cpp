#include "CharacterGameObject.h"
#include "Engine/Managers/GameManager.h"

CharacterGameObject::CharacterGameObject() : TriggerableGameObject()
{
	m_gameObjectType |= GameObjectType::CHARACTER;
}

CharacterGameObject::CharacterGameObject(string identifier, CoolUUID uuid) : TriggerableGameObject(identifier, uuid)
{
	m_gameObjectType |= GameObjectType::CHARACTER;
}

CharacterGameObject::CharacterGameObject(const nlohmann::json& data, CoolUUID uuid) : TriggerableGameObject(data, uuid)
{
	m_gameObjectType |= GameObjectType::CHARACTER;

	m_moveSpeed = data["Movement Speed"];
	m_health = data["Health"];
}

CharacterGameObject::CharacterGameObject(CharacterGameObject const& other) : TriggerableGameObject(other)
{
	m_moveSpeed = other.m_moveSpeed;
	m_health = other.m_health;
	m_direction = other.m_direction;
}

CharacterGameObject::~CharacterGameObject()
{

}

void CharacterGameObject::Update()
{

}

void CharacterGameObject::EditorUpdate()
{

}

void CharacterGameObject::TakeDamage(float damage)
{
	m_health -= damage;
	// To do - Kill at zero health
}

void CharacterGameObject::Serialize(nlohmann::json& jsonData)
{
	TriggerableGameObject::Serialize(jsonData);

	jsonData["Health"] = m_health;
	jsonData["Movement Speed"] = m_moveSpeed;
}