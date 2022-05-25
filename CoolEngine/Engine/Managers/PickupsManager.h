#pragma once
#include <unordered_set>
#include "Engine/Structure/Singleton.h"

class PlayerGameObject;
class PickupsManager : public Singleton<PickupsManager>
{
public:
	void Update();
	unordered_set<string>* GetList();
	void CheckList(list<string>toCheck);

	PickupsManager();
	~PickupsManager() {}

private:

	unordered_set<string> m_pFullEffectList;
	PlayerGameObject* m_pPlayer = nullptr;
};

