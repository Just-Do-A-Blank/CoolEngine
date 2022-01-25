#pragma once
#include "Engine/Physics/Collision.h"
#include "Engine/Structure/EditorUIComponent.h"

class Transform;
class Circle;
class Box;
class OBB;

enum class ShapeType
{
	BOX = 0,
	CIRCLE,
	OBB,
	COUNT
};

// For double dispatch pattern
class Shape : EditorUIComponent
{
public:

	virtual bool Collide(Shape* shape) = 0;
	virtual bool Collide(Circle* circle) = 0;
	virtual bool Collide(Box* box) = 0;
	virtual bool Collide(OBB* obb) = 0;

	virtual bool CollideResponse(Shape* shape) = 0;
	virtual bool CollideResponse(Circle* circle) = 0;
	virtual bool CollideResponse(Box* box) = 0;
	virtual bool CollideResponse(OBB* obb) = 0;

	virtual void Update() = 0;

	ShapeType GetShapeType();

	static string ShapeTypeToString(ShapeType type);

#if EDITOR
	virtual void CreateEngineUI() override;
#endif

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