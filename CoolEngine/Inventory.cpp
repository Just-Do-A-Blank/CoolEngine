#include "Inventory.h"


//Inventory::Inventory(string identifier, CoolUUID uuid) 
//{
//	
//}
//
//Inventory::Inventory(const nlohmann::json& data, CoolUUID uuid)
//{
//	
//}

//Inventory::Inventory(Inventory const& other) 
//{
//	
//}





float Inventory::GetMoney()
{
	return m_money;
}

int Inventory::GetSelectedItemIndex()
{
	return 0;
}

void Inventory::UpdateMoney(float ValueToAdd)
{
	m_money += ValueToAdd;
	//check if bellow 0 or aove max
}

void Inventory::NavigateInventory(int DirectionAndDistance)
{
}

void Inventory::SetSelectedIndex(int SelectedIndex)
{
}

void Inventory::PickupItem(GameObject Item)
{
}

void Inventory::DeleteItem(GameObject Item)
{
}

void Inventory::DeleteItemAtIndex()
{
}
