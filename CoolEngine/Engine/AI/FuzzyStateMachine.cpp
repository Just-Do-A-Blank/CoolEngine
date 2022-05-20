#include "FuzzyStateMachine.h"
#include "Engine/AI/FuzzyState.h"

FuzzyStateMachine::~FuzzyStateMachine()
{
	for (int i = 0; i < m_states.size(); ++i)
	{
		delete m_states[i];
		m_states[i] = nullptr;
	}
}

bool FuzzyStateMachine::AddState(FuzzyState* pstate)
{
	m_states.push_back(pstate);

	return true;
}

void FuzzyStateMachine::Update()
{
	std::vector<FuzzyState*> previousActiveStates = m_activeStates;
	std::vector<FuzzyState*> inactiveStates;

	m_activeStates.clear();

	//Check which states are active
	for (int i = 0; i < m_states.size(); ++i)
	{
		if (m_states[i]->CalculateActivation() >= m_states[i]->GetActivationLevel())
		{
			m_activeStates.push_back(m_states[i]);
		}
		else
		{
			inactiveStates.push_back(m_states[i]);
		}
	}

	//Check if states need to be exited
	for (int i = 0; i < previousActiveStates.size(); ++i)
	{
		for (int j = 0; j < inactiveStates.size(); ++j)
		{
			if (previousActiveStates[i] == inactiveStates[j])
			{
				previousActiveStates[i]->Exit();
			}
		}
	}

	//Check if states need to be entered
	for (int i = 0; i < m_activeStates.size(); ++i)
	{
		bool needEnter = true;

		for (int j = 0; j < previousActiveStates.size(); ++j)
		{
			if (m_activeStates[i] == previousActiveStates[j])
			{
				needEnter = false;
				break;
			}
		}

		if (needEnter == true)
		{
			m_activeStates[i]->Enter();
		}
	}

	//Update active states
	for (int i = 0; i < m_activeStates.size(); ++i)
	{
		m_activeStates[i]->Update();
	}
}

void FuzzyStateMachine::Serialize(nlohmann::json& data)
{
	for (int i = 0; i < m_states.size(); ++i)
	{
		m_states[i]->Serialize(data);
	}
}

const std::vector<FuzzyState*>* FuzzyStateMachine::GetStates()
{
	return &m_states;
}

const std::vector<FuzzyState*>* FuzzyStateMachine::GetActiveStates()
{
	return &m_activeStates;
}
