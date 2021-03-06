#pragma once
#include "FuzzyState.h"

class PlayerGameObject;
class EnemyGameObject;
class MeleeWeaponGameObject;

class MeleeAttackState : public FuzzyState
{
public:
	MeleeAttackState(EnemyGameObject* penemy);
	MeleeAttackState(const nlohmann::json& data, EnemyGameObject* penemy);
	MeleeAttackState(MeleeAttackState const* other, EnemyGameObject* penemy);

	void SetEnemy(EnemyGameObject* penemy);

	void Enter() override;
	void Exit() override;

	float CalculateActivation() override;

#if EDITOR
	void CreateEngineUI() override;
#endif

	void Update() override;

	void Serialize(nlohmann::json& data) override;
	void Deserialize(const nlohmann::json& data) override;

	void Start() override;

protected:

private:
	PlayerGameObject* m_pplayer = nullptr;

	MeleeWeaponGameObject* m_pweapon = nullptr;

	float m_attackRangeVariance = 100.0f;

	bool m_attackedLastFrame = false;
};

