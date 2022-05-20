#include "Engine/GameObjects/PrefabGameObject.h"
#include "Engine/EditorUI/EditorUI.h"
#include "Engine/Managers/GameManager.h"
#include  "Engine/Includes/json.hpp"

#include <fstream>
#include <iostream>

PrefabGameObject::PrefabGameObject() : GameObject()
{
    m_prefabKey = "";
    m_prefabDataFromLaunch = nlohmann::json();
}

PrefabGameObject::PrefabGameObject(PrefabGameObject const& other) : GameObject(other)
{
    GameObject::Init(other);

    m_prefabKey = other.m_prefabKey;
    m_prefabDataFromLaunch = other.m_prefabDataFromLaunch;
}

PrefabGameObject::PrefabGameObject(string identifier, CoolUUID uuid) : GameObject(identifier, uuid)
{
    m_prefabKey = "";
    m_prefabDataFromLaunch = nlohmann::json();
}

PrefabGameObject::PrefabGameObject(const nlohmann::json& data, CoolUUID uuid) : GameObject(data, uuid)
{
    GameObject::Init(data, uuid);

    m_prefabKey = data["PrefabKey"];

    if (m_prefabKey != "")
    {
        //LoadPrefabDataAtLaunch(m_prefabKey);
    }
    else
    {
        m_prefabDataFromLaunch = nlohmann::json();
    }
}

PrefabGameObject::~PrefabGameObject()
{

}

void PrefabGameObject::SaveLocalData(nlohmann::json& jsonData)
{
    jsonData["PrefabKey"] = m_prefabKey;
}

void PrefabGameObject::Serialize(nlohmann::json& jsonData)
{
    GameObject::Serialize(jsonData);

    SaveLocalData(jsonData);
}

void PrefabGameObject::SaveAllPrefabData(nlohmann::json& jsonData)
{
    SaveLocalData(jsonData);
}

#if EDITOR
void PrefabGameObject::CreateEngineUI()
{
    GameObject::CreateEngineUI();

    if (EditorUI::CollapsingSection("Prefab", true))
    {
        ImGui::Spacing();
        EditorUI::InputText("Key", m_prefabKey);
        EditorButtonCallback callbacks = EditorUI::BasicDuelButtons("Save", "Load");

        if (callbacks.m_leftButton)
        {
            SavePrefab(m_prefabKey);
        }

        if (callbacks.m_rightButton)
        {
            LoadPrefab(m_prefabKey);
        }
    }
}
#endif

void PrefabGameObject::SavePrefab(string key)
{
    key.append(".json");

    std::string path = GameManager::GetInstance()->GetWorkingDirectory() + "\\" + key;

    ofstream fileOut(path);

    nlohmann::json outData;

    SaveAllPrefabData(outData);

    fileOut << outData;

    fileOut.close();
}

void PrefabGameObject::LoadPrefab(string key)
{
    key.append(".json");
    std::string path = GameManager::GetInstance()->GetWorkingDirectory() + "\\" + key;
    ifstream fileIn(path);
    nlohmann::json dataIn;
    fileIn >> dataIn;

    LoadAllPrefabData(dataIn);
}

/// <summary>
/// Gets the data loaded at launch. Used to update the object if it is a prefab.
/// </summary>
/// <returns>Prefab data</returns>
const nlohmann::json PrefabGameObject::GetPrefabDataFromLaunch()
{
    return m_prefabDataFromLaunch;
}

void PrefabGameObject::LoadPrefabDataAtLaunch(string key)
{
    key.append(".json");
    std::string path = GameManager::GetInstance()->GetWorkingDirectory() + "\\" + key;
    ifstream fileIn(path);
    nlohmann::json dataIn;
    fileIn >> dataIn;

    m_prefabDataFromLaunch = dataIn;
}