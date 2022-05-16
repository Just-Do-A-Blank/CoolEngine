#include "Engine/Managers/Events/BulletCreator.h"
#include "Engine/Managers/GameManager.h"
#include "Engine/Physics/Box.h"
#include "Engine/Physics/Circle.h"
#include "Engine/Managers/Events/EventManager.h"


BulletCreator::BulletCreator()
{
	EventManager::Instance()->AddClient(EventType::CreateBullet, this);
	EventManager::Instance()->AddClient(EventType::MouseButtonPressed, this);
}

void BulletCreator::CreateBullet(CreateBulletEvent* e)
{
	string name = "Bullet" + to_string(m_bulletCounter);
	++m_bulletCounter;

	GameManager::GetInstance()->CreateGameObject<RenderableCollidableGameObject>(name);
	RenderableCollidableGameObject* p_bullet = GameManager::GetInstance()->GetGameObjectUsingIdentifier<RenderableCollidableGameObject>(name);

	XMFLOAT3 objectPos = e->GetWeapon()->GetTransform()->GetWorldPosition();
	XMFLOAT3 objectScale = e->GetWeapon()->GetTransform()->GetWorldScale();
	XMFLOAT3 objectRot = e->GetWeapon()->GetTransform()->GetWorldRotation();
	bool isCollision = false;

	p_bullet->SetMesh(QUAD_MESH_NAME);
	p_bullet->SetVertexShader(DEFAULT_VERTEX_SHADER_NAME);
	p_bullet->SetPixelShader(DEFAULT_PIXEL_SHADER_NAME);
	p_bullet->SetAlbedo(DEFAULT_IMGUI_IMAGE);
	p_bullet->GetTransform()->SetWorldPosition(objectPos);
	p_bullet->GetTransform()->SetWorldScale(objectScale);
	Box* pbox = new Box(p_bullet->GetTransform());
	pbox->SetIsCollidable(isCollision);
	isCollision = true;
	pbox->SetIsTrigger(isCollision);
	p_bullet->SetShape(pbox);
}

void BulletCreator::TestFire(MouseButtonPressedEvent* e)
{
	string name = "Player";
	RenderableCollidableGameObject* p_player = GameManager::GetInstance()->GetGameObjectUsingIdentifier<RenderableCollidableGameObject>(name);

	EventManager::Instance()->AddEvent(new CreateBulletEvent(p_player));
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