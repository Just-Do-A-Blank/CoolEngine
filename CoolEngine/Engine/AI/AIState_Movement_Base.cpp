#include "AIState_Movement_Base.h"

StateMovementBase::StateMovementBase(EnemyGameObject* enemy, PlayerGameObject* player) : StateBase(enemy, player)
{

}

void StateMovementBase::ExecuteState()
{
	//Sets the new constructed path
	m_pEnemy->SetPath(newPath);
	StateBase::ExecuteState();
}

void StateMovementBase::OnStateEntry()
{
	StateBase::OnStateEntry();
}

void StateMovementBase::OnStateExit()
{
	StateBase::OnStateExit();
}