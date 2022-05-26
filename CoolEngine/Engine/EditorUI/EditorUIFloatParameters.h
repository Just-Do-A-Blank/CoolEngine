#pragma once

#if EDITOR

/// <summary>
/// Parameters optionally required to display a float or set of floats in the editor
/// </summary>
struct EditorUIFloatParameters
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
    float m_minValue;

    /// <summary>
    /// The max value the editor value could go to
    /// </summary>
    float m_maxValue;

    /// <summary>
    /// Adds a tooltip over the input. Blank means nothing is added.
    /// </summary>
    char* m_tooltipText;

	EditorUIFloatParameters();

};

#endif