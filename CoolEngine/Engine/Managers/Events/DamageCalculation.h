#pragma once
#include "Engine/Managers/Events/EventObserver.h"
#include "Engine/Managers/Events/CollisionEvents.h"

enum class ELEMENTS
{
	NONE = 0,
	FIRE = 1,
	WATER = 2,
	POISON = 3,
	ELECTRIC = 4,
	POISONFIRE = 5,
	ELECTRICPOISON = 6,
	WATERELECTRIC = 7,
	FIREWATER = 8,
	COUNT
};

enum class STATUSES
{
	NONE = 0,
	BLEEDING = 1,
	WEAKNESS = 2,
	CONFUSION = 3,
	ADRENALINE = 4,
	CHARM = 5,
	COUNT
};

enum class ELEMENTALSTATUSES
{
	NONE = 0,
	BURN = 1,
	DRENCH = 2,
	POISONED = 3,
	PARALYSIS = 4,
	COUNT
};

#define INVINCIBLE_TIME 1.0f


// e.g. The element weak to a fire attack is poison
// 0 - 0 and 1 - 0
static const ELEMENTS ElementWeakTo[9] =
{
	ELEMENTS::NONE, ELEMENTS::POISON, ELEMENTS::FIRE, ELEMENTS::ELECTRIC, ELEMENTS::WATER, ELEMENTS::ELECTRIC, ELEMENTS::WATER, ELEMENTS::FIRE, ELEMENTS::POISON
};

// e.g. The dual element weak to a fire attack is electric poison
// 0 - 1 and 1 - 1
static const ELEMENTS DualElementWeakTo[9] =
{
	ELEMENTS::NONE, ELEMENTS::ELECTRICPOISON, ELEMENTS::POISONFIRE, ELEMENTS::WATERELECTRIC, ELEMENTS::FIREWATER, ELEMENTS::FIREWATER, ELEMENTS::POISONFIRE, ELEMENTS::ELECTRICPOISON, ELEMENTS::WATERELECTRIC
};

// e.g. The element resistant to a fire attack is water
static const ELEMENTS ElementResistantTo[9] =
{
	ELEMENTS::NONE, ELEMENTS::WATER, ELEMENTS::ELECTRIC, ELEMENTS::FIRE, ELEMENTS::POISON, ELEMENTS::FIRE, ELEMENTS::POISON, ELEMENTS::ELECTRIC, ELEMENTS::WATER
};

// e.g. The dual element resistant to a fire attack is water electric
static const ELEMENTS DualElementResistantTo[9] =
{
	ELEMENTS::NONE, ELEMENTS::WATERELECTRIC, ELEMENTS::ELECTRICPOISON, ELEMENTS::FIREWATER, ELEMENTS::POISONFIRE, ELEMENTS::FIREWATER, ELEMENTS::POISONFIRE, ELEMENTS::ELECTRICPOISON, ELEMENTS::WATERELECTRIC
};


class DamageCalculation : public Observer
{
public:
	// Damage calculation when attack connects
	static float CalculateDamage(float weaponDamage, ELEMENTS weaponElement, ELEMENTS characterElement, ELEMENTALSTATUSES characterStatus);

	void TriggerHold(TriggerHoldEvent* e);
	void CollisionHold(CollisionHoldEvent* e);
	
	void Handle(Event* e) override;

	DamageCalculation();
	~DamageCalculation();
};