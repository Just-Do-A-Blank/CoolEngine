#pragma once

#include <vector>

struct Frame
{
	float m_frameTime;
	ID3D11ShaderResourceView* m_ptexture;
};

class SpriteAnimation
{
	std::vector<Frame>* m_pframes;

	int m_currentFrameIndex = 0;

	float m_timeMilestone = 0.0f;

public:
	SpriteAnimation();
	SpriteAnimation(std::vector<Frame>* pframes);

	void SetFrames(std::vector<Frame>* pframes);
	std::vector<Frame>* GetFrames();

	void Update();

	ID3D11ShaderResourceView* GetCurrentFrame();
};