#pragma once
#include <vector>
#include <string>
#include <map>

using namespace std;

class EnemyGameObject;

enum UnitClassType
{
	Avoid,
	Stationary,
	Aggressive
};

enum unitReaction
{
	Away,
	Towards,
	None
};

class UnitClass
{
public:

	void Update();

	UnitClass(EnemyGameObject* enemy, UnitClassType unitClass, bool attacker);

	//Getters / Setters for max/min ranges
	void SetDesiredRangeFromPlayer(float min, float max);
	void SetMinimumDistanceFromPlayer(float min);
	void SetMaximumDistanceFromPlayer(float max);

	float GetMinimumDistanceFromPlayer() { return minimumDistance; }
	float GetMaximumDistanceFromPlayer() { return maximumDistance; }



protected:
	//Sets if the current Unit should be performing it's attacks
	bool canAttack;

	//Distance from player that the enemy wishes to be from the player. Default will be based on weapon type. Can be changed with setters.
	float minimumDistance;
	float maximumDistance;
	float perfectDistance;

	UnitClassType UnitType;

	EnemyGameObject* pEnemy;

	unitReaction lastReaction;
	unitReaction curReaction;

};

class UnitClassPassive : public UnitClass
{
private:


public:
	UnitClassPassive(EnemyGameObject* enemy, UnitClassType type);

	void Update();
};

class UnitClassPassiveAvoid : public UnitClassPassive
{
public:
	UnitClassPassiveAvoid(EnemyGameObject* enemy);

	void Update();
};

class UnitClassPassiveStationary : public UnitClassPassive
{
public:
	UnitClassPassiveStationary(EnemyGameObject* enemy);

	void Update();
};



class UnitClassAggressive : public UnitClass
{
public:
	UnitClassAggressive(EnemyGameObject* enemy);
	UnitClassAggressive* GetCurrentState() { return this; }
};






class UnitClassMixed : public UnitClass
{
private:



public:
	UnitClassMixed(EnemyGameObject* enemy);
	UnitClassMixed* GetCurrentState() { return this; }


};