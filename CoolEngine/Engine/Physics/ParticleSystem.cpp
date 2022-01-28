#include "ParticleSystem.h"
#include "Engine/Physics/ParticleManager.h"

#include "Engine/EditorUI/EditorUI.h"

ParticleSystem::ParticleSystem(string identifier) : GameObject(identifier)
{
	m_systemLife = 0.0f;
	m_timer = 0.0f;
	m_isActive = false;

	// Particle properties
	m_velocity = { 0, 0 };
	m_accel = { 0, 0 };
	m_particleLife = 1.0f;
	m_spawnInterval = 1.0f;
	m_spawnNumber = 1;
	
	// Randomness
	m_positionRand = 0;
	m_velocityRand = 0;
	m_accelRand = 0;
	m_lifeRand = 0;
}

ParticleSystem::~ParticleSystem()
{

}

void ParticleSystem::Initialise(Transform trans, float life, ID3D11ShaderResourceView* tex)
{
	*m_transform = trans;
	m_transform->UpdateMatrix();

	m_systemLife = life;
	m_timer = 0.0f;
	m_isActive = true;
	m_pTexture = tex;
}

void ParticleSystem::Initialise(Transform trans, float life, ID3D11ShaderResourceView* tex, XMFLOAT2 vel, XMFLOAT2 accel, float partLife, float interval, int number, float randPos, float randVel, float randAccel, float randLife)
{
	Initialise(trans, life, tex);
	SetProperties(vel, accel, partLife, interval, number);
	SetRandomness(randPos, randVel, randAccel, randLife);
}

void ParticleSystem::SetProperties(XMFLOAT2 vel, XMFLOAT2 accel, float life, float interval, int number)
{
	m_velocity = vel;
	m_accel = accel;
	m_particleLife = life;
	m_spawnInterval = interval;
	m_spawnNumber = number;
}

void ParticleSystem::SetRandomness(float randPos, float randVel, float randAccel, float randLife)
{
	m_positionRand = randPos;
	m_velocityRand = randVel;
	m_accelRand = randAccel;
	m_lifeRand = randLife;
}

void ParticleSystem::Update(const float dTime)
{
	// Particle generation process
	if (m_timer >= m_spawnInterval)
	{
		for (unsigned int i = 0; i < m_spawnNumber; ++i)
		{
			ParticleManager::GetInstance()->AddParticle(*m_transform, m_particleLife, m_pTexture, m_velocity, m_accel, m_positionRand, m_velocityRand, m_accelRand, m_lifeRand);
		}
		m_timer = 0;
	}
	m_timer += dTime;

	m_systemLife -= dTime;
	if (m_systemLife <= 0.0f)
	{
		m_isActive = false;
	}
}

#if EDITOR
void ParticleSystem::CreateEngineUI()
{
	GameObject::CreateEngineUI();

	ImGui::Spacing();

	EditorUI::DragFloat2("Velocity", m_velocity, 150.0f, 0.1f, 0, 0);
	ImGui::Spacing();
	EditorUI::DragFloat2("Acceleration", m_accel, 150.0f, 0.1f, 0, 0);

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

	EditorUI::DragFloat("Particle Lifetime", m_particleLife, 150.0f, 0.05f, 0, 10000.0f);
	ImGui::Spacing();
	EditorUI::DragFloat("Particle Interval", m_spawnInterval, 150.0f, 0.05f, 0, 10000.0f);
	ImGui::Spacing();
	EditorUI::DragInt("Particle Quantity", m_spawnNumber, 150.0f, 0.05f, 0, 128);

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

	EditorUI::DragFloat("Position Spread", m_positionRand, 150.0f, 0.1f, 0, 10000.0f);
	ImGui::Spacing();
	EditorUI::DragFloat("Velocity Spread", m_velocityRand, 150.0f, 0.1f, 0, 10000.0f);
	ImGui::Spacing();
	EditorUI::DragFloat("Acceleration Spread", m_accelRand, 150.0f, 0.1f, 0, 10000.0f);
	ImGui::Spacing();
	EditorUI::DragFloat("Lifetime Spread", m_lifeRand, 150.0f, 0.005f, 0, 10000.0f);

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

	EditorUI::Texture("Texture", m_texFilepath, m_pTexture, 150.0f);

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

	EditorUI::DragFloat("System Lifetime", m_systemLife, 150.0f, 0.1f, 0, 10000.0f);
	ImGui::Spacing();
	EditorUI::Checkbox("System Active?", m_isActive, 150.0f);

	ImGui::Spacing();
}
#endif
