#include "GameManager.h"

#include "Engine/GameObjects/GameObject.h"
#include "Engine/GameObjects/WeaponGameObject.h"
#include "Engine/GameObjects/RangedWeaponGameObject.h"
#include "Engine/GameObjects/MeleeWeaponGameObject.h"
#include "Engine/GameObjects/InteractableGameObject.h"
#include "Engine/Scene/Scene.h"
#include "SceneGraph.h"
#include "GraphicsManager.h"
#include "Engine/GameUI/GameUIComponent.h"
#include "Engine/GameUI/ButtonComponent.h"
#include "Engine/GameUI/ImageComponent.h"
#include "Engine/GameUI/UiCanvas.h"
#include "Engine/GameUI/TextComponent.h"
#include "Engine/GameObjects/EditorCameraGameObject.h"
#include "Engine/TileMap/TileMap/TileMap.h"
#include "Engine/Managers/Events/BulletCreator.h"
#include "Engine/GameObjects/LevelChangeGameObject.h"

#include <fstream>


#include <direct.h>

void GameManager::Init()
{
	char buffer[FILEPATH_BUFFER_SIZE];
	_getcwd(buffer, FILEPATH_BUFFER_SIZE);

	m_workingDirectory = string(buffer);

	m_wideWorkingDirectory = wstring(m_workingDirectory.begin(), m_workingDirectory.end());

	CoolUUID uuid;
	m_peditorCamera = new EditorCameraGameObject(std::string("EditorCamera"), uuid);
	m_pbulletCreator = new BulletCreator();
}

Timer* GameManager::GetTimer()
{
	return &m_timer;
}

void GameManager::Start()
{
    switch (m_viewState)
    {
    case ViewState::EDITOR_VIEW:
        if (!m_pcurrentEditorScene)
        {
            return;
        }

        m_pcurrentEditorScene->Start();
        break;

    case ViewState::GAME_VIEW:
        m_pcurrentGameScene->Start();
        break;
    }
}

void GameManager::Update()
{
	switch (m_viewState)
	{
	case ViewState::EDITOR_VIEW:
		if (!m_pcurrentEditorScene)
		{
			return;
		}

		m_peditorCamera->EditorUpdate();
		m_pcurrentEditorScene->EditorUpdate();
		break;

	case ViewState::GAME_VIEW:
		m_pcurrentGameScene->Update();
		m_pbulletCreator->Update();
		break;
	}

}

void GameManager::Render(RenderStruct& renderStruct)
{
	switch (m_viewState)
	{
	case ViewState::EDITOR_VIEW:
		if (!m_pcurrentEditorScene)
		{
			return;
		}

		m_pcurrentEditorScene->Render(renderStruct);
		break;

	case ViewState::GAME_VIEW:
		m_pcurrentGameScene->Render(renderStruct);
		m_pbulletCreator->Render(renderStruct);
		break;
	}
}

void GameManager::DeleteSceneUsingIdentifier(string sceneIdentifier)
{
	m_editorSceneMap.erase(sceneIdentifier);
}

void GameManager::DeleteSelectedScene()
{
	Scene*& pcurrentScene = GetCurrentViewStateScene();
	pcurrentScene->m_psceneGraph->DeleteAllGameObjects();
	m_pbulletCreator->DeleteBullets();

	unordered_map<string, Scene*>& sceneMap = GetCurrentViewStateSceneMap();
	sceneMap.erase(pcurrentScene->GetSceneIdentifier());

	delete pcurrentScene;
	pcurrentScene = nullptr;
	for (unordered_map<string, Scene*>::iterator it = sceneMap.begin(); it != sceneMap.end(); ++it)
	{
		pcurrentScene = it->second;
		return;
	}
}

void GameManager::SwitchAndDeleteScene(string sceneIdentifier)
{
	switch (m_viewState)
	{
	case ViewState::EDITOR_VIEW:
		m_pcurrentEditorScene->m_psceneGraph->DeleteAllGameObjects();
		m_pbulletCreator->DeleteBullets();

		m_editorSceneMap.erase(m_pcurrentEditorScene->GetSceneIdentifier());
		for (unordered_map<string, Scene*>::iterator it = m_editorSceneMap.begin(); it != m_editorSceneMap.end(); ++it)
		{
			m_pcurrentEditorScene = it->second;
			return;
		}
		m_pcurrentEditorScene = nullptr;
		break;

	case ViewState::GAME_VIEW:
		break;
	}
}

bool GameManager::BeginPlay()
{
	if (!m_pcurrentEditorScene->GetActiveCamera())
	{
		return false;
	}

	m_pcurrentGameScene = new Scene(m_pcurrentEditorScene->m_sceneIdentifier);

	CopyScene();

	//m_pcurrentGameScene->InitializeQuadTree();

	m_pcurrentGameScene->SetActiveCameraUsingIdentifier(m_pcurrentEditorScene->GetActiveCamera()->GetIdentifier());
	m_viewState = ViewState::GAME_VIEW;

	GetCurrentViewStateSceneMap().insert(pair<string, Scene*>(m_pcurrentEditorScene->m_sceneIdentifier, m_pcurrentGameScene));

	Start();

	return true;
}

bool GameManager::EndPlay()
{
	if (m_pcurrentGameScene)
	{
		for (unordered_map<string, Scene*>::iterator it = m_gameSceneMap.begin(); it != m_gameSceneMap.end(); ++it)
		{
			it->second->m_psceneGraph->DeleteAllGameObjects();
			delete it->second;
			it->second = nullptr;
			m_pbulletCreator->DeleteBullets();
		}
		m_gameSceneMap.clear();
		m_pcurrentGameScene = nullptr;

		m_viewState = ViewState::EDITOR_VIEW;

		return true;
	}
	return false;
}

