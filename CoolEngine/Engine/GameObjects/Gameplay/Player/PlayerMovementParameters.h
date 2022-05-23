#pragma once
#include "Engine\GameObjects\Transform.h"
#include "Engine\GameObjects\Gameplay\GameplayButtons\InputsAsGameplayButtons.h"
#include "Engine\GameObjects\Gameplay\Player\PlayerMovingBody.h"

class PlayerGameObject;
/// <summary>
/// Everything required to move the player around the scene
/// </summary>
struct PlayerMovementParameters
{
    /// <summary>
    /// Relates inputs to gameplay buttons
    /// </summary>
	PlayerGameObject* m_playerReference;

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
    /// The resource to use for dodging
    /// </summary>
    string* m_dodgeResource;

    /// <summary>
    /// How much the resource changes on use
    /// </summary>
    int* m_dodgeResourceChange;

    /// <summary>
    /// How long to doge for in seconds
    /// </summary>
    float* m_timeInSecondsToDodgeFor;

    /// <summary>
    /// How much to multiply the speed by when actually calulcating the speed when rolling
    /// </summary>
    float* m_rollingSpeed;

    /// <summary>
    /// The resource to use for rolling
    /// </summary>
    string* m_rollResource;

    /// <summary>
    /// How much the resource changes on use
    /// </summary>
    int* m_rollResourceChange;

    /// <summary>
    /// How long to roll for in seconds
    /// </summary>
    float* m_timeInSecondsToRollFor;

    /// <summary>
    /// The input button pressed by the user in the last state
    /// </summary>
    EGAMEPLAYBUTTONCLASS m_lastFirstPressedInputButton;

    /// <summary>
    /// The input button pressed by the user in the last state
    /// </summary>
    EGAMEPLAYBUTTONCLASS m_lastSecondPressedInputButton;

    /// <summary>
    /// Indicators on what the player is currently doing
    /// </summary>
    PlayerMovingBody* m_playerMovingBody;
};
