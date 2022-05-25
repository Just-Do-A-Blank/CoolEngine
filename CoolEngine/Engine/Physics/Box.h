#pragma once
#include "Engine/Physics/Shape.h"
#include "Engine/Includes/IMGUI/imgui.h"
#include "Engine/Includes/json.hpp"
#include "Engine/EditorUI/EditorUI.h"


class Box : public Shape
{
public:
	XMFLOAT2 m_halfSize;

	Box()
	{
		m_pgameObject = nullptr;
		m_halfSize = XMFLOAT2(0, 0);

		m_shapeType = ShapeType::BOX;
	}

	Box(GameObject* gameObject)
	{
		m_pgameObject = gameObject;
		m_halfSize = XMFLOAT2(m_scale.x * m_pgameObject->GetTransform()->GetWorldScale().x, m_scale.y * m_pgameObject->GetTransform()->GetWorldScale().y);

		m_shapeType = ShapeType::BOX;
	}

	Box(GameObject* gameObject, XMFLOAT2 size)
	{
		m_pgameObject = gameObject;
		m_halfSize = size;

		m_shapeType = ShapeType::BOX;
	}

	Box(const nlohmann::json& data, GameObject* gameObject) : Shape(data)
	{
		m_pgameObject = gameObject;
		m_halfSize = XMFLOAT2(m_scale.x * m_pgameObject->GetTransform()->GetWorldScale().x, m_scale.y * m_pgameObject->GetTransform()->GetWorldScale().y);

		m_scale = XMFLOAT2(data["BoxScale"][0], data["BoxScale"][1]);

		m_shapeType = ShapeType::BOX;
	}

	Box(Box const* other, GameObject* pgameobject) : Shape(other, pgameobject)
	{
		m_halfSize = other->m_halfSize;
	}

	~Box()
	{
		m_pgameObject = nullptr;
	}

	XMFLOAT2 GetHalfSize()
	{
		return m_halfSize;
	}

	void Serialize(nlohmann::json& data) override
	{
		Shape::Serialize(data);

		data["BoxScale"] = { m_scale.x, m_scale.y };
	}

	// Based on gamedev.stackexchange.com/questions/20703/bounding-box-of-a-rotated-rectangle-2d
	void UpdateShapeDimensions()
	{
		m_halfSize = XMFLOAT2(m_pgameObject->GetTransform()->GetWorldScale().x * m_scale.x, m_pgameObject->GetTransform()->GetWorldScale().y * m_scale.y);

		XMVECTOR topLeft = XMVectorSet(-m_halfSize.x, m_halfSize.y, 0, 1);
		XMVECTOR topRight = XMVectorSet(m_halfSize.x, m_halfSize.y, 0, 1);
		XMVECTOR bottomLeft = XMVectorSet(-m_halfSize.x, -m_halfSize.y, 0, 1);
		XMVECTOR bottomRight = XMVectorSet(m_halfSize.x, -m_halfSize.y, 0, 1);

		XMMATRIX rot = m_pgameObject->GetTransform()->GetRotationMatrix();

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

	XMFLOAT2 GetShapeDimensions()
	{
		return XMFLOAT2(m_scale.x * m_halfSize.x, m_scale.y * m_halfSize.y);
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

		ImGui::Spacing();

        auto boxParameters = EditorUIFloatParameters();
        boxParameters.m_speed = 0.01f;
        boxParameters.m_minValue = 0;
        boxParameters.m_maxValue = 100;

		EditorUI::DragFloat2("Dimensions", m_scale, boxParameters);
	}
#endif
};