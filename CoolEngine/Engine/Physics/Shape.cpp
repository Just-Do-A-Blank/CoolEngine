#include "Shape.h"

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