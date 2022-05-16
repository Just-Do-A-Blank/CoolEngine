#pragma once
#include <unordered_map>

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

protected:

private:
	std::unordered_map<std::string, TransitionInfo<int>> m_intTransitionInfo;
	std::unordered_map<std::string, TransitionInfo<float>> m_floatTransitionInfo;
	std::unordered_map<std::string, TransitionInfo<bool>> m_boolTransitionInfo;
};

