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
};