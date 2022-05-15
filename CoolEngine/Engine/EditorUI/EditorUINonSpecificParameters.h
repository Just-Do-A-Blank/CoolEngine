#pragma once

/// <summary>
/// Parameters optionally required to add detail to many types of edit entries without many things in common with them
/// other than they are editor inputs.
/// </summary>
struct EditorUINonSpecificParameters
{
    /// <summary>
    /// The width of the coloumn
    /// </summary>
    float m_columnWidth;

    /// <summary>
    /// Adds a tooltip over the input. Blank means nothing is added.
    /// </summary>
    char* m_tooltipText;
};