#include "CharacterGameObject.h"

CharacterGameObject::CharacterGameObject(string identifier) : GameObject(identifier)
{

}

CharacterGameObject::~CharacterGameObject()
{

}

void CharacterGameObject::MoveX(float x)
{
	XMFLOAT3 pos = m_transform.GetPosition();
	pos.x += x;
	m_transform.SetPosition(pos);
}

void CharacterGameObject::MoveY(float y)
{
	XMFLOAT3 pos = m_transform.GetPosition();
	pos.y += y;
	m_transform.SetPosition(pos);
}