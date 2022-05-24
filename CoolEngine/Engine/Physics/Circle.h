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
		m_pgameObject = nullptr;
		m_radius = 0;

		m_shapeType = ShapeType::CIRCLE;
	};

	Circle(GameObject* gameObject)
	{
		m_pgameObject = gameObject;
		m_radius = m_pgameObject->GetTransform()->GetWorldScale().x;

		m_shapeType = ShapeType::CIRCLE;
	}

	/// <summary>
	/// Constructor for circle.
	/// </summary>
	/// <param name="x"></param>
	/// <param name="y"></param>
	/// <param name="radius"></param>
	Circle(GameObject* gameObject, float radius)
	{
		m_pgameObject = gameObject;
		m_radius = radius;

		m_shapeType = ShapeType::CIRCLE;
	}

	Circle(const nlohmann::json& data, GameObject* gameObject) : Shape(data)
	{
		m_pgameObject = gameObject;
		m_radius = data["CircleRadius"];

		m_shapeType = ShapeType::CIRCLE;
	}

	void Serialize(nlohmann::json& data) override
	{
		Shape::Serialize(data);

		data["CircleRadius"] = m_radius;
	}

	float GetRadius()
	{
		return m_radius;
	}

	void UpdateShapeDimensions()
	{
		m_radius = m_pgameObject->GetTransform()->GetWorldScale().x;
	}

	XMFLOAT2 GetShapeDimensions()
	{
		return XMFLOAT2(m_scale.x * m_radius, m_scale.y * m_radius);
	}

	~Circle()
	{
		m_pgameObject = nullptr;
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

#if EDITOR
	void CreateEngineUI() override
	{
        EditorUI::DragFloat("Radius", m_radius);

		ImGui::Spacing();

		Shape::CreateEngineUI();
	}
#endif
};