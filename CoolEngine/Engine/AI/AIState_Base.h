#pragma once
#include <vector>
#include <string>
#include <map>

#include "Engine/GameObjects/EnemyGameObject.h"
#include "Engine/Managers/GameManager.h"
#include "Pathfinding.h"


enum class enemyState
{
	AttackMelee,
	AttackRanged,
	Ability,
	Advance,
	Flee,
	Stationary,
	Wander
};


enum class StateTriggers
{
	DistanceFromPlayer,
};

struct StateTrigger
{
	StateTriggers reactionVariable;
	float triggerMin;
	float triggerMax;

	//Use Int_max / Int_min to range the number to infinity (always trigger when below / above)
	StateTrigger(StateTriggers ReactionVariable, float TriggerMin, float TriggerMax)
	{
		reactionVariable = ReactionVariable;
		triggerMin = TriggerMin;
		triggerMax = TriggerMax;
	}
};

class StateBase
{
public:


	bool CheckStateTrigger(StateTriggers triggerType, float value);

	void AddTrigger(StateTrigger* trigger);
	void AddTrigger(StateTriggers ReactionVariable, float TriggerMin,float TriggerMax);

	enemyState GetState() { return m_curState; }
	virtual void OnStateEntry();
	virtual void OnStateExit();
	virtual void ExecuteState();

protected:
	StateBase(EnemyGameObject* enemy, PlayerGameObject* player);
	~StateBase();

	enemyState m_curState;
	vector<StateTrigger*> m_pTriggers;


	//Pointer to the enemy this state system is part of
	EnemyGameObject* m_pEnemy;
	PlayerGameObject* m_pPlayer;
};