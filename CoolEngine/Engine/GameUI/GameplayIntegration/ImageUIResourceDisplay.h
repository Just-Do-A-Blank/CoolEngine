#pragma once
#include "Engine/GameUI/GameplayIntegration/GameplayUIResourceAttachment.h"
#include "Engine/GameUI/ImageComponent.h"
#include "Engine/GameUI/GameplayIntegration/TextureToResource.h"

class ImageUIResourceDisplay : public GameplayUIResourceAttachment
{
public:
	ImageUIResourceDisplay(ImageComponent* component);
	ImageUIResourceDisplay(nlohmann::json& data, ImageComponent* component);
	ImageUIResourceDisplay(ImageUIResourceDisplay const& other, ImageComponent* component);
	virtual ~ImageUIResourceDisplay() override;

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
    /// The actual image component on the UI
    /// </summary>
    ImageComponent* m_imageComponent;

    /// <summary>
    /// The value last updated to
    /// </summary>
    int m_lastKnownResourceValue;

    /// <summary>
    /// True means this has updated
    /// </summary>
    bool m_haveEverUpdated;

	/// <summary>
	/// Each texture for a resource change
	/// </summary>
	list<TextureToResource*> m_texturesForEachResourceChange;

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

    /// <summary>
    /// Deletes the texture to resource at a given index
    /// </summary>
    /// <param name="index">The index to delete</param>
    void DeleteResourceTextureAtIndex(int index);

    /// <summary>
    /// Update all textures to inform them the order may have changed
    /// </summary>
    void ResetOrdering();
};

