 #include "Particle.h"

Particle::Particle()
{
	m_velocity = XMFLOAT2(0, 0);
	m_accel = XMFLOAT2(0, 0);
	m_pTexture = nullptr;

	m_lifetime = 0.0f;
	m_isActive = false;
}

Particle::~Particle()
{
	m_pTexture = nullptr;
}

void Particle::Update(const float dTime)
{
	// Update physics
	m_velocity.x += m_accel.x * dTime;
	m_velocity.y += m_accel.y * dTime;

	XMFLOAT3 pos = m_transform.GetWorldPosition();
	pos.x += m_velocity.x * dTime;
	pos.y += m_velocity.y * dTime;
	m_transform.SetWorldPosition(pos);

	m_lifetime -= dTime;
	if (m_lifetime <= 0.0f)
	{
		m_isActive = false;
	}
}

void Particle::Render(ID3D11DeviceContext* pContext, Mesh* mesh)
{
	if (m_pTexture == nullptr)
	{
		return;
	}

	GraphicsManager::GetInstance()->RenderQuad(m_pTexture, m_transform.GetWorldPosition(), m_transform.GetWorldScale(), m_transform.GetWorldRotation().z, m_layer);
}

void Particle::Disable()
{
	m_isActive = false;
}

void Particle::Initialise(Transform trans, XMFLOAT2 vel, XMFLOAT2 accel, float life, ID3D11ShaderResourceView* tex, int layer)
{
	m_transform = trans;
	m_velocity = vel;
	m_accel = accel;
	m_lifetime = life;
	m_isActive = true;
	m_pTexture = tex;
	m_layer = layer;
}