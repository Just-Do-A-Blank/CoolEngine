#include "ObjectPool.h"


template<class T>
ObjectPool<T>::ObjectPool()
{
}

template<class T>
ObjectPool<T>::ObjectPool(int objectCount)
{
	m_pObjects.reserve(objectCount);
}

template<class T>
ObjectPool<T>::~ObjectPool()
{
}

template<class T>
ObjectEntry<T>* ObjectPool<T>::CreateEntryInPool(T* entry)
{
	ObjectEntry entry;
	entry.m_pObject = entry;
	entry.m_Active = true;
	entry.m_ID = -1;
	m_pObjects.push_back(entry);
	return ObjectEntry;
}

template<class T>
ObjectEntry<T>* ObjectPool<T>::CreateEntryInPool(T* entry, CoolUUID ID)
{
	ObjectEntry entry;
	entry.m_pObject = entry;
	entry.m_Active = true;
	entry.m_ID = -1;
	m_pObjects.push_back(entry);
	return ObjectEntry;
}

template<class T>
bool ObjectPool<T>::RemoveEntryInPool(ObjectEntry<T> entry)
{
	for (size_t i = 0; i < m_pObjects.size(); i++)
	{
		if (m_pObjects[i] == entry)
		{
			delete	m_pObjects[i];
		}
	}
}

template<class T>
bool ObjectPool<T>::RemoveEntryInPool(CoolUUID ID)
{
	for (size_t i = 0; i < m_pObjects.size(); i++)
	{
		if (m_pObjects[i].m_ID == ID)
		{
			delete	m_pObjects[i];
		}
	}
}

template<class T>
bool ObjectPool<T>::SetObjectActiveValue(ObjectEntry<T> entry, bool value)
{
	for (size_t i = 0; i < m_pObjects.size(); i++)
	{
		if (m_pObjects[i] == entry)
		{
			m_pObjects[i].m_Active = value;
		}
	}
}

template<class T>
void ObjectPool<T>::SetObjectActiveValue(CoolUUID ID, bool value)
{
	for (size_t i = 0; i < m_pObjects.size(); i++)
	{
		if (m_pObjects[i] == m_ID)
		{
			m_pObjects[i].m_Active = value;
		}
	}
}

