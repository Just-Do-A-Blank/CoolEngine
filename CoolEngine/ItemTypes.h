#pragma once
#include "Engine/GameObjects/Gameplay/Interactables/Interactables.h"
#include "Engine/GameObjects/Gameplay/Interactables/EInteractableType.h"

enum class CONSUMABLES
{
	HEALTH = 0,
	STAMINA = 1,
	SE_BOOSTER = 2,
	KEY = 3,
	POTION = 4

};

enum class NONCONSUMABLES
{
	WEAPON = 0,
	CURRENCY = 1,
};
class ItemTypes : Interactables
{
public:
	EINTERACTABLETYPE interactableType;

	void AddEffect();
protected:
	/// <summary>
	/// Setter and Getter for the Type of Interactable Object
	/// </summary>
	/// <param name="type"></param>
	virtual void SetType(EINTERACTABLETYPE type) { interactableType = type; }
	virtual EINTERACTABLETYPE GetType() { return interactableType; }
};

