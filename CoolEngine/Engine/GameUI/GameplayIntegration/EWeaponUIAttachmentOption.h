#pragma once

/// <summary>
/// How weapons are displayed on the UI
/// </summary>
enum class EWEAPONUIATTACHMENTOPTION : int
{
    /// <summary>
    /// No weapon is attached
    /// </summary>
    None = 0,

    /// <summary>
    /// Weapon the character is holding is shown
    /// </summary>
    Holding,

    /// <summary>
    /// Weapon in the inventory is shown
    /// </summary>
    Inventory,

    COUNT,
};