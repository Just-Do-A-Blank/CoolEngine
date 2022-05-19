#include "AnimationState.h"
#include "Engine/Graphics/SpriteAnimation.h"
#include "Engine/Managers/GraphicsManager.h"

void AnimationState::Enter()
{
	m_animation.Play();
}

void AnimationState::Exit()
{
	m_animation.Play();
}

void AnimationState::Update()
{
	m_animation.Update();
}

const SpriteAnimation* AnimationState::GetAnimation() const
{
	if (m_animation.GetFrames() == nullptr)
	{
		return nullptr;
	}

	return &m_animation;
}

void AnimationState::SetAnimation(std::wstring filepath)
{
	SpriteAnimation anim = GraphicsManager::GetInstance()->GetAnimation(filepath);

	m_animation = anim;
}

void AnimationState::SetAnimation(SpriteAnimation anim)
{
	m_animation = anim;
}

void AnimationState::Play()
{
	m_animation.Play();
}

void AnimationState::Pause()
{
	m_animation.Pause();
}

void AnimationState::Serialize(nlohmann::json& data, FiniteStateMachine* pstateMachine)
{
	FiniteState::Serialize(data, pstateMachine);

	std::string tempAnimPath = std::string(m_animation.GetAnimPath().begin(), m_animation.GetAnimPath().end());

	data["Anim"] = tempAnimPath;
}

void AnimationState::Deserialize(const nlohmann::json& data, FiniteStateMachine* pstateMachine)
{
	FiniteState::Deserialize(data, pstateMachine);

	std::string tempAnimPath = data["Anim"];

	m_animation = GraphicsManager::GetInstance()->GetAnimation(std::wstring(tempAnimPath.begin(), tempAnimPath.end()));
}

void AnimationState::CreateEngineUI()
{

}
