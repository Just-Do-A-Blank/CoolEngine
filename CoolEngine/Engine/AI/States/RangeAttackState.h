#pragma once
#include "FuzzyState.h"

class EnemyGameObject;
class PlayerGameObject;
class RangedWeaponGameObject;

class RangeAttackState : public FuzzyState
{
public:
	RangeAttackState(EnemyGameObject* penemy);
	RangeAttackState(const nlohmann::json& data);
	RangeAttackState(RangeAttackState const* other, EnemyGameObject* penemy);

	void SetEnemy(EnemyGameObject* penemy);

	void Enter() override;
	void Exit() override;

	float CalculateActivation() override;

	void Update() override;

	void Serialize(nlohmann::json& data) override;
	void Deserialize(const nlohmann::json& data) override;

#if EDITOR
	void CreateEngineUI() override;
#endif

	void Start() override;

protected:

private:
	PlayerGameObject* m_pplayer = nullptr;
	EnemyGameObject* m_penemy = nullptr;

	RangedWeaponGameObject* m_pweapon = nullptr;

	float m_attackRangeVariance = 100.0f;
};

