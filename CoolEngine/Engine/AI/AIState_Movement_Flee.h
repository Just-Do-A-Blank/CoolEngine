#pragma once
#include "AIState_Movement_Base.h"
class StateFlee : public StateMovementBase
{
public:
	StateFlee(EnemyGameObject* enemy, PlayerGameObject* player);

	void ExecuteState();
	void OnStateEntry();
	void OnStateExit();


};