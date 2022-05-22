#pragma once
#include "AIState_Movement_Base.h"

class StateWander : public StateMovementBase
{
public:
	StateWander(EnemyGameObject* enemy, PlayerGameObject* player);

	void ExecuteState();
	void OnStateEntry();
	void OnStateExit();

};
