#include "Engine/Scene/EditorScene.h"
#include "Engine/Managers/GameManager.h"

EditorScene::EditorScene(string identifier):Scene(identifier)
{
    m_haveSetCamera = false;
}

EditorScene::~EditorScene()
{
    GameManager::GetInstance()->SetCamera(nullptr);
}

void EditorScene::EditorUpdate()
{
    Scene::EditorUpdate();
    if (!m_haveSetCamera)
    {
        /*m_cameraObject = GameManager::GetInstance()->
            GetGameObjectUsingIdentifier<EditorCameraGameObject>(string("Camera"));*/

        //Create camera
        XMFLOAT3 cameraPos = XMFLOAT3(0, 0, -5);
        XMFLOAT3 cameraForward = XMFLOAT3(0, 0, 1);
        XMFLOAT3 cameraUp = XMFLOAT3(0, 1, 0);

        float windowWidth = GraphicsManager::GetInstance()->GetWindowDimensions().x;
        float windowHeight = GraphicsManager::GetInstance()->GetWindowDimensions().y;

        float nearDepth = 0.01f;
        float farDepth = 1000.0f;

        CoolUUID gameCameraUUID;
        m_cameraObject = new EditorCameraGameObject(std::string("EditorCamera"), gameCameraUUID);
        m_cameraObject->Initialize(cameraPos, cameraForward, cameraUp, windowWidth, windowHeight, nearDepth, farDepth);

        GameManager::GetInstance()->SetCamera(m_cameraObject);

        m_haveSetCamera = true;
    }
    else
    {
        m_cameraObject->Update();
    }
    
}

/// <summary>
/// Describes where the scene is running
/// </summary>
ESCENEENVIRONMENT EditorScene::GetCurrentEnvironment()
{
    return ESCENEENVIRONMENT::Editor;
}