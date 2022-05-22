#pragma once
#include "AIState_Movement_Base.h"

class StateAdvance : public StateMovementBase
{
public:
	StateAdvance(EnemyGameObject* enemy, PlayerGameObject* player);

	void ExecuteState();
	void OnStateEntry();
	void OnStateExit();


};
