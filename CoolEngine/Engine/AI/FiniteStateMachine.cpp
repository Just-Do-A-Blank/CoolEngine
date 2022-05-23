#include "FiniteStateMachine.h"
#include "Engine/AI/States/FiniteState.h"
#include "Engine/EditorUI/EditorUI.h"

FiniteStateMachine::~FiniteStateMachine()
{
	for (std::unordered_map<std::string, FiniteState*>::iterator it = m_states.begin(); it != m_states.end(); ++it)
	{
		delete it->second;
		it->second = nullptr;
	}
}

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

void FiniteStateMachine::SetActiveState(std::string stateName)
{
	if (m_states.count(stateName) == 0)
	{
		LOG("Tried to set the active state to a state that doesn't exist!");

		return;
	}

	if (m_pstate != nullptr)
	{
		m_pstate->Exit();
	}

	m_pstate = m_states[stateName];
	m_pstate->Enter();
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

	for (std::unordered_map<std::string, FiniteState*>::iterator it = m_states.begin(); it != m_states.end(); ++it)
	{
		it->second->RemoveTransitions(m_states[stateName]);
	}

	if (m_pstate == m_states[stateName])
	{
		m_pstate = nullptr;
	}

	delete m_states[stateName];
	m_states[stateName] = nullptr;

    m_states.erase(stateName);

    return true;
}

void FiniteStateMachine::Update()
{
	if (m_pstate != nullptr)
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
}

std::string FiniteStateMachine::GetStateName(FiniteState* pstate)
{
	for (std::unordered_map<std::string, FiniteState*>::iterator it = m_states.begin(); it != m_states.end(); ++it)
	{
		if (it->second == pstate)
		{
			return it->first;
		}
	}

	return "";
}

const std::unordered_map<std::string, FiniteState*>* FiniteStateMachine::GetStates()
{
	return &m_states;
}
