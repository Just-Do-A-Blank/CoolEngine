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
	Transform m_transform;
	float m_lifetime;
	float m_timer;
	bool m_isActive;
	SYSTEM_TYPE m_systemType;
	// Every particle from this system will use the same texture
	ID3D11ShaderResourceView* m_pTexture;

public:
	ParticleSystem();
	~ParticleSystem();

	/// <summary>
	/// Activate a free particle system slot
	/// </summary>
	/// <param name="box"></param>
	/// <param name="life"></param>
	/// <param name="type"></param>
	/// <param name="tex"></param>
	void Initialise(Transform trans, float life, SYSTEM_TYPE type, ID3D11ShaderResourceView* tex);

	/// <summary>
	/// Update all stored particles, tick timer down to zero, and generate particles in apttern according to type
	/// </summary>
	/// <param name="dTime"></param>
	void Update(const float dTime);

	void Render(ID3D11DeviceContext* pContext, ConstantBuffer<PerInstanceCB>* pConstantBuffer, Mesh* mesh);

	/// <summary>
	/// Find slot for a new particle, and initialise it
	/// </summary>
	void AddParticle(Transform trans, XMFLOAT2 vel, XMFLOAT2 accel, ID3D11ShaderResourceView* tex, float life);

	bool IsActive() { return m_isActive; }
};