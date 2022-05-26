#include "EnemyStateMachine.h"
#include "FuzzyStateMachine.h"
#include "Engine/AI/States/FuzzyState.h"

EnemyStateMachine::EnemyStateMachine() : FuzzyStateMachine()
{

}

void EnemyStateMachine::Deserialize(const nlohmann::json& data)
{
}

bool EnemyStateMachine::IsStateActive(FuzzyStateType type) const
{
	for (int i = 0; i < m_activeStates.size(); ++i)
	{
		if (m_activeStates[i]->GetStateType() == type)
		{
			return true;
		}
	}

	return false;
}
