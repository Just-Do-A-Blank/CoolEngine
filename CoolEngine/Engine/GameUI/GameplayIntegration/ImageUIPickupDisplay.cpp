#include "ImageUIPickupDisplay.h"
#include "Engine/GameUI/ImageComponent.h"
#include "Engine/Managers/GraphicsManager.h"
#include "Engine\EditorUI\EditorUI.h"

ImageUIPickupDisplay::ImageUIPickupDisplay(ImageComponent* imageComponent) : GameplayUIPickupAttachement()
{
	m_imageComponent = imageComponent;
    m_texturePathAttached = std::wstring();
    m_texturePathNotAttached = std::wstring();
}

ImageUIPickupDisplay::ImageUIPickupDisplay(nlohmann::json& data, ImageComponent* imageComponent) : GameplayUIPickupAttachement(data)
{
	m_imageComponent = imageComponent;
    LoadLocalData(data);
}

ImageUIPickupDisplay::ImageUIPickupDisplay(ImageUIPickupDisplay const& other, ImageComponent* imageComponent) : GameplayUIPickupAttachement(other)
{
	m_imageComponent = imageComponent;

    m_texturePathAttached = other.m_texturePathAttached;
    SetAttachedTexture(m_texturePathAttached);

    m_texturePathNotAttached = other.m_texturePathNotAttached;
    SetAttachedTexture(m_texturePathNotAttached);
}

ImageUIPickupDisplay::~ImageUIPickupDisplay()
{

}

#if EDITOR
	void ImageUIPickupDisplay::CreateEngineUI()
	{
		GameplayUIPickupAttachement::CreateEngineUI();

        if (EditorUI::CollapsingSection("Pickup Image Options", true))
        {
            EditorUI::FullTitle("Override the pickup image in these situations.\nBlank will not override the pickup image");

            float colWidth = 150;
            ImVec2 imageSize = ImVec2(150, 150);
            EditorUI::Texture("Slot is full", m_texturePathAttached, m_ptextureWeaponAttached, colWidth, imageSize);

            if (EditorUI::BasicButton("Reset slot is full"))
            {
                m_texturePathAttached = std::wstring();
                SetAttachedTexture(m_texturePathAttached);
            }

            EditorUI::Texture("Slot is empty", m_texturePathNotAttached, m_ptextureWeaponNotAttached, colWidth, imageSize);

            if (EditorUI::BasicButton("Reset slot is empty"))
            {
                m_texturePathNotAttached = std::wstring();
                SetNotAttachedTexture(m_texturePathNotAttached);
            }
        }
	}
#endif

void ImageUIPickupDisplay::Serialize(nlohmann::json& data)
{
	GameplayUIPickupAttachement::Serialize(data);
    SaveLocalData(data);
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
/// <param name="pickupGameObject">The pickup to display</param>
void ImageUIPickupDisplay::Update(PickupGameObject* pickupGameObject)
{

}

void ImageUIPickupDisplay::LoadLocalData(const nlohmann::json& jsonData)
{
    if (jsonData.contains("ImageUIPickupDisplay_PathAttached"))
    {
        std::string tempPath = jsonData["ImageUIPickupDisplay_PathAttached"];
        m_texturePathAttached = std::wstring(tempPath.begin(), tempPath.end());

        SetAttachedTexture(m_texturePathAttached);
    }

    if (jsonData.contains("ImageUIPickupDisplay_PathNotAttached"))
    {
        std::string tempPath = jsonData["ImageUIPickupDisplay_PathNotAttached"];
        m_texturePathNotAttached = std::wstring(tempPath.begin(), tempPath.end());

        SetNotAttachedTexture(m_texturePathNotAttached);
    }
}

void ImageUIPickupDisplay::SaveLocalData(nlohmann::json& jsonData)
{
    std::string tempPath = std::string(m_texturePathAttached.begin(), m_texturePathAttached.end());
    jsonData["ImageUIPickupDisplay_PathAttached"] = tempPath;

    tempPath = std::string(m_texturePathNotAttached.begin(), m_texturePathNotAttached.end());
    jsonData["ImageUIPickupDisplay_PathNotAttached"] = tempPath;
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