#include "CollidableGameObject.h"

#include "Engine/Physics/Shape.h"
#include "Engine/Physics/Box.h"
#include "Engine/Physics/Circle.h"
#include "Engine/EditorUI/EditorUI.h"

CollidableGameObject::CollidableGameObject() : PrefabGameObject()
{
	m_gameObjectType |= GameObjectType::COLLIDABLE;
}

CollidableGameObject::CollidableGameObject(string identifier, CoolUUID uuid) : PrefabGameObject(identifier, uuid)
{
	m_gameObjectType |= GameObjectType::COLLIDABLE;
}

CollidableGameObject::CollidableGameObject(const nlohmann::json& data, CoolUUID uuid) : PrefabGameObject(data, uuid)
{
	m_gameObjectType |= GameObjectType::COLLIDABLE;

    if (IsPrefab())
    {
        LoadLocalData(GetPrefabDataLoadedAtCreation());
    }
    else
    {
        LoadLocalData(data);
    }
}

CollidableGameObject::CollidableGameObject(CollidableGameObject const& other) : PrefabGameObject(other)
{
	if (other.m_pcollider)
	{
		switch (other.m_pcollider->GetShapeType())
		{
		case ShapeType::BOX:
			m_pcollider = new Box(this);
			break;

		case ShapeType::CIRCLE:
			m_pcollider = new Circle(this);
			break;
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
    PrefabGameObject::CreateEngineUI();

	if (EditorUI::CollapsingSection("Collider", true))
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

//Due to diamond pattern code here needs to be updated in RenderableCollidable as well
void CollidableGameObject::Serialize(nlohmann::json& jsonData)
{
    GameObject::Serialize(jsonData);

    SaveAllPrefabData(jsonData);
}
#endif

void CollidableGameObject::LoadAllPrefabData(const nlohmann::json& jsonData)
{
    PrefabGameObject::LoadAllPrefabData(jsonData);
    LoadLocalData(jsonData);
}

void CollidableGameObject::SaveAllPrefabData(nlohmann::json& jsonData)
{
    SaveLocalData(jsonData);
    PrefabGameObject::SaveAllPrefabData(jsonData);
}

void CollidableGameObject::LoadLocalData(const nlohmann::json& jsonData)
{
    if (jsonData["ShapeType"] != -1)
    {
        if (jsonData["ShapeType"] == "Circle")
        {
            m_pcollider = new Circle(jsonData, this);
        }
        else
        {
            m_pcollider = new Box(jsonData, this);
        }
    }
}

void CollidableGameObject::SaveLocalData(nlohmann::json& jsonData)
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