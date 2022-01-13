#include "GameManager.h"

GameObject* GameManager::GetGameObject(string name)
{
    return m_gameObjectMap[name];
}

Timer* GameManager::GetTimer()
{
    return &m_timer;
}
