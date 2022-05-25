#include "Inventory.h"
#include "Engine/Managers/GameManager.h"

Inventory::Inventory()
{
	
}

Inventory::Inventory(string identifier, CoolUUID uuid)
{

}

Inventory::Inventory(const nlohmann::json& data, CoolUUID uuid)
{

}

Inventory::Inventory(Inventory const& other)
{

}

void Inventory::RemoveWeaponByKey(string key)
{
	WeaponGameObject* weapon = GameManager::GetInstance()->GetGameObjectUsingIdentifier<WeaponGameObject>(key);
	m_pWeaponInventory.erase(weapon);
}

//Will return false if there are not enough of the key or the item doesnt exist in the inventory. Do not remove weapons through this method as it will always return false
bool Inventory::RemovePickupByKey(string key, float strength, int quantityToRemove)
{
	unordered_set<InventoryInfo*>::iterator it;

	for (it = m_pInventoryInfo.begin(); it != m_pInventoryInfo.end(); it++)
	{
		if ((*it)->key == key && (*it)->strength == strength)
		{
			if ((*it)->quantity >= quantityToRemove)
			{
				(*it)->quantity -= quantityToRemove;
				return true;
			}
			else
			{
				return false;
			}
		}
	}

	return false;

}

InventoryInfo* Inventory::GetItemByKey(string key, float strength)
{
	unordered_set<InventoryInfo*>::iterator it;

	for (it = m_pInventoryInfo.begin(); it != m_pInventoryInfo.end(); it++)
	{
		if ((*it)->key == key && (*it)->strength == strength)
		{
			return (*it);
		}
	}

	return nullptr;
}

void Inventory::AddPickup(unordered_set<PickupResource*> pickupEffects)
{
	unordered_set<PickupResource*>::iterator it;
	unordered_set<InventoryInfo*>::iterator jt;

	bool found = false;
	for (it = pickupEffects.begin(); it != pickupEffects.end(); it++)
	{
		//If we have a weapon
		WeaponGameObject* weapon = GameManager::GetInstance()->GetGameObjectUsingIdentifier<WeaponGameObject>((*it)->key);
		//We have a weapon pickup
		if (weapon != nullptr)
		{
			m_pWeaponInventory.insert(weapon);
		}
		else
		{
			for (jt = m_pInventoryInfo.begin(); jt != m_pInventoryInfo.end(); jt++)
			{
				if ((*it)->key == (*jt)->key && (*it)->strength == (*jt)->strength)
				{
					(*jt)->quantity++;
					found = true;
					break;
				}
			}

			if (!found)
			{
				m_pInventoryInfo.insert(new InventoryInfo((*it)->key, (*it)->strength, 1));
			}
			else
			{
				found = false;
			}


		}


	}
}

void Inventory::SaveData(nlohmann::json& jsonData)
{
	unordered_set<InventoryInfo*>::iterator it;
	int i = 0;
	for (it = m_pInventoryInfo.begin();it != m_pInventoryInfo.end(); it++)
	{
		string s = "PickupInventory_" + to_string(i++);

		jsonData[s + "_key"] = (*it)->key;
		jsonData[s + "_strength"] = (*it)->strength;
		jsonData[s + "_quantity"] = (*it)->quantity;
	}


	unordered_set<WeaponGameObject*>::iterator itt;
	i = 0;
	for (itt = m_pWeaponInventory.begin(); itt != m_pWeaponInventory.end(); itt++)
	{
		string s = "WeaponInventory_" + to_string(i++);

		jsonData[s + "_key"] = (*itt)->GetIdentifier();
	}


}


void Inventory::LoadData(const nlohmann::json& jsonData)
{
	m_pInventoryInfo.clear();

	int i = 0;
	string s = "PickupInventory_" + to_string(i);
	while (jsonData.contains(s + "_key"))
	{
		InventoryInfo* newItem = new InventoryInfo(jsonData[s + "_key"], jsonData[s + "_strength"], jsonData[s + "_quantity"]);
		s = "PickupInventory_" + to_string(++i);
		m_pInventoryInfo.insert(newItem);
	}


	m_pWeaponInventory.clear();

	i = 0;
	s = "WeaponInventory_" + to_string(i++);
	while (jsonData.contains(s + "_key"))
	{
		string wepKey = jsonData[s + "_key"];
		WeaponGameObject* wep = GameManager::GetInstance()->GetGameObjectUsingIdentifier<WeaponGameObject>(wepKey);
		s = "WeaponInventory_" + to_string(++i);
		m_pWeaponInventory.insert(wep);
	}
}