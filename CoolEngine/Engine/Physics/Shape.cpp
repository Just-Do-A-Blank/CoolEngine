#include "Shape.h"
#include "Engine/EditorUI/EditorUI.h"

Shape::Shape()
{
#if EDITOR
	m_collideTypeList = GetCollideTypeAsList();
	m_collideTypeSelectedItem = GetCollideTypeFromIndex(0);
#endif
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

#if EDITOR
	m_collideTypeList = GetCollideTypeAsList();
	m_collideTypeSelectedItem = GetCollideTypeFromIndex(CollideBoolsToListIndex(m_isCollidable, m_isTrigger));
#endif
}

Shape::Shape(Shape const* other, GameObject* pgameobject)
{
	m_pgameObject = pgameobject;
	m_scale = other->m_scale;
	m_isTrigger = other->m_isTrigger;
	m_isCollidable = other->m_isCollidable;
	m_isTrigger = other->m_isTrigger;
	m_shapeType = other->m_shapeType;

#if EDITOR
	m_collideTypeList = GetCollideTypeAsList();
	m_collideTypeSelectedItem = GetCollideTypeFromIndex(CollideBoolsToListIndex(m_isCollidable, m_isTrigger));
#endif
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
	if (EditorUI::ComboBox("Collider", m_collideTypeList, m_collideTypeSelectedItem))
	{
		SetTriggerableCollidableFromIndex(m_collideTypeSelectedItem.first);
	}

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

XMFLOAT2 Shape::GetScale()
{
    return m_scale;
}

void Shape::SetScale(XMFLOAT2 scale)
{
    m_scale = scale;
}


#if EDITOR
list<pair<int, string>> Shape::GetCollideTypeAsList()
{
	list<pair<int, string>> returnListItems = list<pair<int, string>>();
	for (int i = 0; i < 3; ++i)
	{
		returnListItems.push_back(GetCollideTypeFromIndex(i));
	}

	return returnListItems;
}

pair<int, string> Shape::GetCollideTypeFromIndex(int index)
{
	pair<int, string> returnItem = pair<int, string>(0, "None");
	switch (index)
	{
		case 1:
			returnItem = pair<int, string>(1, "Collidable");
			break;
		case 2:
			returnItem = pair<int, string>(2, "Triggerable");
			break;
	}

	return returnItem;
}

int Shape::CollideBoolsToListIndex(bool isCollidable, bool isTriggerable)
{
	if (isCollidable)
	{
		return 1;
	}

	if (isTriggerable)
	{
		return 2;
	}

	return 0;
}

void Shape::SetTriggerableCollidableFromIndex(int index)
{
	switch (index)
	{
	case 0:
		m_isTrigger = false;
		m_isCollidable = false;
		break;
	case 1:
		m_isTrigger = false;
		m_isCollidable = true;
		break;
	case 2:
		m_isTrigger = true;
		m_isCollidable = false;
		break;
	}
}
#endif