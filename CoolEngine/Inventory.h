#pragma once
#include "Engine/GameObjects/WeaponGameObject.h"


class Inventory
{
public:

    //Inventory(string identifier, CoolUUID uuid);
    //Inventory(const nlohmann::json& data, CoolUUID uuid);
    //Inventory(Inventory const& other);

  /*  struct inventroryData
    {*/
        WeaponGameObject* m_selectedWeapon;
        float m_money = 0;
        float m_maxMoney;

        bool m_itemSlotsWeapons[4]; // values set as stated in design doc
        bool m_itemSlotsConsumables[3];
        bool m_itemSlotsKeys[2];

        bool m_keyRetreievedRoomN; // Fire Mountain: Ortius (drops bomb on defeat)  Bool for each key as said in design doc
        bool m_keyRetreievedRoomE; // Poison (drops Crystalised Flower)
        bool m_keyRetreievedRoomS; // Electricity (drops Capacitor)
        bool m_keyRetreievedRoomW; // Water (Drops Aquamarine Crystal)
        bool m_keyRetreivedHideoutN;
        bool m_keyRetreivedHideoutE;
        bool m_keyRetreivedHideoutS;
        bool m_keyRetreivedHideoutW;
    //};


    virtual float GetMoney();
    virtual int GetSelectedItemIndex();

    virtual void UpdateMoney(float ValueToAdd); // used to add/subtract money from inventory
    virtual void NavigateInventory(int DirectionAndDistance);
    virtual void SetSelectedIndex(int SelectedIndex);
    virtual void PickupItem(GameObject Item);
    virtual void DeleteItem(GameObject Item);
    virtual void DeleteItemAtIndex();


protected:



private:

};
#pragma once
