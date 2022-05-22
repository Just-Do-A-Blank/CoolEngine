#include "AIState_Movement_Wander.h"


//Wander state, causes the pathfinder to choose a random nearby node and move towards it, will only select a new node when there is no node to move towards currently

StateWander::StateWander(EnemyGameObject* enemy, PlayerGameObject* player) : StateMovementBase(enemy, player)
{
	m_curState = enemyState::Wander;
}

void StateWander::ExecuteState()
{

	//if there is not currently a node for this state to go towards, then generate one
	if (m_pEnemy->GetPath().size() == 0)
	{
		XMFLOAT3 enemyPos = m_pEnemy->GetTransform()->GetPosition();
		node* closest = Pathfinding::GetInstance()->FindClosestNode(enemyPos);

		node* chosenNode;

		do
		{
			int random = rand() % (closest->m_pNeighbours.size() - 1);
			chosenNode = closest->m_pNeighbours[random];
		} while (!chosenNode->m_obstacle);


		//If the node we want to construct a path to isn't the next node that we want to path to
		Pathfinding::GetInstance()->FindPath(m_pEnemy->GetTransform()->GetPosition(), chosenNode->m_pos, newPath);
		StateMovementBase::ExecuteState();
	}
}

void StateWander::OnStateEntry()
{
	StateMovementBase::OnStateEntry();
}

void StateWander::OnStateExit()
{
	StateMovementBase::OnStateExit();
}

