#include "ImageUIWeaponDisplay.h"
#include "Engine/GameUI/ImageComponent.h"
#include "Engine/Managers/GraphicsManager.h"
#include "Engine/EditorUI/EditorUI.h"

ImageUIWeaponDisplay::ImageUIWeaponDisplay(ImageComponent* imageComponent) : GameplayUIWeaponAttachment()
{
	m_texturePathAttached = std::wstring();
	m_texturePathNotAttached = std::wstring();
}

ImageUIWeaponDisplay::ImageUIWeaponDisplay(nlohmann::json& data, ImageComponent* imageComponent) : GameplayUIWeaponAttachment(data)
{
	m_imageComponent = imageComponent;
	LoadLocalData(data);
}

ImageUIWeaponDisplay::ImageUIWeaponDisplay(ImageUIWeaponDisplay const& other, ImageComponent* imageComponent) : GameplayUIWeaponAttachment(other)
{
	m_imageComponent = imageComponent;
	m_texturePathAttached = other.m_texturePathAttached;
	SetAttachedTexture(m_texturePathAttached);

	m_texturePathNotAttached = other.m_texturePathNotAttached;
	SetNotAttachedTexture(m_texturePathNotAttached);
}

ImageUIWeaponDisplay::~ImageUIWeaponDisplay()
{

}

/// <summary>
/// Called after construction, before first Update.
/// </summary>
void ImageUIWeaponDisplay::Start()
{
	GameplayUIWeaponAttachment::Start();
}

/// <summary>
/// Runs during gameplay with the resource value
/// </summary>
/// <param name="weaponGameObject">The weapon to display</param>
void ImageUIWeaponDisplay::Update(WeaponGameObject* weaponGameObject)
{
	switch (m_weaponAttachmentOption)
	{
		case EWEAPONUIATTACHMENTOPTION::Holding:
			if (weaponGameObject != nullptr && m_texturePathAttached == std::wstring())
			{
				m_imageComponent->SetTexture(weaponGameObject->GetUITexturePath());
			}
			else
			{
				m_imageComponent->SetTexture(m_texturePathAttached);
			}
			break;
		case EWEAPONUIATTACHMENTOPTION::Inventory:
			if (weaponGameObject != nullptr && m_texturePathAttached == std::wstring())
			{
				m_imageComponent->SetTexture(weaponGameObject->GetUITexturePath());
			}
			else
			{
				m_imageComponent->SetTexture(m_texturePathNotAttached);
			}
		break;
	}
}

#if EDITOR
	void ImageUIWeaponDisplay::CreateEngineUI()
	{
		GameplayUIWeaponAttachment::CreateEngineUI();
		if (EditorUI::CollapsingSection("Weapon display", true))
		{
			EditorUI::FullTitle("Blank/No texture images mean\nnothing displayed or weapon image displayed.");

			float colWidth = 150;
			ImVec2 imageSize = ImVec2(100, 100);
			EditorUI::Texture("Image if held", m_texturePathAttached, m_ptextureWeaponAttached, colWidth, imageSize);

			if (EditorUI::BasicButton("Reset Image Held"))
			{
				SetAttachedTexture(std::wstring());
			}

			EditorUI::Texture("Image if not held", m_texturePathNotAttached, m_ptextureWeaponNotAttached, colWidth, imageSize);

			if (EditorUI::BasicButton("Reset Image Not Held"))
			{
				SetNotAttachedTexture(std::wstring());
			}

			ImGui::Separator();
		}
	}
#endif

void ImageUIWeaponDisplay::Serialize(nlohmann::json& data)
{
	GameplayUIWeaponAttachment::Serialize(data);
	SaveLocalData(data);
}

void ImageUIWeaponDisplay::LoadAllPrefabData(const nlohmann::json& jsonData)
{
	GameplayUIWeaponAttachment::LoadAllPrefabData(jsonData);
	LoadLocalData(jsonData);
}

void ImageUIWeaponDisplay::SaveAllPrefabData(nlohmann::json& jsonData)
{
	GameplayUIWeaponAttachment::SaveAllPrefabData(jsonData);
	SaveLocalData(jsonData);
}

void ImageUIWeaponDisplay::LoadLocalData(const nlohmann::json& jsonData)
{
	if (jsonData.contains("ImageUIWeaponDisplay_WeaponTexturePathAttached"))
	{
		std::string tempPath = jsonData["ImageUIWeaponDisplay_WeaponTexturePathAttached"];
		m_texturePathAttached = std::wstring(tempPath.begin(), tempPath.end());

		SetAttachedTexture(m_texturePathAttached);

		tempPath = jsonData["ImageUIWeaponDisplay_WeaponTexturePathNotAttached"];
		m_texturePathNotAttached = std::wstring(tempPath.begin(), tempPath.end());

		SetNotAttachedTexture(m_texturePathNotAttached);
	}

}

void ImageUIWeaponDisplay::SaveLocalData(nlohmann::json& jsonData)
{
	std::string tempPath = std::string(m_texturePathAttached.begin(), m_texturePathAttached.end());
	jsonData["ImageUIWeaponDisplay_WeaponTexturePathAttached"] = tempPath;

	tempPath = std::string(m_texturePathNotAttached.begin(), m_texturePathNotAttached.end());
	jsonData["ImageUIWeaponDisplay_WeaponTexturePathNotAttached"] = tempPath;
}

/// <summary>
/// Set the texture when a weapon is displayed
/// </summary>
/// <param name="wsfilepath">Texture path - Blank is valid</param>
void ImageUIWeaponDisplay::SetAttachedTexture(std::wstring wsfilepath)
{
	m_ptextureWeaponAttached = GraphicsManager::GetInstance()->GetShaderResourceView(wsfilepath);

	m_texturePathAttached = wsfilepath;
}

/// <summary>
/// Set the texture when a weapon is not displayed
/// </summary>
/// <param name="wsfilepath">Texture path - Blank is valid</param>
void ImageUIWeaponDisplay::SetNotAttachedTexture(std::wstring wsfilepath)
{
	m_ptextureWeaponNotAttached = GraphicsManager::GetInstance()->GetShaderResourceView(wsfilepath);

	m_texturePathNotAttached = wsfilepath;
}