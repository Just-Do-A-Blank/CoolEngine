#pragma once
#include <Engine\GameObjects\Gameplay\GameplayButtons\InputsAsGameplayButtons.h>
#include <Engine\GameObjects\Transform.h>
#include "Engine/Managers/Events/EventObserver.h"
#include "Engine/Managers/Events/MouseEvents.h"
#include "Engine/Managers/Events/KeyEvents.h"
#include <Engine\GameObjects\GameObject.h>

/// <summary>
/// Handles movement around the scene for the player
/// </summary>
class PlayerController : public Observer
{
public:
    PlayerController(InputsAsGameplayButtons* gameplayButtons, Transform* transformOfTheGameObject);
    ~PlayerController();

    /// <summary>
    /// Handles events from the Observations
    /// </summary>
    virtual void Handle(Event* e) override;

    /// <summary>
    /// Updates the controller
    /// </summary>
    virtual void Update();

private:

    /// <summary>
    /// Occurs when two objects collide without collision on. Fired on enter.
    /// </summary>
    virtual void ReactToTriggerEnter(GameObject* obj1, GameObject* obj2) {}

    /// <summary>
    /// Occurs when two objects collide without collision on. Fired every frame.
    /// </summary>
    virtual void ReactTonTriggerHold(GameObject* obj1, GameObject* obj2) {}

    /// <summary>
    /// Occurs when two objects collide without collision on. Fired the frame the two stop colliding.
    /// </summary>
    virtual void ReactToTriggerExit(GameObject* obj1, GameObject* obj2) {}

    /// <summary>
    /// Occurs when two objects collide with collision on. Fired on enter.
    /// </summary>
    virtual void ReactToCollisionEnter(GameObject* obj1, GameObject* obj2) {}

    /// <summary>
    /// Occurs when two objects collide with collision on. Fired every frame.
    /// </summary>
    virtual void OnCollisionHold(GameObject* obj1, GameObject* obj2) {}

    /// <summary>
    /// Occurs when two objects collide with collision on. Fired the frame the two stop colliding.
    /// </summary>
    virtual void ReactToCollisionExit(GameObject* obj1, GameObject* obj2) {}

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
    /// Relates inputs to gameplay buttons
    /// </summary>
    Transform* m_transform;

    /// <summary>
    /// The force currently applied to the player
    /// </summary>
    XMFLOAT3 m_forceApplied;

    float m_moveSpeed = 100;

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
    void MovePlayer();

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
    void ApplyForce(XMFLOAT3 direction);

    /// <summary>
    /// Moves the player based on forces applied
    /// </summary>
    void MoveByForce();
};

