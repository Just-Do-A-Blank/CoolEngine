#pragma once
#include "ParticleSystem.h"
#include "Engine/Structure/Singleton.h"

#define PARTICLE_MANAGER_SIZE 32
#define PARTICLE_SIZE 1024

// This class holds all the particle systems. It will be initialised at the start and removed at the end.
class ParticleManager : public Singleton<ParticleManager>
{
private:
	Particle* m_pParticles[PARTICLE_SIZE];
	ParticleSystem* m_pParticleSystems[PARTICLE_MANAGER_SIZE];

	// These are used by every particle
	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader* m_pPixelShader;
	Mesh* m_pMesh;

public:
	ParticleManager();
	~ParticleManager();

	/// <summary>
	/// Update all stored systems
	/// </summary>
	/// <param name="dTime"></param>
	void Update(const float dTime);

	/// <summary>
	/// Draw all the particle systems, set up buffers
	/// </summary>
	/// <param name="pContext"></param>
	void Render(ID3D11DeviceContext* pContext);

	/// <summary>
	/// Find slot for a new system, and initialise it
	/// </summary>
	/// <param name="trans"></param>
	/// <param name="life"></param>
	/// <param name="albedoName"></param>
	/// <param name="vel"></param>
	/// <param name="accel"></param>
	/// <param name="partLife"></param>
	/// <param name="interval"></param>
	/// <param name="number"></param>
	/// <param name="randPos"></param>
	/// <param name="randVel"></param>
	/// <param name="randAccel"></param>
	/// <param name="randLife"></param>
	void AddSystem(Transform trans, float life, wstring albedoName, XMFLOAT2 vel, XMFLOAT2 accel, float partLife, float interval, float number, float randPos, float randVel, float randAccel, float randLife);

	/// <summary>
	/// Find slot for a new system, and initialise it
	/// </summary>
	/// <param name="system"></param>
	void AddSystem(ParticleSystem* system);

	/// <summary>
	/// Find slot for a new particle, and initialise it
	/// </summary>
	/// <param name="trans"></param>
	/// <param name="vel"></param>
	/// <param name="accel"></param>
	/// <param name="life"></param>
	/// <param name="randPos"></param>
	/// <param name="randVel"></param>
	/// <param name="randAccel"></param>
	/// <param name="randLife"></param>
	void AddParticle(Transform trans, float life, ID3D11ShaderResourceView* tex, XMFLOAT2 vel, XMFLOAT2 accel, float randPos, float randVel, float randAccel, float randLife);

	// Getters
	ParticleSystem* GetSystem(int index) 
	{ 
		return m_pParticleSystems[index]; 
	}

	Particle* GetParticle(int index)
	{
		return m_pParticles[index];
	}
};