#include "AIState.h"
#include "Engine/GameObjects/EnemyGameObject.h"
#include "Engine/Managers/GameManager.h"
#include "Pathfinding.h"

//Local Getter needed by some systems

XMFLOAT3 GetPlayerPos()
{

	string playerName = "Player";
	RenderableCollidableGameObject* pgameObject = GameManager::GetInstance()->GetGameObjectUsingIdentifier<PlayerGameObject>(playerName);
	//Gets the player's position from the GameManager, for usage within the states for movement / attacking
	return (GameManager::GetInstance()->GetGameObjectUsingIdentifier<PlayerGameObject>(string("Player")))->GetTransform()->GetPosition();
}


//---------------------------------------------------------------------------//
//
// State Controller
//
//---------------------------------------------------------------------------//

//There is no exclusively clause for movement, so the most recently added movement state will be the controller of movement but the other movement states will still process pathfinding. If bad performance, consider exclusivity. 
//Or fix movement states that require pathfinding

StateController::StateController(EnemyGameObject* enemy) : pEnemy(enemy)
{
	
}

StateController::~StateController()
{
}

void StateController::Update()
{

	XMFLOAT3 playerPos = GetPlayerPos();
	float distFromPlayer = MathHelper::Distance(playerPos, pEnemy->GetTransform()->GetPosition());


	//Looping all states to check if they should be active
	for (auto const& state : allStates)
	{
		//Returns true if the state is not found
		bool inactive = (activeStates.find(state.first) == activeStates.end());

		//if this current state should be active this update loop. Call through all the state triggers if more triggers are wanted (for example HP)
		if (state.second->CheckStateTrigger(StateTriggers::DistanceFromPlayer,distFromPlayer))
		{
			//if this state isnt active already, add to the active states list and trigger it's on-entry
			if (inactive)
			{
				activeStates.insert(state);
				state.second->OnStateEntry();
			}
		}
		else
		{
			//If this state is active, remove from the active list and trigger its on-exit
			if (inactive)
			{
				activeStates.erase(state.first);
				state.second->OnStateExit();
			}
		}
	}

	//Trigger all active states. This does involve some looping over already looped items but allows better debugging 
	for (auto const& state : activeStates)
	{
		state.second->ExecuteState();
	}
}

//Adds a state into the controller if the state key doesnt already exist, if it does exist then it won't be added to the list of states
void StateController::AddState(StateBase* state)
{
	allStates.insert({ state->GetState(), state });
}



//---------------------------------------------------------------------------//
//
// Base State
//
//---------------------------------------------------------------------------//
StateBase::StateBase(EnemyGameObject* enemy) : pEnemy(enemy)
{

}

StateBase::~StateBase()
{
}

bool StateBase::CheckStateTrigger(StateTriggers triggerType, float value)
{

	//Looping through the triggers in this state to check if this trigger type and value have a match to trigger this state
	for (int i = 0; i < triggers.size(); i++)
	{
		if (triggers[i]->reactionVariable == triggerType)
		{
			switch (triggers[i]->triggerWhen)
			{
			case StateTriggerValueAt::Above:
				if (triggers[i]->triggerValue < value)
				{
					return true;
				}
				break;
			case StateTriggerValueAt::Below:
				if (triggers[i]->triggerValue > value)
				{
					return true;
				}
				break;
			case StateTriggerValueAt::Equal:
				if (triggers[i]->triggerValue == value)
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

StateAttackBase::StateAttackBase(EnemyGameObject* enemy) : StateBase(enemy)
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
StateAttackMelee::StateAttackMelee(EnemyGameObject* enemy) : StateAttackBase(enemy)
{
	curState = enemyState::AttackMelee;
	isAttacking = true;
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
StateAttackRanged::StateAttackRanged(EnemyGameObject* enemy) : StateAttackBase(enemy)
{
	curState = enemyState::AttackRanged;
	isAttacking = true;
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

StateMovementBase::StateMovementBase(EnemyGameObject* enemy) : StateBase(enemy)
{

}

void StateMovementBase::ExecuteState()
{
	//Sets the new constructed path
	pEnemy->SetPath(newPath);
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
StateAdvance::StateAdvance(EnemyGameObject* enemy) : StateMovementBase(enemy)
{
	curState = enemyState::Advance;
}

void StateAdvance::ExecuteState()
{
	XMFLOAT3 playerPos = GetPlayerPos();
	XMFLOAT3 enemyPos = pEnemy->GetTransform()->GetPosition();

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
StateFlee::StateFlee(EnemyGameObject* enemy) : StateMovementBase(enemy)
{
	curState = enemyState::Flee;
}

void StateFlee::ExecuteState()
{
	XMFLOAT3 playerPos = GetPlayerPos();
	XMFLOAT3 enemyPos = pEnemy->GetTransform()->GetPosition();

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
	if (nodeToPath != pEnemy->GetPath()[0])
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
StateStationary::StateStationary(EnemyGameObject* enemy) : StateMovementBase(enemy)
{
	curState = enemyState::Stationary;
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

StateWander::StateWander(EnemyGameObject* enemy) : StateMovementBase(enemy)
{
	curState = enemyState::Wander;
}

void StateWander::ExecuteState()
{

	//if there is not currently a node for this state to go towards, then generate one
	if (pEnemy->GetPath().size() == 0)
	{
		XMFLOAT3 enemyPos = pEnemy->GetTransform()->GetPosition();
		node* closest = Pathfinding::GetInstance()->FindClosestNode(enemyPos);

		node* chosenNode;

		do
		{
			int random = rand() % (closest->m_pNeighbours.size() - 1);
			chosenNode = closest->m_pNeighbours[random];
		}while (!chosenNode->m_obstacle);


		//If the node we want to construct a path to isn't the next node that we want to path to
		Pathfinding::GetInstance()->FindPath(pEnemy->GetTransform()->GetPosition(), chosenNode->m_pos, newPath);
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

