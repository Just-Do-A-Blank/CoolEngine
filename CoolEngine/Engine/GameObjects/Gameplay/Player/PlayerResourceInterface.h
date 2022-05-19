#pragma once
#include "Engine/GameObjects/Gameplay/Player/PlayerResource.h"
#include <map>

class PlayerResourceInterface
{
public:
	PlayerResourceInterface(map<string, PlayerResource*>* playerResources);

#if EDITOR
    /// <summary>
    /// Shows engine UI
    /// </summary>
    virtual void CreateEngineUI();
#endif

private:
    map<string, PlayerResource*>* m_playerResources;

    string m_entryKey;

    bool m_lastEntryIsError;

    bool AddResourceToList(string key);
};

