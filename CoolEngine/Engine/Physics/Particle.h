#pragma once
//#include "Box.h"
#include "Engine/GameObjects/Transform.h"
#include <Engine/Graphics/Mesh.h>
#include <Engine/Graphics/ConstantBuffer.h>
#include <Engine/Graphics/ConstantBuffers.h>
#include <Engine/Managers/GraphicsManager.h>

// A sprite that disappears over time
class Particle
{
private:
	Transform m_transform;
	XMFLOAT2 m_velocity;
	XMFLOAT2 m_accel;

	float m_lifetime;
	bool m_isActive;

public:
	Particle();
	~Particle();

	/// <summary>
	/// Move particle according to velocity/acceleration, tick timer down to zero
	/// </summary>
	/// <param name="dTime"></param>
	void Update(const float dTime);

	void Render(ID3D11DeviceContext* pContext, Mesh* mesh);

	/// <summary>
	/// Mark the particle as free
	/// </summary>
	void Disable();

	/// <summary>
	/// Activate a free particle slot
	/// </summary>
	/// <param name="trans"></param>
	/// <param name="vel"></param>
	/// <param name="accel"></param>
	/// <param name="life"></param>
	void Initialise(Transform trans, XMFLOAT2 vel, XMFLOAT2 accel, float life);

	// Getters
	bool		GetActive() { return m_isActive; }
	XMFLOAT2	GetVelocity() { return m_velocity; }
	XMFLOAT2	GetAccel() { return m_accel; }
	float		GetLife() { return m_lifetime; }
	Transform	GetTrans() { return m_transform; }

	// Setters
	void SetActive(bool active) { m_isActive = active; }
	void SetVelocity(XMFLOAT2 vel) { m_velocity = vel; }
	void SetAccel(XMFLOAT2 acc) { m_accel = acc; }
	void SetLife(float life) { m_lifetime = life; }
	void SetPosition(XMFLOAT3 pos) { m_transform.SetPosition(pos); }
	void SetScale(XMFLOAT3 scale) { m_transform.SetScale(scale); }
};