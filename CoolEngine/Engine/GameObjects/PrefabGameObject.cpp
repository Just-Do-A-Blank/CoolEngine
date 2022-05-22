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
    m_prefabLoadedKey = "";
}

PrefabGameObject::PrefabGameObject(PrefabGameObject const& other) : GameObject(other)
{
    m_prefabLoadedKey = "";
    m_prefabKey = other.m_prefabKey;

    if (m_prefabKey != "")
    {
        LoadPrefab(m_prefabKey);
    }
}

PrefabGameObject::PrefabGameObject(string identifier, CoolUUID uuid) : GameObject(identifier, uuid)
{
    m_prefabLoadedKey = "";
}

PrefabGameObject::PrefabGameObject(const nlohmann::json& data, CoolUUID uuid) : GameObject(data, uuid)
{
    m_prefabLoadedKey = "";
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

void PrefabGameObject::Start()
{
    GameObject::Start();
    m_prefabFileData = nlohmann::json();
}

void PrefabGameObject::Serialize(nlohmann::json& jsonData)
{
    GameObject::Serialize(jsonData);

    SaveAllLocalData(jsonData);
}

/// <summary>
/// Save all prefab data - this should be overriden and passed up the chain to save the whole prefab.
/// </summary>
/// <param name="jsonData">Data to add to</param>
void PrefabGameObject::SaveAllPrefabData(nlohmann::json& jsonData)
{
    SaveAllLocalData(jsonData);
}

/// <summary>
/// Saves all local data within this class
/// </summary>
/// <param name="jsonData">Data to be added to</param>
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

            EditorUINonSpecificParameters parameters = EditorUINonSpecificParameters();
            parameters.m_tooltipText = "Unique key which becomes the name of the prefab";

            EditorUI::InputText("Key", m_prefabKey, parameters);
            EditorButtonCallback callbacks = EditorUI::BasicDuelButtons("Save", "Load");

            m_basicLoadErrorBox = EditorUI::ErrorPopupBox("PrefabBox", "Could not load Prefab. Ensure it exists and is valid.");
            m_basicSaveErrorBox = EditorUI::ErrorPopupBox("PrefabBox2", 
                "Please enter a valid prefab key.\nIf you want this to no longer be a prefab, leave it blank and save the scene.");

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
                PrepareUserInputAsPrefabFileName(m_prefabKey);

                if (IsThisKeyCurrentKeyOrNewKey(m_prefabKey))
                {
                    if (CanLoadPrefab(m_prefabKey))
                    {
                        EditorUI::ShowError("PrefabBoxButtons");
                        m_buttonErrorBox = true;
                    }
                }

                if (!m_buttonErrorBox && !m_basicSaveErrorBox)
                {
                    if (m_prefabKey == "")
                    {
                        EditorUI::ShowError("PrefabBox2");
                        m_basicSaveErrorBox = true;
                    }
                    else
                    {
                        SavePrefab(m_prefabKey);
                    }
                }
            }

            if (callbacks.m_rightButton)
            {
                PrepareUserInputAsPrefabFileName(m_prefabKey);

                if (CanLoadPrefab(m_prefabKey))
                {
                    LoadPrefab(m_prefabKey);
                }
                else
                {
                    EditorUI::ShowError("PrefabBox");
                    m_basicLoadErrorBox = true;
                }
            }
        }
    }
#endif

/// <summary>
/// Save the prefab to file
/// </summary>
/// <param name="key">The prefab key</param>
void PrefabGameObject::SavePrefab(string key)
{
    key.append(".json");

    std::string path = GameManager::GetInstance()->GetWorkingDirectory() + FOLDERLOCATIONPREFAB + "\\" + key;

    ofstream fileOut(path);

    nlohmann::json outData;

    SaveAllPrefabData(outData);

    fileOut << outData;

    fileOut.close();
}

/// <summary>
/// Load the prefab file.
/// Note this will not work on consutrction as construction is top down.
/// </summary>
/// <param name="key">The prefab file</param>
void PrefabGameObject::LoadPrefab(string key)
{
    key.append(".json");
    std::string path = GameManager::GetInstance()->GetWorkingDirectory() + FOLDERLOCATIONPREFAB + "\\" + key;

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

/// <summary>
/// Stores the data given at launch. Used to allow polymorph children to grab prefab data.
/// </summary>
/// <param name="key">The prefab key</param>
void PrefabGameObject::CachePrefabData(string key)
{
    key.append(".json");
    std::string path = GameManager::GetInstance()->GetWorkingDirectory() + FOLDERLOCATIONPREFAB + "\\" + key;

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

/// <summary>
/// If this is a prefab, this gets the data to use 
/// as opposed to the data saved by the scene which maybe out of date.
/// </summary>
nlohmann::json PrefabGameObject::GetPrefabDataLoadedAtCreation()
{
    return m_prefabFileData;
}

/// <summary>
/// Detirmines if the given key could be loaded if attempted - in theory
/// </summary>
/// <param name="key">The key (file name no extention)</param>
/// <returns>True, means it could be</returns>
bool PrefabGameObject::CanLoadPrefab(string key)
{
    key.append(".json");
    std::string path = GameManager::GetInstance()->GetWorkingDirectory() + FOLDERLOCATIONPREFAB + "\\" + key;
    return IsPrefabFileValid(path);
}

/// <summary>
/// Detirmines if the prefab file at the location looks acceptable
/// </summary>
/// <param name="location">The actual location of the prefab file</param>
/// <returns>True means valid</returns>
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

/// <summary>
/// Ensures the given string is a valid file name for a prefab (no extention)
/// </summary>
/// <param name="text">Text to alter</param>
void PrefabGameObject::PrepareUserInputAsPrefabFileName(std::string& s)
{
    if (s == "")
    {
        return;
    }

    TrimSpacesEitherSideOfText(s);
    TextToLower(s);
}

/// <summary>
/// Trims any space characters on either side of the string
/// </summary>
/// <param name="text">Text to trim</param>
void PrefabGameObject::TrimSpacesEitherSideOfText(std::string& s)
{
    s.erase(0, s.find_first_not_of(' '));
    s.erase(s.find_last_not_of(' ') + 1);
}

/// <summary>
/// Converts string to lowercase
/// </summary>
/// <param name="text">Text to alter</param>
void PrefabGameObject::TextToLower(string& text)
{
    for (int i = 0; i < text.length(); ++i)
    {
        text[i] = tolower(text[i]);
    }
}

bool PrefabGameObject::IsThisKeyCurrentKeyOrNewKey(string key)
{
    if (m_prefabLoadedKey == "")
    {
        return false;
    }

    return key != m_prefabLoadedKey;
}