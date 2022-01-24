#include "Shape.h"
#include "Engine/EditorUI/EditorUI.h"



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

void Shape::CreateEngineUI()
{
	EditorUI::Checkbox("Collidable", m_isCollidable);

	ImGui::Spacing();

	EditorUI::Checkbox("Trigger", m_isTrigger);
}

void Shape::SetIsTrigger(bool value)
{
	m_isTrigger = value;
}

void Shape::SetIsCollidable(bool value)
{
	m_isCollidable = value;
}

bool Shape::IsTrigger() const
{
	return m_isTrigger;
}

bool Shape::IsCollidable() const
{
	return m_isCollidable;
}
