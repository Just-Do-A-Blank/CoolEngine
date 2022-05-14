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
	TRIGGERABLE = 512,
	COUNT
};

DEFINE_ENUM_FLAG_OPERATORS(GameObjectType);


enum class AccumlateType
{
	BASE = 1,
	RENDERABLE  = (int)(GameObjectType::RENDERABLE | GameObjectType::BASE),
	COLLIDABLE = (int)(GameObjectType::COLLIDABLE | GameObjectType::BASE),
	COLLIDABLE_RENDERERABLE = (int)(GameObjectType::COLLIDABLE | GameObjectType::RENDERABLE | GameObjectType::BASE),
	CHARACTER = (int)(GameObjectType::CHARACTER | GameObjectType::COLLIDABLE | GameObjectType::RENDERABLE | GameObjectType::BASE),
	ENEMY = (int)(GameObjectType::ENEMY | GameObjectType::CHARACTER | GameObjectType::COLLIDABLE | GameObjectType::RENDERABLE | GameObjectType::BASE),
	PLAYER = (int)(GameObjectType::PLAYER | GameObjectType::CHARACTER | GameObjectType::COLLIDABLE | GameObjectType::RENDERABLE | GameObjectType::BASE),
	PARTICLESYSTEM = (int)(GameObjectType::PARTICLESYSTEM | GameObjectType::BASE),
	INTERACTABLE = (int)(GameObjectType::INTERACTABLE | GameObjectType::TRIGGERABLE | GameObjectType::BASE),
	WEAPON = (int)(GameObjectType::WEAPON | GameObjectType::TRIGGERABLE | GameObjectType::BASE),
	TRIGGERABLE = (int)(GameObjectType::TRIGGERABLE| GameObjectType::COLLIDABLE | GameObjectType::RENDERABLE| GameObjectType::BASE),
	COUNT = (int)(GameObjectType::COUNT | GameObjectType::TRIGGERABLE | GameObjectType::WEAPON | GameObjectType::INTERACTABLE | GameObjectType::PARTICLESYSTEM | GameObjectType::PLAYER | GameObjectType::ENEMY | GameObjectType::CHARACTER | GameObjectType::COLLIDABLE | GameObjectType::RENDERABLE | GameObjectType::BASE)
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
	GameObject(json data, CoolUUID index); 
	virtual ~GameObject();

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
