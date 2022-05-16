#pragma once
#include <unordered_map>

class FiniteState;

class FiniteStateMachine
{
public:
	FiniteState* GetState(std::string stateName);
	FiniteState* GetActiveState();

	bool AddState(std::string stateName, FiniteState* pstate);
	bool RemoveState(std::string stateName);

	void Update();

protected:

private:
	std::unordered_map<std::string, FiniteState*> m_states;
	FiniteState* m_pstate;
};

