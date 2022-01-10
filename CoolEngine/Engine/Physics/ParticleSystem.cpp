#include "ParticleSystem.h"

ParticleSystem::ParticleSystem()
{
	for (unsigned int i = 0; i < PARTICLE_SYSTEM_SIZE; ++i)
	{
		m_pParticles[i] = new Particle();
	}
	m_position = Vector2();

	m_lifetime = 0.0f;
	m_isActive = false;
	m_systemType = SYSTEM_NONE;
}

ParticleSystem::~ParticleSystem()
{
	delete[] m_pParticles;
}

void ParticleSystem::Initialise(Vector2 pos, float life, SYSTEM_TYPE type)
{
	m_position = pos;
	m_lifetime = life;
	m_isActive = true;
	m_systemType = type;
}

void ParticleSystem::Update(const float dTime)
{
	for (unsigned int i = 0; i < PARTICLE_SYSTEM_SIZE; ++i)
	{
		m_pParticles[i]->Update(dTime);
	}

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

	// Particle generation process
	switch (m_systemType)
	{
	case SYSTEM_NONE:
		break;

	case SYSTEM_TEST:
		// Something
		break;
	}
}