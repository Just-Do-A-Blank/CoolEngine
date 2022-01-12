#pragma once

template<class T>
class Singleton
{
	static T* s_pinstance;

public:
	static T* GetInstance()
	{
		if (s_pinstance == nullptr)
		{
			s_pinstance = new T();
		}

		return s_pinstance;
	}

};

template <class T>
T* Singleton<T>::s_pinstance = nullptr;