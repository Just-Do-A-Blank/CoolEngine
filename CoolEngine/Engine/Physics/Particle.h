#pragma once
#include "Box.h"
#include "Vector2.h"

// Placeholder
class Texture {};

class Particle
{
private:
	Box m_box;
	Vector2 m_velocity;
	Vector2 m_accel;

	Texture* m_pTexture;
	float m_lifetime;
	bool m_isActive;

public:
	Particle();
	Particle(Box box, Vector2 vel, Vector2 accel, Texture* tex, float life);
	~Particle();

	/// <summary>
	/// Move particle according to velocity/acceleration, tick timer down to zero
	/// </summary>
	/// <param name="dTime"></param>
	void Update(const float dTime);

	/// <summary>
	/// Mark the particle as free
	/// </summary>
	void Disable();
};