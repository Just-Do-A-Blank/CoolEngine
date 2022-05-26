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
	m_json = other.m_json;
	m_pInventory = other.m_pInventory;

}


void Inventory::AddItemToInventory(GameObject* pickedUpObject)
{
	m_pInventory.push_back(pickedUpObject);
}

GameObject* Inventory::GetItemInSlot(int pos)
{
	if (pos <= m_pInventory.size())
	{
		return m_pInventory[pos];
	}
	else
	{
		return nullptr;
	}

}


//Quantity to remove only applies to if the item has more than 1 in the slot (for example a potion). Will also only remove upto the amount in slot if not enough. Returns quantity of item used. Make sure you have the effect before calling this
int Inventory::RemoveQuantityInSlot(int pos, int quantityToRemove)
{
	if (m_pInventory[pos]->ContainsType(GameObjectType::PICKUP))
	{
		PickupGameObject* pickup = dynamic_cast<PickupGameObject*>(m_pInventory[pos]);
		int quantityAvailable = pickup->GetQuantity();

		if (quantityAvailable > quantityToRemove)
		{
			quantityAvailable -= quantityToRemove;
			pickup->SetQuantity(quantityAvailable);
			return quantityToRemove;
		}
		//We have now run out of this item, remove it from the inventory (also applies to weapons since they are now held instead of in inventory)
		else if (quantityAvailable == quantityToRemove)
		{
			m_pInventory.erase(m_pInventory.begin() + pos);
			delete pickup;
			return quantityToRemove;
		}
		//We dont have enough of the item
		else
		{
			m_pInventory.erase(m_pInventory.begin() + pos);
			delete pickup;
			return quantityAvailable;
		}
	}
	//We have a weapon
	else if (m_pInventory[pos]->ContainsType(GameObjectType::WEAPON))
	{
		m_pInventory.erase(m_pInventory.begin() + pos);
		return 1;
	}

	//Nothing used
	return 0;


}

void Inventory::SaveData(nlohmann::json& jsonData)
{
	for (int i = 0; i < m_pInventory.size(); i++)
	{
		string s = "PickupInventory_" + to_string(i);

		jsonData[s + "_key"] = to_string(*(m_pInventory[i]->GetUUID()));
	}

}


void Inventory::LoadData(const nlohmann::json& jsonData)
{
	m_json.clear();
	m_json = jsonData;



}

void Inventory::Start()
{


	//m_pInventory.clear();
	bool exists = false;
	int i = 0;
	string name;

	do
	{
		name = "PickupInventory_";
		name.append(to_string(i));
		name = name + "_key";
		if (m_json.contains(name))
		{
			exists = true;
			string id = m_json[name];
			CoolUUID CoolID = stoull(id);
			AddItemToInventory(GameManager::GetInstance()->GetGameObjectUsingUUID<GameObject>(CoolID));
			i++;
		}
		else
		{
			exists = false;
		}

	} while (exists);

}
