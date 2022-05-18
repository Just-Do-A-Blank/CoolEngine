#pragma once
#include "Engine/GameObjects/Gameplay/Player/PlayerMovementState.h"
#include <Engine/GameObjects/Transform.h>
#include "Engine/Managers/Events/EventObserver.h"
#include "Engine/Managers/Events/MouseEvents.h"
#include "Engine/Managers/Events/KeyEvents.h"
#include "Engine/GameObjects/Gameplay/GameplayButtons/InputsAsGameplayButtons.h"
#include "Engine\GameObjects\Gameplay\Player\PlayerMovementParameters.h"
#include "Engine/GameObjects/Gameplay/Player/EPLAYERMOVEMENTSTATE.h"
#include "Engine\GameObjects\Gameplay\Player\PlayerMovingBody.h"

/// <summary>
/// Allows the player to walk around
/// </summary>
class PlayerWalkingState : public PlayerMovementState, public Observer
{
public:
	PlayerWalkingState(PlayerMovementParameters movement);
    virtual ~PlayerWalkingState();

    /// <summary>
    /// Updates the state and moves the player
    /// </summary>
    /// <param name="timeDelta">Time between frames</param>
    /// <returns>True means state may remain, False means the state is complete</returns>
	virtual bool Update(float timeDelta) override;

    /// <summary>
    /// The next state use if this state no longer needed
    /// </summary>
    /// <returns>The next state to use</returns>
    virtual PlayerMovementState* NextState() override;

    /// <summary>
    /// Handles events from the Observations
    /// </summary>
    virtual void Handle(Event* e) override;

private:

    /// <summary>
    /// Next state to use. Should be walking unless the state is over
    /// </summary>
    EPLAYERMOVEMENTSTATE m_nextState;

    /// <summary>
    /// The movement parameters sent in, used to create a new state
    /// </summary>
    PlayerMovementParameters m_playerMovementParameters;

    /// <summary>
    /// The max speed of the player in this state
    /// </summary>
    int *m_moveSpeedMax;

    /// <summary>
    /// How much to move the player per frame (time delta factors into this)
    /// </summary>
    /// <param name="maxSpeed">The max speed of the player in this state</param>
    /// <returns>How much to move the player per frame</returns>
    int* m_moveSpeedPerFrame;

    /// <summary>
    /// How much to drag the player back when buttons are not pressed or direction is changed
    /// </summary>
    /// <param name="maxSpeed">The max speed of the player in this state</param>
    /// <returns>How much to drag the player back when buttons are not pressed or direction is changed</returns>
    int* m_dragSpeedPerFrame;

    /// <summary>
    /// How much to multiply the speed by when actually calulcating the speed
    /// </summary>
    float *m_speedMultiplier;

    /// <summary>
    /// Indicators on what the player is currently doing
    /// </summary>
    PlayerMovingBody* m_playerMovingBody;

	/// <summary>
	/// Relates inputs to gameplay buttons
	/// </summary>
	PlayerGameObject* m_playerReference;

    /// <summary>
    /// Relates inputs to gameplay buttons
    /// </summary>
    InputsAsGameplayButtons* m_gameplayButtons;

    /// <summary>
    /// First button the player held
    /// </summary>
    EGAMEPLAYBUTTONCLASS m_firstMovementButton;

    /// <summary>
    /// Second button the player held
    /// </summary>
    EGAMEPLAYBUTTONCLASS m_secondMovementButton;

    /// <summary>
    /// Handles any keypresses when they are pressed (frame whilst pressed)
    /// </summary>
    void KeyPressed(KeyPressedEvent* e);

    /// <summary>
    /// Handles any keypresses when they are released (first frame).
    /// </summary>
    void KeyReleased(KeyReleasedEvent* e);

    /// <summary>
    /// Updates the button pressed first/second on button pressed
    /// </summary>
    /// <param name="buttons">The gameplay button pressed</param>
    void UpdateButtonOrderOnButtonPressed(EGAMEPLAYBUTTONCLASS button);

    /// <summary>
    /// Updates the button pressed first/second on button released
    /// </summary>
    /// <param name="button">The gameplay button pressed</param>
    void UpdateButtonOrderOnButtonReleased(EGAMEPLAYBUTTONCLASS button);

    /// <summary>
    /// Handles moving the player
    /// </summary>
    /// <param name="timeDelta">Delta time between frames</param>
    void MovePlayer(float timeDelta);

