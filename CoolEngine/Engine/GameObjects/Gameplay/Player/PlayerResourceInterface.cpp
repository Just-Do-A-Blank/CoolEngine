#include "Engine/GameObjects/Gameplay/Player/PlayerResourceInterface.h"
#include "Engine/EditorUI/EditorUI.h"
#include <algorithm>

PlayerResourceInterface::PlayerResourceInterface(map<string, PlayerResource*>* playerResources)
{
	m_playerResources = playerResources;
	m_lastEntryIsError = false;
}

#if EDITOR
	/// <summary>
	/// Shows engine UI
	/// </summary>
	void PlayerResourceInterface::CreateEngineUI()
	{
		if (m_deleteOnNextLoop != "")
		{
			RemoveResourceFromList(m_deleteOnNextLoop);
			m_deleteOnNextLoop = "";
		}

		if (EditorUI::CollapsingSection("Player Resources", false))
		{
			EditorUINonSpecificParameters parameters = EditorUINonSpecificParameters();
			parameters.m_tooltipText = "A unique key for the resource";
			EditorUI::InputText("Resource Key", m_entryKey, parameters);

			m_lastEntryIsError = EditorUI::ErrorPopupBox(m_popupKeyResourceKeyFound, "Key needs to be unique!");
			
			if (EditorUI::BasicButton("Add"))
			{
				if (AddResourceToList(m_entryKey))
				{
					m_entryKey = "";
				}
				else
				{
					m_lastEntryIsError = true;
					EditorUI::ShowError(m_popupKeyResourceKeyFound);
					
				}
			}

			for (
				std::map<string, PlayerResource*>::iterator i = m_playerResources->begin();
				i != m_playerResources->end(); i++)
			{
				ImGui::PushID(i->first.c_str());

				EditorUI::FullTitle(i->first);
				EditorUI::DragInt("Min Value", i->second->m_minValue);
				EditorUI::DragInt("Max Value", i->second->m_maxValue);

				if (EditorUI::BasicButton("Delete"))
				{
					m_deleteOnNextLoop = i->first;
				}

				ImGui::PopID();
			}
		}
	}
#endif

bool PlayerResourceInterface::AddResourceToList(string key)
{
	for (int i = 0; i < key.length(); ++i)
	{
		key[i] = tolower(key[i]);
	}

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