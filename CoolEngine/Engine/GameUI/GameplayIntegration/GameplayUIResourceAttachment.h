#pragma once
#include "Engine/Includes/json.hpp"
#include <Engine/GameObjects/PlayerGameObject.h>

class GameplayUIResourceAttachment
{
public:
	GameplayUIResourceAttachment();
	GameplayUIResourceAttachment(nlohmann::json& data);
	GameplayUIResourceAttachment(GameplayUIResourceAttachment const& other);
	~GameplayUIResourceAttachment();

#if EDITOR
	virtual void CreateEngineUI();

	/// <summary>
	/// A label for when the player is found
	/// </summary>
	string m_buttonFoundPlayerLabel;

	/// <summary>
	/// True means an inital attempt to find the player was completed
	/// </summary>
	bool m_attmptedToFindPlayer;
#endif

	/// <summary>
	/// Called after construction, before first Update.
	/// </summary>
	virtual void Start();

protected:
	virtual void LoadAllPrefabData(const nlohmann::json& jsonData);
	virtual void SaveAllPrefabData(nlohmann::json& jsonData);

	/// <summary>
	/// Gets the resource Manager
	/// </summary>
	/// <returns>Manages the Player Resources (Health and Stamina for example)</returns>
	PlayerResourceManager* GetResourceManager();

private:
	void LoadLocalData(const nlohmann::json& jsonData);
	void SaveLocalData(nlohmann::json& jsonData);

	/// <summary>
	/// Gets the player gameobject. Safe to call in editor or play.
	/// </summary>
	/// <returns>The current player</returns>
	PlayerGameObject* GetPlayer();

	string m_resourceKey;

	int m_resourceCurrentValue;

	/// <summary>
	/// The current player. In game this is the current player.
	/// </summary>
	PlayerGameObject* m_currentPlayer;
};

