#pragma once
#include "Engine/Includes/json.hpp"

class FuzzyState;

class FuzzyStateMachine
{
public:
	virtual ~FuzzyStateMachine();

	bool AddState(FuzzyState* pstate);

	void Update();

	virtual void Serialize(nlohmann::json& data);
	virtual void Deserialize(const nlohmann::json& data) = 0;

	const std::vector<FuzzyState*>* GetStates();
	const std::vector<FuzzyState*>* GetActiveStates();

protected:

	std::vector<FuzzyState*> m_states;
	std::vector<FuzzyState*> m_activeStates;

private:

};

