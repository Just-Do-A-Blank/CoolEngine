#pragma once
#include "ToolBase.h"

#include "Engine/Graphics/SpriteAnimation.h"
#include "Engine/EditorUI/EditorUI.h"

#include <vector>

class GameObject;

struct FrameInfo
{
	Frame m_frame;
	WCHAR m_texName[FILEPATH_BUFFER_SIZE];
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

	GameObject* m_pgameObject = nullptr;

	vector<FrameInfo> m_frames;

	int m_selectedIndex = -1;

	WCHAR m_savePath[FILEPATH_BUFFER_SIZE];
};

