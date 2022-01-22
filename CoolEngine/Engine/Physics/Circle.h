#pragma once
#include "Engine/Physics/Shape.h"

/// <summary>
/// Circle class that contains its position and the radius. Used for Circle collisions.
/// </summary>
class Circle : public Shape
{
public:
	/// <summary>
	/// Default constructor for circle.
	/// </summary>
	Circle()
	{
		m_transform = nullptr;
		m_radius = 0;

		m_shapeType = ShapeType::CIRCLE;
	};

	/// <summary>
	/// Constructor for circle.
	/// </summary>
	/// <param name="x"></param>
	/// <param name="y"></param>
	/// <param name="radius"></param>
	Circle(Transform* t, float radius)
	{
		m_transform = t;
		m_radius = radius;

		m_shapeType = ShapeType::CIRCLE;
	}

	~Circle()
	{
		m_transform = nullptr;
	}

	float m_radius;

	bool Collide(Shape* shape)
	{
		return shape->Collide(this);
	}

	bool Collide(Circle* circle)
	{
		return Collision::CircleCollision(circle, this);
	}

	bool Collide(Box* box)
	{
		return Collision::CircleBoxCollision(this, box);
	}

	bool CollideResponse(Shape* shape)
	{
		return shape->CollideResponse(this);
	}

	bool CollideResponse(Circle* circle)
	{
		return Collision::CircleCollisionAndResponse(circle, this);
	}

	bool CollideResponse(Box* box)
	{
		return Collision::CircleBoxCollisionAndResponse(this, box);
	}

	void CreateEngineUI() override
	{
		IMGUI_LEFT_LABEL(ImGui::DragFloat, "Radius", &m_radius, 1.0f, 0.0f);

		Shape::CreateEngineUI();
	}
};