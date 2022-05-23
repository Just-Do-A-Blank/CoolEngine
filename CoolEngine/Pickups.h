#pragma once
#include "Engine/GameObjects/InteractableGameObject.h"

enum class CONSUMABLETYPE
{
	HEALTH,
	STAMINA,
	SE_BOOSTER,
	KEY,
	POTION,
	NONE
};

enum class PICKUPTYPE
{
	CONSUMABLE,
	NONCONSUMABLE,
	NONE
};

struct ConsumableData
{
	CONSUMABLETYPE consumType;
	PICKUPTYPE pickupType;
	float strength;

	ConsumableData(CONSUMABLETYPE ConsumeType,PICKUPTYPE PickupType, float Strength)
	{
		consumType = ConsumeType;
		pickupType = PickupType;
		strength = Strength;
	}

	ConsumableData()
	{
		consumType = CONSUMABLETYPE::NONE;
		pickupType = PICKUPTYPE::NONE;
		strength = 0;
	}

};

class PlayerGameObject;

class Pickups : public InteractableGameObject
{
public:
	Pickups(PlayerGameObject* player);
	~Pickups();

	

private:
	//Expose this to editor
	ConsumableData m_ConsumableData;
	
	//Collison with player
	//Add item to the player's inventory
	void OnTriggerHold(GameObject* obj1, GameObject* obj2);

};

