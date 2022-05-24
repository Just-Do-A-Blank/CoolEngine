#include "PickupGameObject.h"
#include "Engine/GameObjects/PlayerGameObject.h"
#include "Engine/Managers/Events/EventManager.h"
#include "Engine/Managers/Events/PickupEvent.h"
#include "Engine\EditorUI\EditorUI.h"

#include "Engine/Managers/GameManager.h"

PickupGameObject::PickupGameObject(string identifier, CoolUUID uuid) : InteractableGameObject(identifier, uuid)
{
    m_gameObjectType |= GameObjectType::PICKUP;
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
        //Drop down menu selection
        string currentSelected = GetConsumableTypeString(m_ConsumableData.consumType);

        if (IMGUI_LEFT_LABEL(ImGui::BeginCombo, "Pickup Type", currentSelected.c_str()) == true)
        {
            if (ImGui::Selectable(GetConsumableTypeString(CONSUMABLETYPE::HEALTH).c_str()))
            {
                m_ConsumableData.consumType = CONSUMABLETYPE::HEALTH;
            }
            else if (ImGui::Selectable(GetConsumableTypeString(CONSUMABLETYPE::KEY).c_str()))
            {
                m_ConsumableData.consumType = CONSUMABLETYPE::KEY;
            }
            else if (ImGui::Selectable(GetConsumableTypeString(CONSUMABLETYPE::POTION).c_str()))
            {
                m_ConsumableData.consumType = CONSUMABLETYPE::POTION;
            }
            else if (ImGui::Selectable(GetConsumableTypeString(CONSUMABLETYPE::SE_BOOSTER).c_str()))
            {
                m_ConsumableData.consumType = CONSUMABLETYPE::SE_BOOSTER;
            }
            else if (ImGui::Selectable(GetConsumableTypeString(CONSUMABLETYPE::STAMINA).c_str()))
            {
                m_ConsumableData.consumType = CONSUMABLETYPE::STAMINA;
            }
            else if (ImGui::Selectable(GetConsumableTypeString(CONSUMABLETYPE::NONE).c_str()))
            {
                m_ConsumableData.consumType = CONSUMABLETYPE::NONE;
            }


            ImGui::EndCombo();
        }

        ImGui::Spacing();
        EditorUINonSpecificParameters pickupBoolParameter;
        pickupBoolParameter.m_columnWidth = 150;
        pickupBoolParameter.m_tooltipText = "If the pickup will be consumed by the player on pickup or it will go into inventory";

        EditorUI::Checkbox("Consumed on Pickup", m_ConsumableData.isConsumedOnPickup, pickupBoolParameter);
        ImGui::Spacing();


        EditorUIFloatParameters pickupFloatParameters = EditorUIFloatParameters();
        pickupFloatParameters.m_tooltipText = "Can set to negative to subtract if desired";
        pickupFloatParameters.m_columnWidth = 150;
        EditorUI::DragFloat("Strength", m_ConsumableData.strength, pickupFloatParameters);

    }
}
#endif



void PickupGameObject::LoadLocalData(const nlohmann::json& jsonData)
{
    m_ConsumableData.consumType = jsonData["ConsumableType"];
    m_ConsumableData.isConsumedOnPickup = jsonData["ConsumedOnPickup"];
    m_ConsumableData.strength = jsonData["ConsumableStrength"];
}

void PickupGameObject::SaveLocalData(nlohmann::json& jsonData)
{
    jsonData["ConsumableType"] = m_ConsumableData.consumType;
    jsonData["ConsumedOnPickup"] = m_ConsumableData.isConsumedOnPickup;
    jsonData["ConsumableStrength"] = m_ConsumableData.strength;
}


string PickupGameObject::GetConsumableTypeString(CONSUMABLETYPE type)
{
    switch (type)
    {
    case CONSUMABLETYPE::HEALTH:
        return "Health";
        break;
    case CONSUMABLETYPE::KEY:
        return "Key";
        break;
    case CONSUMABLETYPE::POTION:
        return "Potion";
        break;
    case CONSUMABLETYPE::SE_BOOSTER:
        return "SE_Booster";
        break;
    case CONSUMABLETYPE::STAMINA:
        return "Stamina";
        break;
    case CONSUMABLETYPE::NONE:
        return "None";
        break;
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