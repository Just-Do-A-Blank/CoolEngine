#include "GameplayUIResourceAttachment.h"
#include "Engine/EditorUI/EditorUI.h"
#include "Engine/Managers/GameManager.h"

GameplayUIResourceAttachment::GameplayUIResourceAttachment()
{
	m_resourceKey = "";
	m_resourceCurrentValue = 0;
	m_currentPlayer = nullptr;
}

GameplayUIResourceAttachment::GameplayUIResourceAttachment(nlohmann::json& data)
{
	m_currentPlayer = nullptr;
}

GameplayUIResourceAttachment::GameplayUIResourceAttachment(GameplayUIResourceAttachment const& other)
{
	m_currentPlayer = nullptr;
}

GameplayUIResourceAttachment::~GameplayUIResourceAttachment()
{

}

#if EDITOR
	void GameplayUIResourceAttachment::CreateEngineUI()
	{
		if (EditorUI::CollapsingSection("Resource Attachement", true))
		{
			
		}
	}
#endif


/// <summary>
/// Called after construction, before first Update.
/// </summary>
void GameplayUIResourceAttachment::Start()
{
	GameManager* gm = GameManager::GetInstance();
	PlayerGameObject* player = gm->GetGameObjectUsingIdentifier<PlayerGameObject>(std::string("Player"));
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