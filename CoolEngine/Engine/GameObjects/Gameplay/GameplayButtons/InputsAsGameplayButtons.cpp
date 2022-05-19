#include "InputsAsGameplayButtons.h"
#include <algorithm>

/// <summary>
/// Constructs inputs with gameplay buttons from the start
/// </summary>
/// <param name="buttons">A list of all the gameplay buttons relating to inputs</param>
InputsAsGameplayButtons::InputsAsGameplayButtons(list<GameplayButton> buttons)
{
    for (const GameplayButton& button : buttons)
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
EGAMEPLAYBUTTONCLASS InputsAsGameplayButtons::GetGameplayButtonFromKeyInput(const int keycode)
{
    for (const auto& button : m_currentButtons)
    {
        for (
            std::list<EVIRTUALKEYCODE>::const_iterator iterator = button.second.m_keyCodes.begin(),
            end = button.second.m_keyCodes.end();
            iterator != end;
            ++iterator
            )
        {
            if (IsVirtualKeyCodeEqualToInternalKeyCode (keycode ,*iterator))
            {
                return button.first;
            }
        };
    }

    return EGAMEPLAYBUTTONCLASS::Nothing;
}

/// <summary>
/// Returns the gameplay button which relates to the given keycode
/// </summary>
/// <param name="mouseCode">Mouse keycode to check</param>
/// <return>A gameplay button or the button "Nothing" if the key does not relate to anything</return>
EGAMEPLAYBUTTONCLASS InputsAsGameplayButtons::GetGameplayButtonFromMouseInput(const int mouseCode)
{
    for (const auto& button : m_currentButtons)
    {
        for (
            std::list<EVIRTUALKEYCODE>::const_iterator iterator = button.second.m_mouseCode.begin(),
            end = button.second.m_mouseCode.end();
            iterator != end;
            ++iterator
            )
        {
            if (IsVirtualKeyCodeEqualToInternalKeyCode(mouseCode, *iterator))
            {
                return button.first;
            }
        };
    }

    return EGAMEPLAYBUTTONCLASS::Nothing;
}

/// <summary>
/// Detirmines if the given Virtual key code as defined in Microsofts documentation equals the internal Keycode
/// as defined in the enum
/// </summary>
/// <param name="keycode">Microsoft Input, VK_</param>
/// <param name="compare">The virtual Key code internal</param>
/// <returns>true means the code is found and equal</returns>
bool InputsAsGameplayButtons::IsVirtualKeyCodeEqualToInternalKeyCode(int keycode, EVIRTUALKEYCODE compare)
{
    switch (keycode)
    {
        case VK_LBUTTON: return compare == EVIRTUALKEYCODE::KC_LBUTTON;
        case VK_RBUTTON: return compare == EVIRTUALKEYCODE::KC_RBUTTON;
        case VK_CANCEL: return compare == EVIRTUALKEYCODE::KC_CANCEL;
        case VK_MBUTTON: return compare == EVIRTUALKEYCODE::KC_MBUTTON;
        case VK_XBUTTON1: return compare == EVIRTUALKEYCODE::KC_XBUTTON1;
        case VK_XBUTTON2: return compare == EVIRTUALKEYCODE::KC_XBUTTON2;
        case VK_BACK: return compare == EVIRTUALKEYCODE::KC_BACK;
        case VK_TAB: return compare == EVIRTUALKEYCODE::KC_TAB;
        case VK_CLEAR: return compare == EVIRTUALKEYCODE::KC_CLEAR;
        case VK_RETURN: return compare == EVIRTUALKEYCODE::KC_ENTER;
        case VK_SHIFT: return compare == EVIRTUALKEYCODE::KC_SHIFT;
        case VK_CONTROL: return compare == EVIRTUALKEYCODE::KC_CTRL;
        case VK_MENU: return compare == EVIRTUALKEYCODE::KC_ALT;
        case VK_PAUSE: return compare == EVIRTUALKEYCODE::KC_PAUSE;
        case VK_CAPITAL: return compare == EVIRTUALKEYCODE::KC_CAPS;
        case VK_ESCAPE: return compare == EVIRTUALKEYCODE::KC_ESC;
        case VK_SPACE: return compare == EVIRTUALKEYCODE::KC_SPACE;
        case VK_PRIOR: return compare == EVIRTUALKEYCODE::KC_PGUP;
        case VK_NEXT: return compare == EVIRTUALKEYCODE::KC_PGDN;
        case VK_END: return compare == EVIRTUALKEYCODE::KC_END;
        case VK_HOME: return compare == EVIRTUALKEYCODE::KC_HOME;
        case VK_LEFT: return compare == EVIRTUALKEYCODE::KC_LEFT;
        case VK_UP: return compare == EVIRTUALKEYCODE::KC_UP;
        case VK_RIGHT: return compare == EVIRTUALKEYCODE::KC_RIGHT;
        case VK_DOWN: return compare == EVIRTUALKEYCODE::KC_DOWN;
        case VK_SELECT: return compare == EVIRTUALKEYCODE::KC_SELECT;
        case VK_PRINT: return compare == EVIRTUALKEYCODE::KC_PRINT;
        case VK_EXECUTE: return compare == EVIRTUALKEYCODE::KC_EXECUTE;
        case VK_SNAPSHOT: return compare == EVIRTUALKEYCODE::KC_PRINTSCREEN;
        case VK_INSERT: return compare == EVIRTUALKEYCODE::KC_INS;
        case VK_DELETE: return compare == EVIRTUALKEYCODE::KC_DEL;
        case VK_HELP: return compare == EVIRTUALKEYCODE::KC_HELP;
        case '0': return compare == EVIRTUALKEYCODE::KC_0;
        case '1': return compare == EVIRTUALKEYCODE::KC_1;
        case '2': return compare == EVIRTUALKEYCODE::KC_2;
        case '3': return compare == EVIRTUALKEYCODE::KC_3;
        case '4': return compare == EVIRTUALKEYCODE::KC_4;
        case '5': return compare == EVIRTUALKEYCODE::KC_5;
        case '6': return compare == EVIRTUALKEYCODE::KC_6;
        case '7': return compare == EVIRTUALKEYCODE::KC_7;
        case '8': return compare == EVIRTUALKEYCODE::KC_8;
        case '9': return compare == EVIRTUALKEYCODE::KC_9;
        case 'Q': return compare == EVIRTUALKEYCODE::KC_Q;
        case 'W': return compare == EVIRTUALKEYCODE::KC_W;
        case 'E': return compare == EVIRTUALKEYCODE::KC_E;
        case 'R': return compare == EVIRTUALKEYCODE::KC_R;
        case 'T': return compare == EVIRTUALKEYCODE::KC_T;
        case 'Y': return compare == EVIRTUALKEYCODE::KC_Y;
        case 'U': return compare == EVIRTUALKEYCODE::KC_U;
        case 'I': return compare == EVIRTUALKEYCODE::KC_I;
        case 'O': return compare == EVIRTUALKEYCODE::KC_O;
        case 'P': return compare == EVIRTUALKEYCODE::KC_P;
        case 'A': return compare == EVIRTUALKEYCODE::KC_A;
        case 'S': return compare == EVIRTUALKEYCODE::KC_S;
        case 'D': return compare == EVIRTUALKEYCODE::KC_D;
        case 'F': return compare == EVIRTUALKEYCODE::KC_F;
        case 'G': return compare == EVIRTUALKEYCODE::KC_G;
        case 'H': return compare == EVIRTUALKEYCODE::KC_H;
        case 'J': return compare == EVIRTUALKEYCODE::KC_J;
        case 'K': return compare == EVIRTUALKEYCODE::KC_K;
        case 'L': return compare == EVIRTUALKEYCODE::KC_L;
        case 'Z': return compare == EVIRTUALKEYCODE::KC_Z;
        case 'X': return compare == EVIRTUALKEYCODE::KC_X;
        case 'C': return compare == EVIRTUALKEYCODE::KC_C;
        case 'V': return compare == EVIRTUALKEYCODE::KC_V;
        case 'B': return compare == EVIRTUALKEYCODE::KC_B;
        case 'N': return compare == EVIRTUALKEYCODE::KC_N;
        case 'M': return compare == EVIRTUALKEYCODE::KC_M;
    }

    return false;
}