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

struct ConsumableData
{
	CONSUMABLETYPE consumType;
	bool isConsumedOnPickup;
	float strength;

	ConsumableData(CONSUMABLETYPE ConsumeType,bool isConsumed, float Strength)
	{
		consumType = ConsumeType;
		isConsumedOnPickup = isConsumed;
		strength = Strength;
	}

	ConsumableData()
	{
		consumType = CONSUMABLETYPE::NONE;
		isConsumedOnPickup = false;
		strength = 0;
	}

};

class PickupGameObject : public InteractableGameObject
{
public:
	PickupGameObject(string identifier, CoolUUID uuid);
	PickupGameObject(const nlohmann::json& data, CoolUUID index);
	PickupGameObject(PickupGameObject const& other);
	~PickupGameObject();
	virtual void Serialize(nlohmann::json& data) override;
	const ConsumableData GetConsumableData() const { return m_ConsumableData; }
	void Update() override;

	void SetToBeDeleted(bool ShouldDelete) { m_shouldbeDeleted = ShouldDelete; }

#if EDITOR
	/// <summary>
	/// Shows engine UI
	/// </summary>
	virtual void CreateEngineUI();
#endif


private:
	//Expose this to editor
	ConsumableData m_ConsumableData;
	

	virtual void LoadLocalData(const nlohmann::json& jsonData);
	virtual void SaveLocalData(nlohmann::json& jsonData);


	string GetConsumableTypeString(CONSUMABLETYPE type);

	bool m_shouldbeDeleted;

};

