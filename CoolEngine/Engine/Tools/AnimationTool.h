#pragma once

#if EDITOR
#include "ToolBase.h"

#include "Engine/Graphics/SpriteAnimation.h"
#include "Engine/EditorUI/EditorUI.h"

#include <vector>

class RenderableGameObject;

struct FrameInfo
{
	Frame m_frame;
	wstring m_filepath;
};

class AnimationTool : public ToolBase
{
public:
	void Init(ID3D11Device* pdevice) override;
	void Update() override;
	void Render() override;
	void Destroy() override;

protected:

private:

	RenderableGameObject* m_pgameObject = nullptr;

	vector<FrameInfo> m_frameInfos;
	vector<Frame> m_frames;

	int m_selectedIndex = -1;

	string m_animName = "";

	wstring m_savePath;

	bool SaveAnim(string animName);
};

#endif