void GameManager::CopyScene()
{
	vector<TreeNode<GameObject>*> gameObjectNodeList = m_pcurrentEditorScene->GetSceneGraph()->GetAllNodes();
	for (int it = 0; it < gameObjectNodeList.size(); ++it)
	{
		switch ((AccumlateType)gameObjectNodeList[it]->NodeObject->m_gameObjectType)
		{
		case AccumlateType::BASE:
			if (gameObjectNodeList[it]->PreviousParent)
			{
				TreeNode<GameObject>* parentNode = m_pcurrentGameScene->GetTreeNode(gameObjectNodeList[it]->PreviousParent->NodeObject);
				m_pcurrentGameScene->CopyGameObject<GameObject>(*(dynamic_cast<GameObject*>(gameObjectNodeList[it]->NodeObject)), parentNode);
			}
			else if (gameObjectNodeList[it]->PreviousSibling)
			{
				TreeNode<GameObject>* previousSiblingNode = m_pcurrentGameScene->GetTreeNode(gameObjectNodeList[it]->PreviousSibling->NodeObject);
				m_pcurrentGameScene->CopyGameObject<GameObject>(*(dynamic_cast<GameObject*>(gameObjectNodeList[it]->NodeObject)), nullptr, previousSiblingNode);
			}
			else
			{
				m_pcurrentGameScene->CopyGameObject<GameObject>(*(dynamic_cast<GameObject*>(gameObjectNodeList[it]->NodeObject)));
			}
			break;

		case AccumlateType::RENDERABLE:
			if (gameObjectNodeList[it]->PreviousParent)
			{
				TreeNode<GameObject>* parentNode = m_pcurrentGameScene->GetTreeNode(gameObjectNodeList[it]->PreviousParent->NodeObject);
				m_pcurrentGameScene->CopyGameObject<RenderableGameObject>(*(dynamic_cast<RenderableGameObject*>(gameObjectNodeList[it]->NodeObject)), parentNode);
			}
			else if (gameObjectNodeList[it]->PreviousSibling)
			{
				TreeNode<GameObject>* previousSiblingNode = m_pcurrentGameScene->GetTreeNode(gameObjectNodeList[it]->PreviousSibling->NodeObject);
				m_pcurrentGameScene->CopyGameObject<RenderableGameObject>(*(dynamic_cast<RenderableGameObject*>(gameObjectNodeList[it]->NodeObject)), nullptr, previousSiblingNode);
			}
			else
			{
				m_pcurrentGameScene->CopyGameObject<RenderableGameObject>(*(dynamic_cast<RenderableGameObject*>(gameObjectNodeList[it]->NodeObject)));
			}
			break;

		case AccumlateType::COLLIDABLE:
			if (gameObjectNodeList[it]->PreviousParent)
			{
				TreeNode<GameObject>* parentNode = m_pcurrentGameScene->GetTreeNode(gameObjectNodeList[it]->PreviousParent->NodeObject);
				m_pcurrentGameScene->CopyGameObject<CollidableGameObject>(*(dynamic_cast<CollidableGameObject*>(gameObjectNodeList[it]->NodeObject)), parentNode);
			}
			else if (gameObjectNodeList[it]->PreviousSibling)
			{
				TreeNode<GameObject>* previousSiblingNode = m_pcurrentGameScene->GetTreeNode(gameObjectNodeList[it]->PreviousSibling->NodeObject);
				m_pcurrentGameScene->CopyGameObject<CollidableGameObject>(*(dynamic_cast<CollidableGameObject*>(gameObjectNodeList[it]->NodeObject)), nullptr, previousSiblingNode);
			}
			else
			{
				m_pcurrentGameScene->CopyGameObject<CollidableGameObject>(*(dynamic_cast<CollidableGameObject*>(gameObjectNodeList[it]->NodeObject)));
			}
			break;

		case AccumlateType::COLLIDABLE_RENDERERABLE:
			if (gameObjectNodeList[it]->PreviousParent)
			{
				TreeNode<GameObject>* parentNode = m_pcurrentGameScene->GetTreeNode(gameObjectNodeList[it]->PreviousParent->NodeObject);
				m_pcurrentGameScene->CopyGameObject<RenderableCollidableGameObject>(*(dynamic_cast<RenderableCollidableGameObject*>(gameObjectNodeList[it]->NodeObject)), parentNode);
			}
			else if (gameObjectNodeList[it]->PreviousSibling)
			{
				TreeNode<GameObject>* previousSiblingNode = m_pcurrentGameScene->GetTreeNode(gameObjectNodeList[it]->PreviousSibling->NodeObject);
				m_pcurrentGameScene->CopyGameObject<RenderableCollidableGameObject>(*(dynamic_cast<RenderableCollidableGameObject*>(gameObjectNodeList[it]->NodeObject)), nullptr, previousSiblingNode);
			}
			else
			{
				m_pcurrentGameScene->CopyGameObject<RenderableCollidableGameObject>(*(dynamic_cast<RenderableCollidableGameObject*>(gameObjectNodeList[it]->NodeObject)));
			}
			break;

		case AccumlateType::CHARACTER:
			if (gameObjectNodeList[it]->PreviousParent)
			{
				TreeNode<GameObject>* parentNode = m_pcurrentGameScene->GetTreeNode(gameObjectNodeList[it]->PreviousParent->NodeObject);
				m_pcurrentGameScene->CopyGameObject<CharacterGameObject>(*(dynamic_cast<CharacterGameObject*>(gameObjectNodeList[it]->NodeObject)), parentNode);
			}
			else if (gameObjectNodeList[it]->PreviousSibling)
			{
				TreeNode<GameObject>* previousSiblingNode = m_pcurrentGameScene->GetTreeNode(gameObjectNodeList[it]->PreviousSibling->NodeObject);
				m_pcurrentGameScene->CopyGameObject<CharacterGameObject>(*(dynamic_cast<CharacterGameObject*>(gameObjectNodeList[it]->NodeObject)), nullptr, previousSiblingNode);
			}
			else
			{
				m_pcurrentGameScene->CopyGameObject<CharacterGameObject>(*(dynamic_cast<CharacterGameObject*>(gameObjectNodeList[it]->NodeObject)));
			}
			break;

		case AccumlateType::ENEMY:
			if (gameObjectNodeList[it]->PreviousParent)
			{
				TreeNode<GameObject>* parentNode = m_pcurrentGameScene->GetTreeNode(gameObjectNodeList[it]->PreviousParent->NodeObject);
				m_pcurrentGameScene->CopyGameObject<EnemyGameObject>(*(dynamic_cast<EnemyGameObject*>(gameObjectNodeList[it]->NodeObject)), parentNode);
			}
			else if (gameObjectNodeList[it]->PreviousSibling)
			{
				TreeNode<GameObject>* previousSiblingNode = m_pcurrentGameScene->GetTreeNode(gameObjectNodeList[it]->PreviousSibling->NodeObject);
				m_pcurrentGameScene->CopyGameObject<EnemyGameObject>(*(dynamic_cast<EnemyGameObject*>(gameObjectNodeList[it]->NodeObject)), nullptr, previousSiblingNode);
			}
			else
			{
				m_pcurrentGameScene->CopyGameObject<EnemyGameObject>(*(dynamic_cast<EnemyGameObject*>(gameObjectNodeList[it]->NodeObject)));
			}
			break;

		case AccumlateType::PLAYER:
			if (gameObjectNodeList[it]->PreviousParent)
			{
				TreeNode<GameObject>* parentNode = m_pcurrentGameScene->GetTreeNode(gameObjectNodeList[it]->PreviousParent->NodeObject);
				m_pcurrentGameScene->CopyGameObject<PlayerGameObject>(*(dynamic_cast<PlayerGameObject*>(gameObjectNodeList[it]->NodeObject)), parentNode);
			}
			else if (gameObjectNodeList[it]->PreviousSibling)
			{
				TreeNode<GameObject>* previousSiblingNode = m_pcurrentGameScene->GetTreeNode(gameObjectNodeList[it]->PreviousSibling->NodeObject);
				m_pcurrentGameScene->CopyGameObject<PlayerGameObject>(*(dynamic_cast<PlayerGameObject*>(gameObjectNodeList[it]->NodeObject)), nullptr, previousSiblingNode);
			}
			else
			{
				m_pcurrentGameScene->CopyGameObject<PlayerGameObject>(*(dynamic_cast<PlayerGameObject*>(gameObjectNodeList[it]->NodeObject)));
			}
			break;

		case AccumlateType::PARTICLE_SYSTEM:
			if (gameObjectNodeList[it]->PreviousParent)
			{
				TreeNode<GameObject>* parentNode = m_pcurrentGameScene->GetTreeNode(gameObjectNodeList[it]->PreviousParent->NodeObject);
				m_pcurrentGameScene->CopyGameObject<ParticleSystem>(*(dynamic_cast<ParticleSystem*>(gameObjectNodeList[it]->NodeObject)), parentNode);
			}
			else if (gameObjectNodeList[it]->PreviousSibling)
			{
				TreeNode<GameObject>* previousSiblingNode = m_pcurrentGameScene->GetTreeNode(gameObjectNodeList[it]->PreviousSibling->NodeObject);
				m_pcurrentGameScene->CopyGameObject<ParticleSystem>(*(dynamic_cast<ParticleSystem*>(gameObjectNodeList[it]->NodeObject)), nullptr, previousSiblingNode);
			}
			else
			{
				m_pcurrentGameScene->CopyGameObject<ParticleSystem>(*(dynamic_cast<ParticleSystem*>(gameObjectNodeList[it]->NodeObject)));
			}
			break;

		case AccumlateType::INTERACTABLE:
			if (gameObjectNodeList[it]->PreviousParent)
			{
				TreeNode<GameObject>* parentNode = m_pcurrentGameScene->GetTreeNode(gameObjectNodeList[it]->PreviousParent->NodeObject);
				m_pcurrentGameScene->CopyGameObject<InteractableGameObject>(*(dynamic_cast<InteractableGameObject*>(gameObjectNodeList[it]->NodeObject)), parentNode);
			}
			else if (gameObjectNodeList[it]->PreviousSibling)
			{
				TreeNode<GameObject>* previousSiblingNode = m_pcurrentGameScene->GetTreeNode(gameObjectNodeList[it]->PreviousSibling->NodeObject);
				m_pcurrentGameScene->CopyGameObject<InteractableGameObject>(*(dynamic_cast<InteractableGameObject*>(gameObjectNodeList[it]->NodeObject)), nullptr, previousSiblingNode);
			}
			else
			{
				m_pcurrentGameScene->CopyGameObject<InteractableGameObject>(*(dynamic_cast<InteractableGameObject*>(gameObjectNodeList[it]->NodeObject)));
			}
			break;

		case AccumlateType::WEAPON:
			if (gameObjectNodeList[it]->PreviousParent)
			{
				TreeNode<GameObject>* parentNode = m_pcurrentGameScene->GetTreeNode(gameObjectNodeList[it]->PreviousParent->NodeObject);
				m_pcurrentGameScene->CopyGameObject<WeaponGameObject>(*(dynamic_cast<WeaponGameObject*>(gameObjectNodeList[it]->NodeObject)), parentNode);
			}
			else if (gameObjectNodeList[it]->PreviousSibling)
			{
				TreeNode<GameObject>* previousSiblingNode = m_pcurrentGameScene->GetTreeNode(gameObjectNodeList[it]->PreviousSibling->NodeObject);
				m_pcurrentGameScene->CopyGameObject<WeaponGameObject>(*(dynamic_cast<WeaponGameObject*>(gameObjectNodeList[it]->NodeObject)), nullptr, previousSiblingNode);
			}
			else
			{
				m_pcurrentGameScene->CopyGameObject<WeaponGameObject>(*(dynamic_cast<WeaponGameObject*>(gameObjectNodeList[it]->NodeObject)));
			}
			break;

		case AccumlateType::MELEE_WEAPON:
			if (gameObjectNodeList[it]->PreviousParent)
			{
				TreeNode<GameObject>* parentNode = m_pcurrentGameScene->GetTreeNode(gameObjectNodeList[it]->PreviousParent->NodeObject);
				m_pcurrentGameScene->CopyGameObject<MeleeWeaponGameObject>(*(dynamic_cast<MeleeWeaponGameObject*>(gameObjectNodeList[it]->NodeObject)), parentNode);
			}
			else if (gameObjectNodeList[it]->PreviousSibling)
			{
				TreeNode<GameObject>* previousSiblingNode = m_pcurrentGameScene->GetTreeNode(gameObjectNodeList[it]->PreviousSibling->NodeObject);
				m_pcurrentGameScene->CopyGameObject<MeleeWeaponGameObject>(*(dynamic_cast<MeleeWeaponGameObject*>(gameObjectNodeList[it]->NodeObject)), nullptr, previousSiblingNode);
			}
			else
			{
				m_pcurrentGameScene->CopyGameObject<MeleeWeaponGameObject>(*(dynamic_cast<MeleeWeaponGameObject*>(gameObjectNodeList[it]->NodeObject)));
			}
			break;

		case AccumlateType::RANGE_WEAPON:
			if (gameObjectNodeList[it]->PreviousParent)
			{
				TreeNode<GameObject>* parentNode = m_pcurrentGameScene->GetTreeNode(gameObjectNodeList[it]->PreviousParent->NodeObject);
				m_pcurrentGameScene->CopyGameObject<RangedWeaponGameObject>(*(dynamic_cast<RangedWeaponGameObject*>(gameObjectNodeList[it]->NodeObject)), parentNode);
			}
			else if (gameObjectNodeList[it]->PreviousSibling)
			{
				TreeNode<GameObject>* previousSiblingNode = m_pcurrentGameScene->GetTreeNode(gameObjectNodeList[it]->PreviousSibling->NodeObject);
				m_pcurrentGameScene->CopyGameObject<RangedWeaponGameObject>(*(dynamic_cast<RangedWeaponGameObject*>(gameObjectNodeList[it]->NodeObject)), nullptr, previousSiblingNode);
			}
			else
			{
				m_pcurrentGameScene->CopyGameObject<RangedWeaponGameObject>(*(dynamic_cast<RangedWeaponGameObject*>(gameObjectNodeList[it]->NodeObject)));
			}
			break;

		case AccumlateType::CAMERA:
			if (gameObjectNodeList[it]->PreviousParent)
			{
				TreeNode<GameObject>* parentNode = m_pcurrentGameScene->GetTreeNode(gameObjectNodeList[it]->PreviousParent->NodeObject);
				m_pcurrentGameScene->CopyGameObject<CameraGameObject>(*(dynamic_cast<CameraGameObject*>(gameObjectNodeList[it]->NodeObject)), parentNode);
			}
			else if (gameObjectNodeList[it]->PreviousSibling)
			{
				TreeNode<GameObject>* previousSiblingNode = m_pcurrentGameScene->GetTreeNode(gameObjectNodeList[it]->PreviousSibling->NodeObject);
				m_pcurrentGameScene->CopyGameObject<CameraGameObject>(*(dynamic_cast<CameraGameObject*>(gameObjectNodeList[it]->NodeObject)), nullptr, previousSiblingNode);
			}
			else
			{
				m_pcurrentGameScene->CopyGameObject<CameraGameObject>(*(dynamic_cast<CameraGameObject*>(gameObjectNodeList[it]->NodeObject)));
			}
			break;

		case AccumlateType::LEVEL_CHANGE:
			if (gameObjectNodeList[it]->PreviousParent)
			{
				TreeNode<GameObject>* parentNode = m_pcurrentGameScene->GetTreeNode(gameObjectNodeList[it]->PreviousParent->NodeObject);
				m_pcurrentGameScene->CopyGameObject<LevelChangeGameObject>(*(dynamic_cast<LevelChangeGameObject*>(gameObjectNodeList[it]->NodeObject)), parentNode);
			}
			else if (gameObjectNodeList[it]->PreviousSibling)
			{
				TreeNode<GameObject>* previousSiblingNode = m_pcurrentGameScene->GetTreeNode(gameObjectNodeList[it]->PreviousSibling->NodeObject);
				m_pcurrentGameScene->CopyGameObject<LevelChangeGameObject>(*(dynamic_cast<LevelChangeGameObject*>(gameObjectNodeList[it]->NodeObject)), nullptr, previousSiblingNode);
			}
			else
			{
				m_pcurrentGameScene->CopyGameObject<LevelChangeGameObject>(*(dynamic_cast<LevelChangeGameObject*>(gameObjectNodeList[it]->NodeObject)));
			}
			break;

		case AccumlateType::TILE_MAP:
			if (gameObjectNodeList[it]->PreviousParent)
			{
				TreeNode<GameObject>* parentNode = m_pcurrentGameScene->GetTreeNode(gameObjectNodeList[it]->PreviousParent->NodeObject);
				m_pcurrentGameScene->CopyGameObject<TileMap>(*(dynamic_cast<TileMap*>(gameObjectNodeList[it]->NodeObject)), parentNode);
			}
			else if (gameObjectNodeList[it]->PreviousSibling)
			{
				TreeNode<GameObject>* previousSiblingNode = m_pcurrentGameScene->GetTreeNode(gameObjectNodeList[it]->PreviousSibling->NodeObject);
				m_pcurrentGameScene->CopyGameObject<TileMap>(*(dynamic_cast<TileMap*>(gameObjectNodeList[it]->NodeObject)), nullptr, previousSiblingNode);
			}
			else
			{
				m_pcurrentGameScene->CopyGameObject<TileMap>(*(dynamic_cast<TileMap*>(gameObjectNodeList[it]->NodeObject)));
			}
			break;

		case AccumlateType::UI_COMPONENT:
			GameUIComponent* uiComponent = dynamic_cast<GameUIComponent*>(gameObjectNodeList[it]->NodeObject);
			switch ((AccumulatedUIComponentType)uiComponent->GetComponentType())
			{
			case AccumulatedUIComponentType::BASE:
				if (gameObjectNodeList[it]->PreviousParent)
				{
					TreeNode<GameObject>* parentNode = m_pcurrentGameScene->GetTreeNode(gameObjectNodeList[it]->PreviousParent->NodeObject);
					m_pcurrentGameScene->CopyGameObject<GameUIComponent>(*(dynamic_cast<GameUIComponent*>(gameObjectNodeList[it]->NodeObject)), parentNode);
				}
				else if (gameObjectNodeList[it]->PreviousSibling)
				{
					TreeNode<GameObject>* previousSiblingNode = m_pcurrentGameScene->GetTreeNode(gameObjectNodeList[it]->PreviousSibling->NodeObject);
					m_pcurrentGameScene->CopyGameObject<GameUIComponent>(*(dynamic_cast<GameUIComponent*>(gameObjectNodeList[it]->NodeObject)), nullptr, previousSiblingNode);
				}
				else
				{
					m_pcurrentGameScene->CopyGameObject<GameUIComponent>(*(dynamic_cast<GameUIComponent*>(gameObjectNodeList[it]->NodeObject)));
				}
				break;

			case AccumulatedUIComponentType::BUTTON:
				if (gameObjectNodeList[it]->PreviousParent)
				{
					TreeNode<GameObject>* parentNode = m_pcurrentGameScene->GetTreeNode(gameObjectNodeList[it]->PreviousParent->NodeObject);
					m_pcurrentGameScene->CopyGameObject<ButtonComponent>(*(dynamic_cast<ButtonComponent*>(gameObjectNodeList[it]->NodeObject)), parentNode);
				}
				else if (gameObjectNodeList[it]->PreviousSibling)
				{
					TreeNode<GameObject>* previousSiblingNode = m_pcurrentGameScene->GetTreeNode(gameObjectNodeList[it]->PreviousSibling->NodeObject);
					m_pcurrentGameScene->CopyGameObject<ButtonComponent>(*(dynamic_cast<ButtonComponent*>(gameObjectNodeList[it]->NodeObject)), nullptr, previousSiblingNode);
				}
				else
				{
					m_pcurrentGameScene->CopyGameObject<ButtonComponent>(*(dynamic_cast<ButtonComponent*>(gameObjectNodeList[it]->NodeObject)));
				}
				break;

			case AccumulatedUIComponentType::CANVAS:
				if (gameObjectNodeList[it]->PreviousParent)
				{
					TreeNode<GameObject>* parentNode = m_pcurrentGameScene->GetTreeNode(gameObjectNodeList[it]->PreviousParent->NodeObject);
					m_pcurrentGameScene->CopyGameObject<UICanvas>(*(dynamic_cast<UICanvas*>(gameObjectNodeList[it]->NodeObject)), parentNode);
				}
				else if (gameObjectNodeList[it]->PreviousSibling)
				{
					TreeNode<GameObject>* previousSiblingNode = m_pcurrentGameScene->GetTreeNode(gameObjectNodeList[it]->PreviousSibling->NodeObject);
					m_pcurrentGameScene->CopyGameObject<UICanvas>(*(dynamic_cast<UICanvas*>(gameObjectNodeList[it]->NodeObject)), nullptr, previousSiblingNode);
				}
				else
				{
					m_pcurrentGameScene->CopyGameObject<UICanvas>(*(dynamic_cast<UICanvas*>(gameObjectNodeList[it]->NodeObject)));
				}
				break;

			case AccumulatedUIComponentType::IMAGE:
				if (gameObjectNodeList[it]->PreviousParent)
				{
					TreeNode<GameObject>* parentNode = m_pcurrentGameScene->GetTreeNode(gameObjectNodeList[it]->PreviousParent->NodeObject);
					m_pcurrentGameScene->CopyGameObject<ImageComponent>(*(dynamic_cast<ImageComponent*>(gameObjectNodeList[it]->NodeObject)), parentNode);
				}
				else if (gameObjectNodeList[it]->PreviousSibling)
				{
					TreeNode<GameObject>* previousSiblingNode = m_pcurrentGameScene->GetTreeNode(gameObjectNodeList[it]->PreviousSibling->NodeObject);
					m_pcurrentGameScene->CopyGameObject<ImageComponent>(*(dynamic_cast<ImageComponent*>(gameObjectNodeList[it]->NodeObject)), nullptr, previousSiblingNode);
				}
				else
				{
					m_pcurrentGameScene->CopyGameObject<UICanvas>(*(dynamic_cast<UICanvas*>(gameObjectNodeList[it]->NodeObject)));
				}
				break;

			case AccumulatedUIComponentType::TEXT:
				if (gameObjectNodeList[it]->PreviousParent)
				{
					TreeNode<GameObject>* parentNode = m_pcurrentGameScene->GetTreeNode(gameObjectNodeList[it]->PreviousParent->NodeObject);
					m_pcurrentGameScene->CopyGameObject<TextComponent>(*(dynamic_cast<TextComponent*>(gameObjectNodeList[it]->NodeObject)), parentNode);
				}
				else if (gameObjectNodeList[it]->PreviousSibling)
				{
					TreeNode<GameObject>* previousSiblingNode = m_pcurrentGameScene->GetTreeNode(gameObjectNodeList[it]->PreviousSibling->NodeObject);
					m_pcurrentGameScene->CopyGameObject<TextComponent>(*(dynamic_cast<TextComponent*>(gameObjectNodeList[it]->NodeObject)), nullptr, previousSiblingNode);
				}
				else
				{
					m_pcurrentGameScene->CopyGameObject<TextComponent>(*(dynamic_cast<TextComponent*>(gameObjectNodeList[it]->NodeObject)));
				}
				break;
			}
			break;
		}
	}
}

