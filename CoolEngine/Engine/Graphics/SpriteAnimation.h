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

	int m_currentFrameIndex = -1;

	float m_timeMilestone = 0.0f;
	float m_timeLeft = 0.0f;

	bool m_isLooping = true;
	bool m_isPaused = false;

public:
	SpriteAnimation();
	SpriteAnimation(std::vector<Frame>* pframes);

	void SetFrames(std::vector<Frame>* pframes);
	std::vector<Frame>* GetFrames();

	bool IsLooping();
	void SetLooping(bool isLooping);

	bool IsPaused();

	void Update();

	void Play();
	void Pause();

	void Restart();

	ID3D11ShaderResourceView* GetCurrentFrame();
};