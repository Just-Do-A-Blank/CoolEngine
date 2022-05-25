#include "GameplayUIResourceAttachment.h"
#include "Engine/EditorUI/EditorUI.h"
#include "Engine/Managers/GameManager.h"

GameplayUIResourceAttachment::GameplayUIResourceAttachment()
{
	m_resourceKey = "";
	m_resourceCurrentValue = 0;
	m_currentPlayer = nullptr;
#if EDITOR
	m_attmptedToFindPlayer = false;
#endif
}

GameplayUIResourceAttachment::GameplayUIResourceAttachment(nlohmann::json& data)
{
	m_currentPlayer = nullptr;
#if EDITOR
	m_attmptedToFindPlayer = false;
#endif

	LoadLocalData(data);
}

GameplayUIResourceAttachment::GameplayUIResourceAttachment(GameplayUIResourceAttachment const& other)
{
	m_currentPlayer = nullptr;
#if EDITOR
	m_attmptedToFindPlayer = false;
#endif

	m_resourceKey = other.m_resourceKey;
}

GameplayUIResourceAttachment::~GameplayUIResourceAttachment()
{

}

/// <summary>
/// Called after construction, before first Update.
/// </summary>
void GameplayUIResourceAttachment::Start()
{
    if (m_resourceKey == "None")
    {
        return;
    }

    AttemptToFindPlayer();
}

/// <summary>
/// Called when the UI element should update
/// </summary>
void GameplayUIResourceAttachment::Update()
{
	if (m_resourceKey == "None")
	{
		return;
	}

	PlayerResourceManager* manager = GetResourceManager();
	if (manager != nullptr)
	{
		int value = manager->GetResourceValue(m_resourceKey);
		Update(value);
	}
}

#if EDITOR
	void GameplayUIResourceAttachment::CreateEngineUI()
	{
		if (EditorUI::CollapsingSection("Resource Options", true))
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

				EditorUI::ComboBox("Resource", GetResourceKeys(true), m_resourceKey);

				ImGui::Separator();

				if (EditorUI::BasicButton("Update Resource Keys"))
				{
					AttemptToFindPlayer(true);
				}
			}
		}
	}

	/// <summary>
	/// Get a list of all resource keys
	/// </summary>
	/// <param name="includeNone">True means none should be included</param>
	/// <returns>A list of all keys as strings</returns>
	list<string> GameplayUIResourceAttachment::GetResourceKeys(bool includeNone)
	{
		list<string> keys = m_resourceKeys;
		if (includeNone)
		{
			keys.push_front("None");
		}

		return keys;
	}
#endif

/// <summary>
/// Attempts to find the player once at the start of the objects life
/// </summary>
void GameplayUIResourceAttachment::AttemptToFindPlayer(bool force)
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

	m_resourceKeys = m_currentPlayer->GetPlayerResources()->GetResourceKeys();
}



void GameplayUIResourceAttachment::LoadFromTopLevel(const nlohmann::json& jsonData)
{
	LoadAllPrefabData(jsonData);
}

void GameplayUIResourceAttachment::SaveFromTopLevel(nlohmann::json& jsonData)
{
	SaveAllPrefabData(jsonData);
}

void GameplayUIResourceAttachment::LoadAllPrefabData(const nlohmann::json& jsonData)
{
	LoadLocalData(jsonData);
}
void GameplayUIResourceAttachment::SaveAllPrefabData(nlohmann::json& jsonData)
{
	SaveLocalData(jsonData);
}


void GameplayUIResourceAttachment::LoadLocalData(const nlohmann::json& jsonData)
{
	if (jsonData.contains("GameplayUIResourceAttachment_key"))
	{
		m_resourceKey = jsonData["GameplayUIResourceAttachment_key"];
	}
}

void GameplayUIResourceAttachment::Serialize(nlohmann::json& data)
{
	SaveLocalData(data);
}

void GameplayUIResourceAttachment::SaveLocalData(nlohmann::json& jsonData)
{
	jsonData["GameplayUIResourceAttachment_key"] = m_resourceKey;
}

/// <summary>
/// Gets the player gameobject. Safe to call in editor or play.
/// </summary>
/// <returns>The current player</returns>
PlayerGameObject* GameplayUIResourceAttachment::GetPlayer()
{
	return m_currentPlayer;
}

/// <summary>
/// Gets the resource Manager. USE ONLY DURING PLAY!
/// </summary>
/// <returns>Manages the Player Resources (Health and Stamina for example)</returns>
PlayerResourceManager* GameplayUIResourceAttachment::GetResourceManager()
{
	PlayerResourceManager* manager = nullptr;
	if (m_currentPlayer != nullptr)
	{
		manager = m_currentPlayer->GetPlayerResources();
	}

	return manager;
}