ViewState GameManager::GetViewState()const
{
	return m_viewState;
}

string GameManager::GetWorkingDirectory()
{
	return m_workingDirectory;
}

wstring GameManager::GetWideWorkingDirectory()
{
	return m_wideWorkingDirectory;
}

Scene* GameManager::GetCurrentScene()
{
	return GetCurrentViewStateScene();
}

void GameManager::SwitchScene(Scene* pscene)
{
	if (!pscene)
	{
		return;
	}

	Scene* pcurrentScene = GetCurrentViewStateScene();
	pcurrentScene = pscene;

}

bool GameManager::SwitchSceneUsingIdentifier(string sceneIdentifier)
{
	unordered_map<string, Scene*> sceneMap = GetCurrentViewStateSceneMap();
	if (sceneMap.count(sceneIdentifier) == 0)
	{
		LOG("Scene : " << sceneIdentifier << "; was not found in Scene Map ");
		return false;
	}
	Scene*& pcurrentScene = GetCurrentViewStateScene();
	pcurrentScene = sceneMap[sceneIdentifier];
	return true;
}

void GameManager::DeleteScene(Scene* pscene)
{
	pscene->m_psceneGraph->DeleteAllGameObjects();
	m_pbulletCreator->DeleteBullets();
	GetCurrentViewStateSceneMap().erase(pscene->GetSceneIdentifier());
}

