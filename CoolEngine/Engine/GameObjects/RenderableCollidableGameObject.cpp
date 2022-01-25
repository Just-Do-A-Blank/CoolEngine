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
#endif