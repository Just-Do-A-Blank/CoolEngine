#pragma once
#include <Engine\GameObjects\GameObject.h>
#include <Engine/GameObjects/TriggerableGameObject.h>
#include <Engine/GameObjects/InteractableGameObject.h>
class Interactables : public TriggerableGameObject
{
public:
	/// <summary>
	/// Reference to the current Interactable Game Object and the transform of the current Interactable Game Object
	/// </summary>
	/// <param name="interactableGameObj"></param>
	/// <param name="interactableTransform"></param>
	Interactables(GameObject* interactableGameObj, Transform* interactableTransform);

	~Interactables();

	/// <summary>
	/// Has the Interactable Item collided with the Player
	/// </summary>
	bool interactableCollision;

	/// <summary>
	/// The Radius surrounding the Pickup
	/// </summary>
	float interactableRadius;

	/// <summary>
	/// The current Position of the Player
	/// </summary>
	XMFLOAT3 playerPos;

	/// <summary>
	/// The current position of the Interactable
	/// </summary>
	XMFLOAT3 itemPos;
	/// <summary>
	/// Checks if the Player and Interactable Item have collided
	/// </summary>

	/// <summary>
	/// The Interactable GameObject
	/// </summary>

	InteractableGameObject interactableGameObject;

	/// <summary>
	/// The Triggerable GameObject
	/// </summary>
	
	TriggerableGameObject triggerableGameObject;

	/// <summary>
	/// Handles the collision of all Interactable Items in the Game
	/// </summary>

	virtual void InteractableItemCollision();

	/// <summary>
	/// Depending on the Interactable Type (E.g a Consumable), an Event happens
	/// </summary>
	virtual void HandleInteractableEvent();

	/// <summary>
	/// Occurs when two objects collide without collision on. Fired on enter.
	/// </summary>
	virtual void OnTriggerEnter(GameObject* obj1, GameObject* obj2) override;
};

