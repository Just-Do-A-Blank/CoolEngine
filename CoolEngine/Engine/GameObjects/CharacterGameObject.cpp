#include "CharacterGameObject.h"

CharacterGameObject::CharacterGameObject(string identifier) : GameObject(identifier)
{

}

CharacterGameObject::~CharacterGameObject()
{

}

void CharacterGameObject::Translate(XMFLOAT2 vector)
{
	XMFLOAT3 pos = m_transform.GetPosition();
	pos.x += vector.x;
	pos.y += vector.y;
	m_transform.SetPosition(pos);
}