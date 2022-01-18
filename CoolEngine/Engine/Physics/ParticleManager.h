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
	/// <param name="pconstantBuffer"></param>
	void Render(ID3D11DeviceContext* pContext, ConstantBuffer<PerInstanceCB>* pConstantBuffer);

	/// <summary>
	/// Find slot for a new system, and initialise it
	/// </summary>
	/// <param name="trans"></param>
	/// <param name="life"></param>
	/// <param name="type"></param>
	/// <param name="tex"></param>
	void AddSystem(Transform trans, float life, SYSTEM_TYPE type, wstring albedoName);

	bool SetVertexShader(wstring shaderName);
	bool SetPixelShader(wstring shaderName);
	bool SetMesh(wstring meshName);
};