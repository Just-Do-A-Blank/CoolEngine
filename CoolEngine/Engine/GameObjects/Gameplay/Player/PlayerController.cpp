#include "PlayerController.h"
#include "Engine/Managers/GameManager.h"
#include <Engine\Managers\Events\EventManager.h>
#include "Engine/GameObjects/Gameplay/Player/PlayerWalkingState.h"
#include <Engine\Includes\IMGUI\imgui.h>
#include <Engine\EditorUI\EditorUI.h>

PlayerController::PlayerController(InputsAsGameplayButtons* gameplayButtons, Transform* transformOfTheGameObject)
{
    m_moveSpeedMax = 250;
    m_speedMultiplierWalking = 0.8f;
    m_moveSpeedPerFrame = 500;
    m_dragSpeedPerFrame = 250;

    m_movementParameters.m_transform = transformOfTheGameObject;
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
    float colWidth = 150;

    auto titleParameters = EditorUINonSpecificParameters();
    titleParameters.m_columnWidth = 150;
    
    EditorUI::FullTitle("Player Controller", titleParameters);

    auto speedParameters = EditorUIFloatParameters();
    speedParameters.m_columnWidth = 150;
    speedParameters.m_tooltipText = "When walking this is the main multiplier - sensitive to big changes!";
    speedParameters.m_speed = 0.01f;

    EditorUI::DragFloat("Walking Speed", m_speedMultiplierWalking, speedParameters);


    auto numberParameters = EditorUIIntParameters();
    numberParameters.m_columnWidth = 150;
    numberParameters.m_minValue = 0;
    numberParameters.m_maxValue = 1000;

    EditorUI::DragInt("MaxSpeed", m_moveSpeedMax, numberParameters);

    EditorUI::DragInt("Speed Per Frame", m_moveSpeedPerFrame, numberParameters);

    EditorUI::DragInt("Drag Per Frame", m_dragSpeedPerFrame, numberParameters);
}
#endif