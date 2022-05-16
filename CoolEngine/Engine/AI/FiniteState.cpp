#include "FiniteState.h"

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
