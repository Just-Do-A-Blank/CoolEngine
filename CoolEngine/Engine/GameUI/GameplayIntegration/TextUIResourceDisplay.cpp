#include "TextUIResourceDisplay.h"
#include "Engine/EditorUI/EditorUI.h"

TextUIResourceDisplay::TextUIResourceDisplay(TextComponent* component) : GameplayUIResourceAttachment()
{
	m_prefixText = "";
	m_suffixText = "";
	m_textComponent = component;
}

TextUIResourceDisplay::TextUIResourceDisplay(nlohmann::json& data, TextComponent* component) : GameplayUIResourceAttachment(data)
{
	LoadLocalData(data);
	m_textComponent = component;
}

TextUIResourceDisplay::TextUIResourceDisplay(TextUIResourceDisplay const& other, TextComponent* component) : GameplayUIResourceAttachment(other)
{
	m_prefixText = other.m_prefixText;
	m_suffixText = other.m_suffixText;
	m_textComponent = component;
}

TextUIResourceDisplay::~TextUIResourceDisplay()
{

}

/// <summary>
/// Runs during gameplay with the resource value
/// </summary>
/// <param name="resourceValue">The resource value if set</param>
void TextUIResourceDisplay::Update(int resourceValue)
{
	string newText = m_prefixText;
	newText += to_string(resourceValue);
	newText += m_suffixText;

	m_textComponent->SetText(newText);
}

#if EDITOR
	void TextUIResourceDisplay::CreateEngineUI()
	{
		GameplayUIResourceAttachment::CreateEngineUI();

		if (EditorUI::CollapsingSection("Text as Resource", true))
		{
			EditorUI::FullTitle("Overrides Text with Resource value");
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
	GameplayUIResourceAttachment::Start();
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