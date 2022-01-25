#pragma once
#include "Engine/Managers/SceneGraph.h"
#include "Engine/GameUI/GameUIComponent.h"
#include "Engine/GameObjects/RenderableGameObject.h"

class Transform;
class Mesh;

class UICanvas : public GameUIComponent
{
private:
	string m_canvasIdentifier;
	SceneGraph<GameUIComponent>* m_pUISceneGraph;

	TreeNode<GameUIComponent>* m_pselectedNode = nullptr;
	TreeNode<GameUIComponent>* m_prootTreeNode = nullptr;

	UICanvas* m_pselectedGameUIComponent = nullptr;

	int m_layer = 0;

	//Flags
	bool m_isRenderable = true;

protected:

	virtual void CreateEngineUI(ID3D11Device* pdevice);
public:
	UICanvas(string identifier, XMFLOAT3& position, XMFLOAT3& scale, XMFLOAT3& rotation);

	template<typename T>
	T* CreateUIComponent(string identifier, XMFLOAT3& position, XMFLOAT3& scale, XMFLOAT3& rotation)
	{
		T* uiComponent = new T(identifier, position, scale, rotation);

		m_prootTreeNode = m_pUISceneGraph->GetRootNode();
		if (!m_prootTreeNode)
		{
			m_prootTreeNode = m_pUISceneGraph->NewNode(uiComponent);
		}
		else
		{
			if (!m_pselectedGameUIComponent)
			{
				m_pUISceneGraph->AddChild(m_prootTreeNode, uiComponent);
			}
			else
			{
				m_pUISceneGraph->AddChild(m_pUISceneGraph->GetNodeUsingIdentifier(m_pselectedGameUIComponent->GetIdentifier()), uiComponent);
			}
		}

		return uiComponent;
	}

	template<typename T>
	T* GetGameUIComponentUsingIdentifier(string& identifier)
	{
		return  static_cast<T*>(m_pUISceneGraph->GetGameObjectUsingIdentifier(identifier));
	}

	unordered_map<string, GameUIComponent*>& GetAllGameUIComponents();

	virtual void Render(RenderStruct& renderStruct);
	virtual void Update();

	//Editor UI
	virtual void ShowEngineUI(ID3D11Device* pdevice);

	//Setters
	void SetIsRenderable(bool& condition);

	//Getters
	const string& GetCanvasIdentifier();
	Transform* GetTransform();



	void DeleteSelectedGameUIComponent();
	void DeleteGameUIComponentIdentifier(string identifier);

	//Getters
	TreeNode<GameUIComponent>* GetRootTreeNode();
	TreeNode<GameUIComponent>* GetTreeNode(GameUIComponent* pgameObject);
	GameObject* GetSelectedGameUIComponent();
};

