#include "Interactables.h"
#include <Engine/Managers/Events/CollisionEvents.h>
#include <Engine/GameObjects/TriggerableGameObject.h>
#include <Engine/GameObjects/InteractableGameObject.h>
#include <Engine/GameObjects/PlayerGameObject.h>

/// <summary>
/// Interactable Object Constructor
/// </summary>
/// <param name="interactableGameObj"></param>
/// <param name="interactableTransform"></param>
void Interactables(PlayerGameObject* playerGameObject, Transform* interactableTransform)
{
	/*m_playerGa
	m_playerGameObj = GetGameObjectUsingIdentifier<CharacterGameObject*>(string("Player"));*/

	
}



Interactables::Interactables(GameObject* m_playerObjTrans, GameObject* m_interactableGameObjTrans)
{
	interactableCollision = false;
	triggerRadius = 5.0f;
	// Sets the Player to an Interactable Game Object which will be passed into the OnTriggerEnter() Class
	m_pPlayerGameObj = m_GameManager.GetGameObjectUsingIdentifier<CharacterGameObject>(string("Player"));


	/*Update(playerObjPos, interactableObjPos);*/
}

/// <summary>
/// Interactable Object Destructor
/// </summary>
/// <param name="obj1"></param>
/// <param name="obj2"></param>

//void Interactables::Update(XMFLOAT3* playerObj, XMFLOAT3* interactableObj)
//{
//	if (interactableCollision)
//		OnTriggerEnter(playerObj, interactableObj);
//}
void Interactables::OnTriggerEnter(XMFLOAT3* obj1, XMFLOAT3* obj2)
{
}
void Interactables::OnTriggerHold(GameObject* pObject, GameObject* pObject2)
{
	//Checks if GameObj 1 is a TRIGGERABLE Object Type
	pObject->ContainsType(GameObjectType::TRIGGERABLE);
	//Checks if GameObj 2 is a PLAYER Object Type
	pObject2->ContainsType(GameObjectType::PLAYER);

	//If these GameObjects Match -> Then the TRIGGERABLE Object Type is handled

	HandleInteractableEvent(pObject);
}
Interactables::~Interactables()
{

}


/// <summary>
/// Checks if a Collision has been Triggered of the Player GameObject and the Interactable GameObject
/// </summary>
/// <param name="obj1"></param>
/// <param name="obj2"></param>
//void Interactables::OnTriggerEnter(GameObject* player, GameObject* interactable)
//{
//	float distance = MathHelper::Distance(player, interactable);
//	
//}

/// <summary>
/// If there has been a collision
/// </summary>
void Interactables::InteractableItemCollision()
{
	interactableCollision = false;
}


bool Interactables::InteractableType(EINTERACTABLETYPE type)
{
	/*return (interactableType & type) == type;*/
	return false;
}

/// <summary>
/// Collision is handled based on the type of Interactable Object -> This function will be called by each Interactable
/// </summary>
void Interactables::HandleInteractableEvent(GameObject* triggerableObj)
{
	LOG("Triggerable Works");
	/*if(m_pInteractableObj->ContainsType(EINTERACTABLETYPE::Consumable)*/
}

void Interactables::AddToInventory(GameObject objToAdd)
{

}

