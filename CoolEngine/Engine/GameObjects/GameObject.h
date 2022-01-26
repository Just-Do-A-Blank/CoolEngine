#pragma once
#include "Transform.h"

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

protected:
	Transform* m_transform;

#if EDITOR
	virtual void CreateEngineUI() override;
#endif

	GameObjectType m_gameObjectType = (GameObjectType)0;

public:
	GameObject();
	GameObject(string identifier);

	virtual void Update();

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
