#pragma once
#include "Engine/GameObjects/RenderableCollidableGameObject.h"
#include "Engine/Managers/Events/EventObserver.h"
#include "Engine/Managers/Events/CollisionEvents.h"

// Provides collision and trigger events for the object
class TriggerableGameObject : public RenderableCollidableGameObject, public Observer
{
public:
	TriggerableGameObject();
	TriggerableGameObject(string identifier);
    ~TriggerableGameObject();

    virtual void Handle(Event* e) override;

private:
    // Determines if an event is a collsion event type
    bool IsEventCollision(Event* e);

    // Triggers the events which would be inherited based on collision events
    void TriggerCollisionEvents(CollisionEvent*);

protected:
    // Occurs when two objects collide without collision on. Fired on enter.
    virtual void OnTriggerEnter(GameObject* obj1, GameObject* obj2) {  }

    // Occurs when two objects collide without collision on. Fired every frame.
    virtual void OnTriggerHold(GameObject* obj1, GameObject* obj2) {  }

    // Occurs when two objects collide without collision on. Fired the frame the two stop colliding.
    virtual void OnTriggerExit(GameObject* obj1, GameObject* obj2) {  }

    // Occurs when two objects collide with collision on. Fired on enter.
    virtual void OnCollisionEnter(GameObject* obj1, GameObject* obj2) { }

    // Occurs when two objects collide with collision on. Fired every frame.
    virtual void OnCollisionHold(GameObject* obj1, GameObject* obj2) { }

    // Occurs when two objects collide with collision on. Fired the frame the two stop colliding.
    virtual void OnCollisionExit(GameObject* obj1, GameObject* obj2){ }
};

