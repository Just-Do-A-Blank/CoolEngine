#include "ButtonComponent.h"
#include "Engine/ResourceDefines.h"
#include "Engine/GameObjects/Transform.h"
#include "Engine/EditorUI/EditorUI.h"
#include "Engine/Managers/Events/EventManager.h"
#include "Engine/Managers/Events/MouseEvents.h"
#include "Engine/Managers/GameManager.h"
#include "Engine/GameObjects/CameraGameObject.h"

ButtonComponent::ButtonComponent(string identifier, CoolUUID uuid) : GameUIComponent(identifier, uuid)
{
	m_uiComponentType |= UIComponentType::BUTTON;

	EventManager::Instance()->AddClient(EventType::MouseButtonPressed, this);
	EventManager::Instance()->AddClient(EventType::MouseButtonReleased, this);
}

ButtonComponent::ButtonComponent(nlohmann::json& data, CoolUUID uuid) : GameUIComponent(data, uuid)
{
	m_uiComponentType |= UIComponentType::BUTTON;

	EventManager::Instance()->AddClient(EventType::MouseButtonPressed, this);

	if (GameUIComponent::IsPrefab())
	{
		LoadLocalData(GameUIComponent::GetPrefabDataLoadedAtCreation());
	}
	else
	{
		LoadLocalData(data);
	}
}

ButtonComponent::ButtonComponent(ButtonComponent const& other) : GameUIComponent(other)
{
	for (int i = 0; i < (int)ButtonState::COUNT; ++i)
	{
		m_pButtonTextures[i] = other.m_pButtonTextures[i];
		m_buttonTexFilepath[i] = other.m_buttonTexFilepath[i];
	}
	m_currentButtonState = other.m_currentButtonState;
	m_OnClickFunction = other.m_OnClickFunction;
	m_OnClickFunctionArg = m_OnClickFunctionArg;

	m_uiComponentType |= UIComponentType::BUTTON;

	EventManager::Instance()->AddClient(EventType::MouseButtonPressed, this);
	EventManager::Instance()->AddClient(EventType::MouseButtonReleased, this);
}

ButtonComponent::~ButtonComponent()
{
	EventManager::Instance()->RemoveClientEvent(EventType::MouseButtonPressed, this);
	EventManager::Instance()->RemoveClientEvent(EventType::MouseButtonReleased, this);
}

void ButtonComponent::Update()
{
	XMFLOAT2 mouseWorldPos = GameManager::GetInstance()->GetCamera()->GetMousePositionInWorldSpace();

	XMFLOAT3 buttonPosition = m_transform->GetWorldPosition();
	XMFLOAT3 buttonScale = m_transform->GetWorldScale();

	m_isHovered = false;
	if (mouseWorldPos.x >= buttonPosition.x - (buttonScale.x) && mouseWorldPos.x <= buttonPosition.x + (buttonScale.x))
	{
		if (mouseWorldPos.y >= buttonPosition.y - (buttonScale.y) && mouseWorldPos.y <= buttonPosition.y + (buttonScale.y))
		{
			m_isHovered = true;
		}
	}

	if (!m_isHovered)
	{
		SetButtonState(ButtonState::RELEASED);
	}
	else if (m_buttonClicked)
	{
		SetButtonState(ButtonState::PRESSED);
	}
	else
	{
		SetButtonState(ButtonState::HOVERED);
	}
}

void ButtonComponent::EditorUpdate()
{
}

void ButtonComponent::Render(RenderStruct& renderStruct)
{
	if (m_pButtonTextures[(int)m_currentButtonState] == nullptr)
	{
		return;
	}

	GraphicsManager::GetInstance()->RenderQuad(m_pButtonTextures[(int)m_currentButtonState], m_transform->GetWorldPosition(), m_transform->GetWorldScale(), m_transform->GetWorldRotation().z, m_layer);
}

void ButtonComponent::SetTexture(std::wstring wsfilepath, ButtonState textureType)
{
	m_pButtonTextures[int(textureType)] = GraphicsManager::GetInstance()->GetShaderResourceView(wsfilepath);

	if (m_pButtonTextures[int(textureType)] == nullptr)
	{
		LOG("Tried to get a texture that doesn't exist!");
	}
	else
	{
		m_buttonTexFilepath[int(textureType)] = wsfilepath;
	}
}

