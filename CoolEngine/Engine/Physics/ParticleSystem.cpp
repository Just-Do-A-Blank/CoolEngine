#include "ParticleSystem.h"

#include "Engine/EditorUI/EditorUI.h"

ParticleSystem::ParticleSystem(string identifier) : GameObject(identifier)
{
	for (unsigned int i = 0; i < PARTICLE_SYSTEM_SIZE; ++i)
	{
		m_pParticles[i] = new Particle();
	}

	m_systemLife = 0.0f;
	m_timer = 0.0f;
	m_isActive = false;
	m_pTexture = nullptr;

	// Particle properties
	m_velocity = { 0, 0 };
	m_accel = { 0, 0 };
	m_particleLife = 1.0f;
	m_spawnInterval = 1.0f;
	m_spawnNumber = 1;
}

ParticleSystem::~ParticleSystem()
{
	delete[] m_pParticles;
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

void ParticleSystem::Initialise(Transform trans, float life, ID3D11ShaderResourceView* tex, XMFLOAT2 vel, XMFLOAT2 accel, float partLife, float interval, int number)
{
	Initialise(trans, life, tex);
	SetProperties(vel, accel, partLife, interval, number);
}

void ParticleSystem::SetProperties(XMFLOAT2 vel, XMFLOAT2 accel, float life, float interval, int number)
{
	m_velocity = vel;
	m_accel = accel;
	m_particleLife = life;
	m_spawnInterval = interval;
	m_spawnNumber = number;
}

void ParticleSystem::Update(const float dTime)
{
	for (unsigned int i = 0; i < PARTICLE_SYSTEM_SIZE; ++i)
	{
		if (m_pParticles[i]->GetActive())
		{
			m_pParticles[i]->Update(dTime);
		}
	}

	// Particle generation process
	if (m_timer >= m_spawnInterval)
	{
		for (unsigned int i = 0; i < m_spawnNumber; ++i)
		{
			AddParticle(*m_transform, m_velocity, m_accel, m_particleLife);
		}
		m_timer = 0;
	}
	m_timer += dTime;

	m_systemLife -= dTime;
	if (m_systemLife <= 0.0f)
	{
		// System is dead, shut down all particles
		m_isActive = false;
		for (unsigned int i = 0; i < PARTICLE_SYSTEM_SIZE; ++i)
		{
			m_pParticles[i]->Disable();
		}
	}
}

void ParticleSystem::Render(ID3D11DeviceContext* pContext, Mesh* mesh)
{
	pContext->PSSetShaderResources(0, 1, &m_pTexture);

	for (unsigned int i = 0; i < PARTICLE_SYSTEM_SIZE; ++i)
	{
		if (m_pParticles[i]->GetActive())
		{
			m_pParticles[i]->Render(pContext, mesh);
		}
	}
}

void ParticleSystem::AddParticle(Transform trans, XMFLOAT2 vel, XMFLOAT2 accel, float life)
{
	for (unsigned int i = 0; i < PARTICLE_SYSTEM_SIZE; ++i)
	{
		if (!m_pParticles[i]->GetActive())
		{
			// Initialise one, then break from loop so more are not made
			m_pParticles[i]->Initialise(trans, vel, accel, life);
			break;
		}
	}
}

#if EDITOR
void ParticleSystem::CreateEngineUI()
{
	ImGui::Separator();
	ImGui::Spacing();

	m_transform->CreateEngineUI();
}
#endif
