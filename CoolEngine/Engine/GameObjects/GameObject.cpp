#include "GameObject.h"

void GameObject::CreateRenderableGameObject()
{
	m_isRenderable = true;
}

void GameObject::CreateNonRenderableGameObject()
{
	m_isRenderable = false;
}

void GameObject::AddCollision()
{
	m_isCollidable = true;
}

void GameObject::AddTrigger()
{
	m_isTrigger = true;
}

GameObject::GameObject(string identifier)
{
	m_identifier = identifier;
}

const bool& GameObject::IsRenderable()
{
	return m_isRenderable;
}

const bool& GameObject::IsCollidable()
{
	return m_isCollidable;
}

const bool& GameObject::IsTrigger()
{
	return m_isTrigger;
}

const string& GameObject::GetIdentifier()
{
	return m_identifier;
}
