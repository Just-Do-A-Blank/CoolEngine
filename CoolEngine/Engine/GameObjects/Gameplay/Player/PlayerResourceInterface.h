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

    /// <summary>
    /// The name of the error message popup box
    /// </summary>
    const string m_popupKeyResourceKeyFound = "PlayerResourceInterface_ResourceErrorKey";
#endif

private:
    /// <summary>
    /// Current resources from the player
    /// </summary>
    map<string, PlayerResource*>* m_playerResources;

    /// <summary>
    /// The text in the new key box
    /// </summary>
    string m_entryKey;

    /// <summary>
    /// If not blank this is the key to delete from resources.
    /// Used to delete the key in a safe envrionment outside of loops.
    /// </summary>
    string m_deleteOnNextLoop;

    /// <summary>
    /// True, means the error message box is shown
    /// </summary>
    bool m_lastEntryIsError;

    /// <summary>
    /// Error message to display
    /// </summary>
    string m_errorMessage;

    /// <summary>
    /// Adds a new resource to the list
    /// </summary>
    /// <param name="key">New key to add</param>
    /// <returns>True, means added successfully, False means key was found</returns>
    bool AddResourceToList(string key);

    /// <summary>
    /// Removes key from resources
    /// </summary>
    void RemoveResourceFromList(string key);

    /// <summary>
    /// Trims any space characters on either side of the string
    /// </summary>
    /// <param name="text">Text to trim</param>
    void TrimSpacesEitherSideOfText(string& text);

    /// <summary>
    /// Converts string to lowercase
    /// </summary>
    /// <param name="text">Text to alter</param>
    void TextToLower(string& text);

#if EDITOR
    /// <summary>
    /// Displays interface for the creation / mangement section
    /// </summary>
    void CreateNewResourceInterface();

    /// <summary>
    /// Displays all resources for editing / deleting
    /// </summary>
    void DisplayResourcesForEdit();
#endif
};

