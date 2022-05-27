#pragma once

/// <summary>
/// Indicators on what the player is currently doing
/// </summary>
class PlayerMovingBody
{
public:
    PlayerMovingBody();

    /// <summary>
    /// Gets the current force applied
    /// </summary>
    /// <returns>The current force applied on the player</returns>
    XMFLOAT3 GetForceApplied();

    /// <summary>
    /// Sets a new force applied value
    /// </summary>
    /// <param name="newValue">The new force applied value</param>
    void SetForceApplied(XMFLOAT3 newValue);

    /// <summary>
    /// Gets the current movement speed of the player
    /// </summary>
    /// <returns>The current move speed of the player</returns>
    float GetMoveSpeed();

    /// <summary>
    /// Sets new movement speed
    /// </summary>
    /// <param name="newValue">New movement speed</param>
    void SetMoveSpeed(float newValue);

private:
    /// <summary>
    /// The force currently applied to the player. Shared among states as this is the force on the player as a whole
    /// </summary>
    XMFLOAT3 m_forceApplied;

    /// <summary>
    /// The current speed of the player. Shared among states as this is the force on the player as a whole
    /// </summary>
    float m_moveSpeed;
};

