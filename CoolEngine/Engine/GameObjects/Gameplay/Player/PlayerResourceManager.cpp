#include "PlayerResourceManager.h"

PlayerResourceManager::PlayerResourceManager()
{
	m_resources = map<string, PlayerResource*>();

#if EDITOR
	m_resourceInterface = new PlayerResourceInterface(&m_resources);
#endif

    m_resourceKeys = list<string>();
    UpdateAllKeys();
}

PlayerResourceManager::PlayerResourceManager(const nlohmann::json& data)
{
    LoadData(data);

    m_resourceKeys = list<string>();
    UpdateAllKeys();
}

PlayerResourceManager::PlayerResourceManager(PlayerResourceManager const& other)
{
    map<string, PlayerResource*> otherResources = other.m_resources;

    m_resources = map<string, PlayerResource*>();
    for (
        std::map<string, PlayerResource*>::iterator itt = otherResources.begin();
        itt != otherResources.end(); itt++)
    {
        PlayerResource* playerResource = new PlayerResource(itt->first);
        playerResource->SetMinValue(itt->second->GetMinValue());
        playerResource->SetMaxValue(itt->second->GetMaxValue());
        playerResource->SetDefaultValue(itt->second->GetDefaultValue());
        playerResource->SetAttachesToWeaponDamage(itt->second->GetAttachesToWeaponDamage());
        playerResource->SetKillsOnDrain(itt->second->GetKillsOnDrain());

        m_resources[itt->first] = playerResource;
    }


#if EDITOR
    m_resourceInterface = other.m_resourceInterface;
#endif

    m_resourceKeys = list<string>();
    UpdateAllKeys();
}

PlayerResourceManager::~PlayerResourceManager()
{

}

/// <summary>
/// Run at the start of the game to set default values
/// </summary>
void PlayerResourceManager::Start()
{
    for (
        std::map<string, PlayerResource*>::iterator itt = m_resources.begin();
        itt != m_resources.end(); itt++)
    {
        itt->second->SetValue(itt->second->GetDefaultValue());
    }
}

#if EDITOR
/// <summary>
/// Shows engine UI
/// </summary>
void PlayerResourceManager::CreateEngineUI()
{
    m_resourceInterface->CreateEngineUI();
    if (m_resourceInterface->GetLastUpdateResult() != ERESOURCEMODIFICATIONRESULT::NoListModification)
    {
        UpdateAllKeys();
    }
}
#endif

/// <summary>
/// Saves the player resources
/// </summary>
/// <param name="name">Data to add to</param>
void PlayerResourceManager::SaveData(nlohmann::json& jsonData)
{
    int i = 0;
    for (
        std::map<string, PlayerResource*>::iterator itt = m_resources.begin();
        itt != m_resources.end(); itt++)
    {
        string s = "PlayerResource_" + to_string(i++);
        jsonData[s + "_key"] = itt->second->GetKey();
        jsonData[s + "_minValue"] = itt->second->GetMinValue();
        jsonData[s + "_maxValue"] = itt->second->GetMaxValue();
        jsonData[s + "_defaultValue"] = itt->second->GetDefaultValue();
        jsonData[s + "_attachToWeaponDamage"] = itt->second->GetAttachesToWeaponDamage();
        jsonData[s + "_killOnDrain"] = itt->second->GetKillsOnDrain();
    }
}

/// <summary>
/// Loads the player resources
/// </summary>
/// <param name="name">Data to load from</param>
void PlayerResourceManager::LoadData(const nlohmann::json& jsonData)
{
    for (
        std::map<string, PlayerResource*>::iterator itt = m_resources.begin();
        itt != m_resources.end(); itt++)
    {
        delete itt->second;
    }

    m_resources = map<string, PlayerResource*>();

    int i = 0;
    string s = "PlayerResource_" + to_string(i);
    while (jsonData.contains(s + "_key"))
    {
        PlayerResource* newResource = new PlayerResource(jsonData[s + "_key"]);
        newResource->SetMinValue(jsonData[s + "_minValue"]);
        newResource->SetMaxValue(jsonData[s + "_maxValue"]);
        newResource->SetDefaultValue(jsonData[s + "_defaultValue"]);
        newResource->SetAttachesToWeaponDamage(jsonData[s + "_attachToWeaponDamage"]);
        newResource->SetKillsOnDrain(jsonData[s + "_killOnDrain"]);

        m_resources[jsonData[s + "_key"]] = newResource;

        s = "PlayerResource_" + to_string(++i);
    }

    UpdateAllKeys();

#if EDITOR
    delete m_resourceInterface;
    m_resourceInterface = new PlayerResourceInterface(&m_resources);
#endif
}

