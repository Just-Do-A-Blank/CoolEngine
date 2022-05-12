#include "ButtonComponent.h"
#include "Engine/ResourceDefines.h"
#include "Engine/GameObjects/Transform.h"
#include "Engine/Managers/GraphicsManager.h"
#include "Engine/EditorUI/EditorUI.h"

ButtonComponent::ButtonComponent(string identifier, CoolUUID uuid, XMFLOAT3& position, XMFLOAT3& scale, XMFLOAT3& rotation) : GameUIComponent(identifier, uuid, position, scale, rotation)
{
	m_componentType |= UIComponentType::BUTTON;

	m_ppixelShader = GraphicsManager::GetInstance()->GetPixelShader(BUTTON_PIXEL_SHADER_NAME);
}

ButtonComponent::ButtonComponent(nlohmann::json& data, CoolUUID uuid) : GameUIComponent(data, uuid)
{
	m_componentType |= UIComponentType::BUTTON;
}

void ButtonComponent::SetTexture(std::wstring wsfilepath, ButtonTexture textureType)
{
	m_pButtonTexture[int(textureType)] = GraphicsManager::GetInstance()->GetShaderResourceView(wsfilepath);

	if (m_pButtonTexture[int(ButtonTexture::PRESSED)] == nullptr)
	{
		LOG("Tried to get a texture that doesn't exist!");
	}
	else
	{
		m_buttonTexFilepath[int(textureType)] = wsfilepath;
	}
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

	EditorUI::Texture("Pressed Texture", m_buttonTexFilepath[int(ButtonTexture::PRESSED)], m_pButtonTexture[int(ButtonTexture::PRESSED)], columnWidth, textureSize);
	ImGui::Spacing();
	EditorUI::Texture("Released Texture", m_buttonTexFilepath[int(ButtonTexture::RELEASED)], m_pButtonTexture[int(ButtonTexture::RELEASED)], columnWidth, textureSize);
	ImGui::Spacing();
	EditorUI::Texture("Hovered Texture", m_buttonTexFilepath[int(ButtonTexture::HOVERED)], m_pButtonTexture[int(ButtonTexture::HOVERED)], columnWidth, textureSize);

	ImGui::Spacing();
}
#endif
