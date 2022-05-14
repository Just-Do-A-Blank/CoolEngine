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

GameObject::GameObject(string identifier, CoolUUID uuid)
{
	m_identifier = identifier;
	m_transform = new Transform();

	m_UUID = uuid;

	m_gameObjectType = GameObjectType::BASE;
}

GameObject::GameObject(const json& data, CoolUUID uuid)
{
	m_transform = new Transform();
	m_transform->SetLocalPosition(XMFLOAT3(data["Position"][0], data["Position"][1], data["Position"][2]));
	m_transform->SetLocalRotation(XMFLOAT3(data["Rotation"][0], data["Rotation"][1], data["Rotation"][2]));
	m_transform->SetLocalScale(XMFLOAT3(data["Scale"][0], data["Scale"][1], data["Scale"][2]));

	m_UUID = CoolUUID(*uuid);
	m_identifier = data["Name"];
	m_gameObjectType |= GameObjectType::BASE;
}

GameObject::~GameObject()
{
	delete m_transform;
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
	jsonData["Name"] = m_identifier;
	float position[3] = { m_transform->GetLocalPosition().x ,m_transform->GetLocalPosition().y ,m_transform->GetLocalPosition().z };
	float rotation[3] = { m_transform->GetLocalRotation().x ,m_transform->GetLocalRotation().y ,m_transform->GetLocalRotation().z };
	float scale[3] = { m_transform->GetLocalScale().x ,m_transform->GetLocalScale().y ,m_transform->GetLocalScale().z };
	jsonData["Position"] = position;
	jsonData["Rotation"] = rotation;
	jsonData["Scale"] = scale;
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


