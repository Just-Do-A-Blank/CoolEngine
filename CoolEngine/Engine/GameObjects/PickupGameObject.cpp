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
    m_isConsumedOnPickup = false;
    m_pPickupResourceInterface = new PickupResourceInterface(&m_pResouces);
    m_UItexturePath = std::wstring();
}

PickupGameObject::PickupGameObject(const nlohmann::json& data, CoolUUID index) : InteractableGameObject(data, index)
{
    m_gameObjectType |= GameObjectType::PICKUP;

    m_isConsumedOnPickup = false;
    m_pPickupResourceInterface = new PickupResourceInterface(&m_pResouces);

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
    m_pPickupResourceInterface = new PickupResourceInterface(*other.m_pPickupResourceInterface);
    m_isConsumedOnPickup = other.m_isConsumedOnPickup;

    m_UItexturePath = other.m_UItexturePath;
    SetUITexture(m_UItexturePath);
}

PickupGameObject::~PickupGameObject()
{
    delete m_pPickupResourceInterface;
    m_pPickupResourceInterface = nullptr;
}

void PickupGameObject::Start()
{
    m_pPlayer = GameManager::GetInstance()->GetGameObjectUsingIdentifier<PlayerGameObject>(string("Player"));
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
        EditorUINonSpecificParameters nonPara;
        nonPara.m_columnWidth = 150;
        nonPara.m_tooltipText = "";

        EditorUI::FullTitle("Pickup Parameters", nonPara);

        ImGui::Spacing();
        EditorUIIntParameters intParameter;
        intParameter.m_minValue = 0;
        intParameter.m_tooltipText = "Choose the quantity of this item this pickup represents";
        intParameter.m_columnWidth = 150;
        intParameter.m_speed = 1;
        EditorUI::DragInt("Quantity", m_quantity, intParameter);

        ImGui::Spacing();
        nonPara.m_columnWidth = 150;
        nonPara.m_tooltipText = "If the pickup will be consumed by the player on pickup or it will go into inventory";
        EditorUI::Checkbox("Consumed on Pickup", m_isConsumedOnPickup, nonPara);

        ImGui::Spacing();

        m_pPickupResourceInterface->CreateEngineUI();

        EditorUI::Texture("UI Texture", m_UItexturePath, m_pUItexture, 150, ImVec2(150, 150));

        ImGui::Spacing();
    }
}
#endif



void PickupGameObject::LoadLocalData(const nlohmann::json& jsonData)
{
    m_pResouces.clear();
    m_isConsumedOnPickup = jsonData["ConsumedOnPickup"];
    m_quantity = jsonData["PickupQuantity"];    
    
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
            string quantity = "EffectQuantity";
            quantity.append(to_string(i));
            m_pResouces.insert(new PickupResource(key, jsonData[strength]));
            i++;
            PickupsManager::GetInstance()->GetList()->insert(key);
        }
        else
        {
            exists = false;
        }

    } while (exists);

    if (jsonData.contains("PickupUITexturePath"))
    {
        std::string tempPath = jsonData["PickupUITexturePath"];
        m_UItexturePath = std::wstring(tempPath.begin(), tempPath.end());

        SetUITexture(m_UItexturePath);
    }
}

void PickupGameObject::SaveLocalData(nlohmann::json& jsonData)
{

    jsonData["ConsumedOnPickup"] = m_isConsumedOnPickup;
    jsonData["PickupQuantity"] = m_quantity;

    unordered_set<PickupResource*>::iterator it;
    int i = 0;
    unordered_set<PickupResource*> effects = *m_pPickupResourceInterface->GetEffects();
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

    std::string tempPath = std::string(m_UItexturePath.begin(), m_UItexturePath.end());
    jsonData["PickupUITexturePath"] = tempPath;
}


void PickupGameObject::Update()
{
    InteractableGameObject::Update();
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


void PickupGameObject::SetUITexture(std::wstring wsfilepath)
{
    m_pUItexture = GraphicsManager::GetInstance()->GetShaderResourceView(wsfilepath);

    if (m_pUItexture == nullptr)
    {
        LOG("Tried to get a texture that doesn't exist!");
    }
    else
    {
        m_UItexturePath = wsfilepath;
    }
}

/// <summary>
/// Shows engine UI
/// </summary>
/// <return>Get the UI Texture for the Pickup</return>
std::wstring PickupGameObject::GetUITexturePath()
{
    return m_UItexturePath;
}