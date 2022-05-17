#include "PlayerController.h"
#include "Engine/Managers/GameManager.h"
#include <Engine\Managers\Events\EventManager.h>
#include "Engine/GameObjects/Gameplay/Player/PlayerWalkingState.h"
#include <Engine\Includes\IMGUI\imgui.h>
#include <Engine\EditorUI\EditorUI.h>

PlayerController::PlayerController(InputsAsGameplayButtons* gameplayButtons, PlayerGameObject* playerReference)
{
    m_moveSpeedMax = 250;
    m_speedMultiplierWalking = 0.8f;
    m_moveSpeedPerFrame = 500;
    m_dragSpeedPerFrame = 250;

    m_movementParameters.m_playerReference = playerReference;
    m_movementParameters.m_gameplayButtons = gameplayButtons;
    m_movementParameters.m_maxSpeed = &m_moveSpeedMax;
    m_movementParameters.m_walkingSpeed = &m_speedMultiplierWalking;
    m_movementParameters.m_moveSpeedPerFrame = &m_moveSpeedPerFrame;
    m_movementParameters.m_dragSpeedPerFrame = &m_dragSpeedPerFrame;

    EventManager::Instance()->AddClient(EventType::KeyPressed, this);
    EventManager::Instance()->AddClient(EventType::KeyReleased, this);
    EventManager::Instance()->AddClient(EventType::MouseButtonPressed, this);
    EventManager::Instance()->AddClient(EventType::MouseButtonReleased, this);
    EventManager::Instance()->AddClient(EventType::MouseMoved, this);
}

PlayerController::PlayerController(PlayerController const& other, PlayerGameObject* newPlayer)
{
	m_currentState = other.m_currentState;
	m_movementParameters = other.m_movementParameters;
	m_movementParameters.m_playerReference = newPlayer;
	m_moveSpeedMax = other.m_moveSpeedMax;
	m_speedMultiplierWalking = other.m_speedMultiplierWalking;
	m_moveSpeedPerFrame = other.m_moveSpeedPerFrame;
	m_dragSpeedPerFrame = other.m_dragSpeedPerFrame;
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
        m_currentState = new PlayerWalkingState(m_movementParameters);
    }

    float delta = GameManager::GetInstance()->GetTimer()->DeltaTime();
    if (!m_currentState->Update(delta))
    {
        PlayerMovementState* nextState = m_currentState->NextState();
        delete m_currentState;
        m_currentState = nextState;
    }
}

#if EDITOR
/// <summary>
/// Shows engine UI
/// </summary>
void PlayerController::CreateEngineUI()
{
    EditorUI::FullTitle("Player Controller", 150);

    EditorUI::DragFloat("General Speed", m_speedMultiplierWalking, 150.0f, 0.1f, 0, 100);

    EditorUI::DragInt("MaxSpeed", m_moveSpeedMax, 150.0f, 0.1f, 0, 1000);

    EditorUI::DragInt("Speed Per Frame", m_moveSpeedPerFrame, 150.0f, 0.1f, 0, 1000);

    EditorUI::DragInt("Drag Per Frame", m_dragSpeedPerFrame, 150.0f, 0.1f, 0, 1000);
}
#endif