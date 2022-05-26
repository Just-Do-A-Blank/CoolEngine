#pragma once
#include "FuzzyState.h"

class PlayerGameObject;
class EnemyGameObject;
class RangedWeaponGameObject;

struct node;

class RangeMovementState : public FuzzyState
{
public:
	RangeMovementState(EnemyGameObject* penemy);

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

	RangedWeaponGameObject* m_prangeWeapon = nullptr;

	std::vector<node*> m_path;

	float m_maxActivationDistance = 750;
	float m_nodePopDistance = 3.0f;
	float m_replanPathTime = 1.0f;
	float m_replanPathTimeStamp = 0;
	float m_upperOptimalDistanceMultiplier = 0.8f;
	float m_lowerOptimalDistanceMultiplier = 0.6f;

	XMFLOAT3 CalculateTargetPosition() const;
};

