#include "ParticleSystem.h"

ParticleSystem::ParticleSystem()
{
	for (unsigned int i = 0; i < PARTICLE_SYSTEM_SIZE; ++i)
	{
		m_pParticles[i] = new Particle();
	}

	m_box = Box();
	m_lifetime = 0.0f;
	m_timer = 0.0f;
	m_isActive = false;
	m_systemType = SYSTEM_NONE;
}

ParticleSystem::~ParticleSystem()
{
	delete[] m_pParticles;
}

void ParticleSystem::Initialise(Box box, float life, SYSTEM_TYPE type)
{
	m_box = box;
	m_lifetime = life;
	m_timer = 0.0f;
	m_isActive = true;
	m_systemType = type;
}

void ParticleSystem::Update(const float dTime)
{
	for (unsigned int i = 0; i < PARTICLE_SYSTEM_SIZE; ++i)
	{
		if (m_pParticles[i]->IsActive())
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
			AddParticle(m_box, Vector2(0, 0), Vector2(0, 0), nullptr, 0.5f);
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

void ParticleSystem::AddParticle(Box box, Vector2 vel, Vector2 accel, Texture* tex, float life)
{
	for (unsigned int i = 0; i < PARTICLE_SYSTEM_SIZE; ++i)
	{
		if (!m_pParticles[i]->IsActive())
		{
			// Initialise one, then break from loop so more are not made
			m_pParticles[i]->Initialise(box, vel, accel, tex, life);
			break;
		}
	}
}