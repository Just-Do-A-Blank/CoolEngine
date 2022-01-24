#include "ParticleSystem.h"

ParticleSystem::ParticleSystem()
{
	for (unsigned int i = 0; i < PARTICLE_SYSTEM_SIZE; ++i)
	{
		m_pParticles[i] = new Particle();
	}

	m_lifetime = 0.0f;
	m_timer = 0.0f;
	m_isActive = false;
	m_systemType = SYSTEM_NONE;
	m_pTexture = nullptr;
}

ParticleSystem::~ParticleSystem()
{
	delete[] m_pParticles;
}

void ParticleSystem::Initialise(Transform trans, float life, SYSTEM_TYPE type, ID3D11ShaderResourceView* tex)
{
	*m_transform = trans;
	m_transform->UpdateMatrix();

	m_lifetime = life;
	m_timer = 0.0f;
	m_isActive = true;
	m_systemType = type;
	m_pTexture = tex;
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
	switch (m_systemType)
	{
	case SYSTEM_NONE:
		break;

	case SYSTEM_TEST:
		// Basic test particle effect
		if (m_timer >= 1.0f)
		{
			AddParticle(*m_transform, XMFLOAT2(0, 0), XMFLOAT2(0, 0), 0.5f);
			m_timer = 0;
		}
		break;
	}
	m_timer += dTime;


	m_lifetime -= dTime;
	if (m_lifetime <= 0.0f)
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