#include "ImageUIResourceDisplay.h"
#include "Engine/EditorUI/EditorUI.h"

ImageUIResourceDisplay::ImageUIResourceDisplay(ImageComponent* component) : GameplayUIResourceAttachment()
{
	m_texturesForEachResourceChange = list<TextureToResource*>();
}

ImageUIResourceDisplay::ImageUIResourceDisplay(nlohmann::json& data, ImageComponent* component) : GameplayUIResourceAttachment(data)
{
	m_texturesForEachResourceChange = list<TextureToResource*>();
}

ImageUIResourceDisplay::ImageUIResourceDisplay(ImageUIResourceDisplay const& other, ImageComponent* component) : GameplayUIResourceAttachment(other)
{
	m_texturesForEachResourceChange = list<TextureToResource*>();
}

ImageUIResourceDisplay::~ImageUIResourceDisplay()
{
	for (TextureToResource* const& i : m_texturesForEachResourceChange)
	{
		delete i;
	}
}


#if EDITOR
	void ImageUIResourceDisplay::CreateEngineUI()
	{
		GameplayUIResourceAttachment::CreateEngineUI();

		if (EditorUI::CollapsingSection("Image Link", true))
		{
			EditorButtonCallback callback = EditorUI::BasicDuelButtons("Add to end", "Add to start");
			if (callback.m_leftButton)
			{
				m_texturesForEachResourceChange.push_back(new TextureToResource());
			}
			else if (callback.m_rightButton)
			{
				m_texturesForEachResourceChange.push_front(new TextureToResource());
			}

			int index = 0;
			for (TextureToResource* const& i : m_texturesForEachResourceChange)
			{
				ImGui::PushID(("TextureToResource_" + to_string(index)).c_str());
				i->CreateEngineUI(index++);
				ImGui::PopID();
			}
		}
	}
#endif

void ImageUIResourceDisplay::Serialize(nlohmann::json& data)
{
	GameplayUIResourceAttachment::Serialize(data);
}

/// <summary>
/// Called after construction, before first Update.
/// </summary>
void ImageUIResourceDisplay::Start()
{
	GameplayUIResourceAttachment::Start();
}

/// <summary>
/// Runs during gameplay with the resource value
/// </summary>
/// <param name="resourceValue">The resource value if set</param>
void ImageUIResourceDisplay::Update(int resourceValue)
{

}

void ImageUIResourceDisplay::LoadAllPrefabData(const nlohmann::json& jsonData)
{
	GameplayUIResourceAttachment::LoadAllPrefabData(jsonData);
	LoadLocalData(jsonData);
}

void ImageUIResourceDisplay::SaveAllPrefabData(nlohmann::json& jsonData)
{
	SaveLocalData(jsonData);
	GameplayUIResourceAttachment::SaveAllPrefabData(jsonData);
}

void ImageUIResourceDisplay::LoadLocalData(const nlohmann::json& jsonData)
{
	for (TextureToResource* const& i : m_texturesForEachResourceChange)
	{
		delete i;
	}
	m_texturesForEachResourceChange = list<TextureToResource*>();

	int index = 0;
	string prefix = "TextureToResource_";
	string current = prefix + to_string(index) + "_TexturePath";
	while (jsonData.contains(current))
	{
		m_texturesForEachResourceChange.push_back(new TextureToResource(jsonData, index));
		++index;
		current = prefix + to_string(index) + "_TexturePath";
	}
}

void ImageUIResourceDisplay::SaveLocalData(nlohmann::json& jsonData)
{
	int index = 0;
	for (TextureToResource* const& i : m_texturesForEachResourceChange)
	{
		i->SaveAllPrefabData(jsonData, index++);
	}
}