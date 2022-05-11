#pragma once

#pragma once

/// <summary>
/// Gameplay buttons which are buttons in terms of gameplay uses
/// </summary>
enum class EGAMEPLAYBUTTONCLASS : int
{
    /// <summary>
    /// Button(s) which signify moving up
    /// </summary>
    MoveUp,

    /// <summary>
    /// Button(s) which signify moving down
    /// </summary>
    MoveDown,

    /// <summary>
    /// Button(s) which signify moving left
    /// </summary>
    MoveLeft,

    /// <summary>
    /// Button(s) which signify moving right
    /// </summary>
    MoveRight,

    /// <summary>
    /// Button(s) for dashing as the player
    /// </summary>
    Dash,

    /// <summary>
    /// Button(s) for rolling as the player
    /// </summary>
    Roll,

    /// <summary>
    /// Button(s) for openning the menu
    /// </summary>
    Menu,

    /// <summary>
    /// Button(s) for accepting an item on the menu
    /// </summary>
    Accept,

    /// <summary>
    /// Button(s) for canceling an item on the menu
    /// </summary>
    Cancel
};
