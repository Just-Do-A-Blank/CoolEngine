#pragma once
#include "Engine/GameObjects/CharacterGameObject.h"

class PlayerGameObject : public CharacterGameObject
{
public:

	PlayerGameObject(string identifier, CoolUUID uuid);
	PlayerGameObject(json data, int index);
	~PlayerGameObject();

	void Serialize(json& jsonData);
	void Deserialize(json& jsonData);
protected:

private:

};