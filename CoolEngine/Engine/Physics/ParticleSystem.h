#pragma once
#include "Particle.h"

#define PARTICLE_SYSTEM_SIZE 16

enum SYSTEM_TYPE
{
	SYSTEM_NONE = 0,
	SYSTEM_TEST
};

// This class will manage the movement and life of particles in a single effect. It is initialised when the effect begins and deactivated when the effect is over, but never deleted from the memory pool.
class ParticleSystem
{
private:
	Particle* m_pParticles[PARTICLE_SYSTEM_SIZE];

	// The box of the particle system carries over to the particles, so right now particles generated will have the same size box.
	Box m_box;
	float m_lifetime;
	float m_timer;
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
	void Initialise(Box box, float life, SYSTEM_TYPE type);

	/// <summary>
	/// Update all stored particles, tick timer down to zero, and generate particles in apttern according to type
	/// </summary>
	/// <param name="dTime"></param>
	void Update(const float dTime);

	/// <summary>
	/// Find slot for a new particle, and initialise it
	/// </summary>
	void AddParticle(Box box, Vector2 vel, Vector2 accel, Texture* tex, float life);

	bool IsActive() { return m_isActive; }
};