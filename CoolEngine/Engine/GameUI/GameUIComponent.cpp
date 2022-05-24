#include "GameUIComponent.h"
#include "Engine/ResourceDefines.h"
#include "Engine/GameObjects/Transform.h"
#include "Engine/Graphics/Mesh.h"
#include "Engine/GameObjects/RenderableGameObject.h"
#include "Engine/Managers/GraphicsManager.h"
#include "Engine/EditorUI/EditorUI.h"

GameUIComponent::GameUIComponent(string identifier, CoolUUID uuid):GameUIPrefab(identifier, uuid)
{
	m_gameObjectType |= GameObjectType::GAME_UI_COMPONENT;
	m_uiComponentType |= UIComponentType::BASE;

	m_transform = new Transform();
}

GameUIComponent::GameUIComponent(nlohmann::json& data, CoolUUID uuid) : GameUIPrefab(data, uuid)
{
	m_gameObjectType |= GameObjectType::GAME_UI_COMPONENT;
	m_uiComponentType |= UIComponentType::BASE;

	if (GameUIPrefab::IsPrefab())
	{
		LoadLocalData(GameUIPrefab::GetPrefabDataLoadedAtCreation());
	}
	else
	{
		LoadLocalData(data);
	}
}

GameUIComponent::GameUIComponent(GameUIComponent const& other) : GameUIPrefab(other)
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
	GameUIPrefab::Serialize(data);

	SaveLocalData(data);
}

void GameUIComponent::LoadLocalData(const nlohmann::json& jsonData)
{
	if (jsonData.contains("IsRendering"))
	{
		m_isRenderable = jsonData["IsRendering"];
		m_layer = jsonData["Layer"];

		m_texFilepath = L"";

		std::string tempTexPath = jsonData["TexturePath"];
		std::wstring wideTexPath = std::wstring(tempTexPath.begin(), tempTexPath.end());

		SetTexture(wideTexPath);
	}
}

void GameUIComponent::SaveLocalData(nlohmann::json& jsonData)
{
	std::string tempPath = std::string(m_texFilepath.begin(), m_texFilepath.end());
	jsonData["TexturePath"] = tempPath;
	jsonData["Layer"] = m_layer;
	jsonData["IsRendering"] = m_isRenderable;
	jsonData["UIType"] = (int)m_uiComponentType;
}

void GameUIComponent::LoadAllPrefabData(const nlohmann::json& jsonData)
{
	LoadLocalData(jsonData);
	GameUIPrefab::LoadAllPrefabData(jsonData);
}

void GameUIComponent::SaveAllPrefabData(nlohmann::json& jsonData)
{
	SaveLocalData(jsonData);
	GameUIPrefab::SaveAllPrefabData(jsonData);
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
	GameUIPrefab::CreateEngineUI();
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
