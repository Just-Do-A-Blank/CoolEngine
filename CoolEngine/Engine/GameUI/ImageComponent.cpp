#include "ImageComponent.h"
#include "Engine/ResourceDefines.h"
#include "Engine/GameObjects/Transform.h"
#include "Engine/Managers/GraphicsManager.h"
#include "Engine/EditorUI/EditorUI.h"
#include "Engine/GameUI/GameplayIntegration/ImageUIResourceDisplay.h"
#include "Engine/GameUI/GameplayIntegration/ImageUIWeaponDisplay.h"
#include "Engine/GameUI/GameplayIntegration/ImageUIPickupDisplay.h"

ImageComponent::ImageComponent(string identifier, CoolUUID uuid) : GameUIComponent(identifier, uuid)
{
	m_uiComponentType |= UIComponentType::IMAGE;

	m_resourceAttachementImage = new ImageUIResourceDisplay(this);
	m_resourceAttachement = m_resourceAttachementImage;

	m_imageUIWeaponDisplay = new ImageUIWeaponDisplay(this);
	m_weaponAttachement = m_imageUIWeaponDisplay;

    m_imageUIPickupDisplay = new ImageUIPickupDisplay(this);
    m_pickupAttachement = m_imageUIPickupDisplay;
}

ImageComponent::ImageComponent(nlohmann::json& data, CoolUUID uuid) : GameUIComponent(data, uuid)
{
	m_uiComponentType |= UIComponentType::IMAGE;

	
	if (GameUIComponent::IsPrefab())
	{
		m_resourceAttachementImage = new ImageUIResourceDisplay(GameUIComponent::GetPrefabDataLoadedAtCreation(), this);
		m_resourceAttachement = m_resourceAttachementImage;

		m_imageUIWeaponDisplay = new ImageUIWeaponDisplay(GameUIComponent::GetPrefabDataLoadedAtCreation(), this);
		m_weaponAttachement = m_imageUIWeaponDisplay;

        m_imageUIPickupDisplay = new ImageUIPickupDisplay(GameUIComponent::GetPrefabDataLoadedAtCreation(), this);
        m_pickupAttachement = m_imageUIPickupDisplay;

		LoadAllLocalData(GameUIComponent::GetPrefabDataLoadedAtCreation());
		
	}
	else
	{
		m_resourceAttachementImage = new ImageUIResourceDisplay(data, this);
		m_resourceAttachement = m_resourceAttachementImage;

		m_imageUIWeaponDisplay = new ImageUIWeaponDisplay(data, this);
		m_weaponAttachement = m_imageUIWeaponDisplay;

        m_imageUIPickupDisplay = new ImageUIPickupDisplay(data, this);
        m_pickupAttachement = m_imageUIPickupDisplay;

		LoadAllLocalData(data);
	}
}

ImageComponent::ImageComponent(ImageComponent const& other) : GameUIComponent(other)
{
	m_resourceAttachementImage = new ImageUIResourceDisplay(*other.m_resourceAttachementImage, this);
	m_resourceAttachement = m_resourceAttachementImage;
	
	m_imageUIWeaponDisplay = new ImageUIWeaponDisplay(*other.m_imageUIWeaponDisplay, this);
	m_weaponAttachement = m_imageUIWeaponDisplay;

    m_imageUIPickupDisplay = new ImageUIPickupDisplay(*other.m_imageUIPickupDisplay, this);
    m_pickupAttachement = m_imageUIPickupDisplay;
}

ImageComponent::~ImageComponent()
{
    delete m_resourceAttachementImage;
	m_resourceAttachementImage = nullptr;
	m_resourceAttachement = nullptr;

	delete m_imageUIWeaponDisplay;
	m_imageUIWeaponDisplay = nullptr;
	m_weaponAttachement = nullptr;

    delete m_imageUIPickupDisplay;
    m_imageUIPickupDisplay = nullptr;
    m_pickupAttachement = nullptr;
}

#if EDITOR
void ImageComponent::CreateEngineUI()
{
	GameUIComponent::CreateEngineUI();

	if (EditorUI::CollapsingSection("Image Options", true))
	{
		EditorUI::Texture("Texture", m_texFilepath, m_ptexture);
	}

	m_resourceAttachement->CreateEngineUI();
	m_weaponAttachement->CreateEngineUI();
    m_pickupAttachement->CreateEngineUI();
}
#endif

void ImageComponent::Start()
{
    GameUIComponent::Start();
    m_resourceAttachement->Start();
	m_weaponAttachement->Start();
    m_pickupAttachement->Start();
}

void ImageComponent::Update()
{
    GameUIComponent::Update();
    m_resourceAttachement->Update();
	m_weaponAttachement->Update();
    m_pickupAttachement->Update();
}

void ImageComponent::Serialize(nlohmann::json& jsonData)
{
	GameUIComponent::Serialize(jsonData);
	m_resourceAttachement->SaveFromTopLevel(jsonData);
	m_weaponAttachement->SaveFromTopLevel(jsonData);
    m_pickupAttachement->SaveFromTopLevel(jsonData);
}

/// <summary>
/// Saves all local data within this class
/// </summary>
/// <param name="jsonData">Data to be added to</param>
void ImageComponent::SaveAllLocalData(nlohmann::json& jsonData)
{
	m_resourceAttachement->SaveFromTopLevel(jsonData);
	m_weaponAttachement->SaveFromTopLevel(jsonData);
    m_pickupAttachement->SaveFromTopLevel(jsonData);
}

/// <summary>
/// Loads all local data within this class
/// </summary>
/// <param name="jsonData">Data to be loaded</param>
void ImageComponent::LoadAllLocalData(const nlohmann::json& jsonData)
{
	m_resourceAttachement->LoadFromTopLevel(jsonData);
	m_weaponAttachement->LoadFromTopLevel(jsonData);
    m_pickupAttachement->LoadFromTopLevel(jsonData);
}

/// <summary>
/// Load all prefab data - this should be overriden and passed up the chain to save the whole prefab.
/// </summary>
/// <param name="jsonData">Data to use</param>
void ImageComponent::LoadAllPrefabData(const nlohmann::json& jsonData)
{
	GameUIComponent::LoadAllPrefabData(jsonData);
	m_resourceAttachement->LoadFromTopLevel(jsonData);
	m_weaponAttachement->LoadFromTopLevel(jsonData);
    m_pickupAttachement->LoadFromTopLevel(jsonData);
}

/// <summary>
/// Save all prefab data - this should be overriden and passed up the chain to save the whole prefab.
/// </summary>
/// <param name="jsonData">Data to add to</param>
void ImageComponent::SaveAllPrefabData(nlohmann::json& jsonData)
{
	GameUIComponent::SaveAllPrefabData(jsonData);
	m_resourceAttachement->SaveFromTopLevel(jsonData);
	m_weaponAttachement->SaveFromTopLevel(jsonData);
    m_pickupAttachement->SaveFromTopLevel(jsonData);
}