#pragma once
#include "Interactables.h"
#include "EInteractableType.h"

class HealthConsum : public Interactables
{
	/// <summary>
	/// A Class that Holds all Consumable Items E.G (Health Drops)
	/// </summary>
public:
	EINTERACTABLETYPE interactableType;

	//HealthConsum(CharacterGameObject playerCharacterObj);
	~HealthConsum();
	void AddHealthConsum();
};

