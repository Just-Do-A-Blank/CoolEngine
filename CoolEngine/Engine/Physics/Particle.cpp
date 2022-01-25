 #include "Particle.h"

Particle::Particle()
{
	m_velocity = XMFLOAT2(0, 0);
	m_accel = XMFLOAT2(0, 0);

	m_lifetime = 0.0f;
	m_isActive = false;
}

Particle::~Particle()
{

}

void Particle::Update(const float dTime)
{
	// Update physics
	m_velocity.x += m_accel.x * dTime;
	m_velocity.y += m_accel.y * dTime;

	XMFLOAT3 pos = m_transform.GetPosition();
	pos.x += m_velocity.x * dTime;
	pos.y += m_velocity.y * dTime;
	//pos.z = 5.0f;
	m_transform.SetPosition(pos);

	m_lifetime -= dTime;
	if (m_lifetime <= 0.0f)
	{
		m_isActive = false;
	}
}

void Particle::Render(ID3D11DeviceContext* pContext, Mesh* mesh)
{
	// Update CB
	PerInstanceCB cb;
	XMStoreFloat4x4(&cb.world, XMMatrixTranspose(m_transform.GetWorldMatrix()));

	//pConstantBuffer->Update(cb, pContext);
	GraphicsManager::GetInstance()->m_pperInstanceCB->Update(cb, pContext);
	ID3D11Buffer* pcbBuffer = GraphicsManager::GetInstance()->m_pperInstanceCB->GetBuffer();

	//ID3D11Buffer* pcbBuffer = pConstantBuffer->GetBuffer();

	pContext->VSSetConstantBuffers((int)GraphicsManager::CBOrders::PER_INSTANCE, 1, &pcbBuffer);
	pContext->PSSetConstantBuffers((int)GraphicsManager::CBOrders::PER_INSTANCE, 1, &pcbBuffer);

	// Draw object
	pContext->DrawIndexed(mesh->GetIndexCount(), 0, 0);
}

void Particle::Disable()
{
	m_isActive = false;
}

void Particle::Initialise(Transform trans, XMFLOAT2 vel, XMFLOAT2 accel, float life)
{
	m_transform = trans;
	m_velocity = vel;
	m_accel = accel;

	m_lifetime = life;
	m_isActive = true;
}