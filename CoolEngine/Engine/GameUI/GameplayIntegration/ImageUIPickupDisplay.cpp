#include "ImageUIPickupDisplay.h"
#include "Engine/GameUI/ImageComponent.h"
#include "Engine/Managers/GraphicsManager.h"

ImageUIPickupDisplay::ImageUIPickupDisplay(ImageComponent* imageComponent) : GameplayUIPickupAttachement()
{
	m_imageComponent = imageComponent;
}

ImageUIPickupDisplay::ImageUIPickupDisplay(nlohmann::json& data, ImageComponent* imageComponent) : GameplayUIPickupAttachement(data)
{
	m_imageComponent = imageComponent;
}

ImageUIPickupDisplay::ImageUIPickupDisplay(ImageUIPickupDisplay const& other, ImageComponent* imageComponent) : GameplayUIPickupAttachement(other)
{
	m_imageComponent = imageComponent;
}

ImageUIPickupDisplay::~ImageUIPickupDisplay()
{

}

#if EDITOR
	void ImageUIPickupDisplay::CreateEngineUI()
	{
		GameplayUIPickupAttachement::CreateEngineUI();
	}
#endif

void ImageUIPickupDisplay::Serialize(nlohmann::json& data)
{
	GameplayUIPickupAttachement::Serialize(data);
}

/// <summary>
/// Called after construction, before first Update.
/// </summary>
void ImageUIPickupDisplay::Start()
{
	GameplayUIPickupAttachement::Start();
}

void ImageUIPickupDisplay::LoadAllPrefabData(const nlohmann::json& jsonData)
{
	GameplayUIPickupAttachement::LoadAllPrefabData(jsonData);
	LoadLocalData(jsonData);
}

void ImageUIPickupDisplay::SaveAllPrefabData(nlohmann::json& jsonData)
{
	GameplayUIPickupAttachement::SaveAllPrefabData(jsonData);
	SaveLocalData(jsonData);
}

/// <summary>
/// Runs during gameplay with the resource value
/// </summary>
/// <param name="weaponGameObject">The weapon to display</param>
void ImageUIPickupDisplay::Update(PickupGameObject* weaponGameObject)
{

}

void ImageUIPickupDisplay::LoadLocalData(const nlohmann::json& jsonData)
{

}

void ImageUIPickupDisplay::SaveLocalData(nlohmann::json& jsonData)
{

}

/// <summary>
/// Set the texture when a weapon is displayed
/// </summary>
/// <param name="wsfilepath">Texture path - Blank is valid</param>
void ImageUIPickupDisplay::SetAttachedTexture(std::wstring wsfilepath)
{
	m_ptextureWeaponAttached = GraphicsManager::GetInstance()->GetShaderResourceView(wsfilepath);

	m_texturePathAttached = wsfilepath;
}

/// <summary>
/// Set the texture when a weapon is not displayed
/// </summary>
/// <param name="wsfilepath">Texture path - Blank is valid</param>
void ImageUIPickupDisplay::SetNotAttachedTexture(std::wstring wsfilepath)
{
	m_ptextureWeaponNotAttached = GraphicsManager::GetInstance()->GetShaderResourceView(wsfilepath);

	m_texturePathNotAttached = wsfilepath;
}