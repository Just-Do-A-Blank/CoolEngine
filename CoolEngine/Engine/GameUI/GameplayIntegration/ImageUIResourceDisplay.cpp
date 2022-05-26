#include "ImageUIResourceDisplay.h"
#include "Engine/EditorUI/EditorUI.h"
#include "Engine/GameUI/ImageComponent.h"

ImageUIResourceDisplay::ImageUIResourceDisplay(ImageComponent* component) : GameplayUIResourceAttachment()
{
    m_imageComponent = component;
	m_texturesForEachResourceChange = new list<TextureToResource*>();
    m_lastKnownResourceValue = -1;
    m_haveEverUpdated = false;
}

ImageUIResourceDisplay::ImageUIResourceDisplay(nlohmann::json& data, ImageComponent* component) : GameplayUIResourceAttachment(data)
{
    m_imageComponent = component;
	m_texturesForEachResourceChange = new list<TextureToResource*>();
    LoadLocalData(data);
}

ImageUIResourceDisplay::ImageUIResourceDisplay(ImageUIResourceDisplay const& other, ImageComponent* component) : GameplayUIResourceAttachment(other)
{
    m_imageComponent = component;
	m_texturesForEachResourceChange = new list<TextureToResource*>();
    for (TextureToResource* const& i : *other.m_texturesForEachResourceChange)
    {
        m_texturesForEachResourceChange->push_back(new TextureToResource(*i));
    }

    m_lastKnownResourceValue = -1;
    m_haveEverUpdated = false;

}

ImageUIResourceDisplay::~ImageUIResourceDisplay()
{
	for (TextureToResource* const& i : *m_texturesForEachResourceChange)
	{
		delete i;
	}
    delete m_texturesForEachResourceChange;
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
				m_texturesForEachResourceChange->push_back(new TextureToResource());
                ResetOrdering();
			}
			else if (callback.m_rightButton)
			{
				m_texturesForEachResourceChange->push_front(new TextureToResource());
                ResetOrdering();
			}

            int deletionIndex = -1;
			int index = 0;
			for (TextureToResource* const& i : *m_texturesForEachResourceChange)
			{
                ImGui::PushID(("TextureToResource_" + to_string(index)).c_str());

                if (!i->CreateEngineUI(index))
                {
                    deletionIndex = index;
                }

                index++;
				ImGui::PopID();
			}

            DeleteResourceTextureAtIndex(deletionIndex);
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
    if (m_texturesForEachResourceChange->size() == 0)
    {
        return;
    }

    if (!ShouldUpdate(resourceValue))
    {
        return;
    }

    for (TextureToResource* const& i : *m_texturesForEachResourceChange)
    {
        if (i->WouldSetGivenValue(resourceValue))
        {
            std::wstring proposedTexture = i->GetTextureOut();
            if (proposedTexture != m_imageComponent->GetTextureFilePath())
            {
                m_imageComponent->SetTexture(i->GetTextureOut());
            }
        }
    }

    UpdateComplete(resourceValue);
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
	for (TextureToResource* const& i : *m_texturesForEachResourceChange)
	{
		delete i;
	}
    delete m_texturesForEachResourceChange;
	m_texturesForEachResourceChange = new list<TextureToResource*>();

	int index = 0;
	string prefix = "TextureToResource_";
	string current = prefix + to_string(index) + "_TexturePath";
	while (jsonData.contains(current))
	{
		m_texturesForEachResourceChange->push_back(new TextureToResource(jsonData, index));
		++index;
		current = prefix + to_string(index) + "_TexturePath";
	}

    m_lastKnownResourceValue = -1;
    m_haveEverUpdated = false;
}

void ImageUIResourceDisplay::SaveLocalData(nlohmann::json& jsonData)
{
	int index = 0;
	for (TextureToResource* const& i : *m_texturesForEachResourceChange)
	{
		i->SaveAllPrefabData(jsonData, index++);
	}
}

/// <summary>
/// Detirmines if the component should update
/// </summary>
/// <param name="resourceValue">The resource value if updating</param>
/// <return>True means should update</return>
bool ImageUIResourceDisplay::ShouldUpdate(int currentResourceValue)
{
    if (m_haveEverUpdated && m_lastKnownResourceValue == currentResourceValue)
    {
        return false;
    }

    return true;
}

/// <summary>
/// Flips and switches to confirm this has updated
/// </summary>
/// <param name="resourceValue">The resource value if updating</param>
void ImageUIResourceDisplay::UpdateComplete(int currentResourceValue)
{
    m_haveEverUpdated = true;
    m_lastKnownResourceValue = currentResourceValue;
}

/// <summary>
/// Deletes the texture to resource at a given index
/// </summary>
/// <param name="index">The index to delete</param>
void ImageUIResourceDisplay::DeleteResourceTextureAtIndex(int index)
{
    if (index <= -1)
    {
        return;
    }

    list<TextureToResource*>::iterator it = m_texturesForEachResourceChange->begin();
    advance(it, index);
    m_texturesForEachResourceChange->erase(it);

    ResetOrdering();
}

/// <summary>
/// Update all textures to inform them the order may have changed
/// </summary>
void ImageUIResourceDisplay::ResetOrdering()
{
    for (TextureToResource* const& i : *m_texturesForEachResourceChange)
    {
        i->ResetOrder();
    }
}