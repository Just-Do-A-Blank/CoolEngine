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
}

GameplayUIPickupAttachement::GameplayUIPickupAttachement(nlohmann::json& data)
{
    m_currentPlayer = nullptr;
    m_haveEverUpdatedTheUI = false;

#if EDITOR
    m_attmptedToFindPlayer = false;
#endif

    LoadLocalData(data);
}

GameplayUIPickupAttachement::GameplayUIPickupAttachement(GameplayUIPickupAttachement const& other)
{
    m_currentPlayer = nullptr;
    m_haveEverUpdatedTheUI = false;
    m_pickupSlot = other.m_pickupSlot;

#if EDITOR
    m_attmptedToFindPlayer = false;
#endif
}

GameplayUIPickupAttachement::~GameplayUIPickupAttachement()
{

}

/// <summary>
/// Called after construction, before first Update.
/// </summary>
void GameplayUIPickupAttachement::Start()
{
    AttemptToFindPlayer();
}

/// <summary>
/// Called when the UI element should update
/// </summary>
void GameplayUIPickupAttachement::Update()
{
    if (m_currentPlayer == nullptr)
    {
        return;
    }

    PickupGameObject* playerPickup = GetPickupToDisplay();
    if (ShouldUpdateUI(playerPickup))
    {
        Update(playerPickup);
    }
    UpdatedUI(playerPickup);
}

/// <summary>
/// Gets the weapon used in the UI for this object
/// </summary>
/// <returns>The weapon to use</returns>
PickupGameObject* GameplayUIPickupAttachement::GetPickupToDisplay()
{
    if (m_currentPlayer == nullptr)
    {
        return nullptr;
    }

    PickupGameObject* weaponReturn = nullptr;
    //m_currentPlayer->GetPickupInInventory(number); [INVENTORY]

    return weaponReturn;
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

/// <summary>
/// Detirmines if we should update the UI
/// </summary>
/// <param name="pickup">Current pickup to display</param>
/// <returns>True means we should</returns>
bool GameplayUIPickupAttachement::ShouldUpdateUI(PickupGameObject* pickup)
{
    /*string currentPickupKey = "";
    if (pickup != nullptr)
    {
        currentPickupKey = pickup->GetUniqueKey(); [INVENTORY]
    }

    bool shouldUpdateUI = false;
    if (!m_haveEverUpdatedTheUI || currentPickupKey != m_pickupKey)
    {
        shouldUpdateUI = true;
    }

    return shouldUpdateUI;*/return true;
}

/// <summary>
/// Call after updating the UI
/// </summary>
/// <param name="pickup">Current pickup to display</param>
void GameplayUIPickupAttachement::UpdatedUI(PickupGameObject* pickup)
{
    string currentPickupKey = "";
    if (pickup != nullptr)
    {
        //currentPickupKey = pickup->GetUniqueKey();
    }

    m_pickupKey = currentPickupKey;
    m_haveEverUpdatedTheUI = true;

}