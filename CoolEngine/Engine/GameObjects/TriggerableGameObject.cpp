#include "TriggerableGameObject.h"
#include <Engine\Managers\Events\EventManager.h>

TriggerableGameObject::TriggerableGameObject() : RenderableCollidableGameObject()
{
    EventManager::Instance()->AddClient(EventType::CollisionEnter, this);
    EventManager::Instance()->AddClient(EventType::CollisionExit, this);
    EventManager::Instance()->AddClient(EventType::CollisionHold, this);
    EventManager::Instance()->AddClient(EventType::TriggerEnter, this);
    EventManager::Instance()->AddClient(EventType::TriggerExit, this);
    EventManager::Instance()->AddClient(EventType::TriggerHold, this);
}

TriggerableGameObject::TriggerableGameObject(string identifier) : RenderableCollidableGameObject(identifier)
{
    EventManager::Instance()->AddClient(EventType::CollisionEnter, this);
    EventManager::Instance()->AddClient(EventType::CollisionExit, this);
    EventManager::Instance()->AddClient(EventType::CollisionHold, this);
    EventManager::Instance()->AddClient(EventType::TriggerEnter, this);
    EventManager::Instance()->AddClient(EventType::TriggerExit, this);
    EventManager::Instance()->AddClient(EventType::TriggerHold, this);
}

TriggerableGameObject::~TriggerableGameObject()
{
    EventManager::Instance()->RemoveClientEvent(EventType::CollisionEnter, this);
    EventManager::Instance()->RemoveClientEvent(EventType::CollisionExit, this);
    EventManager::Instance()->RemoveClientEvent(EventType::CollisionHold, this);
    EventManager::Instance()->RemoveClientEvent(EventType::TriggerEnter, this);
    EventManager::Instance()->RemoveClientEvent(EventType::TriggerExit, this);
    EventManager::Instance()->RemoveClientEvent(EventType::TriggerHold, this);
}

void TriggerableGameObject::Handle(Event* e)
{
    if (!IsEventCollision(e))
    {
        return;
    }

    CollisionEvent* collisionEvent = (CollisionEvent*)e;
    TriggerCollisionEvents(collisionEvent);
}

// Detirmines if an event is a collsion event type
bool TriggerableGameObject::IsEventCollision(Event* e)
{
    bool isCollision = false;

    switch (e->GetEventID())
    {
        case EventType::CollisionEnter:
        case EventType::CollisionExit:
        case EventType::CollisionHold:
        case EventType::TriggerEnter:
        case EventType::TriggerExit:
        case EventType::TriggerHold:
            isCollision = true;
            break;
    }

    return isCollision;
}

// Triggers the events which would be inherited based on collision events
void TriggerableGameObject::TriggerCollisionEvents(CollisionEvent* collisionEvent)
{
    // Ensures only a single event is fired
    /*if (collisionEvent->GetGameObject(0) == this)
    {
        return;
    }

    if (collisionEvent->GetGameObject(0) == collisionEvent->GetGameObject(1))
    {
        return;
    }*/

    switch (collisionEvent->GetEventID())
    {
    case EventType::TriggerEnter:
        OnTriggerEnter(collisionEvent->GetGameObject(0), collisionEvent->GetGameObject(1));
        break;
    case EventType::TriggerExit:
        OnTriggerExit(collisionEvent->GetGameObject(0), collisionEvent->GetGameObject(1));
        break;
    case EventType::TriggerHold:
        OnTriggerHold(collisionEvent->GetGameObject(0), collisionEvent->GetGameObject(1));
        break;
    case EventType::CollisionEnter:
        OnCollisionEnter(collisionEvent->GetGameObject(0), collisionEvent->GetGameObject(1));
        break;
    case EventType::CollisionExit:
        OnCollisionExit(collisionEvent->GetGameObject(0), collisionEvent->GetGameObject(1));
        break;
    case EventType::CollisionHold:
        OnCollisionHold(collisionEvent->GetGameObject(0), collisionEvent->GetGameObject(1));
        break;
    }
}