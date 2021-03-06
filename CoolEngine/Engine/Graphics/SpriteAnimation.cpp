#include "SpriteAnimation.h"
#include "Engine/EditorUI/EditorUI.h"
#include "Engine/Managers/GameManager.h"

SpriteAnimation::SpriteAnimation()
{
	m_pframes = nullptr;

	m_currentFrameIndex = -1;
	m_timeMilestone = 0;
}

SpriteAnimation::SpriteAnimation(std::vector<Frame>* frames, std::wstring animPath)
{
	m_pframes = frames;

	m_currentFrameIndex = 0;

	m_animPath = animPath;

	if (frames != nullptr)
	{
		m_timeMilestone = GameManager::GetInstance()->GetTimer()->GameTime() + m_pframes->at(m_currentFrameIndex).m_frameTime;
	}
}

void SpriteAnimation::SetFrames(std::vector<Frame>* pframes)
{
	m_pframes = pframes;
}

std::vector<Frame>* SpriteAnimation::GetFrames() const
{
	return m_pframes;
}

const std::wstring& SpriteAnimation::GetAnimPath() const
{
	return m_animPath;
}

bool SpriteAnimation::IsLooping() const
{
	return m_isLooping;
}

void SpriteAnimation::SetLooping(bool isLooping)
{
	m_isLooping = isLooping;
}

bool SpriteAnimation::IsPaused() const
{
	return m_isPaused;
}

void SpriteAnimation::Update()
{
	if (m_isPaused == true)
	{
		return;
	}

	//If milestone is hit then increment frame
	if (m_timeMilestone <= GameManager::GetInstance()->GetTimer()->GameTime())
	{
		m_currentFrameIndex = (m_currentFrameIndex + 1) % m_pframes->size();

		//If not looping and at last frame then pause.
		if (m_isLooping == false && m_currentFrameIndex == m_pframes->size() - 1)
		{
			Pause();
		}

		m_timeMilestone = GameManager::GetInstance()->GetTimer()->GameTime() + m_pframes->at(m_currentFrameIndex).m_frameTime;
	}
}

void SpriteAnimation::Play()
{
	if (m_isPaused == false)
	{
		Restart();
	}
	else
	{
		m_timeMilestone = GameManager::GetInstance()->GetTimer()->GameTime() + m_timeLeft;

		m_timeLeft = 0;

		m_isPaused = false;
	}
}

void SpriteAnimation::Pause()
{
	if (m_isPaused == true)
	{
		LOG("Tried to pause animation when it wasn't playing!");

		return;
	}

	m_timeLeft = m_timeMilestone - GameManager::GetInstance()->GetTimer()->GameTime();

	m_isPaused = true;
}

void SpriteAnimation::Restart()
{
	m_currentFrameIndex = 0;

	m_timeMilestone = GameManager::GetInstance()->GetTimer()->GameTime() + m_pframes->at(m_currentFrameIndex).m_frameTime;

	m_timeLeft = 0;

	m_isPaused = false;
}

ID3D11ShaderResourceView* SpriteAnimation::GetCurrentFrame() const
{
	if (m_currentFrameIndex == -1)
	{
		return nullptr;
	}

	return m_pframes->at(m_currentFrameIndex).m_ptexture;
}

#if EDITOR
void SpriteAnimation::CreateEngineUI()
{
	ID3D11ShaderResourceView* psrv = nullptr;

	if (m_pframes != nullptr)
	{
		psrv = GetCurrentFrame();
	}

	if (EditorUI::Animation("Animation", m_uiFilePath, psrv) == true)
	{
		SpriteAnimation anim = GraphicsManager::GetInstance()->GetAnimation(m_uiFilePath);

		if (anim.GetFrames() != nullptr)
		{
			SetFrames(anim.GetFrames());

			m_animPath = m_uiFilePath;

			Restart();
		}
	}

	ImGui::Spacing();

	if (EditorUI::Checkbox("Is Looping", m_isLooping) == true)
	{
		if (m_isLooping == true)
		{
			Restart();
		}
	}
}
#endif