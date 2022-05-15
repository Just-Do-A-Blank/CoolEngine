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

CollidableGameObject::CollidableGameObject(const nlohmann::json& data, CoolUUID uuid) : GameObject(data, uuid)
{
	m_gameObjectType |= GameObjectType::COLLIDABLE;

	if (data["ShapeType"] != -1)
	{
		if (data["ShapeType"] == "Circle")
		{
			m_pcollider = new Circle(data, m_transform);
		}
		else
		{
			m_pcollider = new Box(data, m_transform);
		}
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

//Due to diamond pattern code here needs to be updated in RenderableCollidable as well
void CollidableGameObject::Serialize(nlohmann::json& jsonData)
{
	GameObject::Serialize(jsonData);

	if (m_pcollider == nullptr)
	{
		jsonData["ShapeType"] = -1;
	}
	else
	{
		m_pcollider->Serialize(jsonData);
	}
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