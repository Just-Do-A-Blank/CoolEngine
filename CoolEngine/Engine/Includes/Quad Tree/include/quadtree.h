#ifndef QUADTREE_H_
#define QUADTREE_H_

#include "Engine/Physics/Box.h"
#include "Engine/Physics/Circle.h"
#include "Engine/GameObjects/RenderableCollidableGameObject.h"
#include "Engine/GameObjects/PlayerGameObject.h"
#include "Engine/Managers/Events/EventManager.h"
#include "Engine/Managers/DebugDrawManager.h"
#include "Engine/Physics/Collision.h"
#include <vector>
#include <iostream>
#include <cmath>
#include <cassert>

struct RectValues
{
    float m_left = 0;
    float m_top = 0;
    float m_width = 0;
    float m_height = 0;
};

class Quadtree {
public:
    Quadtree(Box* collider);
    ~Quadtree();
    void Init(int left, int top, int width, int height, PlayerGameObject* obj);
    bool InsertElement(GameObject* shape);
    void Subdivide(Quadtree *root);

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

    std::vector<GameObject*> GetChildren();

    void QtreeCheckCollisions(int &num_collisions);
    void QtreeFreeMemory();

    void UpdateScene(bool updateCollidingTreeOnly);

private:
    
    void Update();

    Quadtree* CollisionCheck();




    std::vector<GameObject*> m_children;
    Quadtree *NW_ = nullptr;
    Quadtree *NE_ = nullptr;
    Quadtree *SW_ = nullptr;
    Quadtree *SE_ = nullptr;
    Box* m_BoxCollider = nullptr;
    PlayerGameObject* m_player;
    int m_maxNodeSize = 0;
};

#endif // QUADTREE_H_
