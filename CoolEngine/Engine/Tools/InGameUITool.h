#pragma once

#if EDITOR
#include "ToolBase.h"

#include "Engine/EditorUI/EditorUI.h"

#include <vector>

class GameUIComponent;

class InGameUITool : public ToolBase
{
public:
	void Init(ID3D11Device* pdevice) override;
	void Update() override;
	void Render() override;
	void Destroy() override;

	void DrawUIWindow();

	void TraverseTree(TreeNode<GameUIComponent>* pcurrentNode, int& nodeCount);

protected:

private:

	int num = 1;

	ImGuiTreeNodeFlags m_base_flags;
	int m_selectionMask;
	int m_gameObjectNodeClicked = -1;

	GameUIComponent* m_puiComponent = nullptr;

	vector<GameUIComponent*> m_gameUIComponentList;
	vector<Frame> m_frames;

	int m_selectedIndex = -1;

	string m_canvasName = "";
	string m_imageName = "";
	string m_textName = "";

	wstring m_savePath;

	bool m_createCanvasClicked = false;
	bool m_showUICreation = false;

	bool SaveUI();
	bool LoadUI();
};

#endif