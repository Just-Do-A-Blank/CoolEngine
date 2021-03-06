#include "RenderableCollidableGameObject.h"

#include "Engine/Physics/Shape.h"
#include "Engine/Physics/Box.h"
#include "Engine/Physics/Circle.h"

#include "Engine/EditorUI/EditorUI.h"

RenderableCollidableGameObject::RenderableCollidableGameObject() : RenderableGameObject(), CollidableGameObject()
{

}

RenderableCollidableGameObject::RenderableCollidableGameObject(string identifier, CoolUUID uuid) : RenderableGameObject(identifier, uuid), CollidableGameObject(identifier, uuid)
{

}

RenderableCollidableGameObject::RenderableCollidableGameObject(const nlohmann::json& data, CoolUUID uuid) : CollidableGameObject(data, uuid), RenderableGameObject(data, uuid)
{
    if (PrefabGameObject::IsPrefab())
    {
        CollidableGameObject::LoadLocalData(PrefabGameObject::GetPrefabDataLoadedAtCreation());
    }
    else
    {
        CollidableGameObject::LoadLocalData(data);
    }
}

RenderableCollidableGameObject::RenderableCollidableGameObject(RenderableCollidableGameObject const& other) : RenderableGameObject(other), CollidableGameObject(other)
{
}

RenderableCollidableGameObject::~RenderableCollidableGameObject()
{
}

#if EDITOR
void RenderableCollidableGameObject::CreateEngineUI()
{
	RenderableGameObject::CreateEngineUI();

	if (EditorUI::CollapsingSection("Collidable", true))
	{
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
				m_pcollider = new Box(this);
			}
			else if (ImGui::Selectable(Shape::ShapeTypeToString(ShapeType::CIRCLE).c_str(), shapeType == ShapeType::CIRCLE))
			{
				delete m_pcollider;
				m_pcollider = new Circle(this, 1.0f);
			}

			ImGui::EndCombo();
		}

		if (m_pcollider != nullptr)
		{
			m_pcollider->CreateEngineUI();
		}
	}
}
#endif

void RenderableCollidableGameObject::Serialize(nlohmann::json& jsonData)
{
	RenderableGameObject::Serialize(jsonData);

	SaveLocalData(jsonData);
}

void RenderableCollidableGameObject::LoadAllPrefabData(const nlohmann::json& jsonData)
{
    RenderableGameObject::LoadAllPrefabData(jsonData);
    CollidableGameObject::LoadLocalData(jsonData);
}

void RenderableCollidableGameObject::SaveAllPrefabData(nlohmann::json& jsonData)
{
    SaveLocalData(jsonData);
    RenderableGameObject::SaveAllPrefabData(jsonData);
}

void RenderableCollidableGameObject::SaveLocalData(nlohmann::json& jsonData)
{
    if (m_pcollider == nullptr)
    {
        jsonData["ShapeType"] = -1;
    }
    else
    {
        m_pcollider->Serialize(jsonData);
    }
}