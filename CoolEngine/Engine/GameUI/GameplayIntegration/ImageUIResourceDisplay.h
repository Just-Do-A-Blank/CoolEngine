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
	/// Each texture for a resource change
	/// </summary>
	list<TextureToResource*> m_texturesForEachResourceChange;
};

