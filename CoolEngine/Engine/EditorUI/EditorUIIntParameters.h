#pragma once

/// <summary>
/// Parameters optionally required to display a ints or set of ints in the editor
/// </summary>
struct EditorUIIntParameters
{
    /// <summary>
    /// The width of the coloumn
    /// </summary>
    float m_columnWidth;

    /// <summary>
    /// The step in value as moving the value up and down
    /// </summary>
    float m_speed;

    /// <summary>
    /// The min value the editor value could go to
    /// </summary>
    int m_minValue;

    /// <summary>
    /// The max value the editor value could go to
    /// </summary>
    int m_maxValue;

    /// <summary>
    /// Adds a tooltip over the input. Blank means nothing is added.
    /// </summary>
    char* m_tooltipText;
};