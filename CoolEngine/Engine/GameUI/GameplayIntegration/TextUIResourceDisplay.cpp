#include "TextUIResourceDisplay.h"
#include "Engine/EditorUI/EditorUI.h"

TextUIResourceDisplay::TextUIResourceDisplay() : GameplayUIResourceAttachment()
{
	m_prefixText = "";
	m_suffixText = "";
}

TextUIResourceDisplay::TextUIResourceDisplay(nlohmann::json& data) : GameplayUIResourceAttachment(data)
{
	LoadLocalData(data);
}

TextUIResourceDisplay::TextUIResourceDisplay(TextUIResourceDisplay const& other) : GameplayUIResourceAttachment(other)
{
	m_prefixText = other.m_prefixText;
	m_suffixText = other.m_suffixText;
}

TextUIResourceDisplay::~TextUIResourceDisplay()
{

}

#if EDITOR
	void TextUIResourceDisplay::CreateEngineUI()
	{
		GameplayUIResourceAttachment::CreateEngineUI();

		if (EditorUI::CollapsingSection("Text as Resource", true))
		{
			EditorUI::InputText("Prefix", m_prefixText);
			EditorUI::InputText("Suffix", m_suffixText);
		}
	}
#endif

void TextUIResourceDisplay::Serialize(nlohmann::json& data)
{
	GameplayUIResourceAttachment::Serialize(data);
	SaveLocalData(data);
}

/// <summary>
/// Called after construction, before first Update.
/// </summary>
void TextUIResourceDisplay::Start()
{

}

void TextUIResourceDisplay::LoadAllPrefabData(const nlohmann::json& jsonData)
{
	GameplayUIResourceAttachment::LoadAllPrefabData(jsonData);
	LoadLocalData(jsonData);
}
void TextUIResourceDisplay::SaveAllPrefabData(nlohmann::json& jsonData)
{
	SaveLocalData(jsonData);
	GameplayUIResourceAttachment::SaveAllPrefabData(jsonData);
}

/// <summary>
/// Runs during gameplay with the resource value
/// </summary>
/// <param name="resourceValue">The resource value if set</param>
void TextUIResourceDisplay::Update(int resourceValue)
{

}


void TextUIResourceDisplay::LoadLocalData(const nlohmann::json& jsonData)
{
	if (jsonData.contains("TextUIResourceDisplay_prefix"))
	{
		m_prefixText = jsonData["TextUIResourceDisplay_prefix"];
		m_suffixText = jsonData["TextUIResourceDisplay_suffix"];
	}
}

void TextUIResourceDisplay::SaveLocalData(nlohmann::json& jsonData)
{
	jsonData["TextUIResourceDisplay_prefix"] = m_prefixText;
	jsonData["TextUIResourceDisplay_suffix"] = m_suffixText;
}