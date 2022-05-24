#include "GameUIComponent.h"
#include "Engine/ResourceDefines.h"
#include "Engine/GameObjects/Transform.h"
#include "Engine/Graphics/Mesh.h"
#include "Engine/GameObjects/RenderableGameObject.h"
#include "Engine/Managers/GraphicsManager.h"
#include "Engine/EditorUI/EditorUI.h"

#include "Engine/Managers/GameManager.h"
#include "Engine/Managers/Events/EventManager.h"

GameUIComponent::GameUIComponent(string identifier, CoolUUID uuid):GameObject(identifier, uuid)
{
	m_gameObjectType |= GameObjectType::GAME_UI_COMPONENT;
	m_uiComponentType |= UIComponentType::BASE;

	m_transform = new Transform();

	EventManager::Instance()->AddClient(EventType::KeyPressed, this);
	EventManager::Instance()->AddClient(EventType::KeyReleased, this);
}

GameUIComponent::GameUIComponent(nlohmann::json& data, CoolUUID uuid) : GameObject(data, uuid)
{
	m_gameObjectType |= GameObjectType::GAME_UI_COMPONENT;
	m_uiComponentType |= UIComponentType::BASE;

	m_isRenderable = data["IsRendering"];
	m_layer = data["Layer"];

	m_texFilepath = L"";

	std::string tempTexPath = data["TexturePath"];
	std::wstring wideTexPath = std::wstring(tempTexPath.begin(), tempTexPath.end());

	SetTexture(wideTexPath);

	EventManager::Instance()->AddClient(EventType::KeyPressed, this);
	EventManager::Instance()->AddClient(EventType::KeyReleased, this);
}

GameUIComponent::GameUIComponent(GameUIComponent const& other) : GameObject(other)
{
	m_isRenderable = other.m_isRenderable;
	m_ptexture = other.m_ptexture;

	m_uiComponentType = other.m_uiComponentType;
	m_layer = other.m_layer;

	EventManager::Instance()->AddClient(EventType::KeyPressed, this);
	EventManager::Instance()->AddClient(EventType::KeyReleased, this);
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

void GameUIComponent::Handle(Event* e)
{
	if (e->GetEventID() == EventType::KeyReleased)
	{
		KeyReleasedEvent* released = (KeyReleasedEvent*)e;
		if (released->GetKeyCode() == 'T')
		{
			if (m_identifier == "canvasTest")
			{
				// Get us in the tree
				GameManager* gm = GameManager::GetInstance();
				Scene* scene = GameManager::GetInstance()->GetCurrentScene();
				SceneGraph<GameObject>* sceneGraph = scene->GetSceneGraph();
				TreeNode<GameObject>* gameObjectNode = sceneGraph->GetNodeUsingIdentifier(m_identifier);

				// Delete all children???
				TreeNode<GameObject>* tempChild = nullptr;
				TreeNode<GameObject>* currentChild = gameObjectNode->Child;
				while (currentChild != nullptr)
				{
					tempChild = currentChild;
					currentChild = currentChild->Sibling;
					string iden = tempChild->NodeObject->GetIdentifier();
					gm->DeleteGameObjectUsingIdentifier(iden);
				}
			}

		}
	}
}

void GameUIComponent::EditorUpdate()
{

}

void GameUIComponent::Serialize(nlohmann::json& data)
{
	GameObject::Serialize(data);

	std::string tempPath = std::string(m_texFilepath.begin(), m_texFilepath.end());
	data["TexturePath"] = tempPath;
	data["Layer"] = m_layer;
	data["IsRendering"] = m_isRenderable;
	data["UIType"] = (int)m_uiComponentType;
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
