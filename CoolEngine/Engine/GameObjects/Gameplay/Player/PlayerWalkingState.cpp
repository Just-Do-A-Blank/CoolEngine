#include "Engine/GameObjects/Gameplay/Player/PlayerWalkingState.h"
#include <Engine/Managers/Events/EventManager.h>

PlayerWalkingState::PlayerWalkingState(Transform* transform, InputsAsGameplayButtons* gameplayButtons)
{
	m_transform = transform;
	m_gameplayButtons = gameplayButtons;

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
void PlayerWalkingState::MovePlayer(float timeDelta)
{
    XMFLOAT3 vector = XMFLOAT3(0, 0, 0);
    if (m_firstMovementButton != EGAMEPLAYBUTTONCLASS::Nothing)
    {
        XMFLOAT3 buttonVector = GetVectorChangeForGameplayButton(m_firstMovementButton);
        vector.x += buttonVector.x;
        vector.y += buttonVector.y;
    }

    if (m_secondMovementButton != EGAMEPLAYBUTTONCLASS::Nothing)
    {
        XMFLOAT3 buttonVector = GetVectorChangeForGameplayButton(m_secondMovementButton);
        vector.x += buttonVector.x;
        vector.y += buttonVector.y;
    }

    if (vector.x != 0.0f || vector.y != 0.0f)
    {
        ApplyForce(timeDelta, vector);
    }

    MoveByForce(timeDelta);
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
/// <param name="name">Direction to apply force</param>
void PlayerWalkingState::ApplyForce(float timeDelta, XMFLOAT3 direction)
{
    float originalX = m_forceApplied.x;
    float originalY = m_forceApplied.y;

    if (direction.x != 0)
    {
        m_forceApplied.x += direction.x;
    }

    if (m_forceApplied.x > 1)
    {
        m_forceApplied.x = 1;
    }

    if (m_forceApplied.x < -1)
    {
        m_forceApplied.x = -1;
    }

    if (direction.y != 0)
    {
        m_forceApplied.y += direction.y;
    }


    if (m_forceApplied.y > 1)
    {
        m_forceApplied.y = 1;
    }

    if (m_forceApplied.y < -1)
    {
        m_forceApplied.y = -1;
    }

    m_moveSpeed += m_moveSpeedPerFrame * timeDelta;

    if ((originalX > 0 && m_forceApplied.x < 0) || (originalX < 0 && m_forceApplied.x > 0))
    {
        LOG(m_moveSpeed);
        m_moveSpeed -= (m_moveSpeedMax / 2);
    }
    else if ((originalY > 0 && m_forceApplied.y < 0) || (originalY < 0 && m_forceApplied.y > 0))
    {
        LOG(m_moveSpeed);
        m_moveSpeed -= (m_moveSpeedMax / 2);
    }


    if (m_moveSpeed > m_moveSpeedMax)
    {
        m_moveSpeed = m_moveSpeedMax;
    }
    else if (m_moveSpeed <= 0)
    {
        m_moveSpeed = 0;
        m_forceApplied.x = 0;
        m_forceApplied.y = 0;
    }

}

/// <summary>
/// Moves the player based on forces applied
/// </summary>
void PlayerWalkingState::MoveByForce(float timeDelta)
{
    if (m_moveSpeed > 0 && (m_forceApplied.x != 0 || m_forceApplied.y != 0))
    {

        XMFLOAT3 movement = m_forceApplied;
        float size = sqrt(movement.x * movement.x + movement.y * movement.y);

        movement = MathHelper::Multiply
        (XMFLOAT3((movement.x * m_moveSpeed) / size, (movement.y * m_moveSpeed) / size, 0), timeDelta * m_dragSpeed);
        m_transform->Translate(movement);

        m_moveSpeed -= m_dragSpeedPerFrame * timeDelta;
        if (m_moveSpeed < 0)
        {
            m_moveSpeed = 0;
        }

        if (m_moveSpeed == 0)
        {
            m_forceApplied.x = 0;
            m_forceApplied.y = 0;
        }
        else
        {
            if (m_forceApplied.x != 0)
            {
                if (m_forceApplied.x > 0)
                {
                    m_forceApplied.x -= timeDelta;
                    if (m_forceApplied.x < 0)
                    {
                        m_forceApplied.x = 0;
                    }
                }
                else
                {
                    m_forceApplied.x += timeDelta;
                    if (m_forceApplied.x > 0)
                    {
                        m_forceApplied.x = 0;
                    }
                }
            }

            if (m_forceApplied.y != 0)
            {
                if (m_forceApplied.y > 0)
                {
                    m_forceApplied.y -= timeDelta;
                    if (m_forceApplied.y < 0)
                    {
                        m_forceApplied.y = 0;
                    }
                }
                else
                {
                    m_forceApplied.y += timeDelta;
                    if (m_forceApplied.y > 0)
                    {
                        m_forceApplied.y = 0;
                    }
                }
            }
        }

    }
}