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
	T ResetValue;
	T TransitionValue;
	FiniteState* NewState;
};

class FiniteState
{
public:
	FiniteState();
	FiniteState(const FiniteState* pother);

	virtual void Enter();
	virtual void Exit() = 0;

	virtual void Update() = 0;

	bool SetBool(std::string name, bool value);
	bool SetFloat(std::string name, float value);
	bool SetInt(std::string name, int value);

	bool CreateBoolTransition(std::string name, bool transitionValue, bool initialValue, FiniteState* pnewState);
	bool CreateFloatTransition(std::string name, float transitionValue, float initialValue, FiniteState* pnewState);
	bool CreateIntTransition(std::string name, int transitionValue, int initialValue, FiniteState* pnewState);

	virtual bool Transition(FiniteState*& pnewState);

#if EDITOR
	virtual void CreateEngineUI();
#endif

	virtual void Serialize(nlohmann::json& data, FiniteStateMachine* pstateMachine);
	virtual void Deserialize(const nlohmann::json& data, FiniteStateMachine* pstateMachine);

	void SetName(std::string name);
	std::string GetName();

	void RemoveTransitions(FiniteState* pstate);

	void InitTransitions(FiniteState const* other, FiniteStateMachine* pmachine);

protected:

private:
	std::unordered_map<std::string, TransitionInfo<int>> m_intTransitionInfo;
	std::unordered_map<std::string, TransitionInfo<float>> m_floatTransitionInfo;
	std::unordered_map<std::string, TransitionInfo<bool>> m_boolTransitionInfo;

#if EDITOR
	std::string m_boolTransitionName = "";
	std::string m_floatTransitionName = "";
	std::string m_intTransitionName = "";

	std::string m_selectedTransitionName = "";

	std::string m_name = "";

	void CreateBoolTransitionUI();
	void CreateFloatTransitionUI();
	void CreateIntTransitionUI();
#endif
};

