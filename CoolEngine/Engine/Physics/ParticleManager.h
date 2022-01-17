#pragma once
#include "ParticleSystem.h"

#define PARTICLE_MANAGER_SIZE 32

// This class holds all the particle systems. It will be initialised at the start and removed at the end.
class ParticleManager
{
private:
	ParticleSystem* m_pParticleSystems[PARTICLE_MANAGER_SIZE];

public:
	ParticleManager();
	~ParticleManager();

	/// <summary>
	/// Update all stored systems
	/// </summary>
	/// <param name="dTime"></param>
	void Update(const float dTime);

	/// <summary>
	/// Find slot for a new system, and initialise it
	/// </summary>
	/// <param name="pos"></param>
	/// <param name="life"></param>
	/// <param name="type"></param>
	void AddSystem(Box box, float life, SYSTEM_TYPE type);
};