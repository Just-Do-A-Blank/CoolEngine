#pragma once
#include "Engine/AI/FiniteState.h"
#include "Engine/Graphics/SpriteAnimation.h"

class AnimationState : public FiniteState
{
public:
	AnimationState();
	AnimationState(const AnimationState* pother);

	virtual void Enter() override;
	virtual void Exit() override;

	virtual void Update() override;

	const SpriteAnimation* GetAnimation() const;
	void SetAnimation(std::wstring filepath);
	void SetAnimation(SpriteAnimation anim);

	void Play();
	void Pause();

	void Serialize(nlohmann::json& data, FiniteStateMachine* pstateMachine) override;
	void Deserialize(const nlohmann::json& data, FiniteStateMachine* pstateMachine) override;

#if EDITOR
	void CreateEngineUI() override;
#endif

protected:

private:
	SpriteAnimation m_animation;
};

