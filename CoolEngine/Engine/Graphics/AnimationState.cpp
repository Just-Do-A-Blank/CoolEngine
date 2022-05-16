#include "AnimationState.h"

void AnimationState::Enter()
{
	m_animation.Play();
}

void AnimationState::Exit()
{

}

void AnimationState::Update()
{
	m_animation.Update();
}
