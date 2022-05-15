#pragma once
#include "Engine/Physics/Shape.h"
#include "Engine/EditorUI/EditorUI.h"


/// <summary>
/// Circle class that contains its position and the radius. Used for Circle collisions.
/// </summary>
class Circle : public Shape
{
public:
	float m_radius;

	/// <summary>
	/// Default constructor for circle.
	/// </summary>
	Circle()
	{
		m_transform = nullptr;
		m_radius = 0;

		m_shapeType = ShapeType::CIRCLE;
	};

	Circle(Transform* t)
	{
		m_transform = t;
		m_radius = m_transform->GetWorldScale().x;

		m_shapeType = ShapeType::CIRCLE;
	}

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

	float GetRadius()
	{
		return m_radius;
	}

	void SetShapeDimensions(XMFLOAT3 scale)
	{
		m_radius = m_transform->GetWorldScale().x;
	}

	~Circle()
	{
		m_transform = nullptr;
	}

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

	XMFLOAT2 GetShapeDimensions()
	{
		return XMFLOAT2(m_radius, m_radius);
	}

#if EDITOR
	void CreateEngineUI() override
	{
        EditorUI::DragFloat("Radius", m_radius);

		ImGui::Spacing();

		Shape::CreateEngineUI();
	}
#endif
};