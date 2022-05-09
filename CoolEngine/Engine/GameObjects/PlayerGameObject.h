#pragma once
#include "Engine/GameObjects/CharacterGameObject.h"

class PlayerGameObject : public CharacterGameObject
{
public:

	PlayerGameObject(string identifier, CoolUUID uuid);
	~PlayerGameObject();

	void Serialize(json& jsonData);
	void Deserialize(json& jsonData);
protected:

private:

};