#pragma once
#include "Engine/Physics/Collision.h"

class Transform;
class Circle;
class Box;

// For double dispatch pattern
class Shape
{
public:

	// To Do - Add line collision, if you want it
	virtual bool Collide(Shape* shape) = 0;
	virtual bool Collide(Circle* circle) = 0;
	virtual bool Collide(Box* box) = 0;

	virtual bool CollideResponse(Shape* shape) = 0;
	virtual bool CollideResponse(Circle* circle) = 0;
	virtual bool CollideResponse(Box* box) = 0;

	Transform* m_transform;
};