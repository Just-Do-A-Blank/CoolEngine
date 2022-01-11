#pragma once

template<class T>
class Singleton
{
	static T* m_pinstance = nullptr;

public:
	static T* GetInstance()
	{
		if (m_pinstance == nullptr)
		{
			m_pinstance = new T();
		}

		return m_pinstance;
	}

};