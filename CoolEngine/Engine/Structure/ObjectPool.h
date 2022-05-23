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
	ObjectPool()
	{

	}

	ObjectPool(int objectCount)
	{
		m_pObjects.reserve(objectCount);
		for (size_t i = 0; i < objectCount; i++)
		{
			ObjectEntry<T>* entry = new ObjectEntry<T>();
			entry->m_Active = false;
			entry->m_pObject = new T();
			m_pObjects.push_back(entry);
		}
	}

	~ObjectPool()
	{
		for (size_t i = 0; i < m_pObjects.size(); i++)
		{
			delete m_pObjects[i];
		}
	}

	ObjectEntry<T>* CreateEntryInPool()
	{
		for (size_t i = 0; i < m_pObjects.size(); i++)
		{
			if (!m_pObjects[i]->m_Active)
			{
				m_pObjects[i]->m_Active = true;
				return m_pObjects[i];
			}
		}
	}


	bool ReleaseEntryInPool(ObjectEntry<T>* entry)
	{
		for (size_t i = 0; i < m_pObjects.size(); i++)
		{
			if (m_pObjects[i] == entry)
			{
				m_pObjects[i]->m_Active = false;
				return true;
			}
		}
		return false;
	}

	const std::vector<ObjectEntry<T>*> ReturnPool() { return m_pObjects; }

private:

	std::vector<ObjectEntry<T>*> m_pObjects;

};
