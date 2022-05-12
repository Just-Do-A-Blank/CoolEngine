#pragma once

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

DEFINE_ENUM_FLAG_OPERATORS(ELEMENTS);


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
static const ELEMENTS ElementResistantToDual[9] =
{
	ELEMENTS::NONE, ELEMENTS::WATERELECTRIC, ELEMENTS::ELECTRICPOISON, ELEMENTS::FIREWATER, ELEMENTS::POISONFIRE, ELEMENTS::FIREWATER, ELEMENTS::POISONFIRE, ELEMENTS::ELECTRICPOISON, ELEMENTS::WATERELECTRIC
};


static class DamageCalculation
{
	// Damage calculation when attack connects
	static float CalculateDamage(float weaponDamage, ELEMENTS weaponElement, ELEMENTS characterElement)
	{
		float multiplier = 1.0f;
		if (characterElement == ElementWeakTo[(int)weaponElement])
		{
			// Character weak to attack
			multiplier *= 2.0f;
			if ((int)characterElement >= (int)ELEMENTS::POISONFIRE)
			{
				// Dual element character is extra weak
				multiplier *= 2.0f;
			}
		}
		else if (characterElement == ElementResistantTo[(int)weaponElement])
		{
			// Character resistant to attack
			multiplier /= 2.0f;
			if ((int)characterElement >= (int)ELEMENTS::POISONFIRE)
			{
				// Dual element character is extra weak
				multiplier /= 2.0f;
			}
		}

		return (weaponDamage * multiplier);
	}
};