#pragma once
#include <vector>
#include <string>
#include <map>

using namespace std;

class EnemyGameObject;

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

};

class StateBase
{
public:
	StateBase(EnemyGameObject* enemy);
	~StateBase();

	bool CheckStateTrigger();

	enemyState GetState() { return curState; }
	void OnStateEntry();
	void OnStateExit();
	void ExecuteState();

protected:

	enemyState curState;
	XMFLOAT3 GetPlayerPos();
	StateTrigger trigger;


	//Pointer to the enemy this state system is part of
	EnemyGameObject* pEnemy;
};



//Attack states
class StateAttackBase : public StateBase
{
public:

	bool CheckStateTrigger();
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

	bool CheckStateTrigger();
};

class StateAttackRanged : public StateAttackBase
{
public:

	StateAttackRanged(EnemyGameObject* enemy);

	void ExecuteState();
	void OnStateEntry();
	void OnStateExit();

	bool CheckStateTrigger();
};

class StateAbility : public StateAttackBase
{
	StateAbility(EnemyGameObject* enemy);

	void ExecuteState();
	void OnStateEntry();
	void OnStateExit();

	bool CheckStateTrigger();
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

	bool CheckStateTrigger();

};


class StateAdvance : public StateMovementBase
{
public:
	StateAdvance(EnemyGameObject* enemy);

	void ExecuteState();
	void OnStateEntry();
	void OnStateExit();

	bool CheckStateTrigger();
};

class StateFlee : public StateMovementBase
{
public: 
	StateFlee(EnemyGameObject* enemy);

	void ExecuteState();
	void OnStateEntry();
	void OnStateExit();

	bool CheckStateTrigger();
};

class StateStationary : public StateMovementBase
{
public:
	StateStationary(EnemyGameObject* enemy);

	void ExecuteState();
	void OnStateEntry();
	void OnStateExit();

	bool CheckStateTrigger();
};

class StateWander : public StateMovementBase
{
public:
	StateWander(EnemyGameObject* enemy);

	void ExecuteState();
	void OnStateEntry();
	void OnStateExit();

	bool CheckStateTrigger();
};

