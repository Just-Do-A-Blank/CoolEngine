#pragma once
#include "Engine/GameUI/GameplayIntegration/GameplayUIResourceAttachment.h"

class TextUIResourceDisplay : public GameplayUIResourceAttachment
{
public:
	TextUIResourceDisplay();
	TextUIResourceDisplay(nlohmann::json& data);
	TextUIResourceDisplay(TextUIResourceDisplay const& other);
	~TextUIResourceDisplay();

#if EDITOR
	virtual void CreateEngineUI();
#endif

	virtual void Serialize(nlohmann::json& data);

	/// <summary>
	/// Called after construction, before first Update.
	/// </summary>
	virtual void Start();

protected:
	virtual void LoadAllPrefabData(const nlohmann::json& jsonData);
	virtual void SaveAllPrefabData(nlohmann::json& jsonData);

	/// <summary>
	/// Runs during gameplay with the resource value
	/// </summary>
	/// <param name="resourceValue">The resource value if set</param>
	virtual void Update(int resourceValue);

private:
	void LoadLocalData(const nlohmann::json& jsonData);
	void SaveLocalData(nlohmann::json& jsonData);

	/// <summary>
	/// Text before the number
	/// </summary>
	string m_prefixText;

	/// <summary>
	/// Text after the number
	/// </summary>
	string m_suffixText;
};

