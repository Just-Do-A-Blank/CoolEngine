#include "CharacterGameObject.h"
#include "Engine/Managers/GameManager.h"

CharacterGameObject::CharacterGameObject() : TriggerableGameObject()
{
}

CharacterGameObject::CharacterGameObject(string identifier, CoolUUID uuid) : TriggerableGameObject(identifier, uuid)
{
	m_gameObjectType |= GameObjectType::CHARACTER;
}

CharacterGameObject::~CharacterGameObject()
{

}

void CharacterGameObject::Update()
{

}
