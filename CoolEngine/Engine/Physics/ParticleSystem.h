#pragma once
#include "Particle.h"

#define PARTICLE_SYSTEM_SIZE 32

enum SYSTEM_TYPE
{
	SYSTEM_NONE = 0,
	SYSTEM_TEST
};

class ParticleSystem
{
private:
	Particle* m_pParticles[PARTICLE_SYSTEM_SIZE];
	Vector2 m_position;

	float m_lifetime;
	bool m_isActive;
	SYSTEM_TYPE m_systemType;

public:
	ParticleSystem();
	~ParticleSystem();

	/// <summary>
	/// Activate a free particle system slot
	/// </summary>
	/// <param name="pos"></param>
	/// <param name="life"></param>
	void Initialise(Vector2 pos, float life, SYSTEM_TYPE type);

	/// <summary>
	/// Update all stored particles, tick timer down to zero, and generate particles in apttern according to type
	/// </summary>
	/// <param name="dTime"></param>
	void Update(const float dTime);

	bool IsActive() { return m_isActive; }
};