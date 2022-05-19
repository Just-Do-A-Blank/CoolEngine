#include "Engine/Includes/Quad Tree/include/quadtree.h"
#include <iostream>
#include <cmath>
#include <cassert>



Quadtree::Quadtree(int left, int top, int width, int height, Shape* shape)
{
    m_rectValues.m_height = height;
    m_rectValues.m_width = width;
    m_rectValues.m_top = top;
    m_rectValues.m_left = left;
    m_shape = shape;
}


bool Quadtree::InsertElement(Shape *b) 
{
    switch (m_shape->GetShapeType())
    {
    case ShapeType::BOX:
        bool intersects = b->Collide(dynamic_cast<Box*>(m_shape));
        if (!intersects) {
            return false;
        }
        break;
    case ShapeType::CIRCLE:
        bool intersects = b->Collide(dynamic_cast<Circle*>(m_shape));
        if (!intersects) {
            return false;
        }
        break;
    case ShapeType::COUNT:
        break;
    default:
        break;
    }
    if(GetSize() < kQuadtreeNodeCapacity) {
        children_.push_back(b);
        return true;
    }
    if(NW_ == nullptr) {
        Subdivide(this);
    }
    bool insert_NW = NW_->InsertElement(b);
    bool insert_NE = NE_->InsertElement(b);
    bool insert_SW = SW_->InsertElement(b);
    bool insert_SE = SE_->InsertElement(b);
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
    NW_ = new Quadtree(GetRect().m_left, GetRect().m_top, new_width, new_height, m_shape);
    NE_ = new Quadtree(GetRect().m_left + new_width, GetRect().m_top, new_width, new_height, m_shape);
    SW_ = new Quadtree(GetRect().m_left, GetRect().m_top + new_height, new_width, new_height, m_shape);
    SE_ = new Quadtree(GetRect().m_left + new_width, GetRect().m_top + new_height, new_width, new_height, m_shape);
    for (auto &node: root->children_) {
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

    std::vector<Shape *> children_vec = GetChildren();
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
