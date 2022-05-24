#pragma once
#include "Engine/GameUI/GameplayIntegration/EUIResourceChangeSetting.h"
#include "Engine/Includes/json.hpp"

class TextureToResource
{
public:
	TextureToResource();
	TextureToResource(const nlohmann::json& data, int key);
	TextureToResource(TextureToResource const& other);
	virtual ~TextureToResource();

#if EDITOR
	/// <summary>
	/// Creates UI
	/// </summary>
	/// <param name="key">Used in the heading</param>
	virtual void CreateEngineUI(int key);
#endif

	wstring GetTextureOut();

	int GetResourceValue();

	EUIRESOURCECHANGESETTING GetResourceSetting();

	/// <summary>
	/// Given the input would this texture set (no other input given)
	/// </summary>
	/// <param name="currentResourceValue">Current resource value</param>
	/// <returns>True means yes it would</returns>
	bool WouldSetGivenValue(int currentResourceValue);

	void SetTexture(std::wstring wsfilepath);

	virtual void LoadAllPrefabData(const nlohmann::json& jsonData, int key);
	virtual void SaveAllPrefabData(nlohmann::json& jsonData, int key);

private:

	wstring m_texturePath;

	int m_resourceValue;

	EUIRESOURCECHANGESETTING m_setting;

	list<pair<int, string>> m_settingAsAList;

	pair<int, string> m_selectedSettingValue;

	ID3D11ShaderResourceView* m_ptexture = nullptr;

	void LoadLocalData(const nlohmann::json& jsonData, int key);
	void SaveLocalData(nlohmann::json& jsonData, int key);

	/// <summary>
	/// Used when loading to ensure the settings equals the value we actually use in code
	/// </summary>
	/// <param name="settings">Settings value</param>
	void SetSelectedBasedOnSettingEnum(EUIRESOURCECHANGESETTING settings);
};

