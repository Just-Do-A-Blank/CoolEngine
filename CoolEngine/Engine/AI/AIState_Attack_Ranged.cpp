#include "AIState_Attack_Ranged.h"

//Ranged Attack State, causes the enemy to fire its weapon
StateAttackRanged::StateAttackRanged(EnemyGameObject* enemy, PlayerGameObject* player) : StateAttackBase(enemy, player)
{
	m_curState = enemyState::AttackRanged;
	m_isAttacking = true;
}

void StateAttackRanged::ExecuteState()
{
	//Use respective weapon
	StateAttackBase::ExecuteState();
}

void StateAttackRanged::OnStateEntry()
{
	StateAttackBase::OnStateEntry();
}

void StateAttackRanged::OnStateExit()
{
	StateAttackBase::OnStateExit();
}
