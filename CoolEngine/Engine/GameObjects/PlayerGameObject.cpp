#include "PlayerGameObject.h"

PlayerGameObject::PlayerGameObject(string identifier, CoolUUID uuid) : CharacterGameObject(identifier, uuid)
{
	m_gameObjectType |= GameObjectType::PLAYER;
}

PlayerGameObject::~PlayerGameObject()
{

}