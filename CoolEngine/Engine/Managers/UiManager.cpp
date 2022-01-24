#include "UiManager.h"
#include "Engine/GameUI/GameUIComponent.h"
#include "Engine/GameUI/UICanvas.h"

void UIManager::CreateCanvas(string identifier, XMFLOAT3& position, XMFLOAT3& scale, XMFLOAT3& rotation)
{
	UICanvas* canvas = new UICanvas(identifier, position, scale, rotation);
	m_uiCanvasMap.insert(pair<string, UICanvas*>(identifier, canvas));
    m_pcurrentCanvas = canvas;
}

void UIManager::SelectCanvasUsingIdentifier(string canvasIdentifier)
{
    if (m_uiCanvasMap.count(canvasIdentifier) == 0)
    {
        LOG("Canvas : " << canvasIdentifier << "; was not found in Canvas Map ");
        return;
    }
    m_pcurrentCanvas = m_uiCanvasMap.find(canvasIdentifier)->second;
}

void UIManager::SelectCanvas(UICanvas* pcanvas)
{
    m_pcurrentCanvas = pcanvas;
}

void UIManager::DeleteCanvasUsingIdentifier(string canvasIdentifier)
{
    m_uiCanvasMap.erase(canvasIdentifier);
}

void UIManager::DeleteCanvas(UICanvas* pcanvas)
{
    DeleteCanvasUsingIdentifier(pcanvas->GetCanvasIdentifier());
}

void UIManager::DeleteSelectedCanvas()
{
    DeleteCanvasUsingIdentifier(m_pcurrentCanvas->GetCanvasIdentifier());
}

void UIManager::Render(RenderStruct& renderStruct)
{
    m_pcurrentCanvas->Render(renderStruct);
}

unordered_map<string, UICanvas*> UIManager::GetCanvasList()
{
    return m_uiCanvasMap;
}

unordered_map<string, GameUIComponent*>& UIManager::GetAllUIComponentsInCurrentCanvas()
{
    return m_pcurrentCanvas->GetAllGameUIComponents();
}