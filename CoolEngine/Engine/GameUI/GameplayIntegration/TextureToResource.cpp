#include "Engine/GameUI/GameplayIntegration/TextureToResource.h"
#include "Engine/EditorUI/EditorUI.h"
#include "Engine/Managers/GraphicsManager.h"

TextureToResource::TextureToResource()
{
	m_setting = EUIRESOURCECHANGESETTING::ChangeWhenNumberHit;
	m_texturePath = wstring();
	m_resourceValue = 0;

	m_settingAsAList =
	{
		pair<int, string>(0, "Change when hit"),
		pair<int, string>(1, "Change when above (>)"),
		pair<int, string>(2, "Change when below (<)")
	};

	SetSelectedBasedOnSettingEnum(m_setting);
}

TextureToResource::TextureToResource(const nlohmann::json& data, int key)
{
	LoadLocalData(data, key);

	m_settingAsAList =
	{
		pair<int, string>(0, "Change when hit"),
		pair<int, string>(1, "Change when above (>)"),
		pair<int, string>(2, "Change when below (<)")
	};

	SetSelectedBasedOnSettingEnum(m_setting);
}

TextureToResource::TextureToResource(TextureToResource const& other)
{
	m_setting = other.m_setting;
	m_texturePath = other.m_texturePath;
	m_resourceValue = other.m_resourceValue;

	m_settingAsAList =
	{
		pair<int, string>(0, "Change when hit"),
		pair<int, string>(1, "Change when above (>)"),
		pair<int, string>(2, "Change when below (<)")
	};

	SetSelectedBasedOnSettingEnum(m_setting);
}

TextureToResource::~TextureToResource()
{

}

void TextureToResource::LoadAllPrefabData(const nlohmann::json& jsonData, int key)
{
	LoadLocalData(jsonData, key);
}
void TextureToResource::SaveAllPrefabData(nlohmann::json& jsonData, int key)
{
	SaveLocalData(jsonData, key);
}

void TextureToResource::LoadLocalData(const nlohmann::json& jsonData, int key)
{
	string prefix = "TextureToResource_" + to_string(key) + "_";
	if (jsonData.contains(prefix + "Setting"))
	{
		m_setting = jsonData[prefix + "Setting"];
		
		std::string tempPath = jsonData[prefix + "TexturePath"];
		m_texturePath = std::wstring(tempPath.begin(), tempPath.end());

		SetTexture(m_texturePath);

		m_resourceValue = jsonData[prefix + "ResourceValue"];
	}
}

void TextureToResource::SaveLocalData(nlohmann::json& jsonData, int key)
{
	string prefix = "TextureToResource_" + to_string(key) + "_";

	jsonData[prefix + "Setting"] = m_setting;

	std::string tempPath = std::string(m_texturePath.begin(), m_texturePath.end());
	jsonData[prefix + "TexturePath"] = tempPath;

	jsonData[prefix + "ResourceValue"] = m_resourceValue;
}

#if EDITOR
	void TextureToResource::CreateEngineUI(int key)
	{
		if (EditorUI::CollapsingSection("Image: " + to_string(key), true))
		{
			EditorUI::Texture("Texture", m_texturePath, m_ptexture);

			EditorUIIntParameters intParams = EditorUIIntParameters();
			intParams.m_tooltipText = "What resource value you would like to infer from the setting context";
			EditorUI::DragInt("Target", m_resourceValue);

			if (EditorUI::ComboBox("Setting", m_settingAsAList, m_selectedSettingValue))
			{
				m_setting = (EUIRESOURCECHANGESETTING)m_selectedSettingValue.first;
			}
		}
	}
#endif

wstring TextureToResource::GetTextureOut()
{
	return m_texturePath;
}

int TextureToResource::GetResourceValue()
{
	return GetResourceValue();
}

EUIRESOURCECHANGESETTING TextureToResource::GetResourceSetting()
{
	return m_setting;
}

/// <summary>
/// Given the input would this texture set (no other input given)
/// </summary>
/// <param name="currentResourceValue">Current resource value</param>
/// <returns>True means yes it would</returns>
bool TextureToResource::WouldSetGivenValue(int currentResourceValue)
{
	bool answer = false;
	switch (m_setting)
	{
		case EUIRESOURCECHANGESETTING::ChangeWhenNumberHit:
			answer = currentResourceValue == m_resourceValue;
			break;
		case EUIRESOURCECHANGESETTING::ChangeWhenNumberIsAboveThis:
			answer = currentResourceValue < m_resourceValue;
			break;
		case EUIRESOURCECHANGESETTING::ChangeWhenNumberIsBelowThis:
			answer = currentResourceValue > m_resourceValue;
			break;
	}

	return answer;
}

void TextureToResource::SetTexture(std::wstring wsfilepath)
{
	m_ptexture = GraphicsManager::GetInstance()->GetShaderResourceView(wsfilepath);

	if (m_ptexture == nullptr)
	{
		LOG("Tried to get a texture that doesn't exist!");
	}
	else
	{
		m_texturePath = wsfilepath;
	}
}

/// <summary>
/// Used when loading to ensure the settings equals the value we actually use in code
/// </summary>
/// <param name="settings">Settings value</param>
void TextureToResource::SetSelectedBasedOnSettingEnum(EUIRESOURCECHANGESETTING settings)
{
	switch (settings)
	{
		case EUIRESOURCECHANGESETTING::ChangeWhenNumberHit:
			m_selectedSettingValue = pair<int, string>(0, "Change when hit");
			break;
		case EUIRESOURCECHANGESETTING::ChangeWhenNumberIsAboveThis:
			m_selectedSettingValue = pair<int, string>(1, "Change when above (>)");
			break;
		case EUIRESOURCECHANGESETTING::ChangeWhenNumberIsBelowThis:
			m_selectedSettingValue = pair<int, string>(2, "Change when below (<)");
			break;
	}
}