#include "Engine/GameObjects/Gameplay/Player/PlayerResourceInterface.h"
#include "Engine/EditorUI/EditorUI.h"
#include <algorithm>

PlayerResourceInterface::PlayerResourceInterface(map<string, PlayerResource*>* playerResources)
{
	m_playerResources = playerResources;
	m_lastEntryIsError = false;
    m_errorMessage = "";
    m_modificationResult = ERESOURCEMODIFICATIONRESULT::NoListModification;
}

#if EDITOR
	/// <summary>
	/// Shows engine UI
	/// </summary>
	void PlayerResourceInterface::CreateEngineUI()
	{
        m_modificationResult = ERESOURCEMODIFICATIONRESULT::NoListModification;
		if (EditorUI::CollapsingSection("Player Resources", false))
		{
            CreateNewResourceInterface();

            DisplayResourcesForEdit();
		}
	}
#endif

bool PlayerResourceInterface::AddResourceToList(string key)
{
	bool found = std::any_of(m_playerResources->begin(), m_playerResources->end(),
		[&key](std::pair<const string, PlayerResource*>& entry)
		{
			return (entry.first == key);
		});

	if (!found)
	{
		PlayerResource* resource = new PlayerResource(key);
		m_playerResources->insert(std::pair<const string, PlayerResource*>(key, resource));
	}

	return !found;
}

void PlayerResourceInterface::RemoveResourceFromList(string key)
{
	m_playerResources->erase(key);
}

#if EDITOR
    void PlayerResourceInterface::CreateNewResourceInterface()
    {
        EditorUINonSpecificParameters parameters = EditorUINonSpecificParameters();
        parameters.m_tooltipText = "A unique key for the resource";
        EditorUI::InputText("Resource Key", m_entryKey, parameters);

        m_lastEntryIsError = EditorUI::ErrorPopupBox(m_popupKeyResourceKeyFound, m_errorMessage);

        if (EditorUI::BasicButton("Add"))
        {
            TrimSpacesEitherSideOfText(m_entryKey);
            TextToLower(m_entryKey);

            if (m_entryKey == "")
            {
                m_lastEntryIsError = true;
                m_errorMessage = "Please enter a key!";
                EditorUI::ShowError(m_popupKeyResourceKeyFound);
            }
            else if (AddResourceToList(m_entryKey))
            {
                m_modificationResult = ERESOURCEMODIFICATIONRESULT::Added;
                m_entryKey = "";
            }
            else
            {
                m_lastEntryIsError = true;
                m_errorMessage = "Key needs to be unique!";
                EditorUI::ShowError(m_popupKeyResourceKeyFound);
            }
        }
    }

    /// <summary>
    /// Displays all resources for editing / deleting
    /// </summary>
    void PlayerResourceInterface::DisplayResourcesForEdit()
    {
        if (m_deleteOnNextLoop != "")
        {
            m_modificationResult = ERESOURCEMODIFICATIONRESULT::Removed;
            RemoveResourceFromList(m_deleteOnNextLoop);
            m_deleteOnNextLoop = "";
        }

        EditorUIIntParameters intParameters = EditorUIIntParameters();
        for (
            std::map<string, PlayerResource*>::iterator i = m_playerResources->begin();
            i != m_playerResources->end(); i++)
        {
            ImGui::PushID(i->first.c_str());

            EditorUI::FullTitle(i->first);

            intParameters.m_tooltipText = "The lowest value this resource can be";
            EditorUI::DragInt("Min Value", i->second->m_minValue, intParameters);

            intParameters.m_tooltipText = "The highest value this resource can be";
            EditorUI::DragInt("Max Value", i->second->m_maxValue, intParameters);

            intParameters.m_tooltipText = "On a new game the value of the resource";
            EditorUI::DragInt("Default Value", i->second->m_defaultValue, intParameters);

            EditorUINonSpecificParameters nonSpecificParameters = EditorUINonSpecificParameters();
            nonSpecificParameters.m_tooltipText = "If ticked then the damage from weapons will be taken by this.";
            EditorUI::Checkbox("Is Weapon Damage", i->second->m_attachToWeaponDamage, nonSpecificParameters);

            nonSpecificParameters.m_tooltipText = "If ticked then this will kill when active value equals min value";
            EditorUI::Checkbox("Will Kill", i->second->m_resourceKillOnDrain, nonSpecificParameters);

            if (EditorUI::BasicButton("Delete"))
            {
                m_deleteOnNextLoop = i->first;
            }

            ImGui::PopID();
        }
    }

    /// <summary>
    /// Gets the result of any modification to resources from the last UI loop
    /// </summary>
    /// <returns>The modification result</returns>
    ERESOURCEMODIFICATIONRESULT PlayerResourceInterface::GetLastUpdateResult()
    {
        return m_modificationResult;
    }
#endif

/// <summary>
/// Trims any space characters on either side of the string
/// </summary>
/// <param name="text">Text to trim</param>
void PlayerResourceInterface::TrimSpacesEitherSideOfText(std::string& s)
{
    s.erase(0, s.find_first_not_of(' '));
    s.erase(s.find_last_not_of(' ') + 1);
}

/// <summary>
/// Converts string to lowercase
/// </summary>
/// <param name="text">Text to alter</param>
void PlayerResourceInterface::TextToLower(string& text)
{
    for (int i = 0; i < text.length(); ++i)
    {
        text[i] = tolower(text[i]);
    }
}