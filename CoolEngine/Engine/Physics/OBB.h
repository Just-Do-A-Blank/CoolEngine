#pragma once
#include "Engine/Physics/Shape.h"
#include "Engine/Helpers/MathHelper.h"

class OBB : public Shape
{
public:
	/*XMFLOAT2 m_corners[4];
	XMFLOAT2 m_axes[2];
	float m_origins[2];*/
	XMFLOAT2 m_corners[4];
	XMFLOAT2 m_axes[2];

	OBB()
	{
		m_transform = nullptr;
		UpdateCorners(0.0f);

		m_shapeType = ShapeType::OBB;
	}

	OBB(Transform* trans)
	{
		m_transform = trans;
		UpdateCorners(m_transform->GetRotation().z);

		m_shapeType = ShapeType::OBB;
	}

	~OBB()
	{
		m_transform = nullptr;
	}

	float GetOverlapLength(XMFLOAT2 vec1, XMFLOAT2 vec2)
	{
		if (vec1.x <= vec2.y && vec1.y >= vec2.x)
		{
			float yMin = (vec1.y < vec2.y ? vec1.y : vec2.y);
			float xMax = (vec1.x < vec2.x ? vec2.x : vec1.x);
			return yMin - xMax;
		}

		return 0;
	}

	XMFLOAT2 ProjectOnAxis(XMFLOAT2 axis)
	{
		float limit = 99999;
		float pMin = limit;
		float pMax = -limit;
		for (unsigned int i = 0; i < 4; ++i)
		{
			float p = MathHelper::DotProduct(m_corners[i], axis);
			pMin = (p < pMin ? p : pMin);
			pMax = (p > pMax ? p : pMax);
		}

		return XMFLOAT2(pMin, pMax);
	}

	void UpdateCorners(float rot)
	{
		XMFLOAT2 xAxis = XMFLOAT2(cos(rot), sin(rot));
		XMFLOAT2 yAxis = XMFLOAT2(-sin(rot), cos(rot));

		// Change this once you implement actual widths/heights!
		float halfW = m_transform->GetScale().x;
		float halfH = m_transform->GetScale().y;
		xAxis = MathHelper::Multiply(xAxis, halfW);
		yAxis = MathHelper::Multiply(yAxis, halfH);

		XMFLOAT2 middle = XMFLOAT2(m_transform->GetPosition().x, m_transform->GetPosition().y);
		m_corners[0] = XMFLOAT2(middle.x - xAxis.x - yAxis.x, middle.y - xAxis.y - yAxis.y);
		m_corners[1] = XMFLOAT2(middle.x + xAxis.x - yAxis.x, middle.y + xAxis.y - yAxis.y);
		m_corners[2] = XMFLOAT2(middle.x + xAxis.x + yAxis.x, middle.y + xAxis.y + yAxis.y);
		m_corners[3] = XMFLOAT2(middle.x - xAxis.x + yAxis.x, middle.y - xAxis.y + yAxis.y);
	}

	void UpdateAxes()
	{
		XMFLOAT2 normalised = MathHelper::Normalize(MathHelper::Minus(m_corners[1], m_corners[0]));
		m_axes[0] = { -normalised.y, normalised.x };

		normalised = MathHelper::Normalize(MathHelper::Minus(m_corners[2], m_corners[1]));
		m_axes[1] = { -normalised.y, normalised.x };

		//m_axes[0] = XMFLOAT2(m_corners[1].x - m_corners[0].x, m_corners[1].y - m_corners[0].y);
		//m_axes[1] = XMFLOAT2(m_corners[3].x - m_corners[0].x, m_corners[3].y - m_corners[0].y);

		//// Normalise, then get dot product to get
		//float magSquared;
		//for (unsigned int i = 0; i < 2; ++i)
		//{
		//	magSquared = MathHelper::SquareMagnitude(m_axes[i]);
		//	m_axes[i] = MathHelper::Divide(m_axes[i], magSquared);
		//	m_origins[i] = MathHelper::DotProduct(m_corners[0], m_axes[i]);
		//}
	}

	void Update()
	{
		UpdateCorners(m_transform->GetRotation().z);
		UpdateAxes();
	}

	bool Collide(Shape* shape)
	{
		return shape->Collide(this);
	}

	bool Collide(Circle* circle)
	{
		// Incomplete - OBB and Circle
		return false;
	}

	bool Collide(Box* box)
	{
		// Incomplete - OBB and Box
		return false;
	}

	bool Collide(OBB* obb)
	{
		return Collision::OBBCollision(this, obb);
	}

	bool CollideResponse(Shape* shape)
	{
		return shape->CollideResponse(this);
	}

	bool CollideResponse(Circle* circle)
	{
		// Incomplete - OBB and Circle
		return false;
	}

	bool CollideResponse(Box* box)
	{
		// Incomplete - OBB and Box
		return false;
	}

	bool CollideResponse(OBB* obb)
	{
		// Incomplete - OBB and OBB
		return false;
	}

#if EDITOR
	void CreateEngineUI() override
	{
		Shape::CreateEngineUI();
	}
#endif
};