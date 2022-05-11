#include "PlayerController.h"
#include "Engine/Managers/GameManager.h"

PlayerController::PlayerController(InputsAsGameplayButtons* gameplayButtons, Transform* transformOfTheGameObject)
{
    m_gameplayButtons = gameplayButtons;
    m_transform = transformOfTheGameObject;
}

/// <summary>
/// Handles events from the Observations
/// </summary>
void PlayerController::Handle(Event* e)
{
    switch (e->GetEventID())
    {
    case EventType::KeyPressed:
        KeyPressed((KeyPressedEvent*)e);
        break;
    case EventType::KeyReleased:
        KeyReleased((KeyReleasedEvent*)e);
        break;
    case EventType::MouseButtonPressed:
        MouseButtonPressed((MouseButtonPressedEvent*)e);
        break;
    case EventType::MouseButtonReleased:
        MouseButtonReleased((MouseButtonReleasedEvent*)e);
        break;
    case EventType::MouseMoved:
        MouseMoved((MouseMovedEvent*)e);
        break;

    }
}

/// <summary>
/// Handles any keypresses when they are pressed (frame whilst pressed)
/// </summary>
void PlayerController::KeyPressed(KeyPressedEvent* e)
{
    //Can use 'Letter' or the raw keycode for keyboard inputs.
    if (e->GetKeyCode() == 'C')
    {
        //LOG("C");
    }

    if (e->GetKeyCode() == 0x43)
    {

    }

    if (e->GetKeyCode() == 0x44)
    {
        //LOG("D");
    }

    // Player movement
    XMFLOAT3 vector = XMFLOAT3(0, 0, 0);
    if (e->GetKeyCode() == 'W')
    {
        vector.y = 1.0f;
    }
    if (e->GetKeyCode() == 'S')
    {
        vector.y = -1.0f;
    }
    if (e->GetKeyCode() == 'A')
    {
        vector.x = -1.0f;
    }
    if (e->GetKeyCode() == 'D')
    {
        vector.x = 1.0f;
    }
    if (vector.x != 0.0f || vector.y != 0.0f)
    {
        float size = sqrt(vector.x * vector.x + vector.y * vector.y);
        vector = MathHelper::Multiply(XMFLOAT3((m_moveSpeed * vector.x) / size, (m_moveSpeed * vector.y) / size, 0), GameManager::GetInstance()->GetTimer()->DeltaTime());
        m_transform->Translate(vector);
    }
}

/// <summary>
/// Handles any keypresses when they are released (first frame).
/// </summary>
void PlayerController::KeyReleased(KeyReleasedEvent* e)
{

}

/// <summary>
/// Handles any mouse button presses when pressed (frame whilst pressed)
/// </summary>
void PlayerController::MouseButtonPressed(MouseButtonPressedEvent* e)
{
    if (e->GetButton() == VK_LBUTTON)
    {

        //LOG("MB1");

    }

    if (e->GetButton() == VK_RBUTTON)
    {

        //LOG("MB2");

    }


}

/// <summary>
/// Handles any mouse button when they are released (first frame).
/// </summary>
void PlayerController::MouseButtonReleased(MouseButtonReleasedEvent* e)
{

}

/// <summary>
/// Handles the mouse moving across the window
/// </summary>
void PlayerController::MouseMoved(MouseMovedEvent* e)
{
    //LOG(e->GetX()); 
    //LOG(e->GetY());
}