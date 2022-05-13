#pragma once
#include <Engine\GameObjects\Gameplay\GameplayButtons\EGameplayButtonClass.h>

/// <summary>
/// Stores information about a single gameplay button relating to the inputs required to hit that button
/// </summary>
struct GameplayButton
{
public:
    /// <summary>
    /// Gameplay buttons which are buttons in terms of gameplay uses
    /// </summary>
    EGAMEPLAYBUTTONCLASS m_gameplayButton;

    /// <summary>
    /// Key codes which relate to the gameplay buttons
    /// </summary>
    list<int> m_keyCodes;

    /// <summary>
    /// Mouse button codes relating to the gameplay buttons
    /// </summary>
    list<int> m_mouseCode;
};