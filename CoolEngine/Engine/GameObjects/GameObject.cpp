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

GameObject::GameObject(json data, CoolUUID index)
{
	json j2 = data["Name"];
	json pos = data["Position"];
	json rot = data["Rotation"];
	json sca = data["Scale"];

	m_transform = new Transform();
	m_transform->SetPosition(XMFLOAT3(pos[0], pos[1], pos[2]));
	m_transform->SetPosition(XMFLOAT3(rot[0], rot[1], rot[2]));
	m_transform->SetPosition(XMFLOAT3(sca[0], sca[1], sca[2]));

	m_UUID = CoolUUID(index);
	m_identifier = j2;
	m_gameObjectType |= GameObjectType::BASE;
}

GameObject::~GameObject()
{
	delete m_transform;
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

void GameObject::Serialize(json& jsonData)
{
	jsonData["Name"] = m_identifier;
	float position[3] = { m_transform->GetPosition().x ,m_transform->GetPosition().y ,m_transform->GetPosition().z };
	float rotation[3] = { m_transform->GetRotation().x ,m_transform->GetRotation().y ,m_transform->GetRotation().z };
	float scale[3] = { m_transform->GetScale().x ,m_transform->GetScale().y ,m_transform->GetScale().z };
	jsonData["Position"] = position;
	jsonData["Rotation"] = rotation;
	jsonData["Scale"] = scale;
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


