#pragma once
#include "Transform.h"
#include "Engine/GameObjects/UUID.h"
#include  "Engine/Includes/json.hpp"
#define FILEPATH_BUFFER_SIZE 200
#define ANIM_NAME_SIZE 50



using namespace nlohmann;

enum class GameObjectType
{
	BASE = 1,
	RENDERABLE = 2,
	COLLIDABLE = 4,
	CHARACTER = 8,
	ENEMY = 16,
	PLAYER = 32,
	PARTICLESYSTEM = 64,
    INTERACTABLE = 128,
	WEAPON = 256,
	COUNT
};

DEFINE_ENUM_FLAG_OPERATORS(GameObjectType);

class GameObject : public EditorUIComponent
{
	friend class GameManager;
	friend class Scene;
	friend class FileIO;
private:
	string m_identifier;
	CoolUUID m_UUID;

protected:

	Transform* m_transform;

	GameObject* m_pTest = nullptr;

#if EDITOR
	virtual void CreateEngineUI() override;
#endif

	GameObjectType m_gameObjectType = (GameObjectType)0;

public:
	GameObject();
	GameObject(string identifier, CoolUUID uuid);

	virtual void Update();

	virtual void Serialize(json& jsonData);
	virtual void Deserialize(json& jsonData);

#if EDITOR
	virtual void ShowEngineUI();
#endif

	//Getters
	Transform* GetTransform();
	const GameObjectType& GetGameObjectType() const;

	const string& GetIdentifier();

	//Setters



	bool ContainsType(GameObjectType type);
};
