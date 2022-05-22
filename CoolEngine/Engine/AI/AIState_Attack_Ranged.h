#pragma once
#include "AIState_Attack_Base.h"

class StateAttackRanged : public StateAttackBase
{
public:

	StateAttackRanged(EnemyGameObject* enemy, PlayerGameObject* player);

	void ExecuteState();
	void OnStateEntry();
	void OnStateExit();


};