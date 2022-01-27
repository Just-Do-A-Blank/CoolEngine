#pragma once
#include "Engine/Structure/Singleton.h"
#include "Engine/Helpers/DebugHelper.h"
#include "Engine/GameUI/UICanvas.h"
#include "Engine/Managers/GraphicsManager.h"
#include "Engine/Managers/SceneGraph.h"
#include "Engine/GameUI/GameUIComponent.h"
#include "Engine/GameUI/TextComponent.h"


class GameUIComponent;
class TextComponent;

class UIManager :
    public Singleton<UIManager>
{
private:
	ID3D11Device* m_pDevice;

    TreeNode<GameUIComponent>* m_pselectedUINode = nullptr;
    TreeNode<GameUIComponent>* m_prootTreeNode = nullptr;

    //unordered_map<string, UICanvas*> m_uiCanvasMap;
    SceneGraph<GameUIComponent>* m_pUISceneGraph;

public:
	void Init(ID3D11Device* pDevice);
    void CreateCanvas(string identifier, XMFLOAT3& position, XMFLOAT3& scale, XMFLOAT3& rotation);
    void SelectUIObjectUsingIdentifier(string identifier);
    void SelectUIObject(GameUIComponent* pgameObject);
    void SelectUIObjectUsingTreeNode(TreeNode<GameUIComponent>* pnode);
    void DeleteCanvasUsingIdentifier(string canvasIdentifier);
    void DeleteCanvas(UICanvas* pcanvas);
    void DeleteSelectedCanvas();
    TreeNode<GameUIComponent>* GetRootTreeNode();

    void Render(RenderStruct& renderStruct);

	template<typename T>
	T* CreateUIComponent(string identifier, XMFLOAT3& position, XMFLOAT3& scale, XMFLOAT3& rotation)
	{
        if (!m_pselectedUINode)
        {
            LOG("No UIElement is selected to create the UIComponent in");
            return nullptr;
        }

        T* uiComponent = new T(identifier, position, scale, rotation);

       m_pUISceneGraph->AddChild(m_pselectedUINode, uiComponent);

        return uiComponent;
	}

    template<typename T>
    T* GetUIComponentUsingIdentifier(string& identifier)
    {
        return m_pcurrentCanvas->GetGameUIComponentUsingIdentifier<T>(identifier);
    }


    //Getters
    unordered_map<string, UICanvas*> GetCanvasList();
	vector<GameUIComponent*>& GetAllUIComponents();
};

