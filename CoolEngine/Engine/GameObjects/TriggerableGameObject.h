#pragma once
#include "Engine/GameObjects/RenderableCollidableGameObject.h"
#include "Engine/Managers/Events/EventObserver.h"
#include "Engine/Managers/Events/CollisionEvents.h"

// Provides collision and trigger events for the object
class TriggerableGameObject : public RenderableCollidableGameObject, public Observer
{
public:
	TriggerableGameObject();
	TriggerableGameObject(string identifier, CoolUUID uuid);
    ~TriggerableGameObject();

    /// <summary>
    /// Handles events from the Observations
    /// </summary>
    virtual void Handle(Event* e) override;

private:
    /// <summary>
    /// Determines if an event is a collsion event type
    /// </summary>
    bool IsEventCollision(Event* e);

    /// <summary>
    /// Triggers the events which would be inherited based on collision events
    /// </summary>
    void TriggerCollisionEvents(CollisionEvent*);

protected:

    /// <summary>
    /// Occurs when two objects collide without collision on. Fired on enter.
    /// </summary>
    virtual void OnTriggerEnter(GameObject* obj1, GameObject* obj2) {  }

    /// <summary>
    /// Occurs when two objects collide without collision on. Fired every frame.
    /// </summary>
    virtual void OnTriggerHold(GameObject* obj1, GameObject* obj2) {  }

    /// <summary>
    /// Occurs when two objects collide without collision on. Fired the frame the two stop colliding.
    /// </summary>
    virtual void OnTriggerExit(GameObject* obj1, GameObject* obj2) {  }

    /// <summary>
    /// Occurs when two objects collide with collision on. Fired on enter.
    /// </summary>
    virtual void OnCollisionEnter(GameObject* obj1, GameObject* obj2) { }

    /// <summary>
    /// Occurs when two objects collide with collision on. Fired every frame.
    /// </summary>
    virtual void OnCollisionHold(GameObject* obj1, GameObject* obj2) { }

    /// <summary>
    /// Occurs when two objects collide with collision on. Fired the frame the two stop colliding.
    /// </summary>
    virtual void OnCollisionExit(GameObject* obj1, GameObject* obj2){ }
};

