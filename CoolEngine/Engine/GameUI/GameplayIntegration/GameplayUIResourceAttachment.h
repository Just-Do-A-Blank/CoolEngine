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

	/// <summary>
	/// Attempts to find the player once at the start of the objects life
	/// </summary>
	/// <param name="force">True means it will overide the start of life</param>
	void AttemptToFindPlayer(bool force = false);
#endif

	virtual void Serialize(nlohmann::json& data);

	/// <summary>
	/// Called after construction, before first Update.
	/// </summary>
	virtual void Start();

	void LoadFromTopLevel(const nlohmann::json& jsonData);
	void SaveFromTopLevel(nlohmann::json& jsonData);

protected:
	virtual void LoadAllPrefabData(const nlohmann::json& jsonData);
	virtual void SaveAllPrefabData(nlohmann::json& jsonData);

	/// <summary>
	/// Gets the resource Manager
	/// </summary>
	/// <returns>Manages the Player Resources (Health and Stamina for example)</returns>
	PlayerResourceManager* GetResourceManager();

#if EDITOR
	/// <summary>
	/// Get a list of all resource keys
	/// </summary>
	/// <param name="includeNone">True means none should be included</param>
	/// <returns>A list of all keys as strings</returns>
	list<string> GetResourceKeys(bool includeNone = false);
#endif

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

	/// <summary>
	/// All the resource keys currently on the player
	/// </summary>
	list<string> m_resourceKeys;
};

