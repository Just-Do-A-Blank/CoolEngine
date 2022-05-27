#pragma once
#include "Singleton.h"
#include "Engine/Includes/json.hpp"

template<class T>
class Manager : public Singleton<T>
{
public:
	virtual void Serialize(nlohmann::json& data) = 0;
	virtual void Deserialize(nlohmann::json& data) = 0;

protected:

private:

};

