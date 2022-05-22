#pragma once
#include "AIState_Movement_Base.h"
class StateStationary : public StateMovementBase
{
public:
	StateStationary(EnemyGameObject* enemy, PlayerGameObject* player);

	void ExecuteState();
	void OnStateEntry();
	void OnStateExit();

};