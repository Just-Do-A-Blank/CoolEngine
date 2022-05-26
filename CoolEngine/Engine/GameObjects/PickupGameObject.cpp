#include "PickupGameObject.h"
#include "Engine/GameObjects/PlayerGameObject.h"
#include "Engine/Managers/Events/EventManager.h"
#include "Engine/Managers/Events/PickupEvent.h"
#include "Engine\EditorUI\EditorUI.h"

#include "Engine/Managers/GameManager.h"
#include "Engine/Managers/PickupsManager.h"

PickupGameObject::PickupGameObject(string identifier, CoolUUID uuid) : InteractableGameObject(identifier, uuid)
{
    m_gameObjectType |= GameObjectType::PICKUP;
    m_pPlayer = GameManager::GetInstance()->GetGameObjectUsingIdentifier<PlayerGameObject>(string("Player"));
    m_isConsumedOnPickup = false;
    m_pPickupResourceInterface = new PickupResourceInterface(&m_pResouces);

}

PickupGameObject::PickupGameObject(const nlohmann::json& data, CoolUUID index) : InteractableGameObject(data, index)
{
    m_gameObjectType |= GameObjectType::PICKUP;

    if (PrefabGameObject::IsPrefab())
    {
        LoadLocalData(PrefabGameObject::GetPrefabDataLoadedAtCreation());
    }
    else
    {
        LoadLocalData(data);
    }
}

PickupGameObject::PickupGameObject(PickupGameObject const& other) : InteractableGameObject(other)
{
}

PickupGameObject::~PickupGameObject()
{
}

void PickupGameObject::Serialize(nlohmann::json& data)
{
    InteractableGameObject::Serialize(data);
    SaveLocalData(data);
}



#if EDITOR
void PickupGameObject::CreateEngineUI()
{
    InteractableGameObject::CreateEngineUI();

    if (EditorUI::CollapsingSection("Pickup", true))
    {
        
        m_pPickupResourceInterface->CreateEngineUI();


        ImGui::Spacing();
        EditorUINonSpecificParameters pickupBoolParameter;
        pickupBoolParameter.m_columnWidth = 150;
        pickupBoolParameter.m_tooltipText = "If the pickup will be consumed by the player on pickup or it will go into inventory";
        EditorUI::Checkbox("Consumed on Pickup", m_isConsumedOnPickup, pickupBoolParameter);

    }
}
#endif



void PickupGameObject::LoadLocalData(const nlohmann::json& jsonData)
{
    m_pResouces.clear();
    m_isConsumedOnPickup = jsonData["ConsumedOnPickup"];
    bool exists = false;
    int i = 0;
    string name;

    do
    {
        name = "EffectName";
        name.append(to_string(i));
        if (jsonData.contains(name))
        {
            exists = true;
            string strength = "EffectStrength";
            strength.append(to_string(i));
            string key = jsonData[name];
            m_pResouces.push_back(new PickupResource(key, jsonData[strength]));
            i++;
            PickupsManager::GetInstance()->GetList()->insert(key);
        }
        else
        {
            exists = false;
        }

    } while (exists);

}

void PickupGameObject::SaveLocalData(nlohmann::json& jsonData)
{

    jsonData["ConsumedOnPickup"] = m_isConsumedOnPickup;

    list<PickupResource*>::iterator it;
    int i = 0;
    list<PickupResource*> effects = *m_pPickupResourceInterface->GetEffects();
    for (it = effects.begin(); it != effects.end(); it++)
    {
        string name = "EffectName";
        name.append(to_string(i));

        jsonData[name] = (*it)->key;
        name = "EffectStrength";
        name.append(to_string(i));
        jsonData[name] = (*it)->strength;
        i++;
    }
}


void PickupGameObject::Update()
{
    InteractableGameObject::Update();

    //If this object should be deleted, delete it.
    if (m_shouldbeDeleted)
    {
        GameManager::GetInstance()->DeleteGameObjectUsingIdentifier(GetIdentifier());
    }



}

void PickupGameObject::LoadAllPrefabData(const nlohmann::json& jsonData)
{
    InteractableGameObject::LoadAllPrefabData(jsonData);
    LoadLocalData(jsonData);
}

void PickupGameObject::SaveAllPrefabData(nlohmann::json& jsonData)
{
    SaveLocalData(jsonData);
    InteractableGameObject::SaveAllPrefabData(jsonData);
}
