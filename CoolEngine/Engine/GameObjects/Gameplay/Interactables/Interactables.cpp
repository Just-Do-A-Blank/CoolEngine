#include "Interactables.h"
#include <Engine/Managers/Events/CollisionEvents.h>
#include <Engine/GameObjects/TriggerableGameObject.h>
#include <Engine/GameObjects/InteractableGameObject.h>
#include <Engine/GameObjects/PlayerGameObject.h>
#include "Engine/Managers/Events/EventManager.h"
#include "HealthConsum.h"
#include "ItemTypes.h

/// <summary>
/// interactableObjType -> INTERACTABLE && interactableType -> Consumable (For testing purposes)
/// </summary>
/// <param name="interactableType"></param>
Interactables::Interactables(GameObjectType interactableObjType, EINTERACTABLETYPE interactableType)
{
	//Sets the Object type to PICKUP
	m_gameObjectType |= GameObjectType::PICKUP;


	m_pPlayerGameObj = m_GameManager.GetGameObjectUsingIdentifier<CharacterGameObject>(string("Player"));

}

void Interactables::OnTriggerHold(GameObject* pObject, GameObject* pObject2)
{
	if (pObject->ContainsType(GameObjectType::PLAYER) && pObject2->ContainsType(GameObjectType::PICKUP))
	{
		//Casts the Pickup Obj to an Interactable
		Interactables* pInteractables = dynamic_cast<Interactables*>(pObject2);
		//Returns the Item Type
		EINTERACTABLETYPE interactableType = pInteractables->GetInteractableType();
		//Casts the GameObject to the CharacterGameObject
		CharacterGameObject* pCharacter = dynamic_cast<CharacterGameObject*>(pObject);

		//Passes the Interactable Type and the Character
		HandleInteractableEvent(interactableType, pCharacter);

	}

}

void Interactables::Handle(Event* e)
{
	TriggerableGameObject::Handle(e);
}


Interactables::~Interactables()
{

}

EINTERACTABLETYPE Interactables::GetInteractableType()
{
	ItemTypes itemType;
	return itemType.GetType();

	//Find out whether it is consumable or nonconsumable
	/*HealthConsum consumable;
	return consumable.GetType();*/
}

CONSUMABLES Interactables::GetConsumableType()
{

}

/// <summary>
/// Collision is handled based on the type of Interactable Object -> This function will be called by each Interactable
/// </summary>
void Interactables::HandleInteractableEvent(EINTERACTABLETYPE& type, CharacterGameObject* player)
{

	if (type == EINTERACTABLETYPE::Consumable)
	{
		//Remove Object from Scene
		//Have the Interactable Affect the Player
	}
	else if (type == EINTERACTABLETYPE::NonConsumable)
	{
		AddToInventory(type);
	}
	
}

void Interactables::AddToInventory(EINTERACTABLETYPE& objToAdd)
{
	//Inventory Functionality
}



