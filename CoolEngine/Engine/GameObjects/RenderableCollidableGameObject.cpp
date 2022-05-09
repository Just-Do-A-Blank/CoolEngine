#include "RenderableCollidableGameObject.h"

#include "Engine/Physics/Shape.h"
#include "Engine/Physics/Box.h"
#include "Engine/Physics/Circle.h"

#include "Engine/EditorUI/EditorUI.h"

RenderableCollidableGameObject::RenderableCollidableGameObject() : RenderableGameObject(), CollidableGameObject()
{

}

RenderableCollidableGameObject::RenderableCollidableGameObject(string identifier) : RenderableGameObject(identifier), CollidableGameObject(identifier)
{

}

#if EDITOR
void RenderableCollidableGameObject::CreateEngineUI()
{
	RenderableGameObject::CreateEngineUI();

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
void RenderableCollidableGameObject::Serialize(json& jsonData)
{
	//int gUID = ;
	std::string gUID = "GUID: ";

	std::string name = "Name: ";
	//name.append();

	float position[3] = { m_transform->GetPosition().x ,m_transform->GetPosition().y ,m_transform->GetPosition().z };
	float rotation[3] = { m_transform->GetRotation().x ,m_transform->GetRotation().y ,m_transform->GetRotation().z };
	float scale[3] = { m_transform->GetScale().x ,m_transform->GetScale().y ,m_transform->GetScale().z };

	jsonData["RenderableColliderableGameObject"]["GUID"].push_back(gUID);
	jsonData["RenderableColliderableGameObject"]["Name"].push_back(gUID);
	jsonData["RenderableColliderableGameObject"]["Position"].push_back(position);
	jsonData["RenderableColliderableGameObject"]["Rotation"].push_back(rotation);
	jsonData["RenderableColliderableGameObject"]["Scale"].push_back(scale);
	jsonData["RenderableColliderableGameObject"]["Layers"].push_back(m_layer);
	jsonData["RenderableColliderableGameObject"]["Shape"].push_back(m_pcollider->ShapeTypeToString(m_pcollider->GetShapeType()));

}
void RenderableCollidableGameObject::Deserialize(json& jsonData)
{
}
#endif