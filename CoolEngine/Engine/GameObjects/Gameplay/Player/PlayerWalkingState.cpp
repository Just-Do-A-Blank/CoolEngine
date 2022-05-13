#include "Engine/GameObjects/Gameplay/Player/PlayerWalkingState.h"
#include <Engine/Managers/Events/EventManager.h>

PlayerWalkingState::PlayerWalkingState(PlayerMovementParameters movement)
{
	m_transform = movement.m_transform;
	m_gameplayButtons = movement.m_gameplayButtons;
    m_moveSpeedMax = movement.m_maxSpeed;
    m_speedMultiplier = movement.m_walkingSpeed;
    m_moveSpeedPerFrame = movement.m_moveSpeedPerFrame;
    m_dragSpeedPerFrame = movement.m_dragSpeedPerFrame;

    m_forceApplied = XMFLOAT3(0, 0, 0);

    m_firstMovementButton = EGAMEPLAYBUTTONCLASS::Nothing;
    m_secondMovementButton = EGAMEPLAYBUTTONCLASS::Nothing;

    EventManager::Instance()->AddClient(EventType::KeyPressed, this);
    EventManager::Instance()->AddClient(EventType::KeyReleased, this);
    EventManager::Instance()->AddClient(EventType::MouseButtonPressed, this);
    EventManager::Instance()->AddClient(EventType::MouseButtonReleased, this);
    EventManager::Instance()->AddClient(EventType::MouseMoved, this);
}

PlayerWalkingState::~PlayerWalkingState()
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
void PlayerWalkingState::Handle(Event* e)
{
    switch (e->GetEventID())
    {
    case EventType::KeyPressed:
        KeyPressed((KeyPressedEvent*)e);
        break;
    case EventType::KeyReleased:
        KeyReleased((KeyReleasedEvent*)e);
        break;
    }
}

bool PlayerWalkingState::Update(float timeDelta)
{
    MovePlayer(timeDelta);

    return true;
}

/// <summary>
/// The next state use if this state no longer needed
/// </summary>
/// <returns>The next state to use</returns>
PlayerMovementState* PlayerWalkingState::NextState()
{
    return this;
}

/// <summary>
/// Handles any keypresses when they are pressed (frame whilst pressed)
/// </summary>
void PlayerWalkingState::KeyPressed(KeyPressedEvent* e)
{
    EGAMEPLAYBUTTONCLASS button = m_gameplayButtons->GetGameplayButtonFromKeyInput(e->GetKeyCode());
    if (button != EGAMEPLAYBUTTONCLASS::Nothing)
    {
        UpdateButtonOrderOnButtonPressed(button);
    }
}

/// <summary>
/// Handles any keypresses when they are released (first frame).
/// </summary>
void PlayerWalkingState::KeyReleased(KeyReleasedEvent* e)
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
void PlayerWalkingState::UpdateButtonOrderOnButtonPressed(EGAMEPLAYBUTTONCLASS button)
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
bool PlayerWalkingState::IsVerticalDirection(EGAMEPLAYBUTTONCLASS button)
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
bool PlayerWalkingState::IsHorizontalDirection(EGAMEPLAYBUTTONCLASS button)
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
bool PlayerWalkingState::ButtonIsOnTheSameAxis(EGAMEPLAYBUTTONCLASS current, EGAMEPLAYBUTTONCLASS test)
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
bool PlayerWalkingState::ButtonIsNotTheSameButIsInTheSameAxis(EGAMEPLAYBUTTONCLASS current, EGAMEPLAYBUTTONCLASS test)
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
void PlayerWalkingState::UpdateButtonOrderOnButtonReleased(EGAMEPLAYBUTTONCLASS button)
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
void PlayerWalkingState::MovePlayer(float timeDelta)
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
void PlayerWalkingState::MoveVectorByButton(XMFLOAT3* vector, EGAMEPLAYBUTTONCLASS button)
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
XMFLOAT3 PlayerWalkingState::GetVectorChangeForGameplayButton(EGAMEPLAYBUTTONCLASS button)
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
void PlayerWalkingState::ApplyForce(float timeDelta, XMFLOAT3 direction)
{
    if (direction.x == 0.0f && direction.y == 0.0f)
    {
        return;
    }

    float originalX = m_forceApplied.x;
    float originalY = m_forceApplied.y;
    XMFLOAT3 original = XMFLOAT3(m_forceApplied.x, m_forceApplied.y, 0);

    ApplyForceToSingleAxis(&m_forceApplied.x, direction.x);
    ApplyForceToSingleAxis(&m_forceApplied.y, direction.y);

    m_moveSpeed += *m_moveSpeedPerFrame * timeDelta;

    float drag = ((*m_moveSpeedMax * 100) / 2) * timeDelta;
    SlowSpeedIfDirectionChanged(original, m_forceApplied, drag,  &m_moveSpeed);
    RestrictSpeedAndForceToResonableBounds(&m_moveSpeed, *m_moveSpeedMax, &m_forceApplied);
}