void ButtonComponent::Handle(Event* e)
{
	switch (e->GetEventID())
	{
	case EventType::MouseButtonPressed:
		if (((MouseButtonPressedEvent*)e)->GetButton() == VK_LBUTTON)
		{
			if (m_isHovered)
			{
				m_buttonClicked = true;
			}
		}
		break;

	case EventType::MouseButtonReleased:
		if (((MouseButtonReleasedEvent*)e)->GetButton() == VK_LBUTTON)
		{
			if (m_buttonClicked && m_isHovered && m_OnClickFunction)
			{
				m_OnClickFunction(m_OnClickFunctionArg);
			}
			m_buttonClicked = false;
		}
		break;
	}
}

void ButtonComponent::SetButtonOnClick(void(*onClick)(void*), void* argument)
{
	m_OnClickFunction = onClick;
	m_OnClickFunctionArg = argument;
}

void ButtonComponent::Serialize(nlohmann::json& data)
{
	GameUIComponent::Serialize(data);

	SaveLocalData(data);
}

void ButtonComponent::LoadLocalData(const nlohmann::json& jsonData)
{
	if (jsonData.contains("ButtonPressTexPath"))
	{
		std::string tempPath = jsonData["ButtonPressTexPath"];
		m_buttonTexFilepath[(int)ButtonState::PRESSED] = std::wstring(tempPath.begin(), tempPath.end());

		tempPath = jsonData["ButtonHoveredTexPath"];
		m_buttonTexFilepath[(int)ButtonState::HOVERED] = std::wstring(tempPath.begin(), tempPath.end());

		tempPath = jsonData["ButtonReleasedTexPath"];
		m_buttonTexFilepath[(int)ButtonState::RELEASED] = std::wstring(tempPath.begin(), tempPath.end());

		for (int i = 0; i < (int)ButtonState::COUNT; ++i)
		{
			m_pButtonTextures[i] = GraphicsManager::GetInstance()->GetShaderResourceView(m_buttonTexFilepath[i]);
		}
	}
}

void ButtonComponent::SaveLocalData(nlohmann::json& jsonData)
{
	std::string tempPath = std::string(m_buttonTexFilepath[(int)ButtonState::PRESSED].begin(), m_buttonTexFilepath[(int)ButtonState::PRESSED].end());
	jsonData["ButtonPressTexPath"] = tempPath;

	tempPath = std::string(m_buttonTexFilepath[(int)ButtonState::HOVERED].begin(), m_buttonTexFilepath[(int)ButtonState::HOVERED].end());
	jsonData["ButtonHoveredTexPath"] = tempPath;

	tempPath = std::string(m_buttonTexFilepath[(int)ButtonState::RELEASED].begin(), m_buttonTexFilepath[(int)ButtonState::RELEASED].end());
	jsonData["ButtonReleasedTexPath"] = tempPath;
}

void ButtonComponent::LoadAllPrefabData(const nlohmann::json& jsonData)
{
	LoadLocalData(jsonData);
	GameUIComponent::LoadAllPrefabData(jsonData);
}

void ButtonComponent::SaveAllPrefabData(nlohmann::json& jsonData)
{
	SaveLocalData(jsonData);
	GameUIComponent::SaveAllPrefabData(jsonData);
}

#if EDITOR
void ButtonComponent::CreateEngineUI()
{
	GameUIComponent::CreateEngineUI();

	if (EditorUI::CollapsingSection("Button Options", true))
	{
		ImVec2 textureSize = ImVec2(128, 32);
		float columnWidth = 120.0f;

		EditorUI::Texture("Released Texture", m_buttonTexFilepath[int(ButtonState::RELEASED)], m_pButtonTextures[int(ButtonState::RELEASED)], columnWidth, textureSize);
		ImGui::Spacing();
		EditorUI::Texture("Pressed Texture", m_buttonTexFilepath[int(ButtonState::PRESSED)], m_pButtonTextures[int(ButtonState::PRESSED)], columnWidth, textureSize);
		ImGui::Spacing();
		EditorUI::Texture("Hovered Texture", m_buttonTexFilepath[int(ButtonState::HOVERED)], m_pButtonTextures[int(ButtonState::HOVERED)], columnWidth, textureSize);

		ImGui::Spacing();
	}

}
#endif

void ButtonComponent::SetButtonState(ButtonState buttonState)
{
	m_currentButtonState = buttonState;
	m_ptexture = m_pButtonTextures[(int)buttonState];
}