    /// <summary>
    /// Detirmines if the button is a vertical direction button
    /// </summary>
    /// <param name="button">The gameplay button pressed</param>
    /// <return>True, means the button moves a vertical direction</return>
    bool IsVerticalDirection(EGAMEPLAYBUTTONCLASS button);

    /// <summary>
    /// Detirmines if the button is a horizontal direction button
    /// </summary>
    /// <param name="button">The gameplay button pressed</param>
    /// <return>True, means the button moves a horizontal direction</return>
    bool IsHorizontalDirection(EGAMEPLAYBUTTONCLASS button);

    /// <summary>
    /// Detirmines if the button is on the same axis
    /// </summary>
    /// <param name="current">The button to check</param>
    /// <param name="test">The button to test against</param>
    /// <return>True means the buttons are on the same axis</return>
    bool ButtonIsOnTheSameAxis(EGAMEPLAYBUTTONCLASS current, EGAMEPLAYBUTTONCLASS test);

    /// <summary>
    /// Detirmines if the button is on the same axis but is not the same button
    /// </summary>
    /// <param name="current">The button to check</param>
    /// <param name="test">The button to test against</param>
    /// <return>True means the buttons do not match but the axis match</return>
    bool ButtonIsNotTheSameButIsInTheSameAxis(EGAMEPLAYBUTTONCLASS current, EGAMEPLAYBUTTONCLASS test);

    /// <summary>
    /// Calculates how much the vector should alter based on the gameplay button
    /// </summary>
    /// <param name="button">The button to test against</param>
    /// <return>The direction change to apply to the transform to apply the force of the button</return>
    XMFLOAT3 GetVectorChangeForGameplayButton(EGAMEPLAYBUTTONCLASS button);

    /// <summary>
    /// Applies force in the given direction
    /// </summary>
    /// <param name="timeDelta">Delta time between frames</param>
    /// <param name="direction">Direction to apply force</param>
    void ApplyForce(float timeDelta, XMFLOAT3 direction);

    /// <summary>
    /// Applies force to a single axis
    /// </summary>
    /// <param name="axisValue">The value to update</param>
    /// <param name="direction">The intensity to apply</param>
    void ApplyForceToSingleAxis(float* axisValue, float direction);

    /// <summary>
    /// Moves the player based on forces applied
    /// </summary>
    /// <param name="timeDelta">Delta time between frames</param>
    void MoveByForce(float timeDelta);

    /// <summary>
    /// Move the given vector based on the button pressed
    /// </summary>
    /// <param name="vector">Vector to move</param>
    /// <param name="button">Button pressed</param>
    void MoveVectorByButton(XMFLOAT3* vector, EGAMEPLAYBUTTONCLASS button);

    /// <summary>
    /// Slow the speed value based on the drag amount
    /// </summary>
    /// <param name="originalDirection">The direction the vector was aiming before moving</param>
    /// <param name="newDirection">The direction the vector is moving now</param>
    /// <param name="drag">The drag amount if direction moves</param>
    /// <param name="movementSpeed">The movement speed to adjust</param>
    void SlowSpeedIfDirectionChanged(XMFLOAT3 originalDirection, XMFLOAT3 newDirection, float drag, float& movementSpeed);

    /// <summary>
    /// Restrict speed based on max speed and force
    /// </summary>
    /// <param name="speed">Current speed to restrict</param>
    /// <param name="maxSpeed">The max speed for the instance</param>
    /// <param name="force">Force which is also restricted if speed is 0</param>
    void RestrictSpeedAndForceToResonableBounds(float& speed, float maxSpeed, XMFLOAT3& force);

    /// <summary>
    /// Move the given transform in a direction based on speed and direction
    /// </summary>
    /// <param name="transform">Transform to move</param>
    /// <param name="force">Force to apply</param>
    /// <param name="speed">Speed to use</param>
    /// <param name="distance">Distance to move the player</param>
    void MoveTransformInDirectionByDistance(Transform* transform, XMFLOAT3 force, float speed, float distance);

    /// <summary>
    /// Slow the player based on drag value given
    /// </summary>
    /// <param name="speed">Speed to move the player</param>
    /// <param name="drag">Drag value to apply</param>
    /// <param name="delta">Time delta to use when slowing the player</param>
    void SlowPlayerBasedOnDrag(float& speed, float drag, float delta);

    /// <summary>
    /// Move the given float toward zero by the given amount
    /// </summary>
    /// <param name="value">Value to adjust</param>
    /// <param name="intensity">Intensity to adjust the value by</param>
    void MoveFloatTowardZero(float* value, float intensity);
};