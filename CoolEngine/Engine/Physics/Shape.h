#pragma once
#include "Engine/Physics/Collision.h"
#include "Engine/Structure/EditorUIComponent.h"

class Transform;
class Circle;
class Box;

enum class ShapeType
{
    BOX = 0,
    CIRCLE,
    COUNT
};

// For double dispatch pattern
class Shape : EditorUIComponent
{
public:
	Shape(const nlohmann::json& data);
	Shape(Shape const* other, GameObject* pgameobject);
	Shape();

    virtual bool Collide(Shape* shape) = 0;
    virtual bool Collide(Circle* circle) = 0;
    virtual bool Collide(Box* box) = 0;

    virtual bool CollideResponse(Shape* shape) = 0;
    virtual bool CollideResponse(Circle* circle) = 0;
    virtual bool CollideResponse(Box* box) = 0;

	virtual void UpdateShapeDimensions() = 0;

	ShapeType GetShapeType();

    static string ShapeTypeToString(ShapeType type);

#if EDITOR
    virtual void CreateEngineUI() override;
#endif

	virtual void Serialize(nlohmann::json& data);

    void SetIsTrigger(bool value);
    void SetIsCollidable(bool value);
    void SetIsRendered(bool value);

    bool IsTrigger() const;
    bool IsCollidable() const;
    bool IsRendered() const;

    XMFLOAT2 GetScale();
    virtual XMFLOAT2 GetShapeDimensions() = 0;
    void SetScale(XMFLOAT2 scale);

    GameObject* m_pgameObject;

protected:
    ShapeType m_shapeType;

    bool m_isTrigger = false;
    bool m_isCollidable = true;
    bool m_isRendered = false;

    XMFLOAT2 m_scale = XMFLOAT2(1, 1);

private:
    
#if EDITOR
    list<pair<int, string>> m_collideTypeList;

    pair<int, string> m_collideTypeSelectedItem;

    list<pair<int, string>> GetCollideTypeAsList();

    pair<int, string> GetCollideTypeFromIndex(int index);

    int CollideBoolsToListIndex(bool isCollidable, bool isTriggerable);

    void SetTriggerableCollidableFromIndex(int index);
#endif
};
