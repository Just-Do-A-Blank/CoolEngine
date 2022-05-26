#pragma once
#include "FuzzyState.h"

class PlayerGameObject;
class EnemyGameObject;
class MeleeWeaponGameObject;

struct node;

class MeleeMovementState : public FuzzyState
{
public:
	MeleeMovementState(EnemyGameObject* penemy);
	MeleeMovementState(const nlohmann::json& data);
	MeleeMovementState(MeleeMovementState const* other, EnemyGameObject* penemy);

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

	MeleeWeaponGameObject* m_pweapon = nullptr;

	std::vector<node*> m_path;

	float m_activationDistance = 10.0f;
	float m_maxActivationDistance = 750;
	float m_nodePopDistance = 3.0f;
	float m_replanPathTime = 1.0f;
	float m_replanPathTimeStamp = 0;
};

