#include "AIState_Attack_Melee.h"

//Melee Attack state, causes the enemy to use its melee weapon
StateAttackMelee::StateAttackMelee(EnemyGameObject* enemy, PlayerGameObject* player) : StateAttackBase(enemy, player)
{
	m_curState = enemyState::AttackMelee;
	m_isAttacking = true;
}

void StateAttackMelee::ExecuteState()
{
	//Use respective weapon
	StateAttackBase::ExecuteState();
}

void StateAttackMelee::OnStateEntry()
{
	StateAttackBase::OnStateEntry();
}

void StateAttackMelee::OnStateExit()
{
	StateAttackBase::OnStateExit();
}