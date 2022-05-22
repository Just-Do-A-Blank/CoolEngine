#include "AIState_Base.h"

//---------------------------------------------------------------------------//
//
// Base State
//
//---------------------------------------------------------------------------//
StateBase::StateBase(EnemyGameObject* enemy, PlayerGameObject* player) : m_pEnemy(enemy), m_pPlayer(player)
{

}

StateBase::~StateBase()
{

	for (int i = 0; i < m_pTriggers.size(); i++)
	{
		delete m_pTriggers[i];
	}
	m_pTriggers.clear();
}

bool StateBase::CheckStateTrigger(StateTriggers triggerType, float value)
{

	//Looping through the triggers in this state to check if this trigger type and value have a match to trigger this state
	for (int i = 0; i < m_pTriggers.size(); i++)
	{
		if (m_pTriggers[i]->reactionVariable == triggerType)
		{
			//If value passed in is below the Max and above the min (the trigger should trigger)
			if (m_pTriggers[i]->triggerMax > value &&
				m_pTriggers[i]->triggerMin < value)
			{
				return true;

			}
		}
	}

	return false;

}

void StateBase::AddTrigger(StateTrigger* trigger)
{
	m_pTriggers.push_back(trigger);
}

void StateBase::AddTrigger(StateTriggers ReactionVariable, float TriggerMin, float TriggerMax)
{
	m_pTriggers.push_back(new StateTrigger(ReactionVariable, TriggerMin,TriggerMax));
}

void StateBase::ExecuteState()
{
}

void StateBase::OnStateEntry()
{
}

void StateBase::OnStateExit()
{
}
