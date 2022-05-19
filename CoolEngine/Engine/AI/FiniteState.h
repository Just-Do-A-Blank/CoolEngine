#pragma once
#include <unordered_map>
#include "Engine/Includes/json.hpp"
#include "Engine/GameObjects/UUID.h"

class FiniteStateMachine;
class FiniteState;

template<class T>
struct TransitionInfo
{
public:
	T CurrentValue;
	T TransitionValue;
	FiniteState* NewState;
};

class FiniteState
{
public:

	virtual void Enter() = 0;
	virtual void Exit() = 0;

	virtual void Update() = 0;

	bool SetBool(std::string name, bool value);
	bool SetFloat(std::string name, float value);
	bool SetInt(std::string name, int value);

	bool CreateBoolTransition(std::string name, bool transitionValue, bool initialValue, FiniteState* pnewState);
	bool CreateFloatTransition(std::string name, float transitionValue, float initialValue, FiniteState* pnewState);
	bool CreateIntTransition(std::string name, int transitionValue, int initialValue, FiniteState* pnewState);

	virtual bool Transition(FiniteState*& pnewState);

	virtual void CreateEngineUI() = 0;

	virtual void Serialize(nlohmann::json& data, FiniteStateMachine* pstateMachine);
	virtual void Deserialize(const nlohmann::json& data, FiniteStateMachine* pstateMachine);

protected:

private:
	std::unordered_map<std::string, TransitionInfo<int>> m_intTransitionInfo;
	std::unordered_map<std::string, TransitionInfo<float>> m_floatTransitionInfo;
	std::unordered_map<std::string, TransitionInfo<bool>> m_boolTransitionInfo;
};

