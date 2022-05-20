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
    m_prefabType = other.m_prefabType;

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
        m_prefabType = data["PrefabType"];

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

    jsonData["PrefabKey"] = m_prefabKey;
    jsonData["PrefabType"] = m_gameObjectType;
}

void PrefabGameObject::ValidateKeyAndType()
{
    if (!ContainsType(m_prefabType))
    {
        LOG("Prefab is not of correct type");
        m_prefabKey = "";
        m_prefabType = GameObjectType::BASE;
    }
}

void PrefabGameObject::SavePrefabData(nlohmann::json& jsonData)
{
    jsonData["PrefabKey"] = m_prefabKey;

    m_prefabType = m_gameObjectType;
    jsonData["PrefabType"] = m_gameObjectType;
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

    SavePrefabData(outData);

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

    LoadPrefabData(dataIn);
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

nlohmann::json PrefabGameObject::GetDataLoadedAtCreation()
{
    return m_prefabFileData;
}