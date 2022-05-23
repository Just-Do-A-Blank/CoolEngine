#pragma once
#include "Engine/GameObjects/Gameplay/GameplayButtons/EGameplayButtonClass.h"
#include <Engine\GameObjects\Gameplay\GameplayButtons\GameplayButton.h>
#include <map>

/// <summary>
/// Relates inputs to gameplay buttons
/// </summary>
class InputsAsGameplayButtons
{
public:
    /// <summary>
    /// Constructs inputs with gameplay buttons from the start
    /// </summary>
    /// <param name="buttons">A list of all the gameplay buttons relating to inputs</param>
    InputsAsGameplayButtons(list<GameplayButton> buttons);

    /// <summary>
    /// Updates a single gameplay button with the data given.
    /// Will add if it does not exist
    /// </summary>
    /// <param name="name">A gameplay button with the data to update</param>
    void UpdateAGameplayButton(GameplayButton newData);

    /// <summary>
    /// Returns the gameplay button which relates to the given keycode
    /// </summary>
    /// <param name="keycode">Keycode from the keyboard to check</param>
    /// <return>A gameplay button or the button "Nothing" if the key does not relate to anything</return>
    EGAMEPLAYBUTTONCLASS GetGameplayButtonFromKeyInput(const int keycode);

    /// <summary>
    /// Returns the gameplay button which relates to the given keycode
    /// </summary>
    /// <param name="mouseCode">Mouse keycode to check</param>
    /// <return>A gameplay button or the button "Nothing" if the key does not relate to anything</return>
    EGAMEPLAYBUTTONCLASS GetGameplayButtonFromMouseInput(const int mouseCode);

private:
    /// <summary>
    /// Stores information about a single gameplay button relating to the inputs required to hit that button
    /// </summary>
    map<EGAMEPLAYBUTTONCLASS,GameplayButton> m_currentButtons;

    /// <summary>
    /// Detirmines if the given Virtual key code as defined in Microsofts documentation equals the internal Keycode
    /// as defined in the enum
    /// </summary>
    /// <param name="keycode">Microsoft Input, VK_</param>
    /// <param name="compare">The virtual Key code internal</param>
    /// <returns>true means the code is found and equal</returns>
    bool IsVirtualKeyCodeEqualToInternalKeyCode(int keycode, EVIRTUALKEYCODE compare);
};

