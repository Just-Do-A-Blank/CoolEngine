#include "Engine/Managers/Events/BulletCreator.h"
#include "Engine/Managers/GameManager.h"
#include "Engine/Physics/Box.h"
#include "Engine/Physics/Circle.h"
#include "Engine/Managers/Events/EventManager.h"
#include "Engine/Physics/Collision.h"



BulletCreator::BulletCreator()
{
	EventManager::Instance()->AddClient(EventType::CreateBullet, this);
	EventManager::Instance()->AddClient(EventType::MouseButtonPressed, this);

	m_pBulletPool = new ObjectPool<BulletGameObject>(32);
}

void BulletCreator::Render(RenderStruct& renderStruct)
{
	for (size_t i = 0; i < m_pBulletPool->ReturnPool().size(); ++i)
	{
		if (m_pBulletPool->ReturnPool()[i]->m_Active)
		{
			m_pBulletPool->ReturnPool()[i]->m_pObject->Render(renderStruct);
		}
	}
}

void BulletCreator::Update()
{
	for (size_t i = 0; i < m_pBulletPool->ReturnPool().size(); ++i)
	{
		if (m_pBulletPool->ReturnPool()[i]->m_Active)
		{
			m_pBulletPool->ReturnPool()[i]->m_pObject->Update();
			m_pBulletPool->ReturnPool()[i]->m_Active = m_pBulletPool->ReturnPool()[i]->m_pObject->GetActive();
		}
	}

	Scene* currentScene = GameManager::GetInstance()->GetCurrentScene();
	if (currentScene)
	{
		Collision::Update(currentScene->GetSceneGraph()->GetAllNodeObjects(), m_pBulletPool->ReturnPool());
	}
}

BulletCreator::~BulletCreator()
{
	EventManager::Instance()->RemoveClientEvent(EventType::MouseButtonPressed, this);
}

void BulletCreator::CreateBullet(CreateBulletEvent* e)
{
	BulletGameObject* p_bullet =  m_pBulletPool->CreateEntryInPool()->m_pObject;

	if (e->GetObj() == nullptr)
	{
		return;
	}

	XMFLOAT3 objectPos = e->GetStartPos();
	XMFLOAT3 objectScale = e->GetObj()->GetTransform()->GetWorldScale();
	bool isCollision = false;

	// Game object variables
	p_bullet->SetMesh(QUAD_MESH_NAME);
	p_bullet->SetVertexShader(DEFAULT_VERTEX_SHADER_NAME);
	p_bullet->SetPixelShader(DEFAULT_PIXEL_SHADER_NAME);
	p_bullet->SetAlbedo(e->GetTextureName());
	p_bullet->GetTransform()->SetWorldPosition(objectPos);
	p_bullet->GetTransform()->SetWorldScale(objectScale);
	Box* pbox = new Box(p_bullet);
	pbox->SetIsCollidable(isCollision);
	isCollision = true;
	pbox->SetIsTrigger(isCollision);
	p_bullet->SetShape(pbox);

	// Weapon variables
	p_bullet->SetDamage(e->GetObj()->GetDamage());
	p_bullet->SetDirection(e->GetDirection());
	p_bullet->SetElement(e->GetObj()->GetElement());
	p_bullet->SetDistanceTravelled(e->GetObj()->GetDistanceTravelled());
	p_bullet->SetSpeed(e->GetObj()->GetSpeed());
	p_bullet->SetStatusEffect(e->GetObj()->GetStatusEffect());
	p_bullet->SetTimeLethal(e->GetObj()->GetTimeLethal());
	p_bullet->SetIsPlayerWeapon(e->GetObj()->GetIsPlayerWeapon());
	p_bullet->SetActive(true);
	p_bullet->SetCurrentTime(0.0f);
	p_bullet->SetTotalTime(e->GetObj()->GetDistanceTravelled() / e->GetObj()->GetSpeed());
}

void BulletCreator::TestFire(MouseButtonPressedEvent* e)
{
	// Temporary, uses a weapon object in the scene as a reference. In the final, will probably store weapon data some other way.
	string name = "Weapon";
	RangedWeaponGameObject* p_weapon = GameManager::GetInstance()->GetGameObjectUsingIdentifier<RangedWeaponGameObject>(name);

	// Get player location
	name = "Player";
	RenderableCollidableGameObject* p_player = GameManager::GetInstance()->GetGameObjectUsingIdentifier<RenderableCollidableGameObject>(name);

	if (p_player != nullptr)
	{
		EventManager::Instance()->AddEvent(new CreateBulletEvent(p_weapon, XMFLOAT3(1, 0, 0), p_player->GetTransform()->GetWorldPosition(), DEFAULT_IMGUI_IMAGE));
	}
}

void BulletCreator::Handle(Event* e)
{
	switch (e->GetEventID())
	{
	case EventType::CreateBullet:
		CreateBullet((CreateBulletEvent*)e);
		break;
	case EventType::MouseButtonPressed:
		TestFire((MouseButtonPressedEvent*)e);
		break;
	}
}

void BulletCreator::DeleteBullets()
{
	m_pBulletPool->ClearPool();
}