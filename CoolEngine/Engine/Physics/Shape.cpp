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

#if EDITOR
void Shape::CreateEngineUI()
{
    EditorUI::Checkbox("Collidable", m_isCollidable);

    ImGui::Spacing();

    EditorUI::Checkbox("Trigger", m_isTrigger);

    ImGui::Spacing();

    EditorUI::Checkbox("Render Hitbox", m_isRendered);
}
#endif

void Shape::Serialize(nlohmann::json& data)
{
	switch (m_shapeType)
	{
	case ShapeType::BOX:
		data["ShapeType"] = "Box";
		break;

	case ShapeType::CIRCLE:
		data["ShapeType"] = "Circle";
		break;
	}

	data["IsTrigger"] = m_isTrigger;
	data["IsCollidable"] = m_isCollidable;
}

void Shape::SetIsTrigger(bool value)
{
    m_isTrigger = value;
}

void Shape::SetIsCollidable(bool value)
{
    m_isCollidable = value;
}

void Shape::SetIsRendered(bool value)
{
    m_isRendered = value;
}

bool Shape::IsTrigger() const
{
    return m_isTrigger;
}

bool Shape::IsCollidable() const
{
    return m_isCollidable;
}

bool Shape::IsRendered() const
{
    return m_isRendered;
}

Shape::Shape(const nlohmann::json& data)
{
	if (data["ShapeType"] == "Box")
	{
		m_shapeType = ShapeType::BOX;
	}
	else if (data["ShapeType"] == "Circle")
	{
		m_shapeType = ShapeType::CIRCLE;
	}

	m_isTrigger = data["IsTrigger"];
	m_isCollidable = data["IsCollidable"];
}

Shape::Shape()
{
}
