#pragma once
#include "Engine/GameObjects/WeaponGameObject.h"
#include "Engine/GameObjects/PickupResource.h"
#include <algorithm>

struct InventoryInfo
{
    string key;
    float strength;
    int quantity;

    InventoryInfo(string Key, float Strength, float Quantity)
    {
        key = Key;
        strength = Strength;
        quantity = Quantity;
    }

};



class Inventory
{
public:
    Inventory();
    Inventory(string identifier, CoolUUID uuid);
    Inventory(const nlohmann::json& data, CoolUUID uuid);
    Inventory(Inventory const& other);

    unordered_set<InventoryInfo*> GetPickupInventory() { return m_pInventoryInfo; }
    unordered_set<WeaponGameObject*> GetWeaponInventory() { return m_pWeaponInventory; }
    void RemoveWeapon(WeaponGameObject* weapon) { m_pWeaponInventory.erase(weapon); }
    void RemoveWeaponByKey(string key);

    bool RemovePickupByKey(string key,float strength, int quantityToRemove);
    InventoryInfo* GetItemByKey(string key, float strength);

    void AddPickup(unordered_set<PickupResource*> pickupEffects);


    void SaveData(nlohmann::json& jsonData);
    void LoadData(const nlohmann::json& jsonData);
private:
    unordered_set<InventoryInfo*> m_pInventoryInfo;
    unordered_set<WeaponGameObject*> m_pWeaponInventory;
};
#pragma once
