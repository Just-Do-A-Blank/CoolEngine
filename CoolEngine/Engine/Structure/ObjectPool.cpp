#include "ObjectPool.h"


template<class T>
ObjectPool<T>::ObjectPool(int objectCount)
{
	m_pObjects.reserve(objectCount);
	for (size_t i = 0; i < objectCount; i++)
	{
		ObjectEntry<T> entry;
		entry.m_Active = false;
		entry.m_pObject = new T();
		m_pObjects.push_back(entry);
	}
}

template<class T>
ObjectPool<T>::~ObjectPool()
{
	for (size_t i = 0; i < objectCount; i++)
	{
		delete m_pObjects[i];
	}
}

template<class T>
ObjectEntry<T>* ObjectPool<T>::CreateEntryInPool()
{
	for (size_t i = 0; i < m_pObjects.size(); i++)
	{
		if (!m_pObjects[i].m_Active)
		{
			m_pObjects[i].m_Active = true;
			return	m_pObjects[i];
		}
	}
}

template<class T>
bool ObjectPool<T>::ReleaseEntryInPool(ObjectEntry<T>* entry)
{
	for (size_t i = 0; i < m_pObjects.size(); i++)
	{
		if (m_pObjects[i] == entry)
		{
			m_pObjects[i].m_Active = false;
			return	true;
		}
	}
	return false;
}
