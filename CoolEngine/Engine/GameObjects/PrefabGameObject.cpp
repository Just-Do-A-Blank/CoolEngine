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

            m_basicErrorBox = EditorUI::ErrorPopupBox("PrefabBox", "Could not load Prefab. Ensure it exists and is valid.");

            if (m_buttonErrorBox)
            {
                m_buttonMessageBoxCallback = EditorUI::ErrorPopupBoxWithOptions
                    ("PrefabBoxButtons", 
                    "This is already a valid prefab and you haven't loaded as this prefab.\n Are you sure you want to overrite this?"
                        ,"Cancel", "Save Anyway");

                if (m_buttonMessageBoxCallback.m_leftButton || m_buttonMessageBoxCallback.m_rightButton)
                {
                    m_buttonErrorBox = false;

                    if (m_buttonMessageBoxCallback.m_leftButton)
                    {
                        m_prefabKey = m_prefabLoadedKey;
                    }
                    else if(m_buttonMessageBoxCallback.m_rightButton)
                    {
                        SavePrefab(m_prefabKey);
                    }
                }
            }

            if (callbacks.m_leftButton)
            {
                if (m_prefabKey != m_prefabLoadedKey)
                {
                    if (CanLoadPrefab(m_prefabKey))
                    {
                        EditorUI::ShowError("PrefabBoxButtons");
                        m_buttonErrorBox = true;
                    }
                }

                if (!m_buttonErrorBox)
                {
                    SavePrefab(m_prefabKey);
                }
            }

            if (callbacks.m_rightButton)
            {
                if (CanLoadPrefab(m_prefabKey))
                {
                    LoadPrefab(m_prefabKey);
                }
                else
                {
                    EditorUI::ShowError("PrefabBox");
                    m_basicErrorBox = true;
                }
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

    if (IsPrefabFileValid(path))
    {
        ifstream fileIn(path);
        nlohmann::json dataIn;
        fileIn >> dataIn;

        LoadAllPrefabData(dataIn);

        m_prefabLoadedKey = m_prefabKey;
    }
    else
    {
        m_prefabKey = "";
    }
}

void PrefabGameObject::CachePrefabData(string key)
{
    key.append(".json");
    std::string path = GameManager::GetInstance()->GetWorkingDirectory() + "\\" + key;

    if (IsPrefabFileValid(path))
    {
        ifstream fileIn(path);
        nlohmann::json dataIn;
        fileIn >> dataIn;

        m_prefabFileData = dataIn;

        m_prefabLoadedKey = m_prefabKey;
    }
    else
    {
        m_prefabFileData = nlohmann::json();
        m_prefabKey = "";
    }

}

nlohmann::json PrefabGameObject::GetPrefabDataLoadedAtCreation()
{
    return m_prefabFileData;
}

bool PrefabGameObject::CanLoadPrefab(string key)
{
    key.append(".json");
    std::string path = GameManager::GetInstance()->GetWorkingDirectory() + "\\" + key;
    return IsPrefabFileValid(path);
}

bool PrefabGameObject::IsPrefabFileValid(string location)
{
    ifstream ifile;
    ifile.open(location);

    bool doesFileExist = false;
    if (ifile)
    {
        doesFileExist = true;
    }
    ifile.close();

    return doesFileExist;
}