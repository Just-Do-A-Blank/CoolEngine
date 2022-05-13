#include "CollidableGameObject.h"

#include "Engine/Physics/Shape.h"
#include "Engine/Physics/Box.h"
#include "Engine/Physics/Circle.h"
#include "Engine/EditorUI/EditorUI.h"

CollidableGameObject::CollidableGameObject() : GameObject()
{
	m_gameObjectType |= GameObjectType::COLLIDABLE;
}

CollidableGameObject::CollidableGameObject(string identifier, CoolUUID uuid) : GameObject(identifier, uuid)
{
	m_gameObjectType |= GameObjectType::COLLIDABLE;
}

CollidableGameObject::CollidableGameObject(json data, CoolUUID index) : GameObject(data, index)
{
	json shape = data["Shape"];
	m_gameObjectType |= GameObjectType::COLLIDABLE;

	if (shape != "NULL")
	{
		if (shape == "Circle")
		{
			json radius = data["Radius"];
			m_pcollider = new Circle(m_transform, radius);
		}
		else
		{
			json height = data["Half Size"];
			XMFLOAT2 halfHeight = { height[0],   height[1] };
			m_pcollider = new Box(m_transform, halfHeight);
		}
	}
	else
	{
		LOG("No Collider")
	}

}

CollidableGameObject::~CollidableGameObject()
{
	delete m_pcollider;
}

#if EDITOR
void CollidableGameObject::CreateEngineUI()
{
	GameObject::CreateEngineUI();

	string currentSelected;

	if (m_pcollider != nullptr)
	{
		currentSelected = Shape::ShapeTypeToString(m_pcollider->GetShapeType());
	}
	else
	{
		currentSelected = Shape::ShapeTypeToString(ShapeType::COUNT);
	}

	ShapeType shapeType;

	if (m_pcollider == nullptr)
	{
		shapeType = ShapeType::COUNT;
	}
	else
	{
		shapeType = m_pcollider->GetShapeType();
	}

	if (IMGUI_LEFT_LABEL(ImGui::BeginCombo, "Collider", currentSelected.c_str()) == true)
	{
		if (ImGui::Selectable(Shape::ShapeTypeToString(ShapeType::COUNT).c_str(), shapeType == ShapeType::COUNT))
		{
			delete m_pcollider;
			m_pcollider = nullptr;
		}
		else if (ImGui::Selectable(Shape::ShapeTypeToString(ShapeType::BOX).c_str(), shapeType == ShapeType::BOX))
		{
			delete m_pcollider;
			m_pcollider = new Box(m_transform);
		}
		else if (ImGui::Selectable(Shape::ShapeTypeToString(ShapeType::CIRCLE).c_str(), shapeType == ShapeType::CIRCLE))
		{
			delete m_pcollider;
			m_pcollider = new Circle(m_transform, 1.0f);
		}

		ImGui::EndCombo();
	}

	if (m_pcollider != nullptr)
	{
		m_pcollider->CreateEngineUI();
	}
}
void CollidableGameObject::Serialize(json& jsonData)
{
	GameObject::Serialize(jsonData);
	if (m_pcollider == nullptr)
	{
		jsonData["Shape"].push_back("NULL");
	}
	else
	{
		jsonData["Shape"] = m_pcollider->ShapeTypeToString(m_pcollider->GetShapeType());
	}
	if (m_pcollider->ShapeTypeToString(m_pcollider->GetShapeType()) == "Circle")
	{
		jsonData["Radius"] = ((Circle*)m_pcollider)->m_radius;
		float outputData[2] = { 0.0 };
		jsonData["Half Size"] =outputData;
	}
	else
	{
		float halfHeight[2] = { ((Box*)m_pcollider)->m_halfSize.x, ((Box*)m_pcollider)->m_halfSize.y };
		jsonData["Half Size"] = halfHeight;
		jsonData["Radius"] = 0;
	}
}
void CollidableGameObject::Deserialize(json& jsonData)
{
}
#endif

Shape* CollidableGameObject::GetShape()
{
	return m_pcollider;
}

void CollidableGameObject::SetShape(Shape* collider)
{
	m_pcollider = collider;
}

void CollidableGameObject::SetShapeDimensions(XMFLOAT3 scale)
{
	if (m_pcollider != nullptr)
	{
		m_pcollider->SetShapeDimensions(scale);
	}
}