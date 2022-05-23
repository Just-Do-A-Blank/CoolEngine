#pragma once

/// <summary>
/// Signifies a modification to the resource list
/// </summary>
enum class ERESOURCEMODIFICATIONRESULT : int
{
	/// <summary>
	/// No modification to the list of resources
	/// </summary>
	NoListModification = 0,
	
	/// <summary>
	/// An item is added to the list
	/// </summary>
	Added,

	/// <summary>
	/// An item is removed from the list
	/// </summary>
	Removed,
};