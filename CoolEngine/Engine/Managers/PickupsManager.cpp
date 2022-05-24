#include "PickupsManager.h"
#include <algorithm>
#include "GameManager.h"
#include "Engine/GameObjects/PlayerGameObject.h"

void PickupsManager::CheckList(list<string> toCheck)
{
	list<string>::iterator it;
	for (it = toCheck.begin(); it != toCheck.end(); it++)
	{
		//Only inserts if the item is not present already
		m_pFullEffectList.insert(*it);
	}


}

PickupsManager::PickupsManager()
{
	

}

void PickupsManager::Update()
{

	//If we dont have a player yet, get one
	if (m_pPlayer == nullptr)
	{
		m_pPlayer = GameManager::GetInstance()->GetGameObjectUsingIdentifier<PlayerGameObject>(string("Player"));
	}
	//If we have a player, check if we have the resource keys that the player has
	if (m_pPlayer != nullptr)
	{

		list<string> playerResourceKey = m_pPlayer->GetPlayerResources()->GetResourceKeys();
		list<string>::iterator it;

		for (it = playerResourceKey.begin(); it != playerResourceKey.end(); it++)
		{
			//Only inserts if the item is not present already
			m_pFullEffectList.insert(*it);
		}
	}
}

unordered_set<string>* PickupsManager::GetList()
{

	return &m_pFullEffectList;
}

