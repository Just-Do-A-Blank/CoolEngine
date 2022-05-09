#pragma once
#include "Engine/GameObjects/EnemyGameObject.h"
//#include "Engine/GameObjects/GameObject.h"

class Enemy : virtual public EnemyGameObject

{
protected:

	float m_moveSpeed = 100.0f;

	float m_health = 42.0f;

	XMFLOAT3 m_direction = XMFLOAT3{ 0, 0, 0 };

	bool m_ranged = false;
	bool m_melee = false;

	GameObject PlayerRef;

public:

	Enemy(string identifier);
	~Enemy();

	virtual void Update();

	float GetMoveSpeed() { return m_moveSpeed; }
	void SetSpeed(float speed) { m_moveSpeed = speed; }


#if EDITOR
	virtual void CreateEngineUI() override;
#endif

};

