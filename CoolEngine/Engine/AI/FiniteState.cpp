#include "FiniteState.h"
#include "Engine/AI/FiniteStateMachine.h"

bool FiniteState::SetBool(std::string name, bool value)
{
    if (m_boolTransitionInfo.count(name) == 0)
    {
        LOG("A bool with that name doesn't exist!");

        return false;
    }

    m_boolTransitionInfo[name].CurrentValue = value;

    return true;
}

bool FiniteState::SetFloat(std::string name, float value)
{
    if (m_floatTransitionInfo.count(name) == 0)
    {
        LOG("A float with that name doesn't exist!");

        return false;
    }

    m_floatTransitionInfo[name].CurrentValue = value;

    return true;
}

bool FiniteState::SetInt(std::string name, int value)
{
    if (m_intTransitionInfo.count(name) == 0)
    {
        LOG("An int with that name doesn't exist!");

        return false;
    }

    m_intTransitionInfo[name].CurrentValue = value;

    return true;
}

bool FiniteState::CreateBoolTransition(std::string name, bool transitionValue, bool initialValue, FiniteState* pnewState)
{
    if (m_boolTransitionInfo.count(name) != 0)
    {
        LOG("A bool condition with that name already exists!");

        return false;
    }

    TransitionInfo<bool> info;
    info.CurrentValue = initialValue;
    info.TransitionValue = transitionValue;
    info.NewState = pnewState;

    m_boolTransitionInfo[name] = info;

    return true;
}

bool FiniteState::CreateFloatTransition(std::string name, float transitionValue, float initialValue, FiniteState* pnewState)
{
    if (m_floatTransitionInfo.count(name) != 0)
    {
        LOG("A float with that name already exists!");

        return false;
    }

    TransitionInfo<float> info;
    info.CurrentValue = initialValue;
    info.TransitionValue = transitionValue;
    info.NewState = pnewState;

    m_floatTransitionInfo[name] = info;

    return true;
}

bool FiniteState::CreateIntTransition(std::string name, int transitionValue, int initialValue, FiniteState* pnewState)
{
    if (m_intTransitionInfo.count(name) != 0)
    {
        LOG("An int with that name already exists!");

        return false;
    }

    TransitionInfo<int> info;
    info.CurrentValue = initialValue;
    info.TransitionValue = transitionValue;
    info.NewState = pnewState;

    m_intTransitionInfo[name] = info;

    return true;
}

bool FiniteState::Transition(FiniteState*& pnewState)
{
    for (std::unordered_map<std::string, TransitionInfo<bool>>::iterator it = m_boolTransitionInfo.begin(); it != m_boolTransitionInfo.end(); ++it)
    {
        if (it->second.CurrentValue == it->second.TransitionValue)
        {
            pnewState = it->second.NewState;

            return true;
        }
    }

    for (std::unordered_map<std::string, TransitionInfo<int>>::iterator it = m_intTransitionInfo.begin(); it != m_intTransitionInfo.end(); ++it)
    {
        if (it->second.CurrentValue == it->second.TransitionValue)
        {
            pnewState = it->second.NewState;

            return true;
        }
    }

    for (std::unordered_map<std::string, TransitionInfo<float>>::iterator it = m_floatTransitionInfo.begin(); it != m_floatTransitionInfo.end(); ++it)
    {
        if (std::abs(it->second.CurrentValue - it->second.TransitionValue) <= FLT_EPSILON)
        {
            pnewState = it->second.NewState;

            return true;
        }
    }

    return false;
}

void FiniteState::Serialize(nlohmann::json& data, FiniteStateMachine* pstateMachine)
{
	for (std::unordered_map<std::string, TransitionInfo<bool>>::iterator it = m_boolTransitionInfo.begin(); it != m_boolTransitionInfo.end(); ++it)
	{
		data[it->first]["BoolTransitions"].push_back({ it->second.CurrentValue, it->second.TransitionValue, pstateMachine->GetStateName(it->second.NewState) });
	}

	for (std::unordered_map<std::string, TransitionInfo<int>>::iterator it = m_intTransitionInfo.begin(); it != m_intTransitionInfo.end(); ++it)
	{
		data[it->first]["IntTransitions"].push_back({ it->second.CurrentValue, it->second.TransitionValue, pstateMachine->GetStateName(it->second.NewState) });
	}

	for (std::unordered_map<std::string, TransitionInfo<float>>::iterator it = m_floatTransitionInfo.begin(); it != m_floatTransitionInfo.end(); ++it)
	{
		data[it->first]["FloatTransitions"].push_back({ it->second.CurrentValue, it->second.TransitionValue, pstateMachine->GetStateName(it->second.NewState) });
	}
}

void FiniteState::Deserialize(const nlohmann::json& data, FiniteStateMachine* pstateMachine)
{
	for (nlohmann::json::const_iterator it = data["BoolTransitions"].begin(); it != data["BoolTransitions"].end(); ++it)
	{
		TransitionInfo<bool> info;
		std::string stateName = it.value();
		info.CurrentValue = data["BoolTransitions"][stateName][0];
		info.TransitionValue = data["BoolTransitions"][stateName][1];
		info.NewState = pstateMachine->GetState(data["BoolTransitions"][stateName][2]);

		m_boolTransitionInfo[stateName] = info;
	}

	for (nlohmann::json::const_iterator it = data["IntTransitions"].begin(); it != data["IntTransitions"].end(); ++it)
	{
		TransitionInfo<int> info;
		std::string stateName = it.value();
		info.CurrentValue = data["IntTransitions"][stateName][0];
		info.TransitionValue = data["IntTransitions"][stateName][1];
		info.NewState = pstateMachine->GetState(data["IntTransitions"][stateName][2]);

		m_intTransitionInfo[stateName] = info;
	}

	for (nlohmann::json::const_iterator it = data["FloatTransitions"].begin(); it != data["FloatTransitions"].end(); ++it)
	{
		TransitionInfo<float> info;
		std::string stateName = it.value();
		info.CurrentValue = data["FloatTransitions"][stateName][0];
		info.TransitionValue = data["FloatTransitions"][stateName][1];
		info.NewState = pstateMachine->GetState(data["FloatTransitions"][stateName][2]);

		m_floatTransitionInfo[stateName] = info;
	}
}
