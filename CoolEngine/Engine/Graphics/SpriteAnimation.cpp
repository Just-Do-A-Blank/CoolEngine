#include "SpriteAnimation.h"

#include "Engine/Managers/GameManager.h"

SpriteAnimation::SpriteAnimation()
{
	m_pframes = nullptr;

	m_currentFrameIndex = 0;
	m_timeMilestone = 0;
}

SpriteAnimation::SpriteAnimation(std::vector<Frame>* frames)
{
	m_pframes = frames;

	m_currentFrameIndex = 0;
	m_timeMilestone = GameManager::GetInstance()->GetTimer()->GameTime() + m_pframes->at(m_currentFrameIndex).m_frameTime;
}

void SpriteAnimation::SetFrames(std::vector<Frame>* pframes)
{
	m_pframes = pframes;
}

std::vector<Frame>* SpriteAnimation::GetFrames()
{
	return m_pframes;
}

void SpriteAnimation::Update()
{
	if (m_timeMilestone <= GameManager::GetInstance()->GetTimer()->GameTime())
	{
		m_currentFrameIndex = (m_currentFrameIndex + 1) % m_pframes->size();

		m_timeMilestone = GameManager::GetInstance()->GetTimer()->GameTime() + m_pframes->at(m_currentFrameIndex).m_frameTime;
	}
}

ID3D11ShaderResourceView* SpriteAnimation::GetCurrentFrame()
{
	return m_pframes->at(m_currentFrameIndex).m_ptexture;
}
