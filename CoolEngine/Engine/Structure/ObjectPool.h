#pragma once
#include "Engine/GameObjects/UUID.h"
#include <vector>

template<class T>
struct ObjectEntry
{
	T* m_pObject;
	bool m_Active;
	CoolUUID m_ID;
};

template<class T>
class ObjectPool
{
public:
	ObjectPool();
	ObjectPool(int objectCount);
	~ObjectPool();
	
	ObjectEntry* CreateEntryInPool(T* entry);
	ObjectEntry* CreateEntryInPool(T* entry, CoolUUID ID);

	bool RemoveEntryInPool(ObjectEntry entry);
	bool RemoveEntryInPool(CoolUUID ID);

	bool SetObjectActiveValue(ObjectEntry entry, bool value);
	void SetObjectActiveValue(CoolUUID ID, bool value);

private:

	std::vector<ObjectEntry*> m_pObjects;

};
