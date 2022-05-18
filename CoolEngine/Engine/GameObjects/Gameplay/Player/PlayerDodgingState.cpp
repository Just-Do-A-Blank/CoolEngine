#include "PlayerDodgingState.h"
#include "PlayerWalkingState.h"
#include <Engine/Managers/Events/EventManager.h>
#include "Engine/GameObjects/PlayerGameObject.h"

PlayerDodgingState::PlayerDodgingState(PlayerMovementParameters movement)
{
    m_nextState = EPLAYERMOVEMENTSTATE::Dodging;
    m_playerMovementParameters = movement;

	m_playerReference = movement.m_playerReference;
    m_gameplayButtons = movement.m_gameplayButtons;
    m_moveSpeedMax = movement.m_maxSpeed;
    m_speedMultiplier = movement.m_dodgeSpeed;
    m_moveSpeedPerFrame = movement.m_moveSpeedPerFrame;
    m_dragSpeedPerFrame = movement.m_dragSpeedPerFrame;
    m_timeInSecondsToDodgeFor = movement.m_timeInSecondsToDodgeFor;
    m_playerMovingBody = movement.m_playerMovingBody;

    m_firstMovementButton = movement.m_lastFirstPressedInputButton;
    m_secondMovementButton = movement.m_lastSecondPressedInputButton;

    EventManager::Instance()->AddClient(EventType::KeyPressed, this);
    EventManager::Instance()->AddClient(EventType::KeyReleased, this);
    EventManager::Instance()->AddClient(EventType::MouseButtonPressed, this);
    EventManager::Instance()->AddClient(EventType::MouseButtonReleased, this);
    EventManager::Instance()->AddClient(EventType::MouseMoved, this);
}

PlayerDodgingState::~PlayerDodgingState()
{
    EventManager::Instance()->RemoveClientEvent(EventType::KeyPressed, this);
    EventManager::Instance()->RemoveClientEvent(EventType::KeyReleased, this);
    EventManager::Instance()->RemoveClientEvent(EventType::MouseButtonPressed, this);
    EventManager::Instance()->RemoveClientEvent(EventType::MouseButtonReleased, this);
    EventManager::Instance()->RemoveClientEvent(EventType::MouseMoved, this);
}

/// <summary>
/// Updates the state and moves the player
/// </summary>
/// <param name="timeDelta">Time between frames</param>
/// <returns>True means state may remain, False means the state is complete</returns>
bool PlayerDodgingState::Update(float timeDelta)
{
    m_timeElapsed += timeDelta;
    if (m_timeElapsed >= *m_timeInSecondsToDodgeFor)
    {
        m_nextState = EPLAYERMOVEMENTSTATE::Walking;
    }

    bool areCurrentlyRunning = m_nextState == EPLAYERMOVEMENTSTATE::Dodging;
    if (areCurrentlyRunning)
    {
        MovePlayer(timeDelta);
    }

    return areCurrentlyRunning;
}

/// <summary>
/// The next state use if this state no longer needed
/// </summary>
/// <returns>The next state to use</returns>
PlayerMovementState* PlayerDodgingState::NextState()
{
    m_playerMovementParameters.m_lastFirstPressedInputButton = m_firstMovementButton;
    m_playerMovementParameters.m_lastSecondPressedInputButton = m_secondMovementButton;

	return new PlayerWalkingState(m_playerMovementParameters);
}

/// <summary>
/// Handles events from the Observations
/// </summary>
void PlayerDodgingState::Handle(Event* e)
{
    switch (e->GetEventID())
    {
    case EventType::KeyPressed:
        m_nextState = KeyPressed((KeyPressedEvent*)e);
        break;
    case EventType::KeyReleased:
        KeyReleased((KeyReleasedEvent*)e);
        break;
    }
}

/// <summary>
/// Handles any keypresses when they are pressed (frame whilst pressed)
/// </summary>
EPLAYERMOVEMENTSTATE PlayerDodgingState::KeyPressed(KeyPressedEvent* e)
{
    auto returnState = EPLAYERMOVEMENTSTATE::Dodging;
    EGAMEPLAYBUTTONCLASS button = m_gameplayButtons->GetGameplayButtonFromKeyInput(e->GetKeyCode());
    UpdateButtonOrderOnButtonPressed(button);

    return returnState;
}

