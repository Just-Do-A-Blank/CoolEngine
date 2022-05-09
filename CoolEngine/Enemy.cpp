#include "Enemy.h"
#include "Engine/Managers/GameManager.h"
#include "Engine/EditorUI/EditorUI.h"

XMFLOAT3 TargetLoc = XMFLOAT3(-100.0f, 100.0f, 0.0f);

Enemy::Enemy(string identifier) : EnemyGameObject(identifier)
{
	m_gameObjectType |= GameObjectType::ENEMY;

}

Enemy::~Enemy()
{
	m_moveSpeed = 1;
}

void Enemy::Update()
{
	//SetTarget(TargetLoc);
	float temmpx = (m_direction.x * GameManager::GetInstance()->GetTimer()->DeltaTime() * m_moveSpeed) + m_transform->GetPosition().x;
	float temmpy = (m_direction.y * GameManager::GetInstance()->GetTimer()->DeltaTime() * m_moveSpeed) + m_transform->GetPosition().y;
	float temmpz = (m_direction.z * GameManager::GetInstance()->GetTimer()->DeltaTime() * m_moveSpeed) + m_transform->GetPosition().z;
	m_transform->SetPosition(XMFLOAT3(temmpx, temmpy, temmpz));
	
}

void SetTarget(XMFLOAT3 target)
{
	 
}

#if EDITOR
void Enemy::CreateEngineUI()
{
	GameObject::CreateEngineUI();
	

	ImGui::Spacing();
	EditorUI::Texture("Texture", m_texFilepath, m_palbedoSRV);
	ImGui::Spacing();
	EditorUI::DragInt("Layer", m_layer);
	ImGui::Spacing();
	EditorUI::Checkbox("Ranged", m_ranged);
	ImGui::Spacing();
	EditorUI::Checkbox("Melee", m_melee);
	ImGui::Spacing();
	EditorUI::Animations("Animations", m_animations);
	ImGui::Spacing();
	EditorUI::DragFloat("Health", m_health);
	ImGui::Spacing();
	EditorUI::DragFloat("Move Speed", m_moveSpeed);
	ImGui::Spacing();
	EditorUI::DragFloat3("Move Direction", m_direction);
}
#endif