#include "ParticleManager.h"

ParticleManager::ParticleManager()
{
	for (unsigned int i = 0; i < PARTICLE_MANAGER_SIZE; ++i)
	{
		m_pParticleSystems[i] = new ParticleSystem();
	}
}

ParticleManager::~ParticleManager()
{
	delete[] m_pParticleSystems;
}

void ParticleManager::Update(const float dTime)
{
	for (unsigned int i = 0; i < PARTICLE_MANAGER_SIZE; ++i)
	{
		if (m_pParticleSystems[i]->IsActive())
		{
			m_pParticleSystems[i]->Update(dTime);
		}
	}
}

void ParticleManager::AddSystem(Box box, float life, SYSTEM_TYPE type)
{
	for (unsigned int i = 0; i < PARTICLE_MANAGER_SIZE; ++i)
	{
		if (!m_pParticleSystems[i]->IsActive())
		{
			// Initialise one, then break from loop so more are not made
			m_pParticleSystems[i]->Initialise(box, life, type);
			break;
		}
	}
}