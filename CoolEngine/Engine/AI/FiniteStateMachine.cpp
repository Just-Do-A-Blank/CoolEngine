#include "FiniteStateMachine.h"
#include "FiniteState.h"

FiniteState* FiniteStateMachine::GetState(std::string stateName)
{
    if (m_states.count(stateName) == 0)
    {
        LOG("Tried to get a state that doesn't exist!");

        return nullptr;
    }

    return m_states[stateName];
}

FiniteState* FiniteStateMachine::GetActiveState()
{
    return m_pstate;
}

bool FiniteStateMachine::AddState(std::string stateName, FiniteState* pstate)
{
    if (m_states.count(stateName) != 0)
    {
        LOG("Tried to get a state that doesn't exist!");

        return false;
    }

    m_states[stateName] = pstate;

    return true;
}

bool FiniteStateMachine::RemoveState(std::string stateName)
{
    if (m_states.count(stateName) == 0)
    {
        LOG("Tried to remove a state that doesn't exist!");

        return false;
    }

    m_states.erase(stateName);

    return true;
}

void FiniteStateMachine::Update()
{
    m_pstate->Update();

    FiniteState* pstate = nullptr;

    if (m_pstate->Transition(pstate) == true)
    {
        m_pstate->Exit();

        m_pstate = pstate;

        m_pstate->Enter();
    }
}
