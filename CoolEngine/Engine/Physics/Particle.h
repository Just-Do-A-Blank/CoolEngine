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
	//Box m_box;
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

	
	void Render(ID3D11DeviceContext* pContext, ConstantBuffer<PerInstanceCB>* pConstantBuffer, Mesh* mesh);

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

	bool IsActive() { return m_isActive; }
};