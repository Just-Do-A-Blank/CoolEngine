#include "Pickups.h"
#include "Engine/GameObjects/PlayerGameObject.h"
#include "Engine/Managers/Events/EventManager.h"
#include <Engine\EditorUI\EditorUI.h>

Pickups::Pickups(string identifier, CoolUUID uuid) : InteractableGameObject(identifier, uuid)
{
    m_gameObjectType |= GameObjectType::INTERACTABLE;
}

Pickups::Pickups(const nlohmann::json& data, CoolUUID index) : InteractableGameObject(data, index)
{
    m_gameObjectType |= GameObjectType::INTERACTABLE;

    if (PrefabGameObject::IsPrefab())
    {
        LoadLocalData(PrefabGameObject::GetPrefabDataLoadedAtCreation());
    }
    else
    {
        LoadLocalData(data);
    }
}

Pickups::Pickups(Pickups const& other) : InteractableGameObject(other)
{
}

Pickups::~Pickups()
{
}

//void Pickups::CreateEngineUI()
//{
//
//    float colWidth = 150;
//
//    auto titleParameters = EditorUINonSpecificParameters();
//    titleParameters.m_columnWidth = 150;
//
//    if (EditorUI::CollapsingSection("Pickup", false))
//    {
//        EditorUIFloatParameters speedParameters = EditorUIFloatParameters();
//        speedParameters.m_columnWidth = 150;
//        speedParameters.m_tooltipText = "When walking this is the main multiplier - sensitive to big changes!";
//        speedParameters.m_speed = 0.01f;
//
//        EditorUI::FullTitle("Walking", titleParameters);
//
//        EditorUI::DragFloat("Walking Speed", m_speedMultiplierWalking, speedParameters);
//
//        speedParameters.m_tooltipText = "When dodging this is the main multiplier - sensitive to big changes!";
//
//        EditorUI::FullTitle("Dodge", titleParameters);
//
//        EditorUI::DragFloat("Dodge Speed", m_dodgeSpeed, speedParameters);
//
//        speedParameters.m_tooltipText = "The time in seconds to dodge for before returning to walking!";
//
//        EditorUI::DragFloat("Dodge Time in Seconds ", m_timeInSecondsToDodgeFor, speedParameters);
//
//        speedParameters.m_tooltipText = "When rolling this is the main multiplier - sensitive to big changes!";
//
//        EditorUI::FullTitle("Roll", titleParameters);
//
//        EditorUI::DragFloat("Roll Speed", m_rollSpeed, speedParameters);
//
//        speedParameters.m_tooltipText = "The time in seconds to roll for before returning to walking!";
//
//        EditorUI::DragFloat("Roll Time in Seconds ", m_timeInSecondsToRollFor, speedParameters);
//
//        EditorUI::FullTitle("All Speed Stats", titleParameters);
//
//        EditorUIIntParameters numberParameters = EditorUIIntParameters();
//        numberParameters.m_columnWidth = 150;
//        numberParameters.m_minValue = 0;
//        numberParameters.m_maxValue = 1000;
//
//        EditorUI::DragInt("MaxSpeed", m_moveSpeedMax, numberParameters);
//
//        EditorUI::DragInt("Speed Per Frame", m_moveSpeedPerFrame, numberParameters);
//
//        EditorUI::DragInt("Drag Per Frame", m_dragSpeedPerFrame, numberParameters);
//    }
//}

void Pickups::OnTriggerHold(GameObject* obj1, GameObject* obj2)
{
	//If this pickup has collided with the player
	if (obj1->ContainsType(GameObjectType::PLAYER) && obj2 == this)
	{
		//Send event with the data for the player to react to.
		EventManager::Instance()->AddEvent(EventType::Pickup, &m_ConsumableData);

	}
}

void Pickups::LoadLocalData(const nlohmann::json& jsonData)
{
}

void Pickups::SaveLocalData(nlohmann::json& jsonData)
{
}