/// <summary>
/// Applies force to a single axis
/// </summary>
/// <param name="axisValue">The value to update</param>
/// <param name="direction">The intensity to apply</param>
void PlayerWalkingState::ApplyForceToSingleAxis(float* axisValue, float direction)
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
void PlayerWalkingState::SlowSpeedIfDirectionChanged(XMFLOAT3 originalDirection, XMFLOAT3 newDirection, float drag, float* movementSpeed)
{
    if ((originalDirection.x > 0 && newDirection.x < 0) || (originalDirection.x < 0 && newDirection.x > 0))
    {
        *movementSpeed -= drag;
    }
    else if ((originalDirection.y > 0 && newDirection.y < 0) || (originalDirection.y < 0 && newDirection.y > 0))
    {
        *movementSpeed -= drag;
    }
}

/// <summary>
/// Restrict speed based on max speed and force
/// </summary>
/// <param name="speed">Current speed to restrict</param>
/// <param name="maxSpeed">The max speed for the instance</param>
/// <param name="force">Force which is also restricted if speed is 0</param>
void PlayerWalkingState::RestrictSpeedAndForceToResonableBounds(float* speed, float maxSpeed, XMFLOAT3* force)
{
    if (*speed > maxSpeed)
    {
        *speed = maxSpeed;
    }
    else if (*speed <= 0)
    {
        *speed = 0;
        force->x = 0;
        force->y = 0;
    }
}

/// <summary>
/// Moves the player based on forces applied
/// </summary>
/// <param name="timeDelta">Delta time between frames</param>
void PlayerWalkingState::MoveByForce(float timeDelta)
{
    if (m_moveSpeed > 0 && (m_forceApplied.x != 0 || m_forceApplied.y != 0))
    {

        XMFLOAT3 movement = m_forceApplied;
        MoveTransformInDirectionByDistance(m_transform, m_forceApplied, m_moveSpeed, timeDelta * *m_speedMultiplier);
        SlowPlayerBasedOnDrag(&m_moveSpeed, *m_dragSpeedPerFrame, timeDelta);

        if (m_moveSpeed == 0)
        {
            m_forceApplied.x = 0;
            m_forceApplied.y = 0;
        }
        else
        {
            MoveFloatTowardZero(&m_forceApplied.x, timeDelta);
            MoveFloatTowardZero(&m_forceApplied.y, timeDelta);
        }

    }
}

/// <summary>
/// Slow the player based on drag value given
/// </summary>
/// <param name="speed">Speed to move the player</param>
/// <param name="drag">Drag value to apply</param>
/// <param name="delta">Time delta to use when slowing the player</param>
void PlayerWalkingState::SlowPlayerBasedOnDrag(float* speed, float drag, float delta)
{
    *speed -= drag * delta;
    if (*speed < 0)
    {
        *speed = 0;
    }
}

/// <summary>
/// Move the given transform in a direction based on speed and direction
/// </summary>
/// <param name="transform">Transform to move</param>
/// <param name="force">Force to apply</param>
/// <param name="speed">Speed to use</param>
/// <param name="distance">Distance to move the player</param>
void PlayerWalkingState::MoveTransformInDirectionByDistance(Transform* transform, XMFLOAT3 force, float speed, float distance)
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
void PlayerWalkingState::MoveFloatTowardZero(float* value, float intensity)
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