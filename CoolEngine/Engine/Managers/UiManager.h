#pragma once
#include "Engine/Structure/Manager.h"
#include "Engine/Helpers/DebugHelper.h"
#include "Engine/GameUI/UICanvas.h"
#include "Engine/Managers/GraphicsManager.h"
#include "Engine/Managers/SceneGraph.h"
#include "Engine/GameUI/GameUIComponent.h"
#include "Engine/GameUI/TextComponent.h"


class GameUIComponent;
class TextComponent;

class UIManager : public Manager<UIManager>
{
private:
	ID3D11Device* m_pDevice;

    TreeNode<GameUIComponent>* m_pselectedUINode = nullptr;
    TreeNode<GameUIComponent>* m_prootTreeNode = nullptr;

    SceneGraph<GameUIComponent>* m_pUISceneGraph = nullptr;

	GameUIComponent* GetAndRemoveObjectFromUUID(std::vector<GameUIComponent*>& components, const CoolUUID& uuid);

public:
	void Init(ID3D11Device* pDevice);
    void CreateCanvas(string identifier, XMFLOAT3& position, XMFLOAT3& scale, XMFLOAT3& rotation);
    void CreateCanvas(string identifier, CoolUUID uuid, XMFLOAT3& position, XMFLOAT3& scale, XMFLOAT3& rotation);
    void SelectUIObjectUsingIdentifier(string identifier);
    void SelectUIObject(GameUIComponent* pgameObject);
    void SelectUIObjectUsingTreeNode(TreeNode<GameUIComponent>* pnode);
    TreeNode<GameUIComponent>* GetRootTreeNode();

    void Render(RenderStruct& renderStruct);

	template<typename T>
	T* CreateUIComponent(string identifier, CoolUUID uuid, XMFLOAT3& position, XMFLOAT3& scale, XMFLOAT3& rotation)
	{
		if (!m_pselectedUINode)
		{
			LOG("No UIElement is selected to create the UIComponent in");
			return nullptr;
		}

		T* uiComponent = new T(identifier, uuid, position, scale, rotation);

		m_pUISceneGraph->AddChild(m_pselectedUINode, uiComponent);

		return uiComponent;
	}

	template<typename T>
	T* CreateUIComponent(string identifier, XMFLOAT3& position, XMFLOAT3& scale, XMFLOAT3& rotation)
	{
		CoolUUID uuid;
        return CreateUIComponent<T>(identifier, uuid, position, scale, rotation);
	}

    template<typename T>
    T* GetUIComponentUsingIdentifier(string& identifier)
    {
        return m_pcurrentCanvas->GetGameUIComponentUsingIdentifier<T>(identifier);
    }

    template<typename T>
    void DeleteUIElementUsingNode(TreeNode<T>* currentNode)
    {
        m_pUISceneGraph->DeleteGameObjectUsingNode(currentNode);
    }

    void DeleteSelectedUIComponent();


    void Serialize(nlohmann::json& data) override;
    void Deserialize(nlohmann::json& data) override;

    //Getters
	vector<GameUIComponent*>& GetAllUIComponents();
};

