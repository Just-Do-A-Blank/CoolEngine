#include "ImageComponent.h"
#include "Engine/ResourceDefines.h"
#include "Engine/GameObjects/Transform.h"
#include "Engine/Managers/GraphicsManager.h"
#include "Engine/EditorUI/EditorUI.h"

ImageComponent::ImageComponent(string identifier, CoolUUID uuid) : GameUIComponent(identifier, uuid)
{
	m_uiComponentType |= UIComponentType::IMAGE;
}

ImageComponent::ImageComponent(nlohmann::json& data, CoolUUID uuid) : GameUIComponent(data, uuid)
{
	m_uiComponentType |= UIComponentType::IMAGE;
}

ImageComponent::ImageComponent(ImageComponent const& other) : GameUIComponent(other)
{
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
