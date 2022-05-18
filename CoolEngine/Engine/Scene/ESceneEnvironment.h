#pragma once

/// <summary>
/// Describes where the scene is running
/// </summary>
enum class ESCENEENVIRONMENT : int
{
    /// <summary>
    /// In the editor with access to editing game objects
    /// </summary>
    Editor = 0,

    /// <summary>
    /// In the game which may create new scenes but only through gameplay
    /// </summary>
    Game,
};