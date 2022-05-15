#include "GameObject.h"

#include "Engine/EditorUI/EditorUI.h"

void GameObject::SetIdentifier(std::string identifier)
{
	m_identifier = identifier;
}

void GameObject::SetUUID(CoolUUID uuid)
{
	m_UUID = CoolUUID(*uuid);
}

GameObject::GameObject()
{
	m_transform = new Transform();

	m_gameObjectType |= GameObjectType::BASE;
}

GameObject::GameObject(GameObject const &other)
{
	m_identifier = other.m_identifier;
	m_UUID = other.m_UUID;
	m_transform = new Transform(*other.m_transform);

	m_pTest = other.m_pTest;

	m_gameObjectType = other.m_gameObjectType;

}

GameObject::GameObject(string identifier, CoolUUID uuid)
{
	m_identifier = identifier;
	m_transform = new Transform();

	m_UUID = uuid;

	m_gameObjectType = GameObjectType::BASE;
}

GameObject::~GameObject()
{
}

void GameObject::Update()
{
	if (m_pTest != nullptr)
	{
		LOG(m_pTest->GetIdentifier());
	}
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

string& GameObject::GetIdentifier()
{
	return m_identifier;
}

const CoolUUID& GameObject::GetUUID()
{
	return m_UUID;
}

bool GameObject::ContainsType(GameObjectType type)
{
	return (m_gameObjectType & type) == type;
}


