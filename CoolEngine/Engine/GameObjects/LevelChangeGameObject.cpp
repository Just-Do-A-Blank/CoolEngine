#include "LevelChangeGameObject.h"
#include "Engine/Managers/Events/EventManager.h"
#include "Engine/Managers/GameManager.h"
#include "Engine/EditorUI/EditorUI.h"

LevelChangeGameObject::LevelChangeGameObject() : TriggerableGameObject()
{
    m_gameObjectType |= GameObjectType::LEVEL_CHANGE;
}

LevelChangeGameObject::LevelChangeGameObject(string identifier, CoolUUID uuid) : TriggerableGameObject(identifier, uuid)
{
    m_gameObjectType |= GameObjectType::LEVEL_CHANGE;
}

LevelChangeGameObject::LevelChangeGameObject(const nlohmann::json& data, CoolUUID uuid) : TriggerableGameObject(data, uuid)
{
    m_gameObjectType |= GameObjectType::LEVEL_CHANGE;
}

LevelChangeGameObject::LevelChangeGameObject(LevelChangeGameObject const& other) : TriggerableGameObject(other)
{
    m_gameObjectType |= GameObjectType::LEVEL_CHANGE;
}

LevelChangeGameObject::~LevelChangeGameObject()
{
    
}

void LevelChangeGameObject::Serialize(nlohmann::json& jsonData)
{
    RenderableCollidableGameObject::Serialize(jsonData);
}

void LevelChangeGameObject::LoadAllPrefabData(const nlohmann::json& jsonData)
{
    RenderableCollidableGameObject::LoadAllPrefabData(jsonData);
}

void LevelChangeGameObject::SaveAllPrefabData(nlohmann::json& jsonData)
{
    RenderableCollidableGameObject::SaveAllPrefabData(jsonData);
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
    if (obj1->ContainsType(GameObjectType::PLAYER))
    {
        string testSceneFilePath = GameManager::GetInstance()->GetWorkingDirectory() + "\\Resources\\Levels\\" + m_sceneName + ".json";
        if (!GameManager::GetInstance()->LoadSceneFromFile(testSceneFilePath))
        {
            GameManager* pgameManager = GameManager::GetInstance();
            pgameManager->CreateScene("EmptyScene");
            pgameManager->SwitchSceneUsingIdentifier("EmptyScene");
            pgameManager->CreateGameObject<CameraGameObject>("SceneCamera");
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
        ImGui::Separator();
        ImGui::Spacing();

        m_panimationStateMachine->CreateEngineUI();
    }
}
#endif