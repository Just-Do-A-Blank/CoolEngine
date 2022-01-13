#include "Particle.h"

Particle::Particle()
{
	m_box = Box();
	m_velocity = XMFLOAT2(0, 0);
	m_accel = XMFLOAT2(0, 0);

	m_pTexture = nullptr;
	m_lifetime = 0.0f;
	m_isActive = false;
}

Particle::~Particle()
{
	delete m_pTexture;
}

void Particle::Update(const float dTime)
{
	// Update physics
	m_velocity.x += m_accel.x * dTime;
	m_velocity.y += m_accel.y * dTime;

	m_box.m_x += m_velocity.x * dTime;
	m_box.m_y += m_velocity.y * dTime;

	m_lifetime -= dTime;
	if (m_lifetime <= 0.0f)
	{
		m_isActive = false;
	}
}

void Particle::Disable()
{
	m_isActive = false;
}

void Particle::Initialise(Box box, XMFLOAT2 vel, XMFLOAT2 accel, Texture* tex, float life)
{
	m_box = box;
	m_velocity = vel;
	m_accel = accel;

	m_pTexture = tex;
	m_lifetime = life;
	m_isActive = true;
}