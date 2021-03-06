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
	m_isEnabled = true;
	m_gameObjectType |= GameObjectType::BASE;
}

GameObject::GameObject(GameObject const &other)
{
	m_identifier = other.m_identifier;
	m_UUID = other.m_UUID;
	m_transform = new Transform(*other.m_transform);

	m_pTest = other.m_pTest;
	m_isEnabled = other.m_isEnabled;

	m_gameObjectType = other.m_gameObjectType;
}

GameObject::GameObject(string identifier, CoolUUID uuid)
{
	m_identifier = identifier;
	m_transform = new Transform();

	m_UUID = uuid;
	m_isEnabled = true;
	m_gameObjectType = GameObjectType::BASE;
}

GameObject::GameObject(const nlohmann::json& data, CoolUUID uuid)
{
	m_transform = new Transform();
	m_transform->SetLocalPosition(XMFLOAT3(data["Position"][0], data["Position"][1], data["Position"][2]));
	m_transform->SetLocalRotation(XMFLOAT3(data["Rotation"][0], data["Rotation"][1], data["Rotation"][2]));
	m_transform->SetLocalScale(XMFLOAT3(data["Scale"][0], data["Scale"][1], data["Scale"][2]));

	if (data.count("isEnabled"))
	{
		m_isEnabled = data["isEnabled"];

	}
	else
	{
		m_isEnabled = true;
	}



	m_UUID = CoolUUID(*uuid);
	m_identifier = data["Name"];

	m_gameObjectType = GameObjectType::BASE;
}

GameObject::~GameObject()
{
	delete m_transform;
}

void GameObject::Start()
{
    if (m_pTest != nullptr)
    {
        LOG(m_pTest->GetIdentifier());
    }
}

void GameObject::Update()
{
	if (m_pTest != nullptr)
	{
		LOG(m_pTest->GetIdentifier());
	}
}

void GameObject::EditorUpdate()
{
	if (m_pTest != nullptr)
	{
		LOG(m_pTest->GetIdentifier());
	}
}

void GameObject::Serialize(nlohmann::json& jsonData)
{
	jsonData["Name"] = m_identifier;
	float position[3] = { m_transform->GetLocalPosition().x ,m_transform->GetLocalPosition().y ,m_transform->GetLocalPosition().z };
	float rotation[3] = { m_transform->GetLocalRotation().x ,m_transform->GetLocalRotation().y ,m_transform->GetLocalRotation().z };
	float scale[3] = { m_transform->GetLocalScale().x ,m_transform->GetLocalScale().y ,m_transform->GetLocalScale().z };
	jsonData["Position"] = position;
	jsonData["Rotation"] = rotation;
	jsonData["Scale"] = scale;
	jsonData["isEnabled"] = m_isEnabled;
}

void GameObject::Init(const nlohmann::json& data, CoolUUID uuid)
{
	m_transform->SetLocalPosition(XMFLOAT3(data["Position"][0], data["Position"][1], data["Position"][2]));
	m_transform->SetLocalRotation(XMFLOAT3(data["Rotation"][0], data["Rotation"][1], data["Rotation"][2]));
	m_transform->SetLocalScale(XMFLOAT3(data["Scale"][0], data["Scale"][1], data["Scale"][2]));

	if (data.count("isEnabled"))
	{
		m_isEnabled = data["isEnabled"];

	}
	else
	{
		m_isEnabled = true;
	}

	m_UUID = CoolUUID(*uuid);
	m_identifier = data["Name"];


}

void GameObject::Init(GameObject const& other)
{
	m_identifier = other.m_identifier;
	m_UUID = other.m_UUID;
	*m_transform = *other.m_transform;

	m_pTest = other.m_pTest;
	m_isEnabled = other.m_isEnabled;

	m_gameObjectType = other.m_gameObjectType;
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

	if (EditorUI::CollapsingSection("Game Object Options", true))
	{

		m_transform->CreateEngineUI();

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		EditorUI::ReferenceField("Reference test", m_pTest);

		ImGui::Spacing();
		EditorUI::Checkbox("Enable", m_isEnabled);

	}
}
#endif

Transform* GameObject::GetTransform() const
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


