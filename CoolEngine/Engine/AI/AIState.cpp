#include "AIState.h"
#include "Engine/GameObjects/EnemyGameObject.h"
#include "Engine/Managers/GameManager.h"
#include "Pathfinding.h"
//---------------------------------------------------------------------------//
//
// State Controller
//
//---------------------------------------------------------------------------//

//There is no exclusively clause for movement, so the most recently added movement state will be the controller of movement but the other movement states will still process pathfinding. If bad performance, consider exclusivity. 
//Or fix movement states that require pathfinding

StateController::StateController(EnemyGameObject* enemy, PlayerGameObject* player) : m_pEnemy(enemy), m_pPlayer(player)
{
	
}

StateController::~StateController()
{
}

void StateController::Update()
{

	float distFromPlayer = MathHelper::Distance(m_pPlayer->GetTransform()->GetPosition(), m_pEnemy->GetTransform()->GetPosition());

	//To avoid using auto
	map<enemyState, StateBase*>::iterator state;

	//Looping all states to check if they should be active
	for (state = m_pAllStates.begin(); state != m_pAllStates.end();state++)
	{
		//Returns true if the state is not found
		bool inactive = (m_pActiveStates.find(state->first) == m_pActiveStates.end());

		//if this current state should be active this update loop. Call through all the state triggers if more triggers are wanted (for example HP)
		if (state->second->CheckStateTrigger(StateTriggers::DistanceFromPlayer,distFromPlayer))
		{
			//if this state isnt active already, add to the active states list and trigger it's on-entry
			if (inactive)
			{
				m_pActiveStates.insert(*state);
				state->second->OnStateEntry();
			}
		}
		else
		{
			//If this state is active, remove from the active list and trigger its on-exit
			if (inactive)
			{
				m_pActiveStates.erase(state->first);
				state->second->OnStateExit();
			}
		}
	}
	
	//Trigger all active states. This does involve some looping over already looped items but allows better debugging 
	for (state = m_pActiveStates.begin(); state != m_pActiveStates.end(); state++)
	{
		state->second->ExecuteState();
	}
}

//Adds a state into the controller if the state key doesnt already exist, if it does exist then it won't be added to the list of states
void StateController::AddState(StateBase* state)
{
	m_pAllStates.insert({ state->GetState(), state });
}



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
			switch (m_pTriggers[i]->triggerWhen)
			{
			case StateTriggerValueAt::Above:
				if (m_pTriggers[i]->triggerValue < value)
				{
					return true;
				}
				break;
			case StateTriggerValueAt::Below:
				if (m_pTriggers[i]->triggerValue > value)
				{
					return true;
				}
				break;
			case StateTriggerValueAt::Equal:
				if (m_pTriggers[i]->triggerValue == value)
				{
					return true;
				}
				break;
			}
		}
	}

	return false;

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



//---------------------------------------------------------------------------//
// 
//Attacking states
// 
//---------------------------------------------------------------------------//

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

//---------------------------------------------------------------------------//
// 
//Movement States
// 
//---------------------------------------------------------------------------//

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
		}while (!chosenNode->m_obstacle);


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

