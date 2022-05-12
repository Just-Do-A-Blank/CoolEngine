#pragma once
#include <Engine\GameObjects\Gameplay\GameplayButtons\InputsAsGameplayButtons.h>
#include <Engine\GameObjects\Transform.h>
#include "Engine/Managers/Events/EventObserver.h"
#include "Engine/Managers/Events/MouseEvents.h"
#include "Engine/Managers/Events/KeyEvents.h"
#include <Engine\GameObjects\GameObject.h>
#include "Engine/GameObjects/Gameplay/Character/ECharacterDirectionClass.h"
#include <Engine/GameObjects/Gameplay/Player/EPLAYERMOVEMENTSTATE.h>
#include <Engine/GameObjects/Gameplay/Player/PlayerMovementState.h>

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
    Transform* m_transform;

    /// <summary>
    /// Relates inputs to gameplay buttons
    /// </summary>
    InputsAsGameplayButtons* m_gameplayButtons;

    /// <summary>
    /// The state to move the player
    /// </summary>
    PlayerMovementState* m_currentState;

    PlayerMovementState* SwitchMovementState(EPLAYERMOVEMENTSTATE newState);

    ECHARACTERDIRECTIONCLASS GetCharacterDirection(float x, float y);
};

