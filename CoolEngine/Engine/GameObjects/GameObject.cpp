#include "GameObject.h"

#include "Engine/EditorUI/EditorUI.h"

GameObject::GameObject()
{
	m_transform = new Transform();

	m_gameObjectType |= GameObjectType::BASE;
}

GameObject::GameObject(string identifier, CoolUUID uuid)
{
	m_identifier = identifier;
	m_transform = new Transform();

	m_UUID = uuid;

	m_gameObjectType = GameObjectType::BASE;
}

void GameObject::Update()
{
	if (m_pTest != nullptr)
	{
		LOG(m_pTest->GetIdentifier());
	}
}

void GameObject::Serialize(json& jsonData)
{
}

void GameObject::Deserialize(json& jsonData)
{
}

#if EDITOR
void GameObject::ShowEngineUI()
{
	ImGui::Begin("Properties");

	EditorUI::IdentifierText("Identifier", m_identifier);

	ImGui::Spacing();
	ImGui::Separator();

	CreateEngineUI();

	ImGui::End();
}

void GameObject::CreateEngineUI()
{
	ImGui::Spacing();

	m_transform->CreateEngineUI();
	
	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

	EditorUI::ReferenceField("Reference test", m_pTest);
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


