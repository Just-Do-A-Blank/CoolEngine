#include "ImageComponent.h"
#include "Engine/ResourceDefines.h"
#include "Engine/GameObjects/Transform.h"
#include "Engine/Managers/GraphicsManager.h"
#include "Engine/EditorUI/EditorUI.h"

ImageComponent::ImageComponent(string identifier, CoolUUID uuid, XMFLOAT3& position, XMFLOAT3& scale, XMFLOAT3& rotation) : GameUIComponent(identifier, uuid, position, scale, rotation)
{
	m_componentType |= UIComponentType::IMAGE;
}

ImageComponent::ImageComponent(nlohmann::json& data, CoolUUID uuid) : GameUIComponent(data, uuid)
{
	m_componentType |= UIComponentType::IMAGE;
}

#if EDITOR
void ImageComponent::CreateEngineUI()
{
	GameUIComponent::CreateEngineUI();

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

	EditorUI::Texture("Texture", m_texFilepath, m_ptexture);

	ImGui::Spacing();
}
#endif
