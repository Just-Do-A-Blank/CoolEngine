#pragma once
#include "Particle.h"
#include "Engine/GameObjects/GameObject.h"

#define PARTICLE_SYSTEM_SIZE 16

enum SYSTEM_TYPE
{
	SYSTEM_NONE = 0,
	SYSTEM_TEST
};

// This class will manage the movement and life of particles in a single effect. It is initialised when the effect begins and deactivated when the effect is over, but never deleted from the memory pool.
class ParticleSystem : public GameObject
{
private:
	Particle* m_pParticles[PARTICLE_SYSTEM_SIZE];

	// The box of the particle system carries over to the particles, so right now particles generated will have the same size box.
	float m_lifetime;
	float m_timer;
	bool m_isActive;
	SYSTEM_TYPE m_systemType;
	// Every particle from this system will use the same texture
	// If you would like to make every particle have a different texture, just tell me
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
	/// Update all stored particles, tick timer down to zero, and generate particles in pattern according to type
	/// </summary>
	/// <param name="dTime"></param>
	void Update(const float dTime);

	void Render(ID3D11DeviceContext* pContext, Mesh* mesh);

	/// <summary>
	/// Find slot for a new particle, and initialise it
	/// </summary>
	void AddParticle(Transform trans, XMFLOAT2 vel, XMFLOAT2 accel, float life);

	// Getters
	bool GetActive() 
	{ 
		return m_isActive; 
	}

	ID3D11ShaderResourceView* GetTexture() 
	{ 
		return m_pTexture; 
	}

	float GetLife() 
	{ 
		return m_lifetime; 
	}

	Transform* GetTrans() 
	{
		return m_transform; 
	}

	SYSTEM_TYPE	GetType() 
	{ 
		return m_systemType; 
	}

	// Setters
	void SetActive(bool active) 
	{ 
		m_isActive = active; 
	}

	void SetTexture(ID3D11ShaderResourceView* tex) 
	{ 
		m_pTexture = tex; 
	}

	void SetLife(float life) 
	{ 
		m_lifetime = life; 
	}

	void SetType(SYSTEM_TYPE type) 
	{ 
		m_systemType = type; 
	}
};