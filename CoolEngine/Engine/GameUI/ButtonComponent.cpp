#include "ButtonComponent.h"
#include "Engine/ResourceDefines.h"
#include "Engine/GameObjects/Transform.h"
#include "Engine/EditorUI/EditorUI.h"
#include "Engine/Managers/Events/EventManager.h"
#include "Engine/Managers/Events/MouseEvents.h"
#include "Engine/Managers/GameManager.h"
#include "Engine/GameObjects/CameraGameObject.h"

ButtonComponent::ButtonComponent(string identifier, CoolUUID uuid, XMFLOAT3& position, XMFLOAT3& scale, XMFLOAT3& rotation) : GameUIComponent(identifier, uuid, position, scale, rotation)
{
	m_componentType |= UIComponentType::BUTTON;

	EventManager::Instance()->AddClient(EventType::MouseButtonPressed, this);
	EventManager::Instance()->AddClient(EventType::MouseButtonReleased, this);
}

ButtonComponent::ButtonComponent(nlohmann::json& data, CoolUUID uuid) : GameUIComponent(data, uuid)
{
	m_componentType |= UIComponentType::BUTTON;

	EventManager::Instance()->AddClient(EventType::MouseButtonPressed, this);
}

ButtonComponent::~ButtonComponent()
{
	EventManager::Instance()->RemoveClientAllEvents(this);
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

#if EDITOR
void ButtonComponent::CreateEngineUI()
{
	GameUIComponent::CreateEngineUI();

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

	ImVec2 textureSize = ImVec2(128, 32);
	float columnWidth = 120.0f;

	EditorUI::Texture("Released Texture", m_buttonTexFilepath[int(ButtonState::RELEASED)], m_pButtonTextures[int(ButtonState::RELEASED)], columnWidth, textureSize);
	ImGui::Spacing();
	EditorUI::Texture("Pressed Texture", m_buttonTexFilepath[int(ButtonState::PRESSED)], m_pButtonTextures[int(ButtonState::PRESSED)], columnWidth, textureSize);
	ImGui::Spacing();
	EditorUI::Texture("Hovered Texture", m_buttonTexFilepath[int(ButtonState::HOVERED)], m_pButtonTextures[int(ButtonState::HOVERED)], columnWidth, textureSize);

	ImGui::Spacing();
}
#endif

void ButtonComponent::SetButtonState(ButtonState buttonState)
{
	m_currentButtonState = buttonState;
	m_ptexture = m_pButtonTextures[(int)buttonState];
}
