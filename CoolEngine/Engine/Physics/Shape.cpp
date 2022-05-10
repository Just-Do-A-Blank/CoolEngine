#include "Shape.h"
#include "Engine/EditorUI/EditorUI.h"

Shape::Shape()
{
#if EDITOR
    SetCheckboxTriggerValue(m_isTrigger);
    SetCheckboxCollidableValue(m_isCollidable);


    string s = "m_isTrigger: " + GetBoolAnswer(m_isTrigger);
    LOG(s);

    s = "m_isTriggerCheckboxValue: " + GetBoolAnswer(m_isTriggerCheckboxValue);
    LOG(s);

    s = "m_isCollidableCheckboxValue: " + GetBoolAnswer(m_isCollidableCheckboxValue);
    LOG(s);

    s = "m_isCollidable: " + GetBoolAnswer(m_isCollidable);
    LOG(s);
#endif
}

string Shape::GetBoolAnswer(bool value)
{
    if (value)
    {
        return "true";
    }
    else
    {
        return "false";
    }
}

ShapeType Shape::GetShapeType()
{
	return m_shapeType;
}

string Shape::ShapeTypeToString(ShapeType type)
{
	switch (type)
	{
	case ShapeType::BOX:
		return "Box";

	case ShapeType::CIRCLE:
		return "Circle";

	default:
		return "None";
	}
}

#if EDITOR
void Shape::CreateEngineUI()
{
	EditorUI::Checkbox("Collidable", m_isCollidableCheckboxValue);

	ImGui::Spacing();

	EditorUI::Checkbox("Trigger", m_isTriggerCheckboxValue);
}
#endif

void Shape::SetIsTrigger(bool value)
{
	m_isTrigger = value;
#if EDITOR
    SetCheckboxTriggerValue(m_isTriggerCheckboxValue);
#endif
}

void Shape::SetIsCollidable(bool value)
{
	m_isCollidable = value;
#if EDITOR
    SetCheckboxCollidableValue(m_isCollidableCheckboxValue);
#endif
}

bool Shape::IsTrigger() const
{
#if EDITOR
	return GetCheckboxTriggerValue();
#else
    return m_isTrigger;
#endif
}

bool Shape::IsCollidable() const
{
#if EDITOR
    return GetCheckboxCollidableValue();
#else
    return m_isCollidable;
#endif
}

#if EDITOR
void Shape::SetCheckboxTriggerValue(bool newValue)
{
    m_isTriggerCheckboxValue = newValue;
}

void Shape::SetCheckboxCollidableValue(bool newValue)
{
    m_isCollidableCheckboxValue = newValue;
}

bool Shape::GetCheckboxTriggerValue() const
{
    return m_isTriggerCheckboxValue;
}

bool Shape::GetCheckboxCollidableValue() const
{
    return m_isCollidableCheckboxValue;
}
#endif