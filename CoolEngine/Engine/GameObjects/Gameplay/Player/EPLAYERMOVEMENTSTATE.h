#pragma once

/// <summary>
/// The way the player is currently moving
/// </summary>
enum class EPLAYERMOVEMENTSTATE : int
{
	/// <summary>
	/// Player is walking
	/// </summary>
	Walking = 0,

	/// <summary>
	/// Player is dashing
	/// </summary>
	Dodging,

	/// <summary>
	/// Player is rolling
	/// </summary>
	Rolling,
};