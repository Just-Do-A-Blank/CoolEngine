#include "GameCameraGameObject.h"

GameCameraGameObject::GameCameraGameObject(string identifier, CoolUUID uuid):CameraGameObject(identifier,uuid)
{

}

GameCameraGameObject::GameCameraGameObject(const nlohmann::json& data, CoolUUID index) : CameraGameObject(data, index)
{

}

GameCameraGameObject::~GameCameraGameObject()
{
    m_followingObject = nullptr;
}

void GameCameraGameObject::Update()
{
    if (m_followingObject == nullptr)
    {
        return;
    }

    Transform* objectTransform = m_followingObject->GetTransform();
    XMFLOAT3 objectWorldPosition = objectTransform->GetWorldPosition();
    GetTransform()->SetWorldPosition(objectWorldPosition);
}

void GameCameraGameObject::FollowGameObject(GameObject* objectToFollow)
{
    m_followingObject = objectToFollow;
}

void GameCameraGameObject::ResetFollowingObject()
{
    m_followingObject = nullptr;
}