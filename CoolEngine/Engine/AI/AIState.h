#pragma once
#include <vector>
#include <string>
#include <map>

using namespace std;

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
	StateBase(EnemyGameObject* enemy);
	~StateBase();

	bool CheckStateTrigger(StateTriggers triggerType, float value);



	enemyState GetState() { return curState; }
	void OnStateEntry();
	void OnStateExit();
	void ExecuteState();

protected:

	enemyState curState;
	vector<StateTrigger*> triggers;


	//Pointer to the enemy this state system is part of
	EnemyGameObject* pEnemy;
};

class StateController
{
public:
	StateController(EnemyGameObject* enemy);
	~StateController();
	void Update();


	map<enemyState, StateBase*> GetAllStates() { return allStates; }
	map<enemyState, StateBase*> GetAllActiveStates() { return activeStates; }

	void AddState(StateBase* state);

private:
	map<enemyState,StateBase*> allStates;
	map<enemyState,StateBase*> activeStates;

	EnemyGameObject* pEnemy;

};





//Attack states
class StateAttackBase : public StateBase
{
public:


	void ExecuteState();
	void OnStateEntry();
	void OnStateExit();

protected:
	StateAttackBase(EnemyGameObject* enemy);
	bool isAttacking;
};

class StateAttackMelee : public StateAttackBase
{
public:

	StateAttackMelee(EnemyGameObject* enemy);

	void ExecuteState();
	void OnStateEntry();
	void OnStateExit();


};

class StateAttackRanged : public StateAttackBase
{
public:

	StateAttackRanged(EnemyGameObject* enemy);

	void ExecuteState();
	void OnStateEntry();
	void OnStateExit();


};

class StateAbility : public StateAttackBase
{
	StateAbility(EnemyGameObject* enemy);

	void ExecuteState();
	void OnStateEntry();
	void OnStateExit();


};




//Movement controlling states
class StateMovementBase : public StateBase
{
protected:
	StateMovementBase(EnemyGameObject* enemy);

	vector<node*> newPath;

	void ExecuteState();
	void OnStateEntry();
	void OnStateExit();



};


class StateAdvance : public StateMovementBase
{
public:
	StateAdvance(EnemyGameObject* enemy);

	void ExecuteState();
	void OnStateEntry();
	void OnStateExit();


};

class StateFlee : public StateMovementBase
{
public: 
	StateFlee(EnemyGameObject* enemy);

	void ExecuteState();
	void OnStateEntry();
	void OnStateExit();


};

class StateStationary : public StateMovementBase
{
public:
	StateStationary(EnemyGameObject* enemy);

	void ExecuteState();
	void OnStateEntry();
	void OnStateExit();

};

class StateWander : public StateMovementBase
{
public:
	StateWander(EnemyGameObject* enemy);

	void ExecuteState();
	void OnStateEntry();
	void OnStateExit();

};

