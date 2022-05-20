#pragma once
#include "Interactables.h"
#include "EInteractableType.h"
#include "ItemTypes.h"

class HealthConsum : public Interactables
{
protected:
	/// <summary>
	/// Health is a Consumable Interactable
	/// </summary>
	EINTERACTABLETYPE interactableType = EINTERACTABLETYPE::Consumable;

public:

	GameObject interactableGameObj;
	
	HealthConsum();
	~HealthConsum();
	void AddHealthConsum();


	/// <summary>
	/// Setter and Getter for the Type of Interactable Object
	/// </summary>
	/// <param name="type"></param>
	void SetType(EINTERACTABLETYPE type) { interactableType = type; } 
	EINTERACTABLETYPE GetType() { return interactableType; } 
};

