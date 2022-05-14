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
	PARTICLE_SYSTEM = 64,
    INTERACTABLE = 128,
	WEAPON = 256,
	TRIGGERABLE = 512,
	MELEE_WEAPON = 1024,
	RANGE_WEAPON = 2048,
};

DEFINE_ENUM_FLAG_OPERATORS(GameObjectType);


enum class AccumlateType
{
	BASE = 1,
	PARTICLE_SYSTEM = (int)(GameObjectType::PARTICLE_SYSTEM | GameObjectType::BASE),
	RENDERABLE  = (int)(GameObjectType::RENDERABLE | GameObjectType::BASE),
	COLLIDABLE = (int)(GameObjectType::COLLIDABLE | GameObjectType::BASE),
	COLLIDABLE_RENDERERABLE = (int)(GameObjectType::COLLIDABLE | GameObjectType::RENDERABLE | GameObjectType::BASE),
	TRIGGERABLE = (int)((int)GameObjectType::TRIGGERABLE | COLLIDABLE_RENDERERABLE),
	INTERACTABLE = (int)((int)GameObjectType::INTERACTABLE | TRIGGERABLE),
	CHARACTER = (int)((int)GameObjectType::CHARACTER | TRIGGERABLE),
	ENEMY = (int)((int)GameObjectType::ENEMY | CHARACTER),
	PLAYER = (int)((int)GameObjectType::PLAYER | CHARACTER),
	WEAPON = (int)((int)GameObjectType::WEAPON | TRIGGERABLE),
	MELEE_WEAPON = (int)((int)GameObjectType::MELEE_WEAPON | WEAPON),
	RANGE_WEAPON = (int)((int)GameObjectType::RANGE_WEAPON | WEAPON),
};


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

	void SetIdentifier(std::string identifier);
	void SetUUID(CoolUUID uuid);

public:
	GameObject();
	GameObject(string identifier, CoolUUID uuid);
	GameObject(const json& data, CoolUUID uuid);
	virtual ~GameObject();

	virtual void Update();

	virtual void Serialize(json& jsonData);

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
