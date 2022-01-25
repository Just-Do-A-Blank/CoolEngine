#pragma once
#include "Transform.h"
#include "Engine/Graphics/Mesh.h"
#include "Engine/Graphics/ConstantBuffer.h"
#include "Engine/Graphics/ConstantBuffers.h"
#include "Engine/Graphics/SpriteAnimation.h"
#include "Engine/Structure/EditorUIComponent.h"
#include "Engine/ResourceDefines.h"

#include <string>

class SpriteAnimation;
class Shape;


struct RenderStruct;

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


	void SetShape(Shape* collider);


	bool ContainsType(GameObjectType type);
};
