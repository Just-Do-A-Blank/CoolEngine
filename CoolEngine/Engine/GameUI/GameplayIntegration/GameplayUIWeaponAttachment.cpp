#include "Engine/GameUI/GameplayIntegration/GameplayUIWeaponAttachment.h"
#include "Engine/EditorUI/EditorUI.h"
#include "Engine/Managers/GameManager.h"

GameplayUIWeaponAttachment::GameplayUIWeaponAttachment()
{
    m_currentPlayer = nullptr;
    m_weaponAttachmentOption = EWEAPONUIATTACHMENTOPTION::None;

    m_weaponKey = "";
    m_haveEverUpdatedTheUI = false;

#if EDITOR
    m_attmptedToFindPlayer = false;

    m_attachmentSettingList = GetattachmentSettingsAsList();
    m_attachmentSettingSelectedItem = GetattachmentSettingsFromIndex((int)m_weaponAttachmentOption);
#endif

    m_weaponCurrentlyInInventory = nullptr;

    EventManager::Instance()->AddClient(EventType::Pickup, this);
}

GameplayUIWeaponAttachment::GameplayUIWeaponAttachment(nlohmann::json& data)
{
    m_weaponCurrentlyInInventory = nullptr;
    m_currentPlayer = nullptr;
    m_weaponKey = "";
    m_haveEverUpdatedTheUI = false;

#if EDITOR
    m_attmptedToFindPlayer = false;
    m_attachmentSettingList = GetattachmentSettingsAsList();
#endif

    LoadLocalData(data);

    EventManager::Instance()->AddClient(EventType::Pickup, this);
}

GameplayUIWeaponAttachment::GameplayUIWeaponAttachment(GameplayUIWeaponAttachment const& other)
{
    m_weaponCurrentlyInInventory = nullptr;
    m_currentPlayer = nullptr;
    m_weaponAttachmentOption = (EWEAPONUIATTACHMENTOPTION)other.m_weaponAttachmentOption;
    m_weaponKey = "";
    m_haveEverUpdatedTheUI = false;

#if EDITOR
    m_attmptedToFindPlayer = false;

    m_attachmentSettingList = GetattachmentSettingsAsList();
    m_attachmentSettingSelectedItem = GetattachmentSettingsFromIndex((int)m_weaponAttachmentOption);
#endif

    EventManager::Instance()->AddClient(EventType::Pickup, this);
}

GameplayUIWeaponAttachment::~GameplayUIWeaponAttachment()
{
    EventManager::Instance()->RemoveClientEvent(EventType::Pickup, this);
}

/// <summary>
/// Called after construction, before first Update.
/// </summary>
void GameplayUIWeaponAttachment::Start()
{
    if (m_weaponAttachmentOption == EWEAPONUIATTACHMENTOPTION::None)
    {
        return;
    }

    AttemptToFindPlayer();
}

/// <summary>
/// Called when the UI element should update
/// </summary>
void GameplayUIWeaponAttachment::Update()
{
    if (m_weaponAttachmentOption == EWEAPONUIATTACHMENTOPTION::None)
    {
        return;
    }

    if (m_currentPlayer == nullptr)
    {
        return;
    }

    WeaponGameObject* playerWeapon = GetWeaponGameObject();
    if (ShouldUpdateUI(playerWeapon))
    {
        Update(playerWeapon);
    }
    UpdatedUI(playerWeapon);
}

/// <summary>
/// Gets the weapon used in the UI for this object
/// </summary>
/// <returns>The weapon to use</returns>
WeaponGameObject* GameplayUIWeaponAttachment::GetWeaponGameObject()
{
    if (m_currentPlayer == nullptr)
    {
        return nullptr;
    }

    WeaponGameObject* weaponReturn = nullptr;
    switch (m_weaponAttachmentOption)
    {
        case EWEAPONUIATTACHMENTOPTION::Holding:
            weaponReturn = m_currentPlayer->GetWeapon();
            break;
        case EWEAPONUIATTACHMENTOPTION::Inventory:
            if (m_playerPickedUpItem)
            {
                m_weaponCurrentlyInInventory = GetWeaponInInventory();
            }
            m_playerPickedUpItem = false;
            weaponReturn = m_weaponCurrentlyInInventory;
            break;
    }

    return weaponReturn;
}

/// <summary>
/// Handles events from the Observations
/// </summary>
void GameplayUIWeaponAttachment::Handle(Event* e)
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
void GameplayUIWeaponAttachment::PlayerPickedupItem(PickupEvent* e)
{
    m_playerPickedUpItem = true;
}

