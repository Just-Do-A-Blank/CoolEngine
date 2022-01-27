#pragma once
#include "Engine/Physics/Shape.h"

class Box : public Shape
{
public:
	XMFLOAT2 m_halfSize;

	Box()
	{
		m_transform = nullptr;
		m_halfSize = { 0,0 };

		m_shapeType = ShapeType::BOX;
	}

	Box(Transform* trans)
	{
		m_transform = trans;
		m_halfSize = { m_transform->GetScale().x, m_transform->GetScale().y };

		m_shapeType = ShapeType::BOX;
	}

	Box(Transform* trans, XMFLOAT2 size)
	{
		m_transform = trans;
		m_halfSize = size;

		m_shapeType = ShapeType::BOX;
	}

	~Box()
	{
		m_transform = nullptr;
	}

	XMFLOAT2 GetHalfSize()
	{
		return m_halfSize;
	}

	void SetHalfSize(XMFLOAT2 size)
	{
		m_halfSize = size;
	}

	bool Collide(Shape* shape)
	{
		return shape->Collide(this);
	}

	bool Collide(Circle* circle)
	{
		return Collision::CircleBoxCollision(circle, this);
	}

	bool Collide(Box* box)
	{
		return Collision::BoxCollision(box, this);
	}

	bool CollideResponse(Shape* shape)
	{
		return shape->CollideResponse(this);
	}

	bool CollideResponse(Circle* circle)
	{
		return Collision::CircleBoxCollisionAndResponse(circle, this);
	}

	bool CollideResponse(Box* box)
	{
		return Collision::BoxCollisionAndResponse(box, this);
	}

#if EDITOR
	void CreateEngineUI() override
	{
		Shape::CreateEngineUI();
	}
#endif
};