#include "UiCanvas.h"
#include "Engine/GameUI/GameUIComponent.h"
#include "Engine/Graphics/Mesh.h"
#include "Engine/ResourceDefines.h"
#include "Engine/Managers/GraphicsManager.h"

void UICanvas::CreateEngineUI(ID3D11Device* pdevice)
{
}

UICanvas::UICanvas(string identifier, XMFLOAT3& position, XMFLOAT3& scale, XMFLOAT3& rotation) : GameUIComponent(identifier, position, scale, rotation)
{
	m_canvasIdentifier = identifier;	
}

void UICanvas::Render(RenderStruct& renderStruct)
{
}

void UICanvas::Update()
{
}

void UICanvas::ShowEngineUI(ID3D11Device* pdevice)
{
}

void UICanvas::SetIsRenderable(bool& condition)
{
	m_isRenderable = condition;
}

const string& UICanvas::GetCanvasIdentifier()
{
	return m_canvasIdentifier;
}

Transform* UICanvas::GetTransform()
{
	return m_transform;
}
