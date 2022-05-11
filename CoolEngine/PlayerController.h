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

    /// <summary>
    /// Handles events from the Observations
    /// </summary>
    virtual void Handle(Event* e) override;

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
    /// Relates inputs to gameplay buttons
    /// </summary>
    Transform* m_transform;

    float m_moveSpeed = 100;

    /// <summary>
    /// Handles any keypresses when they are pressed (frame whilst pressed)
    /// </summary>
    void KeyPressed(KeyPressedEvent* e);

    /// <summary>
    /// Handles any keypresses when they are released (first frame).
    /// </summary>
    void KeyReleased(KeyReleasedEvent* e);

    /// <summary>
    /// Handles any mouse button presses when pressed (frame whilst pressed)
    /// </summary>
    void MouseButtonPressed(MouseButtonPressedEvent* e);

    /// <summary>
    /// Handles any mouse button when they are released (first frame).
    /// </summary>
    void MouseButtonReleased(MouseButtonReleasedEvent* e);

    /// <summary>
    /// Handles the mouse moving across the window
    /// </summary>
    void MouseMoved(MouseMovedEvent* e);
};

