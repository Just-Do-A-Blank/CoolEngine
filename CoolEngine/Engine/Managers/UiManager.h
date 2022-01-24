#pragma once
#include "Engine/Structure/Singleton.h"
#include "Engine/Helpers/DebugHelper.h"
#include "Engine/GameUI/UICanvas.h"
#include "Engine/Managers/GraphicsManager.h"

class GameUIComponent;

class UIManager :
    public Singleton<UIManager>
{
private:
    unordered_map<string, UICanvas*> m_uiCanvasMap;

    UICanvas* m_pcurrentCanvas = nullptr;
public:
    void CreateCanvas(string identifier, XMFLOAT3& position, XMFLOAT3& scale, XMFLOAT3& rotation);
    void SelectCanvasUsingIdentifier(string canvasIdentifier);
    void SelectCanvas(UICanvas* pcanvas);
    void DeleteCanvasUsingIdentifier(string canvasIdentifier);
    void DeleteCanvas(UICanvas* pcanvas);
    void DeleteSelectedCanvas();

    void Render(RenderStruct& renderStruct);

    template<typename T>
    T* CreateUIComponent(string identifier, XMFLOAT3& position, XMFLOAT3& scale, XMFLOAT3& rotation)
    {
        return m_pcurrentCanvas->CreateUIComponent<T>(identifier, position, scale, rotation);
    }

    template<typename T>
    T* GetUIComponentUsingIdentifier(string& identifier)
    {
        return m_pcurrentCanvas->GetGameUIComponentUsingIdentifier<T>(identifier);
    }


    //Getters
    unordered_map<string, UICanvas*> GetCanvasList();
    unordered_map<string, GameUIComponent*>& GetAllUIComponentsInCurrentCanvas();
};

