#pragma once
#include "Engine/GameObjects/InteractableGameObject.h"
#include "Engine/GameObjects/PickupResource.h"
#include <unordered_set>

class PlayerGameObject;

class PickupGameObject : public InteractableGameObject
{
public:
	PickupGameObject(string identifier, CoolUUID uuid);
	PickupGameObject(const nlohmann::json& data, CoolUUID index);
	PickupGameObject(PickupGameObject const& other);
	~PickupGameObject();
	virtual void Serialize(nlohmann::json& data) override;
	unordered_set<PickupResource*> GetConsumableData() { return m_pResouces; }
	int GetQuantity() { return m_quantity; }
	void SetQuantity(int quantity) { m_quantity = quantity; }
	bool GetConsumeOnPickup() { return m_isConsumedOnPickup; }

	void Update() override;

	void LoadAllPrefabData(const nlohmann::json& jsonData) override;

	void SaveAllPrefabData(nlohmann::json& jsonData) override;

	void Start() override;

#if EDITOR
	/// <summary>
	/// Shows engine UI
	/// </summary>
	virtual void CreateEngineUI();
#endif

    void SetUITexture(std::wstring wsfilepath);

    /// <summary>
    /// Shows engine UI
    /// </summary>
    /// <return>Get the UI Texture for the Pickup</return>
    std::wstring GetUITexturePath();
private:
	//Expose this to editor
	PickupResourceInterface* m_pPickupResourceInterface;

	unordered_set<string>* m_pFullResourceList;
	unordered_set<PickupResource*> m_pResouces;

	bool m_isConsumedOnPickup;
	int m_quantity;

	virtual void LoadLocalData(const nlohmann::json& jsonData);
	virtual void SaveLocalData(nlohmann::json& jsonData);

	PlayerGameObject* m_pPlayer;

    ID3D11ShaderResourceView* m_pUItexture = nullptr;
    wstring m_UItexturePath;
};

