#include "ImageComponent.h"
#include "Engine/ResourceDefines.h"
#include "Engine/GameObjects/Transform.h"
#include "Engine/Managers/GraphicsManager.h"
#include "Engine/EditorUI/EditorUI.h"
#include "Engine/GameUI/GameplayIntegration/ImageUIResourceDisplay.h"
#include "Engine\GameUI\GameplayIntegration\GameplayUIWeaponAttachment.h"

ImageComponent::ImageComponent(string identifier, CoolUUID uuid) : GameUIComponent(identifier, uuid)
{
	m_uiComponentType |= UIComponentType::IMAGE;

	m_resourceAttachement = new ImageUIResourceDisplay(this);
    //m_weaponAttachment = new GameplayUIWeaponAttachment();

    //test = 1;
}

ImageComponent::ImageComponent(nlohmann::json& data, CoolUUID uuid) : GameUIComponent(data, uuid)
{
	m_uiComponentType |= UIComponentType::IMAGE;

	if (GameUIComponent::IsPrefab())
	{
		m_resourceAttachement = new ImageUIResourceDisplay(GameUIComponent::GetPrefabDataLoadedAtCreation(), this);
        //m_weaponAttachment = new GameplayUIWeaponAttachment(GameUIComponent::GetPrefabDataLoadedAtCreation());

		LoadAllLocalData(GameUIComponent::GetPrefabDataLoadedAtCreation());
		
	}
	else
	{
		m_resourceAttachement = new ImageUIResourceDisplay(data, this);
        //m_weaponAttachment = new GameplayUIWeaponAttachment(data);

		LoadAllLocalData(data);
	}
    //test = 1;
}

ImageComponent::ImageComponent(ImageComponent const& other) : GameUIComponent(other)
{
	ImageUIResourceDisplay* imageUI = dynamic_cast<ImageUIResourceDisplay*>(other.m_resourceAttachement);

	if (imageUI != nullptr)
	{
		m_resourceAttachement = new ImageUIResourceDisplay(*imageUI, this);
	}
	
    //m_weaponAttachment = new GameplayUIWeaponAttachment(*other.m_weaponAttachment);
    //test = 1;
}

ImageComponent::~ImageComponent()
{
    delete m_resourceAttachement;
    //delete m_weaponAttachment;
}

#if EDITOR
void ImageComponent::CreateEngineUI()
{
	GameUIComponent::CreateEngineUI();

	if (EditorUI::CollapsingSection("Image Options", true))
	{
		EditorUI::Texture("Texture", m_texFilepath, m_ptexture);
	}

    ImGui::PushID("GameplayUIResourceAttachment");
    if (EditorUI::CollapsingSection("Resource Attachement", true))
    {
        m_resourceAttachement->CreateEngineUI();
    }
    ImGui::PopID();

    ImGui::PushID("GameplayUIWeaponAttachment");
    if (EditorUI::CollapsingSection("Weapon Attachement", true))
    {
       //m_weaponAttachment->CreateEngineUI();
    }
    ImGui::PopID();
}
#endif

void ImageComponent::Start()
{
    GameUIComponent::Start();
    m_resourceAttachement->Start();
    //m_weaponAttachment->Start();
}

void ImageComponent::Update()
{
    GameUIComponent::Update();
    m_resourceAttachement->Update();
    //m_weaponAttachment->Update();
}

void ImageComponent::Serialize(nlohmann::json& jsonData)
{
	GameUIComponent::Serialize(jsonData);
	m_resourceAttachement->SaveFromTopLevel(jsonData);
    //m_weaponAttachment->SaveFromTopLevel(jsonData);
}

/// <summary>
/// Saves all local data within this class
/// </summary>
/// <param name="jsonData">Data to be added to</param>
void ImageComponent::SaveAllLocalData(nlohmann::json& jsonData)
{
	m_resourceAttachement->SaveFromTopLevel(jsonData);
    //m_weaponAttachment->SaveFromTopLevel(jsonData);
}

/// <summary>
/// Loads all local data within this class
/// </summary>
/// <param name="jsonData">Data to be loaded</param>
void ImageComponent::LoadAllLocalData(const nlohmann::json& jsonData)
{
	m_resourceAttachement->LoadFromTopLevel(jsonData);
    //m_weaponAttachment->LoadFromTopLevel(jsonData);
}

/// <summary>
/// Load all prefab data - this should be overriden and passed up the chain to save the whole prefab.
/// </summary>
/// <param name="jsonData">Data to use</param>
void ImageComponent::LoadAllPrefabData(const nlohmann::json& jsonData)
{
	GameUIComponent::LoadAllPrefabData(jsonData);
	m_resourceAttachement->LoadFromTopLevel(jsonData);
    //m_weaponAttachment->LoadFromTopLevel(jsonData);
}

/// <summary>
/// Save all prefab data - this should be overriden and passed up the chain to save the whole prefab.
/// </summary>
/// <param name="jsonData">Data to add to</param>
void ImageComponent::SaveAllPrefabData(nlohmann::json& jsonData)
{
    GameUIComponent::SaveAllPrefabData(jsonData);
    m_resourceAttachement->SaveFromTopLevel(jsonData);
    //m_weaponAttachment->SaveFromTopLevel(jsonData);
}