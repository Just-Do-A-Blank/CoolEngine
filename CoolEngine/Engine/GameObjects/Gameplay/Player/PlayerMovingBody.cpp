#include "PlayerMovingBody.h"

PlayerMovingBody::PlayerMovingBody()
{
    m_forceApplied = XMFLOAT3(0, 0, 0);
    m_moveSpeed = 0;
}

/// <summary>
/// Gets the current force applied
/// </summary>
/// <returns>The current force applied on the player</returns>
XMFLOAT3 PlayerMovingBody::GetForceApplied()
{
    return m_forceApplied;
}

/// <summary>
/// Sets a new force applied value
/// </summary>
/// <param name="newValue">The new force applied value</param>
void PlayerMovingBody::SetForceApplied(XMFLOAT3 newValue)
{
    m_forceApplied = newValue;
}

/// <summary>
/// Gets the current movement speed of the player
/// </summary>
/// <returns>The current move speed of the player</returns>
float PlayerMovingBody::GetMoveSpeed()
{
    return m_moveSpeed;
}

/// <summary>
/// Sets new movement speed
/// </summary>
/// <param name="newValue">New movement speed</param>
void PlayerMovingBody::SetMoveSpeed(float newValue)
{
    m_moveSpeed = newValue;
}