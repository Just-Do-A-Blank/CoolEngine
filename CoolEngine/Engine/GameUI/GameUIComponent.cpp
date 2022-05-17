#include "GameUIComponent.h"
#include "Engine/ResourceDefines.h"
#include "Engine/GameObjects/Transform.h"
#include "Engine/Graphics/Mesh.h"
#include "Engine/GameObjects/RenderableGameObject.h"
#include "Engine/Managers/GraphicsManager.h"
#include "Engine/EditorUI/EditorUI.h"

GameUIComponent::GameUIComponent(string identifier, CoolUUID uuid):GameObject(identifier, uuid)
{
	m_gameObjectType |= GameObjectType::GAME_UI_COMPONENT;
	m_uiComponentType |= UIComponentType::BASE;

	m_transform = new Transform();
}

GameUIComponent::GameUIComponent(nlohmann::json& data, CoolUUID uuid)
{
	m_gameObjectType |= GameObjectType::GAME_UI_COMPONENT;
	m_uiComponentType |= UIComponentType::BASE;

	m_UUID = CoolUUID(*uuid);
	std::string uuidString = to_string(*uuid);

	SetIdentifier(data["GameUI"][(int)m_uiComponentType][uuidString]["Identifier"]);
	m_transform = new Transform();

	m_transform->SetLocalPosition(XMFLOAT3(data["GameUI"][(int)m_uiComponentType][uuidString]["Position"][0], data["GameUI"][(int)m_uiComponentType][uuidString]["Position"][1], data["GameUI"][(int)m_uiComponentType][uuidString]["Position"][2]));
	m_transform->SetLocalRotation(XMFLOAT3(data["GameUI"][(int)m_uiComponentType][uuidString]["rotation"][0], data["GameUI"][(int)m_uiComponentType][uuidString]["rotation"][1], data["GameUI"][(int)m_uiComponentType][uuidString]["rotation"][2]));
	m_transform->SetLocalScale(XMFLOAT3(data["GameUI"][(int)m_uiComponentType][uuidString]["scale"][0], data["GameUI"][(int)m_uiComponentType][uuidString]["scale"][1], data["GameUI"][(int)m_uiComponentType][uuidString]["scale"][2]));

	m_isRenderable = data["GameUI"][(int)m_uiComponentType][uuidString]["IsRendering"];
	m_layer = data["GameUI"][(int)m_uiComponentType][uuidString]["Layer"];

	SetTexture(data["GameUI"][(int)m_uiComponentType][uuidString]["TexturePath"]);
}

GameUIComponent::GameUIComponent(GameUIComponent const& other) : GameObject(other)
{
	m_isRenderable = other.m_isRenderable;
	m_ptexture = other.m_ptexture;

	m_uiComponentType = other.m_uiComponentType;
	m_layer = other.m_layer;
}

void GameUIComponent::Render(RenderStruct& renderStruct)
{
	if (m_ptexture == nullptr)
	{
		return;
	}
	GraphicsManager::GetInstance()->RenderQuad(m_ptexture, m_transform->GetWorldPosition(), m_transform->GetWorldScale(), m_transform->GetWorldRotation().z, m_layer);
}

void GameUIComponent::Update()
{
}

void GameUIComponent::EditorUpdate()
{
}

void GameUIComponent::Serialize(nlohmann::json& data)
{
	std::string uuidString = to_string(*m_UUID);

	data["GameUI"][(int)m_uiComponentType][uuidString]["Identifier"].push_back(m_identifier);
	data["GameUI"][(int)m_uiComponentType][uuidString]["Position"].push_back({ m_transform->GetLocalPosition().x, m_transform->GetLocalPosition().y, m_transform->GetLocalPosition().z });
	data["GameUI"][(int)m_uiComponentType][uuidString]["Rotation"].push_back({ m_transform->GetLocalRotation().x, m_transform->GetLocalRotation().y, m_transform->GetLocalRotation().z });
	data["GameUI"][(int)m_uiComponentType][uuidString]["Scale"].push_back({ m_transform->GetLocalScale().x, m_transform->GetLocalScale().y, m_transform->GetLocalScale().z });
	data["GameUI"][(int)m_uiComponentType][uuidString]["TexturePath"].push_back(m_texFilepath);
	data["GameUI"][(int)m_uiComponentType][uuidString]["Layer"].push_back(m_layer);
	data["GameUI"][(int)m_uiComponentType][uuidString]["IsRendering"].push_back(m_isRenderable);
}

void GameUIComponent::SetIsRenderable(bool& condition)
{
	m_isRenderable = condition;
}

void GameUIComponent::SetLayer(const int& layer)
{
	m_layer = layer;
}

void GameUIComponent::SetTexture(std::wstring wsfilepath)
{
	m_ptexture = GraphicsManager::GetInstance()->GetShaderResourceView(wsfilepath);

	if (m_ptexture == nullptr)
	{
		LOG("Tried to get a texture that doesn't exist!");
	}
	else
	{
		m_texFilepath = wsfilepath;
	}
}

#if EDITOR
void GameUIComponent::ShowEngineUI()
{
	ImGui::Begin("Properties");

	EditorUI::IdentifierText("Identifier", m_identifier);

	ImGui::Spacing();
	ImGui::Separator();

	CreateEngineUI();

	ImGui::End();
}
void GameUIComponent::CreateEngineUI()
{
	GameObject::CreateEngineUI();
}
UIComponentType GameUIComponent::GetUIComponentType() const
{
	return m_uiComponentType;
}
#endif

int& GameUIComponent::GetLayer()
{
	return m_layer;
}

const bool& GameUIComponent::IsRenderable()
{
	return m_isRenderable;
}

Transform* GameUIComponent::GetTransform()
{
	return m_transform;
}

const UIComponentType& GameUIComponent::GetComponentType() const
{
	return m_uiComponentType;
}

bool GameUIComponent::ContainsType(UIComponentType type)
{
	return (m_uiComponentType & type) == type;
}
