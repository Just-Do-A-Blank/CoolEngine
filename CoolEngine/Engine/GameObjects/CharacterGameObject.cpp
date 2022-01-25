#include "CharacterGameObject.h"
#include "Engine/Managers/GameManager.h"

CharacterGameObject::CharacterGameObject(string identifier) : RenderableCollidableGameObject(identifier)
{
	m_gameObjectType |= GameObjectType::CHARACTER;
}

CharacterGameObject::~CharacterGameObject()
{

}

void CharacterGameObject::Update()
{

}