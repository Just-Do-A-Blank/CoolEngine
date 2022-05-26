#include "EditorUIFloatParameters.h"
#include "EditorUI.h"

#if EDITOR

EditorUIFloatParameters::EditorUIFloatParameters()
{
	m_columnWidth = EditorUI::GetDefaultColumnWidth();
	m_speed = EditorUI::GetDefaultSpeed();
	m_minValue = EditorUI::GetDefaultMinimumValue();
	m_maxValue = EditorUI::GetDefaultMaximumValue();
	m_tooltipText = "";
}

#endif
