#pragma once
#include "Engine/GameObjects/CameraGameObject.h"


class GameCameraGameObject : public CameraGameObject
{
public:
    GameCameraGameObject(string identifier, CoolUUID uuid);
    GameCameraGameObject(const nlohmann::json& data, CoolUUID index);

    virtual ~GameCameraGameObject()override;

    virtual void Update()override;

    virtual void FollowGameObject(GameObject* objectToFollow);

    virtual void ResetFollowingObject();

private:

    GameObject* m_followingObject;
};

