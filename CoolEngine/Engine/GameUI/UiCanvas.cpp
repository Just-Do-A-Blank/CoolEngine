#include "UiCanvas.h"
#include "Engine/GameUI/GameUIComponent.h"
#include "Engine/Graphics/Mesh.h"
#include "Engine/ResourceDefines.h"
#include "Engine/Managers/GraphicsManager.h"

UICanvas::UICanvas(string identifier, CoolUUID uuid, XMFLOAT3& position, XMFLOAT3& scale, XMFLOAT3& rotation) : GameUIComponent(identifier, uuid, position, scale, rotation)
{
	m_componentType |= UIComponentType::CANVAS;
}

UICanvas::UICanvas(nlohmann::json& data, CoolUUID uuid) : GameUIComponent(data, uuid)
{
	m_componentType |= UIComponentType::CANVAS;
}

void UICanvas::Render(RenderStruct& renderStruct)
{
}

void UICanvas::Update()
{
}