vector<GameObject*>& GameManager::GetAllGameObjects()
{
	return GetCurrentViewStateScene()->GetAllGameObjects();
}

CameraGameObject* GameManager::GetCamera()
{
	switch (m_viewState)
	{
	case ViewState::EDITOR_VIEW:
		return m_peditorCamera;
		break;

	case ViewState::GAME_VIEW:
		return m_pcurrentGameScene->GetActiveCamera();
		break;
	}
}

void GameManager::SetActiveCameraUsingIdentifier(string identifier)
{
	if (m_viewState == ViewState::GAME_VIEW)
	{
		m_pcurrentGameScene->SetActiveCameraUsingIdentifier(identifier);
	}
}

void GameManager::DeleteGameObjectUsingNode(TreeNode<GameObject>* currentNode)
{
	GetCurrentViewStateScene()->DeleteGameObjectUsingNode(currentNode);
}

void GameManager::DeleteGameObjectUsingIdentifier(string identifier)
{
	GetCurrentViewStateScene()->DeleteGameObjectUsingIdentifier(identifier);
}

TreeNode<GameObject>* GameManager::GetRootTreeNode()
{
	return GetCurrentViewStateScene()->GetRootTreeNode();
}

TreeNode<GameObject>* GameManager::GetTreeNode(GameObject* pgameObject)
{
	return GetCurrentViewStateScene()->GetTreeNode(pgameObject);
}

