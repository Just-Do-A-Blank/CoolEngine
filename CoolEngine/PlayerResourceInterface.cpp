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
		if (EditorUI::CollapsingSection("Player Resources", false))
		{
			EditorUINonSpecificParameters parameters = EditorUINonSpecificParameters();
			parameters.m_tooltipText = "A unique key for the resource";
			EditorUI::InputText("Resource Key", m_entryKey, parameters);

			if (m_lastEntryIsError)
			{
				EditorUI::ShowError("Key already used!");
			}

			if (EditorUI::BasicButton("Add"))
			{
				if (AddResourceToList(m_entryKey))
				{
					LOG("TRUE");
					m_entryKey = "";
					m_lastEntryIsError = false;
					
				}
				else
				{
					LOG("FALSE");
					m_lastEntryIsError = true;
					
				}
			}
		}
	}
#endif

bool PlayerResourceInterface::AddResourceToList(string key)
{
	std::map<string, PlayerResource*> m = *m_playerResources;

	bool found = std::any_of(m.begin(), m.end(),
		[&key](std::pair<const string, PlayerResource*>& entry) {
			return (entry.first == key);
		});

	if (!found)
	{
		PlayerResource* resource = new PlayerResource(key);
		m_playerResources->insert(std::pair<const string, PlayerResource*>(key, resource));
	}

	return !found;
}