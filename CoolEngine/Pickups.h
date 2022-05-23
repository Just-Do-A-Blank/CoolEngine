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

class Pickups : public InteractableGameObject
{
public:
	Pickups(string identifier, CoolUUID uuid);
	Pickups(const nlohmann::json& data, CoolUUID index);
	Pickups(Pickups const& other);
	~Pickups();


#if EDITOR
	/// <summary>
	/// Shows engine UI
	/// </summary>
	virtual void CreateEngineUI() override;
#endif


private:
	//Expose this to editor
	ConsumableData m_ConsumableData;
	
	//Checking for collision with player and itself
	void OnTriggerHold(GameObject* obj1, GameObject* obj2);

	virtual void LoadLocalData(const nlohmann::json& jsonData);
	virtual void SaveLocalData(nlohmann::json& jsonData);

};