string& GameManager::GetCurrentSceneName()
{
	return GetCurrentViewStateScene()->GetSceneIdentifier();
}

vector<GameObject*>& GameManager::GetAllGameObjectsInCurrentScene()
{
	switch (m_viewState)
	{
	case ViewState::EDITOR_VIEW:
		return m_pcurrentEditorScene->GetAllGameObjects();
		break;

	case ViewState::GAME_VIEW:
		return m_pcurrentGameScene->GetAllGameObjects();
		break;
	}
}

void GameManager::Serialize(nlohmann::json& data)
{
	vector<GameObject*> gameObjectsInScene = m_pcurrentEditorScene->GetAllGameObjects();

	data["SceneName"] = m_pcurrentEditorScene->GetSceneIdentifier();

	for (size_t i = 0; i < gameObjectsInScene.size(); i++)
	{
		std::string typeIdentifier = std::to_string((int)gameObjectsInScene[i]->GetGameObjectType());
		gameObjectsInScene[i]->Serialize(data[typeIdentifier][std::to_string(*gameObjectsInScene[i]->m_UUID)]);
	}

	TreeNode<GameObject>* pnode = nullptr;


	if (m_pcurrentEditorScene->GetRootTreeNode() == nullptr)
	{
		data["RootNode"] = -1;
	}
	else
	{
		data["RootNode"] = *m_pcurrentEditorScene->GetRootTreeNode()->NodeObject->m_UUID;
	}

	for (int i = 0; i < gameObjectsInScene.size(); ++i)
	{
		pnode = m_pcurrentEditorScene->GetTreeNode(gameObjectsInScene[i]);

		if (pnode != nullptr)
		{
			std::string uuidString = to_string(*pnode->NodeObject->m_UUID);

			if (pnode->Sibling == nullptr)
			{
				data[to_string((int)pnode->NodeObject->GetGameObjectType())][uuidString]["Sibling"] = -1;
			}
			else
			{
				data[to_string((int)pnode->NodeObject->GetGameObjectType())][uuidString]["Sibling"] = *pnode->Sibling->NodeObject->m_UUID;
			}

			if (pnode->PreviousSibling == nullptr)
			{
				data[to_string((int)pnode->NodeObject->GetGameObjectType())][uuidString]["PreviousSibling"] = -1;
			}
			else
			{
				data[to_string((int)pnode->NodeObject->GetGameObjectType())][uuidString]["PreviousSibling"] = *pnode->PreviousSibling->NodeObject->m_UUID;
			}

			if (pnode->PreviousParent == nullptr)
			{
				data[to_string((int)pnode->NodeObject->GetGameObjectType())][uuidString]["PreviousParent"] = -1;
			}
			else
			{
				data[to_string((int)pnode->NodeObject->GetGameObjectType())][uuidString]["PreviousParent"] = *pnode->PreviousParent->NodeObject->m_UUID;
			}

			if (pnode->Child == nullptr)
			{
				data[to_string((int)pnode->NodeObject->GetGameObjectType())][uuidString]["Child"] = -1;
			}
			else
			{
				data[to_string((int)pnode->NodeObject->GetGameObjectType())][uuidString]["Child"] = *pnode->Child->NodeObject->m_UUID;
			}
		}
	}
}