/// <summary>
/// Handles any keypresses when they are released (first frame).
/// </summary>
void PlayerDodgingState::KeyReleased(KeyReleasedEvent* e)
{
    EGAMEPLAYBUTTONCLASS button = m_gameplayButtons->GetGameplayButtonFromKeyInput(e->GetKeyCode());
    if (button != EGAMEPLAYBUTTONCLASS::Nothing)
    {
        UpdateButtonOrderOnButtonReleased(button);
    }
}

/// <summary>
/// Updates the button pressed first/second on button pressed
/// </summary>
/// <param name="buttons">The gameplay button pressed</param>
void PlayerDodgingState::UpdateButtonOrderOnButtonPressed(EGAMEPLAYBUTTONCLASS button)
{
    if (m_firstMovementButton == button || m_secondMovementButton == button)
    {
        return;
    }

    if (m_firstMovementButton == EGAMEPLAYBUTTONCLASS::Nothing)
    {
        m_firstMovementButton = button;
    }
    else if (ButtonIsNotTheSameButIsInTheSameAxis(m_firstMovementButton, button))
    {
        m_firstMovementButton = button;
    }
    else if (m_secondMovementButton == EGAMEPLAYBUTTONCLASS::Nothing)
    {
        m_secondMovementButton = button;
    }
    else if (ButtonIsNotTheSameButIsInTheSameAxis(m_secondMovementButton, button))
    {
        m_secondMovementButton = button;
    }
}

/// <summary>
/// Detirmines if the button is a vertical direction button
/// </summary>
/// <param name="button">The gameplay button pressed</param>
/// <return>True, means the button moves a vertical direction</return>
bool PlayerDodgingState::IsVerticalDirection(EGAMEPLAYBUTTONCLASS button)
{
    bool isVertical = false;
    switch (button)
    {
    case EGAMEPLAYBUTTONCLASS::MoveUp:
    case EGAMEPLAYBUTTONCLASS::MoveDown:
        isVertical = true;
        break;
    }

    return isVertical;
}

/// <summary>
/// Detirmines if the button is a horizontal direction button
/// </summary>
/// <param name="button">The gameplay button pressed</param>
/// <return>True, means the button moves a horizontal direction</return>
bool PlayerDodgingState::IsHorizontalDirection(EGAMEPLAYBUTTONCLASS button)
{
    bool isVertical = false;
    switch (button)
    {
    case EGAMEPLAYBUTTONCLASS::MoveLeft:
    case EGAMEPLAYBUTTONCLASS::MoveRight:
        isVertical = true;
        break;
    }

    return isVertical;
}

/// <summary>
/// Detirmines if the button is on the same axis
/// </summary>
/// <param name="current">The button to check</param>
/// <param name="test">The button to test against</param>
/// <return>True means the buttons are on the same axis</return>
bool PlayerDodgingState::ButtonIsOnTheSameAxis(EGAMEPLAYBUTTONCLASS current, EGAMEPLAYBUTTONCLASS test)
{
    bool sameAxis = IsVerticalDirection(current) && IsVerticalDirection(test);
    if (!sameAxis)
    {
        sameAxis = IsHorizontalDirection(current) && IsHorizontalDirection(test);
    }

    return sameAxis;
}

/// <summary>
/// Detirmines if the button is on the same axis but is not the same button
/// </summary>
/// <param name="current">The button to check</param>
/// <param name="test">The button to test against</param>
/// <return>True means the buttons do not match but the axis match</return>
bool PlayerDodgingState::ButtonIsNotTheSameButIsInTheSameAxis(EGAMEPLAYBUTTONCLASS current, EGAMEPLAYBUTTONCLASS test)
{
    bool buttonsSameAndOnSameAxis = current != test;
    if (buttonsSameAndOnSameAxis)
    {
        buttonsSameAndOnSameAxis = ButtonIsOnTheSameAxis(current, test);
    }

    return buttonsSameAndOnSameAxis;
}

/// <summary>
/// Updates the button pressed first/second on button released
/// </summary>
/// <param name="buttons">The gameplay button pressed</param>
void PlayerDodgingState::UpdateButtonOrderOnButtonReleased(EGAMEPLAYBUTTONCLASS button)
{
    if (m_firstMovementButton == button)
    {
        if (m_secondMovementButton == EGAMEPLAYBUTTONCLASS::Nothing)
        {
            m_firstMovementButton = EGAMEPLAYBUTTONCLASS::Nothing;
        }
        else
        {
            m_firstMovementButton = m_secondMovementButton;
            m_secondMovementButton = EGAMEPLAYBUTTONCLASS::Nothing;
        }
    }
    else if (m_secondMovementButton == button)
    {
        m_secondMovementButton = EGAMEPLAYBUTTONCLASS::Nothing;
    }
}

