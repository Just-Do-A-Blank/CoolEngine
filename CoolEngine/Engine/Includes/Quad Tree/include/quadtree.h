#ifndef QUADTREE_H_
#define QUADTREE_H_

#include "Engine/Physics/Box.h"
#include "Engine/Physics/Circle.h"
#include "Engine/GameObjects/RenderableCollidableGameObject.h"
#include "Engine/GameObjects/PlayerGameObject.h"
#include <vector>

struct RectValues
{
    float m_left = 0;
    float m_top = 0;
    float m_width = 0;
    float m_height = 0;
};

class Quadtree {
public:
    Quadtree(float objectRadius);
    void Init(int left, int top, int width, int height, PlayerGameObject* obj);
    bool InsertElement(GameObject* shape);
    void Subdivide(Quadtree *root);
    int GetSize() const {
        return children_.size();
    }
    Quadtree *GetNW() const {
        return NW_;
    }
    Quadtree *GetNE() const {
        return NE_;
    }
    Quadtree *GetSW() const {
        return SW_;
    }
    Quadtree *GetSE() const {
        return SE_;
    }
    RectValues GetRect() const
    {
        return m_rectValues;
    }
    std::vector<GameObject*> GetChildren() const {
        return m_children;
    }
    void QtreeCheckCollisions(int &num_collisions);
    void QtreeFreeMemory();
private:
    std::vector<GameObject*> m_children;
    Quadtree *NW_ = nullptr;
    Quadtree *NE_ = nullptr;
    Quadtree *SW_ = nullptr;
    Quadtree *SE_ = nullptr;
    RectValues m_rectValues;
    PlayerGameObject* m_player;
    Circle* collider;
};

#endif // QUADTREE_H_
