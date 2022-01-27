#pragma once
#include "ParticleSystem.h"

#define PARTICLE_MANAGER_SIZE 32

// This class holds all the particle systems. It will be initialised at the start and removed at the end.
class ParticleManager
{
private:
	ParticleSystem* m_pParticleSystems[PARTICLE_MANAGER_SIZE];

	// These are used by every particle
	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader* m_pPixelShader;
	Mesh* m_pMesh;

public:
	ParticleManager(wstring meshName, wstring vShaderName, wstring pShaderName);
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
	void AddSystem(Transform trans, float life, wstring albedoName, XMFLOAT2 vel, XMFLOAT2 accel, float partLife, float interval, float number);

	/// <summary>
	/// Find slot for a new system, and initialise it
	/// </summary>
	/// <param name="system"></param>
	void AddSystem(ParticleSystem* system);

	// Getters
	ParticleSystem* GetSystem(int index) 
	{ 
		return m_pParticleSystems[index]; 
	}
};