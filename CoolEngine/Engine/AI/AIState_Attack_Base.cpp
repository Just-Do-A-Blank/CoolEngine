#include "AIState_Attack_Base.h"

StateAttackBase::StateAttackBase(EnemyGameObject* enemy, PlayerGameObject* player) : StateBase(enemy, player)
{

}

void StateAttackBase::ExecuteState()
{
	StateBase::ExecuteState();
}

void StateAttackBase::OnStateEntry()
{
	StateBase::OnStateEntry();
}

void StateAttackBase::OnStateExit()
{
	StateBase::OnStateExit();
}