void GameManager::Deserialize(nlohmann::json& data)
{
	std::unordered_map<uint64_t, GameObject*> gameObjects;

	string mainCameraIdentifier;

	string sceneName = data["SceneName"];
	Scene* pnewScene = new Scene(sceneName);

	for (nlohmann::json::const_iterator typeIt = data.begin(); typeIt != data.end(); ++typeIt)
	{
		if (typeIt.key() == "RootNode" || typeIt.key() == "AudioManager" || typeIt.key() == "GraphicsManager" || typeIt.key() == "GameUI" || typeIt.key() == "FontManager" || typeIt.key() == "SceneName")
		{
			continue;
		}

		for (nlohmann::json::const_iterator objectIt = data[typeIt.key()].begin(); objectIt != data[typeIt.key()].end(); ++objectIt)
		{
			CoolUUID uuid = CoolUUID(stoull(objectIt.key()));
			std::string uuidString = to_string(*uuid);

			switch ((AccumlateType)stoi(typeIt.key()))
			{
			case AccumlateType::BASE:
				gameObjects[*uuid] = new GameObject(data[typeIt.key()][uuidString], uuid);
				gameObjects[*uuid]->m_UUID = uuid;
				break;

			case AccumlateType::RENDERABLE:
				gameObjects[*uuid] = new RenderableGameObject(data[typeIt.key()][uuidString], uuid);
				gameObjects[*uuid]->m_UUID = uuid;
				break;

			case AccumlateType::COLLIDABLE:
				gameObjects[*uuid] = new CollidableGameObject(data[typeIt.key()][uuidString], uuid);
				gameObjects[*uuid]->m_UUID = uuid;
				break;

			case AccumlateType::COLLIDABLE_RENDERERABLE:
				gameObjects[*uuid] = new RenderableCollidableGameObject(data[typeIt.key()][uuidString], uuid);
				gameObjects[*uuid]->m_UUID = uuid;
				break;

			case AccumlateType::CHARACTER:
				gameObjects[*uuid] = new CharacterGameObject(data[typeIt.key()][uuidString], uuid);
				gameObjects[*uuid]->m_UUID = uuid;
				break;

			case AccumlateType::ENEMY:
				gameObjects[*uuid] = new EnemyGameObject(data[typeIt.key()][uuidString], uuid);
				gameObjects[*uuid]->m_UUID = uuid;
				break;

			case AccumlateType::PLAYER:
				gameObjects[*uuid] = new PlayerGameObject(data[typeIt.key()][uuidString], uuid);
				gameObjects[*uuid]->m_UUID = uuid;
				break;

			case AccumlateType::PARTICLE_SYSTEM:
				gameObjects[*uuid] = new ParticleSystem(data[typeIt.key()][uuidString], uuid);
				gameObjects[*uuid]->m_UUID = uuid;
				break;

			case AccumlateType::INTERACTABLE:
				gameObjects[*uuid] = new InteractableGameObject(data[typeIt.key()][uuidString], uuid);
				gameObjects[*uuid]->m_UUID = uuid;
				break;

			case AccumlateType::WEAPON:
				gameObjects[*uuid] = new WeaponGameObject(data[typeIt.key()][uuidString], uuid);
				gameObjects[*uuid]->m_UUID = uuid;
				break;

			case AccumlateType::MELEE_WEAPON:
				gameObjects[*uuid] = new MeleeWeaponGameObject(data[typeIt.key()][uuidString], uuid);
				gameObjects[*uuid]->m_UUID = uuid;
				break;

			case AccumlateType::RANGE_WEAPON:
				gameObjects[*uuid] = new RangedWeaponGameObject(data[typeIt.key()][uuidString], uuid);
				gameObjects[*uuid]->m_UUID = uuid;
				break;

			case AccumlateType::CAMERA:
				gameObjects[*uuid] = new CameraGameObject(data[typeIt.key()][uuidString], uuid);
				gameObjects[*uuid]->m_UUID = uuid;

				pnewScene->m_cameraGameObjectMap[gameObjects[*uuid]->m_identifier] = dynamic_cast<CameraGameObject*>(gameObjects[*uuid]);

				if (pnewScene->GetActiveCamera() == nullptr)
				{
					mainCameraIdentifier = gameObjects[*uuid]->GetIdentifier();
				}
				break;

			case AccumlateType::TILE_MAP:
				gameObjects[*uuid] = new TileMap(data[typeIt.key()][uuidString], uuid);
				gameObjects[*uuid]->m_UUID = uuid;

				break;

			case AccumlateType::UI_COMPONENT:
			{
				AccumulatedUIComponentType type = data[typeIt.key()][uuidString]["UIType"];
				switch (type)
				{
				case AccumulatedUIComponentType::BUTTON:
					gameObjects[*uuid] = new ButtonComponent(data[typeIt.key()][uuidString], uuid);
					break;

				case AccumulatedUIComponentType::CANVAS:
					gameObjects[*uuid] = new UICanvas(data[typeIt.key()][uuidString], uuid);
					break;

				case AccumulatedUIComponentType::IMAGE:
					gameObjects[*uuid] = new ImageComponent(data[typeIt.key()][uuidString], uuid);
					break;

				case AccumulatedUIComponentType::TEXT:
					gameObjects[*uuid] = new TextComponent(data[typeIt.key()][uuidString], uuid, GraphicsManager::GetInstance()->GetDevice());
					break;
				}
			}
				break;

			default:
				LOG("Tried to deserialize an object that doesn't have a correct object type!");
				break;
			}
		}
	}

	TreeNode<GameObject>* pnode = nullptr;
	GameObject* pcomponent = nullptr;

	pcomponent = gameObjects[data["RootNode"]];



	pnode = pnewScene->m_psceneGraph->NewNode(pcomponent);

	std::stack<TreeNode<GameObject>*> toPush;
	toPush.push(pnode);

	while (toPush.size() != 0)
	{
		pnode = toPush.top();
		toPush.pop();

		uint64_t siblingCheck = data[std::to_string((int)pnode->NodeObject->GetGameObjectType())][std::to_string(*pnode->NodeObject->m_UUID)]["Sibling"];
		if (siblingCheck != -1)
		{
			pcomponent = gameObjects[siblingCheck];
			toPush.push(pnewScene->m_psceneGraph->AddSibling(pnode, pcomponent));
		}

		uint64_t childCheck = data[std::to_string((int)pnode->NodeObject->GetGameObjectType())][std::to_string(*pnode->NodeObject->m_UUID)]["Child"];
		if (childCheck != -1)
		{
			pcomponent = gameObjects[childCheck];
			toPush.push(pnewScene->m_psceneGraph->AddChild(pnode, pcomponent));
		}
	}

	pnewScene->SetActiveCameraUsingIdentifier(mainCameraIdentifier);

	pnode = pnewScene->m_psceneGraph->GetRootNode();


	while (pnode != nullptr)
	{
		pnode->NodeObject->GetTransform()->UpdateMatrix();

		pnode = pnode->Sibling;
	}

	GetCurrentViewStateSceneMap().insert(pair<string, Scene*>(sceneName, pnewScene));
	Scene*& pcurrentScene = GetCurrentViewStateScene();
	pcurrentScene = pnewScene;
}

