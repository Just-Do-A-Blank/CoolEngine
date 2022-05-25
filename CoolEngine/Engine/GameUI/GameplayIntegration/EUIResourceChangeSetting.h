#pragma once

/// <summary>
/// How to decide what to do with UI regarding resources
/// </summary>
enum class EUIRESOURCECHANGESETTING : int
{
	/// <summary>
	/// Change when the resource equals value
	/// </summary>
	ChangeWhenNumberHit = 0,

	/// <summary>
	/// Change when number is above the target
	/// </summary>
	ChangeWhenNumberIsAboveThis,

	/// <summary>
	/// Change when number is below the target
	/// </summary>
	ChangeWhenNumberIsBelowThis,
};