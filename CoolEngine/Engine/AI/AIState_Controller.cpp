#include "AIState_Controller.h"

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
	for (state = m_pAllStates.begin(); state != m_pAllStates.end(); state++)
	{
		//Returns true if the state is not found
		bool inactive = (m_pActiveStates.find(state->first) == m_pActiveStates.end());

		//if this current state should be active this update loop. Call through all the state triggers if more triggers are wanted (for example HP)
		if (state->second->CheckStateTrigger(StateTriggers::DistanceFromPlayer, distFromPlayer))
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
