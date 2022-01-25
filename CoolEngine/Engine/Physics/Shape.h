#pragma once
#include "Engine/Physics/Collision.h"
#include "Engine/Structure/EditorUIComponent.h"

class Transform;
class Circle;
class Box;

enum class ShapeType
{
	BOX = 0,
	CIRCLE,
	COUNT
};

// For double dispatch pattern
class Shape : EditorUIComponent
{
public:

	virtual bool Collide(Shape* shape) = 0;
	virtual bool Collide(Circle* circle) = 0;
	virtual bool Collide(Box* box) = 0;

	virtual bool CollideResponse(Shape* shape) = 0;
	virtual bool CollideResponse(Circle* circle) = 0;
	virtual bool CollideResponse(Box* box) = 0;

	ShapeType GetShapeType();

	static string ShapeTypeToString(ShapeType type);

	virtual void CreateEngineUI() override;

	void SetIsTrigger(bool value);
	void SetIsCollidable(bool value);

	bool IsTrigger() const;
	bool IsCollidable() const;

	Transform* m_transform;

protected:
	ShapeType m_shapeType;

	bool m_isTrigger = false;
	bool m_isCollidable = true;
};