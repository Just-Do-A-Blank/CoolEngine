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

	void SetEnemy(EnemyGameObject* penemy);

	void Enter() override;
	void Exit() override;

	float CalculateActivation() override;

	void Update() override;

	void Serialize(nlohmann::json& data) override;
	void Deserialize(const nlohmann::json& data) override;

protected:

private:
	PlayerGameObject* m_pplayer = nullptr;
	EnemyGameObject* m_penemy = nullptr;

	MeleeWeaponGameObject* m_pweapon = nullptr;

	std::vector<node*> m_path;

	float m_activationDistanceSq = 100.0f;
	float m_maxActivationDistanceSq = 90000;
	float m_nodePopDistanceSq = 10.0f;
	float m_replanPathTime = 1.0f;
	float m_replanPathTimeStamp = 0;
};