/// <summary>
/// Handles moving the player
/// </summary>
/// <param name="timeDelta">Delta time between frames</param>
void PlayerDodgingState::MovePlayer(float timeDelta)
{
    XMFLOAT3 vector = XMFLOAT3(0, 0, 0);
    MoveVectorByButton(&vector, m_firstMovementButton);
    MoveVectorByButton(&vector, m_secondMovementButton);
    ApplyForce(timeDelta, vector);

    MoveByForce(timeDelta);
}

/// <summary>
/// Move the given vector based on the button pressed
/// </summary>
/// <param name="vector">Vector to move</param>
/// <param name="button">Button pressed</param>
void PlayerDodgingState::MoveVectorByButton(XMFLOAT3* vector, EGAMEPLAYBUTTONCLASS button)
{
    if (button != EGAMEPLAYBUTTONCLASS::Nothing)
    {
        XMFLOAT3 buttonVector = GetVectorChangeForGameplayButton(button);
        vector->x += buttonVector.x;
        vector->y += buttonVector.y;
    }
}

/// <summary>
/// Calculates how much the vector should alter based on the gameplay button
/// </summary>
/// <param name="button">The button to test against</param>
/// <return>The direction change to apply to the transform to apply the force of the button</return>
XMFLOAT3 PlayerDodgingState::GetVectorChangeForGameplayButton(EGAMEPLAYBUTTONCLASS button)
{
    XMFLOAT3 vector = XMFLOAT3(0, 0, 0);
    switch (button)
    {
    case EGAMEPLAYBUTTONCLASS::MoveUp:
        vector.y += 1.0f;
        break;
    case EGAMEPLAYBUTTONCLASS::MoveLeft:
        vector.x -= 1.0f;
        break;
    case EGAMEPLAYBUTTONCLASS::MoveRight:
        vector.x += 1.0f;
        break;
    case EGAMEPLAYBUTTONCLASS::MoveDown:
        vector.y -= 1.0f;
        break;
    }

    return vector;
}

/// <summary>
/// Applies force in the given direction
/// </summary>
/// <param name="timeDelta">Delta time between frames</param>
/// <param name="direction">Direction to apply force</param>
void PlayerDodgingState::ApplyForce(float timeDelta, XMFLOAT3 direction)
{
    if (direction.x == 0.0f && direction.y == 0.0f)
    {
        return;
    }

    XMFLOAT3 original = m_playerMovingBody->GetForceApplied();

    XMFLOAT3 newForce = m_playerMovingBody->GetForceApplied();
    ApplyForceToSingleAxis(&newForce.x, direction.x);
    ApplyForceToSingleAxis(&newForce.y, direction.y);

    float moveSpeed = m_playerMovingBody->GetMoveSpeed();
    moveSpeed += *m_moveSpeedPerFrame * timeDelta;

    float drag = ((*m_moveSpeedMax * 100) / 2) * timeDelta;
    SlowSpeedIfDirectionChanged(original, newForce, drag, moveSpeed);
    RestrictSpeedAndForceToResonableBounds(moveSpeed, *m_moveSpeedMax, newForce);

    m_playerMovingBody->SetForceApplied(newForce);
    m_playerMovingBody->SetMoveSpeed(moveSpeed);
}

/// <summary>
/// Applies force to a single axis
/// </summary>
/// <param name="axisValue">The value to update</param>
/// <param name="direction">The intensity to apply</param>
void PlayerDodgingState::ApplyForceToSingleAxis(float* axisValue, float direction)
{
    if (direction != 0)
    {
        *axisValue += direction;
    }

    if (*axisValue > 1)
    {
        *axisValue = 1;
    }

    if (*axisValue < -1)
    {
        *axisValue = -1;
    }
}

