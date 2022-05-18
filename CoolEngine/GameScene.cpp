#include "Engine/Scene/GameScene.h"
#include "Engine/Managers/GameManager.h"
#include "Engine\GameObjects\GameCameraGameObject.h"

GameScene::GameScene(string identifier):Scene(identifier)
{
    m_haveSetCamera = false;
}

GameScene::~GameScene()
{
    GameManager::GetInstance()->SetCamera(nullptr);
    delete m_cameraObject;
}

void GameScene::Update()
{
    Scene::Update();
    if (!m_haveSetCamera)
    {
        GameManager* m = GameManager::GetInstance();

        GameObject* player =
            m->GetGameObjectUsingIdentifier<GameObject>(string("Player"));

        //Create camera
        XMFLOAT3 cameraPos = XMFLOAT3(0, 0, -5);
        XMFLOAT3 cameraForward = XMFLOAT3(0, 0, 1);
        XMFLOAT3 cameraUp = XMFLOAT3(0, 1, 0);

        float windowWidth = GraphicsManager::GetInstance()->GetWindowDimensions().x;
        float windowHeight = GraphicsManager::GetInstance()->GetWindowDimensions().y;

        float nearDepth = 0.01f;
        float farDepth = 1000.0f;

        CoolUUID gameCameraUUID;
        m_cameraObject = new GameCameraGameObject(std::string("GameCamera"), gameCameraUUID);
        m_cameraObject->Initialize(cameraPos, cameraForward, cameraUp, windowWidth, windowHeight, nearDepth, farDepth);

        GameManager::GetInstance()->SetCamera(m_cameraObject);

        m_haveSetCamera = true;
    }

    m_cameraObject->Update();
}

/// <summary>
/// Describes where the scene is running
/// </summary>
ESCENEENVIRONMENT GameScene::GetCurrentEnvironment()
{
    return ESCENEENVIRONMENT::Game;
}