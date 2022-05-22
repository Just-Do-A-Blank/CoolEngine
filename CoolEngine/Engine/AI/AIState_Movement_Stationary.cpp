#include "AIState_Movement_Stationary.h"

//Stationary state, clears the current path as the enemy doesnt want to move
StateStationary::StateStationary(EnemyGameObject* enemy, PlayerGameObject* player) : StateMovementBase(enemy, player)
{
	m_curState = enemyState::Stationary;
}

void StateStationary::ExecuteState()
{
	newPath.clear();
	StateMovementBase::ExecuteState();
}

void StateStationary::OnStateEntry()
{
	StateMovementBase::OnStateEntry();
}

void StateStationary::OnStateExit()
{
	StateMovementBase::OnStateExit();
}