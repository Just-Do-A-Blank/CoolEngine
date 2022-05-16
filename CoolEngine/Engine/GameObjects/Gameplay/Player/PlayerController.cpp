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
    m_dodgeSpeed = 1.6f;
    m_moveSpeedPerFrame = 500;
    m_dragSpeedPerFrame = 250;
    m_timeInSecondsToDodgeFor = 0.4f;
    m_forceApplied = new XMFLOAT3(0, 0, 0);
    m_moveSpeed = 0;

    m_movementParameters.m_transform = transformOfTheGameObject;
    m_movementParameters.m_gameplayButtons = gameplayButtons;
    m_movementParameters.m_maxSpeed = &m_moveSpeedMax;
    m_movementParameters.m_walkingSpeed = &m_speedMultiplierWalking;
    m_movementParameters.m_moveSpeedPerFrame = &m_moveSpeedPerFrame;
    m_movementParameters.m_dragSpeedPerFrame = &m_dragSpeedPerFrame;
    m_movementParameters.m_dodgeSpeed = &m_dodgeSpeed;
    m_movementParameters.m_timeInSecondsToDodgeFor = &m_timeInSecondsToDodgeFor;
    m_movementParameters.m_lastFirstPressedInputButton = EGAMEPLAYBUTTONCLASS::Nothing;
    m_movementParameters.m_lastSecondPressedInputButton = EGAMEPLAYBUTTONCLASS::Nothing;
    m_movementParameters.m_forceApplied = m_forceApplied;
    m_movementParameters.m_moveSpeed = &m_moveSpeed;

    EventManager::Instance()->AddClient(EventType::KeyPressed, this);
    EventManager::Instance()->AddClient(EventType::KeyReleased, this);
    EventManager::Instance()->AddClient(EventType::MouseButtonPressed, this);
    EventManager::Instance()->AddClient(EventType::MouseButtonReleased, this);
    EventManager::Instance()->AddClient(EventType::MouseMoved, this);
}

PlayerController::~PlayerController()
{
    delete m_forceApplied;
    if (m_currentState == nullptr)
    {
        delete m_currentState;
    }

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
    
    if (EditorUI::CollapsingSection("Player Controller", false))
    {
        auto speedParameters = EditorUIFloatParameters();
        speedParameters.m_columnWidth = 150;
        speedParameters.m_tooltipText = "When walking this is the main multiplier - sensitive to big changes!";
        speedParameters.m_speed = 0.01f;

        EditorUI::DragFloat("Walking Speed", m_speedMultiplierWalking, speedParameters);

        speedParameters.m_tooltipText = "When dodging this is the main multiplier - sensitive to big changes!";

        EditorUI::DragFloat("Dodge Speed", m_dodgeSpeed, speedParameters);

        speedParameters.m_tooltipText = "The time in seconds to dodge for before returning to walking!";

        EditorUI::DragFloat("Dodge Time in Seconds ", m_timeInSecondsToDodgeFor, speedParameters);


        auto numberParameters = EditorUIIntParameters();
        numberParameters.m_columnWidth = 150;
        numberParameters.m_minValue = 0;
        numberParameters.m_maxValue = 1000;

        EditorUI::DragInt("MaxSpeed", m_moveSpeedMax, numberParameters);

        EditorUI::DragInt("Speed Per Frame", m_moveSpeedPerFrame, numberParameters);

        EditorUI::DragInt("Drag Per Frame", m_dragSpeedPerFrame, numberParameters);
    }
}
#endif