#include "Interactables.h"
#include <Engine/Managers/Events/CollisionEvents.h>
#include <Engine/GameObjects/TriggerableGameObject.h>
#include <Engine/GameObjects/InteractableGameObject.h>

/// <summary>
/// Interactable Object Constructor
/// </summary>
/// <param name="interactableGameObj"></param>
/// <param name="interactableTransform"></param>
void Interactables(GameObject* interactableGameObj, Transform* interactableTransform)
{
	
}

/// <summary>
/// Interactable Object Destructor
/// </summary>
/// <param name="obj1"></param>
/// <param name="obj2"></param>

Interactables::~Interactables()
{

}

/// <summary>
/// Checks if a Collision has been Triggered of the Player GameObject and the Interactable GameObject
/// </summary>
/// <param name="obj1"></param>
/// <param name="obj2"></param>
void Interactables::OnTriggerEnter(GameObject* obj1, GameObject* obj2)
{

}

/// <summary>
/// If there has been a collision
/// </summary>
void Interactables::InteractableItemCollision()
{
	interactableCollision = false;

}

/// <summary>
/// Collision is handled based on the type of Interactable Object
/// </summary>
void Interactables::HandleInteractableEvent()
{
}

