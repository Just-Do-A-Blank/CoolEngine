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

	m_pBulletPool = new ObjectPool<BulletGameObject>(64);
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
	BulletGameObject* p_bullet =  m_pBulletPool->CreateEntryInPool();

	if (e->GetObj() == nullptr || p_bullet == nullptr)
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
	p_bullet->GetTransform()->SetWorldPosition(objectPos);
	p_bullet->GetTransform()->SetWorldScale(e->GetObj()->GetBulletScale());
	Circle* pcircle = new Circle(p_bullet);
	pcircle->SetIsCollidable(isCollision);
	isCollision = true;
	pcircle->SetIsTrigger(isCollision);
	pcircle->SetScale(e->GetObj()->GetCollisionScale());
	p_bullet->SetShape(pcircle);

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
	p_bullet->SetAlbedo(e->GetObj()->GetBulletTexturePath());
}


void BulletCreator::Handle(Event* e)
{
	switch (e->GetEventID())
	{
	case EventType::CreateBullet:
		CreateBullet((CreateBulletEvent*)e);
		break;
	}
}

void BulletCreator::DeleteBullets()
{
	m_pBulletPool->ClearPool();
}