/// <summary>
/// Checks to see if the player is dead
/// </summary>
bool PlayerResourceManager::CheckForPlayerDeath()
{
    bool isPlayerAlive = true;
    for (
        std::map<string, PlayerResource*>::iterator itt = m_resources.begin();
        itt != m_resources.end(); itt++)
    {
        if (itt->second->GetKillsOnDrain())
        {
            int value = itt->second->GetValue();
            int minValue = itt->second->GetMinValue();
            return false;
        }
    }

    return isPlayerAlive;
}

/// <summary>
/// Takes damage if Weapon is ticked
/// </summary>
/// <param name="damage">Damage amount</param>
void PlayerResourceManager::TakeWeaponDamage(float damage)
{
    for (
        std::map<string, PlayerResource*>::iterator itt = m_resources.begin();
        itt != m_resources.end(); itt++)
    {
        if (itt->second->GetAttachesToWeaponDamage())
        {
            int value = itt->second->GetValue();
            int newValue = value - damage;
            if (newValue < itt->second->GetMinValue())
            {
                newValue = itt->second->GetMinValue();
            }

            itt->second->SetValue(newValue);
        }
    }
}

/// <summary>
/// Update key list. This should be called when the key list is updated.
/// </summary>
void PlayerResourceManager::UpdateAllKeys()
{
    m_resourceKeys = list<string>();
    for (std::pair<string, PlayerResource*> const& resource : m_resources)
    {
        m_resourceKeys.push_back(resource.first);
    }
}

/// <summary>
/// Gets all the keys in the resource list
/// </summary>
/// <returns>All the keys for resources</returns>
list<string> PlayerResourceManager::GetResourceKeys()
{
    return m_resourceKeys;
}

/// <summary>
/// Uses a resource
/// </summary>
/// <param name="key">Resource to use</param>
/// <param name="value">Amount to use</param>
/// <returns>True means it was used, False means it could not be used</returns>
bool PlayerResourceManager::UseResource(string key, int value)
{
    bool found = std::any_of(m_resources.begin(), m_resources.end(),
        [&key](std::pair<const string, PlayerResource*>& entry)
        {
            return (entry.first == key);
        });

    bool didUseResource = false;
    if (found)
    {
        int min = m_resources[key]->GetMinValue();
        int newValue = m_resources[key]->GetValue() - value;
        if (newValue >= min)
        {
            m_resources[key]->SetValue(newValue);
            didUseResource = true;
        }
    }

    return didUseResource;
}

/// <summary>
/// Gives a resource
/// </summary>
/// <param name="key">Resource to give</param>
/// <param name="value">Amount to give</param>
/// <returns>True means it was given, False means it could not be given</returns>
bool PlayerResourceManager::GiveResource(string key, int value)
{
    bool found = std::any_of(m_resources.begin(), m_resources.end(),
        [&key](std::pair<const string, PlayerResource*>& entry)
        {
            return (entry.first == key);
        });

    bool didUseResource = false;
    if (found)
    {
        int max = m_resources[key]->GetMaxValue();
        int newValue = m_resources[key]->GetValue() + value;
        if (newValue <= max)
        {
            m_resources[key]->SetValue(newValue);
            didUseResource = true;
        }
    }

    return didUseResource;
}

/// <summary>
/// Gets the value of a resource
/// </summary>
/// <param name="key">Keyt of the resource</param>
/// <returns>The value or -1 if not found</returns>
int PlayerResourceManager::GetResourceValue(string key)
{
    bool found = std::any_of(m_resources.begin(), m_resources.end(),
        [&key](std::pair<const string, PlayerResource*>& entry)
        {
            return (entry.first == key);
        });

    int resourceValue = -1;
    if (found)
    {
        resourceValue = m_resources[key]->GetValue();
    }

    return resourceValue;
}