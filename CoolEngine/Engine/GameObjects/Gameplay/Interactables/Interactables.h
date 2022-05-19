#pragma once
#include <Engine\GameObjects\GameObject.h>
#include <Engine/GameObjects/InteractableGameObject.h>
#include <Engine/GameObjects/PlayerGameObject.h>
#include <Engine/Managers/GameManager.h>
#include "EInteractableType.h"

class Interactables : public TriggerableGameObject
{
public:
	/// <summary>
	/// Reference to the current Interactable Game Object and the transform of the current Interactable Game Object
	/// </summary>
	/// <param name="interactableGameObj"></param>
	/// <param name="interactableTransform"></param>
	Interactables(GameObject* m_playerObjTrans, GameObject* m_interactableGameObjTrans);

	~Interactables();
private:
	/// <summary>
	/// Has the Interactable Item collided with the Player
	/// </summary>
	bool interactableCollision;


	/// <summary>
	/// If the distance between the Player and Interactable Obj is < than this value, then a collision has occured
	/// </summary>
	float triggerRadius;



	/// <summary>
	/// The Player GameObject
	/// </summary>
	GameObject* m_pPlayerGameObj;

	/// <summary>
	/// The Interactable GameObject
	/// </summary>
	Interactables* m_pInteractableObj;

	/// <summary>
	/// The Game Manager is used to access the Player Game Object
	/// </summary
	GameManager m_GameManager;

	EINTERACTABLETYPE interactableType = (EINTERACTABLETYPE)0;

	/// <summary>
	/// Handles the collision of all Interactable Items in the Game
	/// </summary>

	virtual void InteractableItemCollision();

	/// <summary>
	/// Returns the Type of Interactable Object
	/// </summary>
	/// <param name="type"></param>
	/// <returns></returns>
	bool InteractableType(EINTERACTABLETYPE type);

	/// <summary>
	/// Removes the Interactable Type from the Scene and applies a direct affect to the player
	/// </summary>
	virtual void HandleInteractableEvent(GameObject* objToDestory);

	/// <summary>
	/// Adds the Interactable Type to the Inventory System
	/// </summary>
	
	virtual void AddToInventory(GameObject objToAdd);


	/// <summary>
	/// Occurs when two objects collide without collision on. Fired on enter.
	/// </summary>
	virtual void OnTriggerEnter(XMFLOAT3* obj1, XMFLOAT3* obj2);

	//Create an override of OnTriggerHold

	virtual void OnTriggerHold(GameObject* pObject, GameObject* pObject2) override;
};


