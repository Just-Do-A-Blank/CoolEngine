#include "Engine/Includes/Quad Tree/include/quadtree.h"
#include <iostream>
#include <cmath>
#include <cassert>



Quadtree::Quadtree(float objectRadius)
{
    colliderSize = objectRadius;
}

void Quadtree::Init(int left, int top, int width, int height, PlayerGameObject* obj)
{
    m_rectValues.m_height = height;
    m_rectValues.m_width = width;
    m_rectValues.m_top = top;
    m_rectValues.m_left = left;
    m_player = obj;
}


bool Quadtree::InsertElement(GameObject* obj)
{
    if (obj->ContainsType(GameObjectType::COLLIDABLE))
    {
        bool intersects = true;
        Shape* shape;
        switch ((AccumlateType)obj->GetGameObjectType())
        {
        case AccumlateType::COLLIDABLE:
                switch (collider->GetShapeType())
                {
                case ShapeType::BOX:
                    intersects = (dynamic_cast<CollidableGameObject*>(obj))->GetShape()->Collide(dynamic_cast<Box*>(collider));
                    if (!intersects) {
                        return false;
                    }
                    break;
                case ShapeType::CIRCLE:
                    intersects = (dynamic_cast<CollidableGameObject*>(obj))->GetShape()->Collide(dynamic_cast<Circle*>(collider));
                    if (!intersects) {
                        return false;
                    }
                case ShapeType::COUNT:
                    break;
                default:
                    break;
                }
            break;
        case AccumlateType::COLLIDABLE_RENDERERABLE:
                switch (m_player->GetShape()->GetShapeType())
                {
                case ShapeType::BOX:
                    intersects = (dynamic_cast<RenderableCollidableGameObject*>(obj))->GetShape()->Collide(dynamic_cast<Box*>(m_player->GetShape()));
                    if (!intersects) {
                        return false;
                    }
                    break;
                case ShapeType::CIRCLE:
                    intersects = (dynamic_cast<RenderableCollidableGameObject*>(obj))->GetShape()->Collide(dynamic_cast<Circle*>(m_player->GetShape()));
                    if (!intersects) {
                        return false;
                    }
                    break;
                case ShapeType::COUNT:
                    break;
                default:
                    break;
                }
                break;

            default:
                break;
        }
        if (collider->Collide(shape))
        {
            m_children.push_back(obj);
            return true;
        }
    }
    else
    {
        Box renderableTempCollider = Box(obj->GetTransform());
        if (collider->Collide(&renderableTempCollider))
        {
            m_children.push_back(obj);
            return true;
        }
    }
    if(NW_ == nullptr) {
        Subdivide(this);
    }
    bool insert_NW = NW_->InsertElement(obj);
    bool insert_NE = NE_->InsertElement(obj);
    bool insert_SW = SW_->InsertElement(obj);
    bool insert_SE = SE_->InsertElement(obj);
    if(insert_NW || insert_NE || insert_SW || insert_SE) {
        return true;
    }
    std::cout << "In Insert element(), this should never happen." << std::endl;
    //assert(false);
    return false;
}


void Quadtree::Subdivide(Quadtree * root) {
    int new_width = GetRect().m_width / 2;
    int new_height = GetRect().m_height/ 2;
    NW_ = new Quadtree(collider->GetRadius());
    NE_ = new Quadtree(collider->GetRadius());
    SW_ = new Quadtree(collider->GetRadius());
    SE_ = new Quadtree(collider->GetRadius());
    for (auto &node: root->m_children) 
    {
        NW_->InsertElement(node);
        NE_->InsertElement(node);
        SW_->InsertElement(node);
        SE_->InsertElement(node);
    }
}


void Quadtree::QtreeCheckCollisions(int &cnt) {
    Quadtree *NW = GetNW();
    Quadtree *NE = GetNE();
    Quadtree *SW = GetSW();
    Quadtree *SE = GetSE();

    if (NW != nullptr) {
        NW->QtreeCheckCollisions(cnt);
        NE->QtreeCheckCollisions(cnt);
        SW->QtreeCheckCollisions(cnt);
        SE->QtreeCheckCollisions(cnt);
        return;
    }

    std::vector<GameObject*> children_vec = GetChildren();
    for (auto node_one: children_vec) {
        for (auto node_two: children_vec) {
            if (node_one == node_two) {
                continue;
            }
            node_one->CollideResponse(node_two);
            ++cnt;
        }
    }
}


void Quadtree::QtreeFreeMemory() {
    if(GetNW()){
        GetNW()->QtreeFreeMemory();
        GetNE()->QtreeFreeMemory();
        GetSW()->QtreeFreeMemory();
        GetSE()->QtreeFreeMemory();
    }
    delete this;
}
