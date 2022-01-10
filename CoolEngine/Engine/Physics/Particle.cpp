#include "Particle.h"

Particle::Particle()
{
	m_box = Box();
	m_velocity = Vector2();
	m_accel = Vector2();

	m_pTexture = nullptr;
	m_lifetime = 0.0f;
	m_isActive = false;
}

Particle::Particle(Box box, Vector2 vel, Vector2 accel, Texture* tex, float life)
{
	m_box = box;
	m_velocity = vel;
	m_accel = accel;

	m_pTexture = tex;
	m_lifetime = life;
	m_isActive = true;
}

Particle::~Particle()
{
	delete m_pTexture;
}

void Particle::Update(const float dTime)
{
	// Update physics
	m_velocity.m_x += m_accel.m_x * dTime;
	m_velocity.m_y += m_accel.m_y * dTime;

	m_box.m_x += m_velocity.m_x * dTime;
	m_box.m_y += m_velocity.m_y * dTime;

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