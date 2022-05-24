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
#include <cstdlib>

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
        return m_NW;
    }
    Quadtree *GetNE() const {
        return m_NE;
    }
    Quadtree *GetSW() const {
        return m_SW;
    }
    Quadtree *GetSE() const {
        return m_SE;
    }

    void QtreeCheckCollisions(XMFLOAT2 PlayerPosition);

    void QtreeFreeMemory();

    void UpdateScene(XMFLOAT2 updatePoint);

    bool RemoveObject(GameObject* pgameObjectAddress);

    void UpdateQuadTreeStucture();

    void GetUpdateList(XMFLOAT2 updatePoint, std::vector<GameObject*>& list);

    GameObject* SimpleQueryByIdentifier(std::string identifier);

    GameObject* QueryByIdentifier(std::string identifier, XMFLOAT2 point);

private:

    GameObject* SearchQuadTree(std::string identifier);

    void CheckForObjectUpdate(std::vector<GameObject*>& list);

    bool LoacteObjectAndOverwrite(GameObject* dest,  GameObject* value);

    std::vector<GameObject*> m_children;
    Quadtree * m_NW = nullptr;
    Quadtree * m_NE = nullptr;
    Quadtree * m_SW = nullptr;
    Quadtree * m_SE = nullptr;
    XMFLOAT2 m_angle;
    CompassFacing m_direction;
    int m_maxNodeSize = 0;
    int m_objectOffset;
};

#endif // QUADTREE_H_
