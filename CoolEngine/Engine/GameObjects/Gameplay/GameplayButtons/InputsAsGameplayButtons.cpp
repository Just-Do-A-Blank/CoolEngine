#include "InputsAsGameplayButtons.h"

/// <summary>
/// Constructs inputs with gameplay buttons from the start
/// </summary>
/// <param name="buttons">A list of all the gameplay buttons relating to inputs</param>
InputsAsGameplayButtons::InputsAsGameplayButtons(list<GameplayButton> buttons)
{
    for (const auto& button : buttons)
    {
        m_currentButtons.insert(pair<EGAMEPLAYBUTTONCLASS,GameplayButton>
            (button.m_gameplayButton, button));
    }
}

/// <summary>
/// Updates a single gameplay button with the data given.
/// Will add if it does not exist
/// </summary>
/// <param name="name">A gameplay button with the data to update</param>
void InputsAsGameplayButtons::UpdateAGameplayButton(GameplayButton newData)
{
    if (m_currentButtons.find(newData.m_gameplayButton) == m_currentButtons.end())
    {
        m_currentButtons.insert(pair<EGAMEPLAYBUTTONCLASS, GameplayButton>
            (newData.m_gameplayButton, newData));
    }
    else
    {
        m_currentButtons.at(newData.m_gameplayButton) = newData;
    }
}

/// <summary>
/// Returns the gameplay button which relates to the given keycode
/// </summary>
/// <param name="keycode">Keycode from the keyboard to check</param>
/// <return>A gameplay button or the button "Nothing" if the key does not relate to anything</return>
EGAMEPLAYBUTTONCLASS InputsAsGameplayButtons::GetGameplayButtonFromKeyInput(int keycode)
{
    for (const auto& button : m_currentButtons)
    {
        for (const auto& buttonKeycode : button.second.m_keyCodes)
        {
            if (buttonKeycode == keycode)
            {
                return button.first;
            }
        }
    }
}

/// <summary>
/// Returns the gameplay button which relates to the given keycode
/// </summary>
/// <param name="mouseCode">Mouse keycode to check</param>
/// <return>A gameplay button or the button "Nothing" if the key does not relate to anything</return>
EGAMEPLAYBUTTONCLASS InputsAsGameplayButtons::GetGameplayButtonFromMouseInput(int mouseCode)
{
    for (const auto& button : m_currentButtons)
    {
        for (const auto& mouseKeycode : button.second.m_mouseCode)
        {
            if (mouseKeycode == mouseCode)
            {
                return button.first;
            }
        }
    }
}