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

enum class CompassFacing
{
    NorthWest = 0,
    NorthEast = 1,
    SouthWest = 2,
    SouthEast = 3,
    Centre = 4
};

class Quadtree {
public:
    Quadtree(XMFLOAT2 pos, int childrenSize, CompassFacing direction, int distantOffset);
    Quadtree(XMFLOAT2 pos, int childrenSize, int distantOffset);
    ~Quadtree();
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

    void QtreeCheckCollisions(XMFLOAT2 PlayerPosition);

    void QtreeFreeMemory();

    void UpdateScene(XMFLOAT2 updatePoint);

    bool RemoveObject(GameObject* pgameObjectAddress);

    void UpdateQuadTreeStucture();

private:

    bool LoacteObjectAndOverwrite(GameObject* dest,  GameObject* value);

    std::vector<GameObject*> m_children;
    Quadtree *NW_ = nullptr;
    Quadtree *NE_ = nullptr;
    Quadtree *SW_ = nullptr;
    Quadtree *SE_ = nullptr;
    XMFLOAT2 m_angle;
    CompassFacing m_direction;
    int m_maxNodeSize = 0;
    int m_objectOffset;
};

#endif // QUADTREE_H_
