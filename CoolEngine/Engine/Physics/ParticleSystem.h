#pragma once
#include "Particle.h"
#include "Engine/GameObjects/GameObject.h"

class ParticleManager;

// This class will manage the movement and life of particles in a single effect. It is initialised when the effect begins and deactivated when the effect is over, but never deleted from the memory pool.
class ParticleSystem : public GameObject
{
private:
	// The transform of the particle system carries over to the particles, so right now particles generated will have the same size.
	float m_systemLife;
	float m_timer;
	bool m_isActive;

	// Used to initialise particles
	XMFLOAT2 m_velocity;
	XMFLOAT2 m_accel;
	float m_particleLife;
	float m_spawnInterval;
	int m_spawnNumber;
	ID3D11ShaderResourceView* m_pTexture;

	// Randomness factors
	float m_positionRand;
	float m_velocityRand;
	float m_accelRand;
	float m_lifeRand;

	wstring m_texFilepath;

public:
	ParticleSystem(string identifier);
	~ParticleSystem();

	/// <summary>
	/// Activate a free particle system slot
	/// </summary>
	/// <param name="trans"></param>
	/// <param name="life"></param>
	/// <param name="tex"></param>
	void Initialise(Transform trans, float life, ID3D11ShaderResourceView* tex);

	/// <summary>
	/// Activate a free particle system slot, and set particle properties
	/// </summary>
	/// <param name="trans"></param>
	/// <param name="life"></param>
	/// <param name="tex"></param>
	/// <param name="vel"></param>
	/// <param name="accel"></param>
	/// <param name="partLife"></param>
	/// <param name="interval"></param>
	/// <param name="number"></param>
	/// <param name="randPos"></param>
	/// <param name="randVel"></param>
	/// <param name="randAccel"></param>
	/// <param name="randLife"></param>
	void Initialise(Transform trans, float life, ID3D11ShaderResourceView* tex, XMFLOAT2 vel, XMFLOAT2 accel, float partLife, float interval, int number, float randPos, float randVel, float randAccel, float randLife);

	/// <summary>
	/// Set the properties used to create particles
	/// </summary>
	/// <param name="vel"></param>
	/// <param name="accel"></param>
	/// <param name="life"></param>
	/// <param name="interval"></param>
	/// <param name="number"></param>
	void SetProperties(XMFLOAT2 vel, XMFLOAT2 accel, float life, float interval, int number);

	void SetRandomness(float randPos, float randVel, float randAccel, float randLife);

	/// <summary>
	/// Tick timer down to zero, and generate particles in pattern according to type
	/// </summary>
	/// <param name="dTime"></param>
	void Update(const float dTime);

#if EDITOR
	void CreateEngineUI() override;
#endif

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
		return m_systemLife;
	}

	Transform* GetTrans() 
	{
		return m_transform; 
	}

	XMFLOAT2 GetVelocity() 
	{ 
		return m_velocity; 
	}

	XMFLOAT2 GetAccel() 
	{ 
		return m_accel; 
	}

	float GetParticleLife()
	{
		return m_particleLife;
	}

	float GetSpawnInterval()
	{
		return m_spawnInterval;
	}

	float GetSpawnNumber()
	{
		return m_spawnNumber;
	}

	float GetRandomPos()
	{
		return m_positionRand;
	}

	float GetRandomVelocity()
	{
		return m_velocityRand;
	}

	float GetRandomAccel()
	{
		return m_accelRand;
	}

	float GetRandomLife()
	{
		return m_lifeRand;
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
		m_systemLife = life;
	}

	void SetVelocity(XMFLOAT2 vel)
	{
		m_velocity = vel;
	}

	void SetAccel(XMFLOAT2 acc)
	{
		m_accel = acc;
	}

	void SetParticleLife(float life)
	{
		m_particleLife = life;
	}

	void SetSpawnInterval(float interval)
	{
		m_spawnInterval = interval;
	}

	void SetSpawnNumber(float num)
	{
		m_spawnNumber = num;
	}

	void SetRandomPos(float rand)
	{
		m_positionRand = rand;
	}

	void SetRandomVelocity(float rand)
	{
		m_velocityRand = rand;
	}

	void SetRandomAccel(float rand)
	{
		m_accelRand = rand;
	}

	void SetRandomLife(float rand)
	{
		m_lifeRand = rand;
	}
};