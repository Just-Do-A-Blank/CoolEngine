#pragma once
#include "Engine/GameObjects/Gameplay/Player/PlayerMovementState.h"
#include <Engine/GameObjects/Transform.h>
#include "Engine/Managers/Events/EventObserver.h"
#include "Engine/Managers/Events/MouseEvents.h"
#include "Engine/Managers/Events/KeyEvents.h"
#include <Engine/GameObjects/Gameplay/GameplayButtons/InputsAsGameplayButtons.h>

class PlayerWalkingState : public PlayerMovementState, public Observer
{
public:
	PlayerWalkingState(Transform* transform, InputsAsGameplayButtons* gameplayButtons);
    ~PlayerWalkingState();

    /// <summary>
    /// Handles events from the Observations
    /// </summary>
    virtual void Handle(Event* e) override;

	virtual bool Update(float timeDelta) override;

	virtual EPLAYERMOVEMENTSTATE NextState() override { LOG("YH"); return EPLAYERMOVEMENTSTATE::Walking; }

private:
	/// <summary>
	/// Relates inputs to gameplay buttons
	/// </summary>
	Transform* m_transform;

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
    /// The way the player is currently moving
    /// </summary>
    EPLAYERMOVEMENTSTATE m_movementState;





    /// <summary>
    /// The force currently applied to the player
    /// </summary>
    XMFLOAT3 m_forceApplied;

    float m_moveSpeedMax = 200;
    float m_moveSpeedPerFrame = 500;
    float m_dragSpeedPerFrame = 250;
    float m_moveSpeed = 0;

    //ECHARACTERDIRECTIONCLASS m_vDirection;
    //ECHARACTERDIRECTIONCLASS m_hDirection;

    float m_dragSpeed = 0.8f;

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
    /// <param name="name">Direction to apply force</param>
    void ApplyForce(float timeDelta, XMFLOAT3 direction);

    /// <summary>
    /// Moves the player based on forces applied
    /// </summary>
    void MoveByForce(float timeDelta);


};