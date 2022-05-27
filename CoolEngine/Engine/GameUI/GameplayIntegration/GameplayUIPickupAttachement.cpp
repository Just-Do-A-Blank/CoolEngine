#include "Engine/GameUI/GameplayIntegration/GameplayUIPickupAttachement.h"
#include "Engine/EditorUI/EditorUI.h"
#include "Engine/Managers/GameManager.h"
#include "Engine/GameObjects/PlayerGameObject.h"

GameplayUIPickupAttachement::GameplayUIPickupAttachement()
{
    m_currentPlayer = nullptr;

    m_attmptedToFindPlayer = "";
    m_haveEverUpdatedTheUI = false;
    m_pickupSlot = 0;

#if EDITOR
    m_attmptedToFindPlayer = false;
#endif

    EventManager::Instance()->AddClient(EventType::Pickup, this);
}

GameplayUIPickupAttachement::GameplayUIPickupAttachement(nlohmann::json& data)
{
    m_currentPlayer = nullptr;
    m_haveEverUpdatedTheUI = false;

#if EDITOR
    m_attmptedToFindPlayer = false;
#endif

    LoadLocalData(data);

    EventManager::Instance()->AddClient(EventType::Pickup, this);
}

GameplayUIPickupAttachement::GameplayUIPickupAttachement(GameplayUIPickupAttachement const& other)
{
    m_currentPlayer = nullptr;
    m_haveEverUpdatedTheUI = false;
    m_pickupSlot = other.m_pickupSlot;

#if EDITOR
    m_attmptedToFindPlayer = false;
#endif

    EventManager::Instance()->AddClient(EventType::Pickup, this);
}

GameplayUIPickupAttachement::~GameplayUIPickupAttachement()
{
    EventManager::Instance()->RemoveClientEvent(EventType::Pickup, this);
}

/// <summary>
/// Called after construction, before first Update.
/// </summary>
void GameplayUIPickupAttachement::Start()
{
    AttemptToFindPlayer();
    m_playerPickedUpItem = true;
}

/// <summary>
/// Called when the UI element should update
/// </summary>
void GameplayUIPickupAttachement::Update()
{
    // This ensures we only update when data changes
    // see handle Player Pickups
    if (!m_playerPickedUpItem)
    {
        return;
    }
    m_playerPickedUpItem = false;

    if (m_currentPlayer == nullptr)
    {
        return;
    }

    PickupGameObject* playerPickup = GetPickupToDisplay();
    Update(playerPickup);
}

/// <summary>
/// Handles events from the Observations
/// </summary>
void GameplayUIPickupAttachement::Handle(Event* e)
{
    switch (e->GetEventID())
    {
    case EventType::Pickup:
        PlayerPickedupItem((PickupEvent*)e);
        break;
    }
}

/// <summary>
/// Handles pickup events
/// </summary>
void GameplayUIPickupAttachement::PlayerPickedupItem(PickupEvent* e)
{
    m_playerPickedUpItem = true;
}

/// <summary>
/// Gets the weapon used in the UI for this object
/// </summary>
/// <returns>The weapon to use</returns>
PickupGameObject* GameplayUIPickupAttachement::GetPickupToDisplay()
{
    if (m_pickupSlot == 0)
    {
        return nullptr;
    }

    if (m_currentPlayer == nullptr)
    {
        return nullptr;
    }

    PickupGameObject* pickupGameObject = nullptr;
    int slot = m_pickupSlot;
    Inventory* inventory = m_currentPlayer->GetInventory();
    if (inventory != nullptr)
    {
        int i = -1;
        while(slot > 0)
        {
            ++i;
            if (inventory->GetItemInSlot(i) == nullptr)
            {
                --slot;
            }
            else if (inventory->GetItemInSlot(i)->ContainsType(GameObjectType::PICKUP))
            {
                --slot;
            }
        }

        if (inventory->GetItemInSlot(i) != nullptr)
        {
            pickupGameObject = dynamic_cast<PickupGameObject*>(inventory->GetItemInSlot(i));
        }
    }

    return pickupGameObject;
}

#if EDITOR
void GameplayUIPickupAttachement::CreateEngineUI()
{
    if (EditorUI::CollapsingSection("Pickup Attachement", true))
    {
        AttemptToFindPlayer();
        PlayerGameObject* player = GetPlayer();

        if (player == nullptr)
        {
            EditorUI::FullTitle("Players must be named 'Player' in the scene.");
            if (EditorUI::BasicButton("Find Player"))
            {
                AttemptToFindPlayer(true);
            }
        }
        else
        {
            EditorUI::FullTitle("Found Player");

            EditorUIIntParameters param = EditorUIIntParameters();
            param.m_tooltipText = "0 means do not use this.\n Above 0 is the slot to display.";
            param.m_minValue = 0;
            EditorUI::DragInt("Slot", m_pickupSlot, param);
        }
    }
}


#endif

/// <summary>
/// Attempts to find the player once at the start of the objects life
/// </summary>
void GameplayUIPickupAttachement::AttemptToFindPlayer(bool force)
{
    if (!force && m_attmptedToFindPlayer)
    {
        return;
    }
    m_attmptedToFindPlayer = true;

    GameManager* gm = GameManager::GetInstance();
    m_currentPlayer = gm->GetGameObjectUsingIdentifier<PlayerGameObject>(std::string("Player"));

    if (m_currentPlayer == nullptr)
    {
        return;
    }
}



void GameplayUIPickupAttachement::LoadFromTopLevel(const nlohmann::json& jsonData)
{
    LoadAllPrefabData(jsonData);
}

void GameplayUIPickupAttachement::SaveFromTopLevel(nlohmann::json& jsonData)
{
    SaveAllPrefabData(jsonData);
}

void GameplayUIPickupAttachement::LoadAllPrefabData(const nlohmann::json& jsonData)
{
    LoadLocalData(jsonData);
}
void GameplayUIPickupAttachement::SaveAllPrefabData(nlohmann::json& jsonData)
{
    SaveLocalData(jsonData);
}


void GameplayUIPickupAttachement::LoadLocalData(const nlohmann::json& jsonData)
{
    if (jsonData.contains("GameplayUIPickupAttachement_PickupSlot"))
    {
        m_pickupSlot = jsonData["GameplayUIPickupAttachement_PickupSlot"];
    }
}

void GameplayUIPickupAttachement::Serialize(nlohmann::json& data)
{
    SaveLocalData(data);
}

void GameplayUIPickupAttachement::SaveLocalData(nlohmann::json& jsonData)
{
    jsonData["GameplayUIPickupAttachement_PickupSlot"] = m_pickupSlot;
}

/// <summary>
/// Gets the player gameobject. Safe to call in editor or play.
/// </summary>
/// <returns>The current player</returns>
PlayerGameObject* GameplayUIPickupAttachement::GetPlayer()
{
    return m_currentPlayer;
}