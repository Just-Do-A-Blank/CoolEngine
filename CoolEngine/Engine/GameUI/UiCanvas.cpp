#include "UiCanvas.h"
#include "Engine/GameUI/GameUIComponent.h"
#include "Engine/Graphics/Mesh.h"
#include "Engine/ResourceDefines.h"
#include "Engine/Managers/GraphicsManager.h"

UICanvas::UICanvas(string identifier, CoolUUID uuid) : GameUIComponent(identifier, uuid)
{
	m_uiComponentType |= UIComponentType::CANVAS;
}

UICanvas::UICanvas(nlohmann::json& data, CoolUUID uuid) : GameUIComponent(data, uuid)
{
	m_uiComponentType |= UIComponentType::CANVAS;
}

UICanvas::UICanvas(UICanvas const& other) : GameUIComponent(other)
{
}

void UICanvas::Render(RenderStruct& renderStruct)
{
}

void UICanvas::Update()
{
}

void UICanvas::EditorUpdate()
{
}
