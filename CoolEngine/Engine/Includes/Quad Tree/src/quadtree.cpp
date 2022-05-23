#include "Engine/Includes/Quad Tree/include/quadtree.h"



Quadtree::Quadtree(Box* collider)
{
    m_BoxCollider = collider;
}

Quadtree::~Quadtree()
{
}

void Quadtree::Init(int left, int top, int width, int height, PlayerGameObject* obj)
{
    
    m_player = obj;
}


bool Quadtree::InsertElement(GameObject* obj)
{
    if (m_children == nullptr)
    {
        m_Child = obj;
        return true;
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

    if (NW_ != nullptr)
    {

    }
    if (NE_ != nullptr)
    {

    }
    if (SW_ != nullptr)
    {

    }
    if (SE_ != nullptr)
    {

    }


    int new_width = GetRect().m_width / 2;
    int new_height = GetRect().m_height/ 2;
    NW_ = new Quadtree(m_maxNodeSize);
    NE_ = new Quadtree(m_maxNodeSize);
    SW_ = new Quadtree(m_maxNodeSize);
    SE_ = new Quadtree(m_collider->m_transform, m_collider->GetRadius());
    for (auto &node: root->m_tr) 
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

    CollidableGameObject* pcollidable1 = nullptr;
    CollidableGameObject* pcollidable2 = nullptr;

    for (int it1 = 0; it1 < children_vec.size(); ++it1)
    {
        if (children_vec[it1]->ContainsType(GameObjectType::COLLIDABLE) == false)
        {
            continue;
        }
        pcollidable1 = dynamic_cast<CollidableGameObject*>(children_vec[it1]);
        pcollidable1->SetShapeDimensions(pcollidable1->GetTransform()->GetWorldScale());

        if (pcollidable1->GetShape() != nullptr && pcollidable1->GetShape()->IsRendered())
        {
            XMFLOAT3 p = pcollidable1->GetTransform()->GetWorldPosition();
            XMFLOAT2 d = pcollidable1->GetShape()->GetShapeDimensions();
            DebugDrawManager::GetInstance()->CreateWorldSpaceDebugRect(p, XMFLOAT3(d.x, d.y, 1), DebugDrawManager::DebugColour::PURPLE);
        }
    }


    for (int it1 = 0; it1 < children_vec.size(); ++it1)
    {
        for (int it2 = 0; it2 < children_vec.size(); ++it2)
        {
            if (it1 != it2)
            {
                if (children_vec[it1]->ContainsType(GameObjectType::COLLIDABLE) == false || children_vec[it2]->ContainsType(GameObjectType::COLLIDABLE) == false)
                {
                    continue;
                }

                pcollidable1 = dynamic_cast<CollidableGameObject*>(children_vec[it1]);
                pcollidable2 = dynamic_cast<CollidableGameObject*>(children_vec[it2]);

                if (pcollidable1->GetShape() == nullptr || pcollidable2->GetShape() == nullptr)
                {
                    continue;
                }

                // Whether to just collide or collide with response
                if (pcollidable1->GetShape()->IsCollidable() && pcollidable2->GetShape()->IsCollidable())
                {
                    bool hasCollided = pcollidable1->GetShape()->CollideResponse(pcollidable2->GetShape());
                    if (hasCollided)
                    {
                        EventManager::Instance()->AddEvent(new CollisionHoldEvent(pcollidable1, pcollidable2));
                    }
                }
                else if (pcollidable1->GetShape()->IsTrigger() && pcollidable2->GetShape()->IsTrigger() || pcollidable1->GetShape()->IsCollidable() && pcollidable2->GetShape()->IsTrigger() || pcollidable2->GetShape()->IsCollidable() && pcollidable1->GetShape()->IsTrigger())
                {
                    bool hasCollided = pcollidable1->GetShape()->Collide(pcollidable2->GetShape());
                    if (hasCollided)
                    {
                        EventManager::Instance()->AddEvent(new TriggerHoldEvent(pcollidable1, pcollidable2));
                    }
                }
            }
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

void Quadtree::UpdateScene(bool updateCollidingTreeOnly)
{
    if (updateCollidingTreeOnly)
    {
        Quadtree* collisionQuadtree = CollisionCheck();

        for (size_t i = 0; i < collisionQuadtree->m_children.size(); i++)
        {
            std::vector<GameObject*> childrenVec = collisionQuadtree->GetChildren();

            childrenVec.push_back(m_player);

            for (GameObject* node_one : childrenVec)
            {
                for (GameObject* node_two : childrenVec)
                {
                    //if (node_one == node_two) {
                    //    continue;
                    //}
                    //if ()
                    //{

                    //}

                    //->CollideResponse(node_two);
                    //++cnt;
                }
            }
        }
    }
    else
    {
        for (size_t i = 0; i < m_children.size(); i++)
        {
            m_children[i]->Update();
        }

        int legacyCodeCompliance = 0;
        
        QtreeCheckCollisions(legacyCodeCompliance);

    }
}

Quadtree* Quadtree::CollisionCheck()
{
    if (NW_ != nullptr) {

        Quadtree* Collision = NW_->CollisionCheck();
        if (Collision != nullptr)
        {
            return Collision;
        }
        Collision = NE_->CollisionCheck();
        if (Collision != nullptr)
        {
            return Collision;
        }
        Collision = SW_->CollisionCheck();
        if (Collision != nullptr)
        {
            return Collision;
        }
        Collision = SE_->CollisionCheck();
        if (Collision != nullptr)
        {
            return Collision;
        }
    }
    else if (m_player->GetShape()->Collide(m_collider))
    {
        return this;
    }
    else
    {
        return nullptr;
    }
}