/// <summary>
/// Gets the weapon in the inventory
/// </summary>
/// <returns>The weapon to use</returns>
WeaponGameObject* GameplayUIWeaponAttachment::GetWeaponInInventory()
{
    if (m_currentPlayer == nullptr)
    {
        return nullptr;
    }

    WeaponGameObject* weaponGameObject = nullptr;
    int slot = 1;
    Inventory* inventory = m_currentPlayer->GetInventory();
    if (inventory != nullptr)
    {
        int i = -1;
        while (slot > 0)
        {
            ++i;
            if (inventory->GetItemInSlot(i) == nullptr)
            {
                --slot;
            }
            else if (inventory->GetItemInSlot(i)->ContainsType(GameObjectType::WEAPON))
            {
                --slot;
            }
        }

        if (inventory->GetItemInSlot(i) != nullptr)
        {
            weaponGameObject = dynamic_cast<WeaponGameObject*>(inventory->GetItemInSlot(i));
        }
    }

    return weaponGameObject;
}

#if EDITOR
void GameplayUIWeaponAttachment::CreateEngineUI()
{
    if (EditorUI::CollapsingSection("Weapon Attachement", true))
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

            if (EditorUI::ComboBox("Attach Style", m_attachmentSettingList, m_attachmentSettingSelectedItem))
            {
                m_weaponAttachmentOption = (EWEAPONUIATTACHMENTOPTION)m_attachmentSettingSelectedItem.first;
            }
        }
    }
}


#endif

/// <summary>
/// Attempts to find the player once at the start of the objects life
/// </summary>
void GameplayUIWeaponAttachment::AttemptToFindPlayer(bool force)
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



void GameplayUIWeaponAttachment::LoadFromTopLevel(const nlohmann::json& jsonData)
{
    LoadAllPrefabData(jsonData);
}

void GameplayUIWeaponAttachment::SaveFromTopLevel(nlohmann::json& jsonData)
{
    SaveAllPrefabData(jsonData);
}

void GameplayUIWeaponAttachment::LoadAllPrefabData(const nlohmann::json& jsonData)
{
    LoadLocalData(jsonData);
}
void GameplayUIWeaponAttachment::SaveAllPrefabData(nlohmann::json& jsonData)
{
    SaveLocalData(jsonData);
}


void GameplayUIWeaponAttachment::LoadLocalData(const nlohmann::json& jsonData)
{
    if (jsonData.contains("GameplayUIWeaponAttachment_WeaponAttachmentOption"))
    {
        m_weaponAttachmentOption = jsonData["GameplayUIWeaponAttachment_WeaponAttachmentOption"];

#if EDITOR
        m_attachmentSettingSelectedItem = GetattachmentSettingsFromIndex((int)m_weaponAttachmentOption);
#endif
    }
}

void GameplayUIWeaponAttachment::Serialize(nlohmann::json& data)
{
    SaveLocalData(data);
}

void GameplayUIWeaponAttachment::SaveLocalData(nlohmann::json& jsonData)
{
    jsonData["GameplayUIWeaponAttachment_WeaponAttachmentOption"] = m_weaponAttachmentOption;
}

/// <summary>
/// Gets the player gameobject. Safe to call in editor or play.
/// </summary>
/// <returns>The current player</returns>
PlayerGameObject* GameplayUIWeaponAttachment::GetPlayer()
{
    return m_currentPlayer;
}

#if EDITOR
list<pair<int, string>> GameplayUIWeaponAttachment::GetattachmentSettingsAsList()
{
    list<pair<int, string>> newList = list<pair<int, string>>();
    for (int i = 0; i < (int)EWEAPONUIATTACHMENTOPTION::COUNT; ++i)
    {
        newList.push_back(GetattachmentSettingsFromIndex(i));
    }

    return newList;
}

pair<int, string> GameplayUIWeaponAttachment::GetattachmentSettingsFromIndex(int index)
{
    pair<int, string> returnPair = pair<int, string>(0, "None");
    switch (index)
    {
        case 1:
            returnPair = pair<int, string>(1, "Holding");
            break;
        case 2:
            returnPair = pair<int, string>(2, "Inventory");
            break;
    }

    return returnPair;
}
#endif

/// <summary>
/// Detirmines if we should update the UI
/// </summary>
/// <param name="weapon">Current weapon to display</param>
/// <returns>True means we should</returns>
bool GameplayUIWeaponAttachment::ShouldUpdateUI(WeaponGameObject* weapon)
{
    string currentWeaponKey = "";
    if (weapon != nullptr)
    {
        currentWeaponKey = weapon->GetUniqueKey();
    }

    bool shouldUpdateUI = false;
    if (!m_haveEverUpdatedTheUI || currentWeaponKey != m_weaponKey)
    {
        shouldUpdateUI = true;
    }

    return shouldUpdateUI;
}

/// <summary>
/// Call after updating the UI
/// </summary>
/// <param name="weapon">Current weapon to display</param>
void GameplayUIWeaponAttachment::UpdatedUI(WeaponGameObject* weapon)
{
    string currentWeaponKey = "";
    if (weapon != nullptr)
    {
        currentWeaponKey = weapon->GetUniqueKey();
    }

    m_weaponKey = currentWeaponKey;
    m_haveEverUpdatedTheUI = true;

}