#pragma once
#include "Engine/GameObjects/UUID.h"
#include <vector>

template<class T>
struct ObjectEntry
{
	T* m_pObject;
	bool m_Active;
};

template<class T>
class ObjectPool
{
public:
	ObjectPool(int objectCount);
	~ObjectPool();
	
	ObjectEntry<T>* CreateEntryInPool();

	bool ReleaseEntryInPool(ObjectEntry<T>* entry);

	const std::vector<ObjectEntry<T>*> ReturnPool() { return &m_pObjects; }

private:

	std::vector<ObjectEntry<T>*> m_pObjects;

};
