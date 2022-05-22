#pragma once
#include "AIState_Attack_Melee.h"
#include "AIState_Attack_Ranged.h"
#include "AIState_Movement_Advance.h"
#include "AIState_Movement_Flee.h"
#include "AIState_Movement_Stationary.h"
#include "AIState_Movement_Wander.h"



class StateController
{
public:
	StateController(EnemyGameObject* enemy, PlayerGameObject* player);
	~StateController();
	void Update();


	map<enemyState, StateBase*> GetAllStates() { return m_pAllStates; }
	map<enemyState, StateBase*> GetAllActiveStates() { return m_pActiveStates; }

	void AddState(StateBase* state);

private:
	map<enemyState, StateBase*> m_pAllStates;
	map<enemyState, StateBase*> m_pActiveStates;

	EnemyGameObject* m_pEnemy;
	PlayerGameObject* m_pPlayer;

};
