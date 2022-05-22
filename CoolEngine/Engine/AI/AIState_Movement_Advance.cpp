#include "AIState_Movement_Advance.h"

//Advance state, causes the enemy to pathfind towards the player
StateAdvance::StateAdvance(EnemyGameObject* enemy, PlayerGameObject* player) : StateMovementBase(enemy, player)
{
	m_curState = enemyState::Advance;
}

void StateAdvance::ExecuteState()
{
	XMFLOAT3 playerPos = m_pPlayer->GetTransform()->GetPosition();
	XMFLOAT3 enemyPos = m_pEnemy->GetTransform()->GetPosition();

	//Generating a new path towards the player with each update loop
	Pathfinding::GetInstance()->FindPath(enemyPos, playerPos, newPath);

	StateMovementBase::ExecuteState();
}

void StateAdvance::OnStateEntry()
{
	StateMovementBase::OnStateEntry();
}

void StateAdvance::OnStateExit()
{
	StateMovementBase::OnStateExit();
}