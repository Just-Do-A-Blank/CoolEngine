#pragma once
#include "Scene.h"
#include "Engine\GameObjects\EditorCameraGameObject.h"

class EditorScene : public Scene
{
public:
    EditorScene(string identifier);
    virtual ~EditorScene();

    virtual void EditorUpdate() override;

    /// <summary>
    /// Describes where the scene is running
    /// </summary>
    virtual ESCENEENVIRONMENT GetCurrentEnvironment() override;

private:
    /// <summary>
    /// True means camera is setup
    /// </summary>
    bool m_haveSetCamera;

    EditorCameraGameObject* m_cameraObject;
};

