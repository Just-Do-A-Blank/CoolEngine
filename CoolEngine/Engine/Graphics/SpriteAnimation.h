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

	std::wstring m_animPath = L"";

public:
	SpriteAnimation();
	SpriteAnimation(std::vector<Frame>* pframes, std::wstring animPath);

	void SetFrames(std::vector<Frame>* pframes);
	const std::vector<Frame>* GetFrames() const;

	const std::wstring& GetAnimPath() const;

	bool IsLooping() const;
	void SetLooping(bool isLooping);

	bool IsPaused() const;

	void Update();

	void Play();
	void Pause();

	void Restart();

	ID3D11ShaderResourceView* GetCurrentFrame() const;
};