#pragma once
#include <unordered_map>
#include "Engine/Includes/json.hpp"

class FiniteState;

class FiniteStateMachine
{
public:
	virtual ~FiniteStateMachine();

	FiniteState* GetState(std::string stateName);
	FiniteState* GetActiveState();

	void SetActiveState(std::string stateName);

	bool AddState(std::string stateName, FiniteState* pstate);
	bool RemoveState(std::string stateName);

	void Update();

	std::string GetStateName(FiniteState* pstate);

	virtual void Serialize(nlohmann::json& data) = 0;
	virtual void Deserialize(const nlohmann::json& data) = 0;

#if EDITOR
	virtual void CreateEngineUI() = 0;
#endif

	const std::unordered_map<std::string, FiniteState*>* GetStates();

protected:

	std::unordered_map<std::string, FiniteState*> m_states;
	FiniteState* m_pstate;

private:

};

