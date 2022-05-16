#pragma once
#include <Engine\GameObjects\Transform.h>
#include <Engine\GameObjects\Gameplay\GameplayButtons\InputsAsGameplayButtons.h>

/// <summary>
/// Everything required to move the player around the scene
/// </summary>
struct PlayerMovementParameters
{
    /// <summary>
    /// Relates inputs to gameplay buttons
    /// </summary>
    Transform* m_transform;
    
    /// <summary>
    /// Relates inputs to gameplay buttons
    /// </summary>
    InputsAsGameplayButtons* m_gameplayButtons;

    /// <summary>
    /// The max speed of the player in this state
    /// </summary>
    int* m_maxSpeed;

    /// <summary>
    /// How much to move per frame
    /// </summary>
    int* m_moveSpeedPerFrame;
    
    /// <summary>
    /// Drag speed per frame
    /// </summary>
    int* m_dragSpeedPerFrame;
    
    /// <summary>
    /// How much to multiply the speed by when actually calulcating the speed when walking
    /// </summary>
    float* m_walkingSpeed;

    /// <summary>
    /// How much to multiply the speed by when actually calulcating the speed when dodging
    /// </summary>
    float* m_dodgeSpeed;

    /// <summary>
    /// How long to doge for in seconds
    /// </summary>
    float* m_timeInSecondsToDodgeFor;

    /// <summary>
    /// The input button pressed by the user in the last state
    /// </summary>
    EGAMEPLAYBUTTONCLASS m_lastFirstPressedInputButton;

    /// <summary>
    /// The input button pressed by the user in the last state
    /// </summary>
    EGAMEPLAYBUTTONCLASS m_lastSecondPressedInputButton;

    /// <summary>
    /// The force currently applied to the player. Shared among states as this is the force on the player as a whole
    /// </summary>
    XMFLOAT3* m_forceApplied;

    /// <summary>
    /// The current speed of the player. Shared among states as this is the force on the player as a whole
    /// </summary>
    float* m_moveSpeed;
};