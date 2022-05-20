#pragma once
#include "Engine/AI/FiniteStateMachine.h"

class AnimationStateMachine : public FiniteStateMachine
{
public:
	AnimationStateMachine();
	AnimationStateMachine(AnimationStateMachine const* other);

	virtual void Serialize(nlohmann::json& data);
	virtual void Deserialize(const nlohmann::json& data);

#if EDITOR
	virtual void CreateEngineUI();
#endif

protected:

private:
#if EDITOR
	std::string m_newStateName = "";
	std::string m_selectedState = "";
#endif
};

