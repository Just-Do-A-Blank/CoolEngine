#pragma once

/// <summary>
/// Moves the player once the player controller allows it to
/// </summary>
class PlayerMovementState
{
public:

	/// <summary>
	/// Updates the state and moves the player
	/// </summary>
	/// <param name="timeDelta">Time between frames</param>
	/// <returns>True means state may remain, False means the state is complete</returns>
	virtual bool Update(float timeDelta) = 0;

	/// <summary>
	/// The next state use if this state no longer needed
	/// </summary>
	/// <returns>The next state to use</returns>
	virtual PlayerMovementState* NextState() = 0;

};