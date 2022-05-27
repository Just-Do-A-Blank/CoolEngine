#include "LevelChangeGameObject.h"
#include "Engine/Managers/Events/EventManager.h"
#include "Engine/Managers/GameManager.h"
#include "Engine/EditorUI/EditorUI.h"

LevelChangeGameObject::LevelChangeGameObject() : TriggerableGameObject()
{
    m_gameObjectType |= GameObjectType::LEVEL_CHANGE;

    m_playerName = "Player";
}

LevelChangeGameObject::LevelChangeGameObject(string identifier, CoolUUID uuid) : TriggerableGameObject(identifier, uuid)
{
    m_gameObjectType |= GameObjectType::LEVEL_CHANGE;
    m_playerName = "Player";
}

LevelChangeGameObject::LevelChangeGameObject(const nlohmann::json& data, CoolUUID uuid) : TriggerableGameObject(data, uuid)
{
    m_gameObjectType |= GameObjectType::LEVEL_CHANGE;

    m_sceneName = data["LevelChangeName"];
    m_playerName = data["PlayerName"];
}

LevelChangeGameObject::LevelChangeGameObject(LevelChangeGameObject const& other) : TriggerableGameObject(other)
{
    m_sceneName = other.m_sceneName;
    m_playerName = other.m_playerName;
}

LevelChangeGameObject::~LevelChangeGameObject()
{
    
}

void LevelChangeGameObject::Serialize(nlohmann::json& jsonData)
{
    TriggerableGameObject::Serialize(jsonData);

    SaveLocalData(jsonData);
}

void LevelChangeGameObject::SaveLocalData(nlohmann::json& jsonData)
{
    jsonData["LevelChangeName"] = m_sceneName;
    jsonData["PlayerName"] = m_playerName;
}

void LevelChangeGameObject::LoadAllPrefabData(const nlohmann::json& jsonData)
{
    TriggerableGameObject::LoadAllPrefabData(jsonData);
}

void LevelChangeGameObject::SaveAllPrefabData(nlohmann::json& jsonData)
{
    TriggerableGameObject::SaveAllPrefabData(jsonData);
}

bool LevelChangeGameObject::IsEventCollision(Event* e)
{
    return false;
}

void LevelChangeGameObject::TriggerCollisionEvents(CollisionEvent* collisionEvent)
{
    switch (collisionEvent->GetEventID())
    {
    case EventType::TriggerHold:
        OnTriggerHold(collisionEvent->GetGameObject(0), collisionEvent->GetGameObject(1));
        break;
    case EventType::CollisionHold:
        OnCollisionHold(collisionEvent->GetGameObject(0), collisionEvent->GetGameObject(1));
        break;
    }
}

void LevelChangeGameObject::SetSceneName(string name)
{
    m_sceneName = name;
}

void LevelChangeGameObject::OnTriggerHold(GameObject* obj1, GameObject* obj2)
{
	if (obj1->ContainsType(GameObjectType::LEVEL_CHANGE) && obj2->ContainsType(GameObjectType::PLAYER) ||
		obj2->ContainsType(GameObjectType::LEVEL_CHANGE) && obj1->ContainsType(GameObjectType::PLAYER))
	{
		string sceneFilePath = GameManager::GetInstance()->GetWorkingDirectory() + "\\Resources\\Levels\\" + m_sceneName + ".json";
		if (!GameManager::GetInstance()->SwitchSceneUsingIdentifier(sceneFilePath))
		{
			GameManager::GetInstance()->LoadSceneFromFile(sceneFilePath, m_playerName, true);
		}
	}
}

#if EDITOR
void LevelChangeGameObject::CreateEngineUI()
{
    TriggerableGameObject::CreateEngineUI();

    if (EditorUI::CollapsingSection("Level Change"))
    {
        ImGui::Spacing();

        EditorUINonSpecificParameters parameters = EditorUINonSpecificParameters();
        parameters.m_tooltipText = "Name of the scene, not the full file path!";

        EditorUI::InputText("Scene Name", m_sceneName, parameters);
        ImGui::Spacing();
        EditorUI::InputText("Player Name", m_playerName, parameters);

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();
    }
}
#endif