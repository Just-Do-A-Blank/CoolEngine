#include "Engine/GameObjects/PrefabGameObject.h"
#include "Engine\EditorUI\EditorUI.h"
#include "Engine/Managers/GameManager.h"

#include  "Engine/Includes/json.hpp"

#include <fstream>
#include <iostream>
#include <windows.h>
#include <codecvt>
#include <locale>

PrefabGameObject::PrefabGameObject() : GameObject()
{

}

PrefabGameObject::PrefabGameObject(PrefabGameObject const& other) : GameObject(other)
{
    m_prefabKey = other.m_prefabKey;

    if (m_prefabKey != "")
    {
        LoadPrefab(m_prefabKey);
    }
}

PrefabGameObject::PrefabGameObject(string identifier, CoolUUID uuid) : GameObject(identifier, uuid)
{

}

PrefabGameObject::PrefabGameObject(const nlohmann::json& data, CoolUUID uuid) : GameObject(data, uuid)
{
    if (data.contains("PrefabKey"))
    {
        m_prefabKey = data["PrefabKey"];

        if (m_prefabKey != "")
        {
            CachePrefabData(m_prefabKey);
        }
    }
}

PrefabGameObject::~PrefabGameObject()
{

}

void PrefabGameObject::Serialize(nlohmann::json& jsonData)
{
    GameObject::Serialize(jsonData);

    SaveAllLocalData(jsonData);
}

void PrefabGameObject::SaveAllPrefabData(nlohmann::json& jsonData)
{
    SaveAllLocalData(jsonData);
}

void PrefabGameObject::SaveAllLocalData(nlohmann::json& jsonData)
{
    jsonData["PrefabKey"] = m_prefabKey;
}

/// <summary>
/// True, means this is a prefab
/// </summary>
bool PrefabGameObject::IsPrefab()
{
    return m_prefabKey != "";
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

void PrefabGameObject::CachePrefabData(string key)
{
    key.append(".json");
    std::string path = GameManager::GetInstance()->GetWorkingDirectory() + "\\" + key;
    ifstream fileIn(path);
    nlohmann::json dataIn;
    fileIn >> dataIn;

    m_prefabFileData = dataIn;
}

nlohmann::json PrefabGameObject::GetPrefabDataLoadedAtCreation()
{
    return m_prefabFileData;
}