/// <summary>
/// Slow the speed value based on the drag amount
/// </summary>
/// <param name="originalDirection">The direction the vector was aiming before moving</param>
/// <param name="newDirection">The direction the vector is moving now</param>
/// <param name="drag">The drag amount if direction moves</param>
/// <param name="movementSpeed">The movement speed to adjust</param>
void PlayerDodgingState::SlowSpeedIfDirectionChanged(XMFLOAT3 originalDirection, XMFLOAT3 newDirection, float drag, float& movementSpeed)
{
    if ((originalDirection.x > 0 && newDirection.x < 0) || (originalDirection.x < 0 && newDirection.x > 0))
    {
        movementSpeed -= drag;
    }
    else if ((originalDirection.y > 0 && newDirection.y < 0) || (originalDirection.y < 0 && newDirection.y > 0))
    {
        movementSpeed -= drag;
    }
}

/// <summary>
/// Restrict speed based on max speed and force
/// </summary>
/// <param name="speed">Current speed to restrict</param>
/// <param name="maxSpeed">The max speed for the instance</param>
/// <param name="force">Force which is also restricted if speed is 0</param>
void PlayerDodgingState::RestrictSpeedAndForceToResonableBounds(float& speed, float maxSpeed, XMFLOAT3& force)
{
    if (speed > maxSpeed)
    {
        speed = maxSpeed;
    }
    else if (speed <= 0)
    {
        speed = 0;
        force.x = 0;
        force.y = 0;
    }
}

/// <summary>
/// Moves the player based on forces applied
/// </summary>
/// <param name="timeDelta">Delta time between frames</param>
void PlayerDodgingState::MoveByForce(float timeDelta)
{
    XMFLOAT3 newForce = m_playerMovingBody->GetForceApplied();
    float bodySpeed = m_playerMovingBody->GetMoveSpeed();
    if (bodySpeed > 0 && (newForce.x != 0 || newForce.y != 0))
    {
        MoveTransformInDirectionByDistance(m_transform, newForce, bodySpeed, timeDelta * *m_speedMultiplier);
        SlowPlayerBasedOnDrag(bodySpeed, *m_dragSpeedPerFrame, timeDelta);

        XMFLOAT3 movement = *m_forceApplied;
        MoveTransformInDirectionByDistance(m_playerReference->GetTransform(), *m_forceApplied, *m_moveSpeed, timeDelta * *m_speedMultiplier);
        SlowPlayerBasedOnDrag(m_moveSpeed, *m_dragSpeedPerFrame, timeDelta);

        if (bodySpeed == 0)
        {
            newForce.x = 0;
            newForce.y = 0;
        }
        else
        {
            MoveFloatTowardZero(&newForce.x, timeDelta);
            MoveFloatTowardZero(&newForce.y, timeDelta);
            m_playerMovingBody->SetForceApplied(newForce);
        }

        m_playerMovingBody->SetMoveSpeed(bodySpeed);
    }
}

/// <summary>
/// Slow the player based on drag value given
/// </summary>
/// <param name="speed">Speed to move the player</param>
/// <param name="drag">Drag value to apply</param>
/// <param name="delta">Time delta to use when slowing the player</param>
void PlayerDodgingState::SlowPlayerBasedOnDrag(float& speed, float drag, float delta)
{
    speed -= drag * delta;
    if (speed < 0)
    {
        speed = 0;
    }
}

/// <summary>
/// Move the given transform in a direction based on speed and direction
/// </summary>
/// <param name="transform">Transform to move</param>
/// <param name="force">Force to apply</param>
/// <param name="speed">Speed to use</param>
/// <param name="distance">Distance to move the player</param>
void PlayerDodgingState::MoveTransformInDirectionByDistance(Transform* transform, XMFLOAT3 force, float speed, float distance)
{
    float size = sqrt(force.x * force.x + force.y * force.y);

    force = MathHelper::Multiply
    (XMFLOAT3((force.x * speed) / size, (force.y * speed) / size, 0), distance);
    transform->Translate(force);
}

/// <summary>
/// Move the given float toward zero by the given amount
/// </summary>
/// <param name="value">Value to adjust</param>
/// <param name="intensity">Intensity to adjust the value by</param>
void PlayerDodgingState::MoveFloatTowardZero(float* value, float intensity)
{
    if (*value != 0)
    {
        if (*value > 0)
        {
            *value -= intensity;
            if (*value < 0)
            {
                *value = 0;
            }
        }
        else
        {
            *value += intensity;
            if (*value > 0)
            {
                *value = 0;
            }
        }
    }
}
