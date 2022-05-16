#pragma once
#include "Engine/AI/FiniteState.h"
#include "Engine/Graphics/SpriteAnimation.h"

class AnimationState : public FiniteState
{
public:
	virtual void Enter() override;
	virtual void Exit() override;

	virtual void Update() override;

protected:

private:
	SpriteAnimation m_animation;
};

