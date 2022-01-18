#pragma once
#include "Engine/Physics/Shape.h"

class Box : public Shape
{
public:
	Box()
	{
		m_transform = nullptr;
	}

	Box(Transform* trans)
	{
		m_transform = trans;
	}

	~Box()
	{
		m_transform = nullptr;
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
};