#include "PlayerGameObject.h"

PlayerGameObject::PlayerGameObject(string identifier) : CharacterGameObject(identifier)
{
	m_gameObjectType |= GameObjectType::PLAYER;
}

PlayerGameObject::~PlayerGameObject()
{

}