unordered_map<string, Scene*>& GameManager::GetCurrentViewStateSceneMap()
{
	switch (m_viewState)
	{
	case ViewState::EDITOR_VIEW:
		return m_editorSceneMap;
		break;

	case ViewState::GAME_VIEW:
		return m_gameSceneMap;
		break;
	}
}

Scene*& GameManager::GetCurrentViewStateScene()
{
	switch (m_viewState)
	{
	case ViewState::EDITOR_VIEW:
		return m_pcurrentEditorScene;
		break;

	case ViewState::GAME_VIEW:
		return m_pcurrentGameScene;
		break;
	}
}

unordered_map<string, Scene*> GameManager::GetSceneList()
{
	return GetCurrentViewStateSceneMap();
}

void GameManager::CreateScene(string sceneIdentifier, bool unloadCurrentScene)
{
	if (unloadCurrentScene)
	{
		DeleteSelectedScene();
	}

	Scene* newScene = new Scene(sceneIdentifier);
	GetCurrentViewStateSceneMap().insert(pair<string, Scene*>(sceneIdentifier, newScene));

	Scene*& pcurrentScene = GetCurrentViewStateScene();
	pcurrentScene = newScene;
}

bool GameManager::LoadSceneFromFile(std::string fileLocation, bool unloadCurrentScene)
{
	ifstream fileIn(fileLocation);
	if (fileIn.is_open())
	{
		nlohmann::json dataIn;
		fileIn >> dataIn;

		AudioManager::GetInstance()->Deserialize(dataIn);
		GraphicsManager::GetInstance()->Deserialize(dataIn);
		FontManager::GetInstance()->Deserialize(dataIn);

		if (unloadCurrentScene && GetCurrentViewStateScene())
		{
			DeleteSelectedScene();
		}

		Deserialize(dataIn);
		return true;
	}
	return false;
}

GameObject* GameManager::GetSelectedGameObject()
{
	if (m_pcurrentEditorScene == nullptr)
	{
		return nullptr;
	}

	return m_pcurrentEditorScene->GetSelectedGameObject();
}
