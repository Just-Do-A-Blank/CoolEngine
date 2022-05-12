#include "PlayerController.h"
#include "Engine/Managers/GameManager.h"
#include <Engine\Managers\Events\EventManager.h>
#include "Engine/GameObjects/Gameplay/Player/PlayerWalkingState.h"

PlayerController::PlayerController(InputsAsGameplayButtons* gameplayButtons, Transform* transformOfTheGameObject)
{
    m_gameplayButtons = gameplayButtons;
    m_transform = transformOfTheGameObject;

    EventManager::Instance()->AddClient(EventType::KeyPressed, this);
    EventManager::Instance()->AddClient(EventType::KeyReleased, this);
    EventManager::Instance()->AddClient(EventType::MouseButtonPressed, this);
    EventManager::Instance()->AddClient(EventType::MouseButtonReleased, this);
    EventManager::Instance()->AddClient(EventType::MouseMoved, this);
}

PlayerController::~PlayerController()
{
    EventManager::Instance()->RemoveClientEvent(EventType::KeyPressed, this);
    EventManager::Instance()->RemoveClientEvent(EventType::KeyReleased, this);
    EventManager::Instance()->RemoveClientEvent(EventType::MouseButtonPressed, this);
    EventManager::Instance()->RemoveClientEvent(EventType::MouseButtonReleased, this);
    EventManager::Instance()->RemoveClientEvent(EventType::MouseMoved, this);
}

/// <summary>
/// Handles events from the Observations
/// </summary>
void PlayerController::Handle(Event* e)
{
    switch (e->GetEventID())
    {
    case EventType::KeyPressed:
        //KeyPressed((KeyPressedEvent*)e);
        break;
    case EventType::KeyReleased:
        //KeyReleased((KeyReleasedEvent*)e);
        break;
    }
}

/// <summary>
/// Updates the controller
/// </summary>
void PlayerController::Update()
{
    if (m_currentState == nullptr)
    {
        m_currentState = SwitchMovementState(EPLAYERMOVEMENTSTATE::Walking);
    }

    float delta = GameManager::GetInstance()->GetTimer()->DeltaTime();
    if (!m_currentState->Update(delta))
    {
        EPLAYERMOVEMENTSTATE nextState = m_currentState->NextState();
        delete m_currentState;
        m_currentState = SwitchMovementState(nextState);
    }

    //MovePlayer();
}

PlayerMovementState* PlayerController::SwitchMovementState(EPLAYERMOVEMENTSTATE newState)
{
    switch (newState)
    {
    case EPLAYERMOVEMENTSTATE::Walking:
        return new PlayerWalkingState(m_transform, m_gameplayButtons);
    }

    return nullptr;
}

ECHARACTERDIRECTIONCLASS PlayerController::GetCharacterDirection(float x, float y)
{
    ECHARACTERDIRECTIONCLASS returnDirection = ECHARACTERDIRECTIONCLASS::Down;
    if (x < 0)
    {
        returnDirection = ECHARACTERDIRECTIONCLASS::Left;
    }
    else if (x > 0)
    {
        returnDirection = ECHARACTERDIRECTIONCLASS::Right;
    }
    else if (y > 0)
    {
        returnDirection = ECHARACTERDIRECTIONCLASS::Down;
    }
    else if (y < 0)
    {
        returnDirection = ECHARACTERDIRECTIONCLASS::Up;
    }

    return returnDirection;
}