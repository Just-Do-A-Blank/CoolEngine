#include "CharacterGameObject.h"

CharacterGameObject::CharacterGameObject(string identifier) : RenderableCollidableGameObject(identifier)
{
	m_gameObjectType |= GameObjectType::CHARACTER;
}

CharacterGameObject::~CharacterGameObject()
{

}