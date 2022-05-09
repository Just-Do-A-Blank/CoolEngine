#pragma once
#include "Engine/Physics/Shape.h"
#include "Engine/Managers/DebugDrawManager.h"
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
		m_halfSize = { m_transform->GetScale().x * 50, m_transform->GetScale().y * 50 };

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

	// Based on gamedev.stackexchange.com/questions/20703/bounding-box-of-a-rotated-rectangle-2d
	void SetShapeDimensions(XMFLOAT3 scale)
	{
		m_halfSize = { m_transform->GetScale().x * 50.0f, m_transform->GetScale().y * 50.0f };

		XMVECTOR topLeft = XMVectorSet(-m_halfSize.x, m_halfSize.y, 0, 1);
		XMVECTOR topRight = XMVectorSet(m_halfSize.x, m_halfSize.y, 0, 1);
		XMVECTOR bottomLeft = XMVectorSet(-m_halfSize.x, -m_halfSize.y, 0, 1);
		XMVECTOR bottomRight = XMVectorSet(m_halfSize.x, -m_halfSize.y, 0, 1);

		XMMATRIX rot = m_transform->GetRotationMatrix();

		topLeft = XMVector3Transform(topLeft, rot);
		topRight = XMVector3Transform(topRight, rot);
		bottomLeft = XMVector3Transform(bottomLeft, rot);
		bottomRight = XMVector3Transform(bottomRight, rot);

		XMFLOAT4 tL, tR, bL, bR;
		XMStoreFloat4(&tL, topLeft);
		XMStoreFloat4(&tR, topRight);
		XMStoreFloat4(&bL, bottomLeft);
		XMStoreFloat4(&bR, bottomRight);

		float minX = min(tL.x, min(tR.x, min(bL.x, bR.x)));
		float maxX = max(tL.x, max(tR.x, max(bL.x, bR.x)));
		float minY = min(tL.y, min(tR.y, min(bL.y, bR.y)));
		float maxY = max(tL.y, max(tR.y, max(bL.y, bR.y)));

		m_halfSize = XMFLOAT2(maxX - minX, maxY - minY);
		m_halfSize.x /= 2.0f;
		m_halfSize.y /= 2.0f;
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