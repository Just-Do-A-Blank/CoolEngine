#pragma once
#include "AIState_Attack_Base.h"

class StateAttackMelee : public StateAttackBase
{
public:

	StateAttackMelee(EnemyGameObject* enemy, PlayerGameObject* player);

	void ExecuteState();
	void OnStateEntry();
	void OnStateExit();


};