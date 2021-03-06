#pragma once
#include <algorithm>
#include "Engine/GameObjects/PickupGameObject.h"
#include "Engine/GameObjects/WeaponGameObject.h"

class Inventory
{
public:
    Inventory();
    Inventory(string identifier, CoolUUID uuid);
    Inventory(const nlohmann::json& data, CoolUUID uuid);
    Inventory(Inventory const& other);

    vector<GameObject*>* GetInventory() { return &m_pInventory; }
   
    void AddItemToInventory(GameObject* pickedUpObject);
    int RemoveQuantityInSlot(int pos, int quantityToRemove);
    GameObject* GetItemInSlot(int pos);


    void SaveData(nlohmann::json& jsonData);
    void LoadData(const nlohmann::json& jsonData);
    void Start();
    
private:

    nlohmann::json m_json;
    vector<GameObject*> m_pInventory;
};


#pragma once
