#include "GameObject.h"

#include "Engine/EditorUI/EditorUI.h"

GameObject::GameObject()
{
	m_transform = new Transform();

	m_gameObjectType |= GameObjectType::BASE;
}

GameObject::GameObject(string identifier)
{
	m_identifier = identifier;
	m_transform = new Transform();

	m_gameObjectType = GameObjectType::BASE;
}

void GameObject::Update()
{

}

#if EDITOR
void GameObject::ShowEngineUI()
{
	ImGui::Begin("Properties");

	CreateEngineUI();

	ImGui::End();
}

void GameObject::CreateEngineUI()
{
	ImGui::Separator();
	ImGui::Spacing();

	m_transform->CreateEngineUI();
	
	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();


}
#endif

Transform* GameObject::GetTransform()
{
	return m_transform;
}

const GameObjectType& GameObject::GetGameObjectType() const
{
	return m_gameObjectType;
}

const string& GameObject::GetIdentifier()
{
	return m_identifier;
}

bool GameObject::ContainsType(GameObjectType type)
{
	return (m_gameObjectType & type) == type;
}


