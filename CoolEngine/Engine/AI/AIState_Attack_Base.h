#pragma once
#include "AIState_Base.h"

class StateAttackBase : public StateBase
{
public:


	virtual void ExecuteState();
	virtual void OnStateEntry();
	virtual void OnStateExit();

protected:
	StateAttackBase(EnemyGameObject* enemy, PlayerGameObject* player);
	bool m_isAttacking;
};