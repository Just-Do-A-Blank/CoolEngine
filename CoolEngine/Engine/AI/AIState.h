#pragma once
#include <vector>
#include <string>
#include <map>

class PlayerGameObject;
class EnemyGameObject;
struct node;

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


enum class StateTriggerValueAt
{
	Above,
	Below,
	Equal
};

enum class StateTriggers
{
	DistanceFromPlayer,
};

struct StateTrigger 
{
	StateTriggers reactionVariable;
	float checkValue;
	float triggerValue;
	StateTriggerValueAt triggerWhen;


	StateTrigger(StateTriggers ReactionVariable, float valueToTrigger,float TriggerValueWhen, StateTriggerValueAt TriggerWhen)
	{
		reactionVariable = ReactionVariable;
		checkValue = valueToTrigger;
		triggerValue = TriggerValueWhen;
		triggerWhen = TriggerWhen;
	}

};

class StateBase
{
public:
	StateBase(EnemyGameObject* enemy, PlayerGameObject* player);
	~StateBase();

	bool CheckStateTrigger(StateTriggers triggerType, float value);



	enemyState GetState() { return m_curState; }
	void OnStateEntry();
	void OnStateExit();
	void ExecuteState();

protected:

	enemyState m_curState;
	vector<StateTrigger*> m_pTriggers;


	//Pointer to the enemy this state system is part of
	EnemyGameObject* m_pEnemy;
	PlayerGameObject* m_pPlayer;
};

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
	map<enemyState,StateBase*> m_pAllStates;
	map<enemyState,StateBase*> m_pActiveStates;

	EnemyGameObject* m_pEnemy;
	PlayerGameObject* m_pPlayer;

};





//Attack states
class StateAttackBase : public StateBase
{
public:


	void ExecuteState();
	void OnStateEntry();
	void OnStateExit();

protected:
	StateAttackBase(EnemyGameObject* enemy, PlayerGameObject* player);
	bool m_isAttacking;
};

class StateAttackMelee : public StateAttackBase
{
public:

	StateAttackMelee(EnemyGameObject* enemy, PlayerGameObject* player);

	void ExecuteState();
	void OnStateEntry();
	void OnStateExit();


};

class StateAttackRanged : public StateAttackBase
{
public:

	StateAttackRanged(EnemyGameObject* enemy, PlayerGameObject* player);

	void ExecuteState();
	void OnStateEntry();
	void OnStateExit();


};

class StateAbility : public StateAttackBase
{
	StateAbility(EnemyGameObject* enemy, PlayerGameObject* player);

	void ExecuteState();
	void OnStateEntry();
	void OnStateExit();


};




//Movement controlling states
class StateMovementBase : public StateBase
{
protected:
	StateMovementBase(EnemyGameObject* enemy, PlayerGameObject* player);

	vector<node*> newPath;

	void ExecuteState();
	void OnStateEntry();
	void OnStateExit();



};


class StateAdvance : public StateMovementBase
{
public:
	StateAdvance(EnemyGameObject* enemy, PlayerGameObject* player);

	void ExecuteState();
	void OnStateEntry();
	void OnStateExit();


};

class StateFlee : public StateMovementBase
{
public: 
	StateFlee(EnemyGameObject* enemy, PlayerGameObject* player);

	void ExecuteState();
	void OnStateEntry();
	void OnStateExit();


};

class StateStationary : public StateMovementBase
{
public:
	StateStationary(EnemyGameObject* enemy, PlayerGameObject* player);

	void ExecuteState();
	void OnStateEntry();
	void OnStateExit();

};

class StateWander : public StateMovementBase
{
public:
	StateWander(EnemyGameObject* enemy, PlayerGameObject* player);

	void ExecuteState();
	void OnStateEntry();
	void OnStateExit();

};

