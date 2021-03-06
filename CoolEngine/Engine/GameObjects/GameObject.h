#pragma once
#include "Transform.h"
#include "Engine/GameObjects/UUID.h"
#include  "Engine/Includes/json.hpp"
#define FILEPATH_BUFFER_SIZE 200
#define ANIM_NAME_SIZE 50

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
	BULLET = 4096,
	GAME_UI_COMPONENT = 8192,
	CAMERA = 16384,
	LEVEL_CHANGE = 32768,
	TILE_MAP = 65536,
	PICKUP = 131072,
	SOUND = 262144,
};

DEFINE_ENUM_FLAG_OPERATORS(GameObjectType);


enum class AccumlateType
{
	BASE = 1,
	PARTICLE_SYSTEM = (int)(GameObjectType::PARTICLE_SYSTEM | GameObjectType::BASE),
	RENDERABLE  = (int)(GameObjectType::RENDERABLE | GameObjectType::BASE),
	COLLIDABLE = (int)(GameObjectType::COLLIDABLE | GameObjectType::BASE),
	COLLIDABLE_RENDERERABLE = (int)(GameObjectType::COLLIDABLE | GameObjectType::RENDERABLE | GameObjectType::BASE),
	TRIGGERABLE = ((int)GameObjectType::TRIGGERABLE | COLLIDABLE_RENDERERABLE),
	INTERACTABLE = ((int)GameObjectType::INTERACTABLE | TRIGGERABLE),
	CHARACTER = ((int)GameObjectType::CHARACTER | TRIGGERABLE),
	ENEMY = ((int)GameObjectType::ENEMY | CHARACTER),
	PLAYER = ((int)GameObjectType::PLAYER | CHARACTER),
	WEAPON = ((int)GameObjectType::WEAPON | TRIGGERABLE),
	MELEE_WEAPON = ((int)GameObjectType::MELEE_WEAPON | WEAPON),
	RANGE_WEAPON = ((int)GameObjectType::RANGE_WEAPON | WEAPON),
	BULLET = ((int)GameObjectType::BULLET | WEAPON),
	CAMERA = ((int)GameObjectType::CAMERA | BASE),
	UI_COMPONENT = ((int)GameObjectType::GAME_UI_COMPONENT | BASE),
	LEVEL_CHANGE = ((int)GameObjectType::LEVEL_CHANGE | TRIGGERABLE),
	TILE_MAP = ((int)GameObjectType::TILE_MAP | RENDERABLE),
	PICKUP = ((int)GameObjectType::PICKUP | INTERACTABLE),
	SOUND = ((int)GameObjectType::SOUND | TRIGGERABLE)
};


class GameObject : public EditorUIComponent
{
	friend class GameManager;
	friend class Scene;
	friend class FileIO;
private:

protected:

	Transform* m_transform;
	string m_identifier;
	CoolUUID m_UUID;
	bool m_isEnabled;

	GameObject* m_pTest = nullptr;

	bool m_isStatic = false;

#if EDITOR
	virtual void CreateEngineUI() override;
#endif

	GameObjectType m_gameObjectType = (GameObjectType)0;

	void SetIdentifier(std::string identifier);
	void SetUUID(CoolUUID uuid);

public:
	GameObject();
	GameObject(GameObject const& other);
	GameObject(string identifier, CoolUUID uuid);
	GameObject(const nlohmann::json& data, CoolUUID uuid);
	virtual ~GameObject();

	/// <summary>
	/// Called after construction, before first Update.
	/// </summary>
	virtual void Start();

	virtual void Update();
	virtual void EditorUpdate();

	virtual void Serialize(nlohmann::json& jsonData);

	void Init(const nlohmann::json& data, CoolUUID uuid);
	void Init(GameObject const& other);

	void SetEnabled(bool isEnabled) { m_isEnabled = isEnabled; }
	bool GetEnabled() { return m_isEnabled; }

#if EDITOR
	virtual void ShowEngineUI();
#endif

	//Getters
	Transform* GetTransform() const;
	const GameObjectType& GetGameObjectType() const;

	string& GetIdentifier();
	const CoolUUID& GetUUID();

	//Setters



	bool ContainsType(GameObjectType type);
};
