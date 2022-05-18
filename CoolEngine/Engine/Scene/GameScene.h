#pragma once
#include "Scene.h"
#include "Engine\GameObjects\GameCameraGameObject.h"

class GameScene :public Scene
{
public:
    GameScene(string identifier);
    virtual ~GameScene();

    virtual void Update() override;

    /// <summary>
    /// Describes where the scene is running
    /// </summary>
    virtual ESCENEENVIRONMENT GetCurrentEnvironment() override;

private:
    /// <summary>
    /// True means camera is setup
    /// </summary>
    bool m_haveSetCamera;

    /// <summary>
    /// Camera object for the scene
    /// </summary>
    GameCameraGameObject* m_cameraObject;
};

