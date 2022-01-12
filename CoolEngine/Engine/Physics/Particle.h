#pragma once
#include "Box.h"
#include "Vector2.h"

// Placeholder
class Texture {};

// A sprite that disappears over time
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

	/// <summary>
	/// Activate a free particle slot
	/// </summary>
	/// <param name="box"></param>
	/// <param name="vel"></param>
	/// <param name="accel"></param>
	/// <param name="tex"></param>
	/// <param name="life"></param>
	void Initialise(Box box, Vector2 vel, Vector2 accel, Texture* tex, float life);

	bool IsActive() { return m_isActive; }
};