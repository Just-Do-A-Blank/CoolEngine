#include "AIState_Movement_Flee.h"

//Flee State, causes the enemy to pathfind away from the player
StateFlee::StateFlee(EnemyGameObject* enemy, PlayerGameObject* player) : StateMovementBase(enemy, player)
{
	m_curState = enemyState::Flee;
}

void StateFlee::ExecuteState()
{
	XMFLOAT3 playerPos = m_pPlayer->GetTransform()->GetPosition();
	XMFLOAT3 enemyPos = m_pEnemy->GetTransform()->GetPosition();

	//Set path to be the furthest node from the player
	node* closestNode = Pathfinding::GetInstance()->FindClosestNode(enemyPos);
	node* nodeToPath = nullptr;
	float distToFurthest = INT64_MIN;

	for (int i = 0; closestNode->m_pNeighbours.size(); i++)
	{
		//Skip obstacles
		if (closestNode->m_pNeighbours[i]->m_obstacle)
		{
			continue;
		}

		float curDist = MathHelper::Distance(closestNode->m_pNeighbours[i]->m_pos, playerPos);

		if (curDist > distToFurthest)
		{
			distToFurthest = curDist;
			nodeToPath = closestNode->m_pNeighbours[i];
		}
	}


	//If the node we want to construct a path to isn't the next node that we want to path to
	if (nodeToPath != m_pEnemy->GetPath()[0])
	{
		Pathfinding::GetInstance()->FindPath(enemyPos, nodeToPath->m_pos, newPath);
		StateMovementBase::ExecuteState();
	}

	//if it is then continue as usual, avoids constructing a new path, the constructed path will almost always be of size 1. Possible to change but this is super simple to get the enemy to run in the opposite direction of the player (or an available node)



}

void StateFlee::OnStateEntry()
{
	StateMovementBase::OnStateEntry();
}

void StateFlee::OnStateExit()
{
	StateMovementBase::OnStateExit();
}