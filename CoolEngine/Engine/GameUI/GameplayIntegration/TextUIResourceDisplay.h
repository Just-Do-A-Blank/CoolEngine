#pragma once
#include "Engine/GameUI/GameplayIntegration/GameplayUIResourceAttachment.h"
#include "Engine/GameUI/TextComponent.h"

class TextUIResourceDisplay : public GameplayUIResourceAttachment
{
public:
	TextUIResourceDisplay(TextComponent* component);
	TextUIResourceDisplay(nlohmann::json& data, TextComponent* component);
	TextUIResourceDisplay(TextUIResourceDisplay const& other, TextComponent* component);
	virtual ~TextUIResourceDisplay() override;

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
	virtual void Update(int resourceValue) override;

private:
	void LoadLocalData(const nlohmann::json& jsonData);
	void SaveLocalData(nlohmann::json& jsonData);

	/// <summary>
	/// The actual UI text
	/// </summary>
	TextComponent* m_textComponent;

	/// <summary>
	/// Text before the number
	/// </summary>
	string m_prefixText;

	/// <summary>
	/// Text after the number
	/// </summary>
	string m_suffixText;

    /// <summary>
    /// The value last updated to
    /// </summary>
    int m_lastKnownResourceValue;

    /// <summary>
    /// True means this has updated
    /// </summary>
    bool m_haveEverUpdated;

    /// <summary>
    /// Detirmines if the component should update
    /// </summary>
    /// <param name="currentResourceValue">The resource value if updating</param>
    /// <return>True means should update</return>
    bool ShouldUpdate(int currentResourceValue);

    /// <summary>
    /// Flips and switches to confirm this has updated
    /// </summary>
    /// <param name="currentResourceValue">The resource value if updating</param>
    void UpdateComplete(int currentResourceValue);
};

