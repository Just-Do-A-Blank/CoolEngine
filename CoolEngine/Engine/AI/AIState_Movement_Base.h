#pragma once
#include "AIState_Base.h"

//Movement controlling states
class StateMovementBase : public StateBase
{
protected:
	StateMovementBase(EnemyGameObject* enemy, PlayerGameObject* player);

	vector<node*> newPath;

	virtual void ExecuteState();
	virtual void OnStateEntry();
	virtual void OnStateExit();



};