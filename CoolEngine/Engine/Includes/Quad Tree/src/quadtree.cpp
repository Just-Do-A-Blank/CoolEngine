#include "Engine/Includes/Quad Tree/include/quadtree.h"



Quadtree::Quadtree(XMFLOAT2 pos , int childrenSize, CompassFacing direction, int distantOffset)
{
    m_angle = pos;
    m_children.reserve(childrenSize);
    m_maxNodeSize = childrenSize;
    m_direction = direction;
    m_objectOffset = distantOffset;
}

Quadtree::Quadtree(XMFLOAT2 pos, int childrenSize, int distantOffset)
{
    m_angle = pos;
    m_children.reserve(childrenSize);
    m_maxNodeSize = childrenSize;
    m_direction = CompassFacing::Centre;
    m_objectOffset = distantOffset;

    NW_ = new Quadtree(XMFLOAT2(-1.f + pos.x, pos.y), childrenSize, CompassFacing::NorthWest, distantOffset);
    NE_ = new Quadtree(XMFLOAT2(1.f + pos.x, pos.y), childrenSize, CompassFacing::NorthEast, distantOffset);
    SW_ = new Quadtree(XMFLOAT2(-1.f + pos.x, pos.y), childrenSize, CompassFacing::SouthWest, distantOffset);
    SE_ = new Quadtree(XMFLOAT2(1.f + pos.x, pos.y), childrenSize, CompassFacing::SouthEast, distantOffset);
}



Quadtree::~Quadtree()
{
}

bool Quadtree::InsertElement(GameObject* obj)
{
    if (m_children.size() < m_maxNodeSize)
    {
        m_children.push_back(obj);
        return true;
    }
    if(NW_ == nullptr) {
        Subdivide(this);
    }

    XMFLOAT3 pos = obj->GetTransform()->GetWorldPosition();

    bool insert_NW = false;
    bool insert_NE = false;
    bool insert_SE = false;
    bool insert_SW = false;

    if (pos.x <= NW_->m_angle.x && pos.y >= NW_->m_angle.y)
    {
        insert_NW = NW_->InsertElement(obj);
        return insert_NW;
    }
    if (pos.x >= NE_->m_angle.y && pos.y >= NE_->m_angle.y)
    {
        insert_NE = NE_->InsertElement(obj);
        return insert_NE;
    }
    if (pos.x <= SW_->m_angle.x && pos.y <= SW_->m_angle.y)
    {
        insert_SE = SW_->InsertElement(obj);
        return insert_SE;
    }
    if (pos.x >= SE_->m_angle.x && pos.y <= SE_->m_angle.y)
    {
        insert_SW = SE_->InsertElement(obj);
        return insert_SW;
    }


    std::cout << "In Insert element(), this should never happen." << std::endl;
    //assert(false);
    return false;
}


void Quadtree::Subdivide(Quadtree * root) {

    if (NW_ != nullptr)
    {
        delete NW_;
    }
    if (NE_ != nullptr)
    {
        delete NE_;
    }
    if (SW_ != nullptr)
    {
        delete SW_;
    }
    if (SE_ != nullptr)
    {
        delete SE_;
    }

    NW_ = new Quadtree(XMFLOAT2(m_angle.x + -1.f, m_angle.y + 1.f), m_maxNodeSize, CompassFacing::NorthWest, m_objectOffset);
    NE_ = new Quadtree(XMFLOAT2(m_angle.x + 1.f, m_angle.y + 1.f), m_maxNodeSize, CompassFacing::NorthEast, m_objectOffset);
    SW_ = new Quadtree(XMFLOAT2(m_angle.x + -1.f, m_angle.y + -1.f), m_maxNodeSize, CompassFacing::SouthWest, m_objectOffset);
    SE_ = new Quadtree(XMFLOAT2(m_angle.x + 1.f, m_angle.y + -1.f), m_maxNodeSize, CompassFacing::SouthEast, m_objectOffset);
}

void Quadtree::QtreeCheckCollisions(XMFLOAT2 playerPosition) {
    Quadtree *NW = GetNW();
    Quadtree *NE = GetNE();
    Quadtree *SW = GetSW();
    Quadtree *SE = GetSE();

    XMFLOAT2 playerPositionOffset = { playerPosition.x + m_objectOffset, playerPosition.y + m_objectOffset };

    if (NW != nullptr)
    {
        NW->QtreeCheckCollisions(playerPosition);
        NE->QtreeCheckCollisions(playerPosition);
        SW->QtreeCheckCollisions(playerPosition);
        SE->QtreeCheckCollisions(playerPosition);
    }
    
    CollidableGameObject* pcollidable1 = nullptr;
    CollidableGameObject* pcollidable2 = nullptr;

    for (int it1 = 0; it1 < m_children.size(); ++it1)
    {
        m_children[it1]->Update();
        if (m_children[it1]->ContainsType(GameObjectType::COLLIDABLE) == false)
        {
            continue;
        }
        pcollidable1 = dynamic_cast<CollidableGameObject*>(m_children[it1]);
        pcollidable1->UpdateShapeDimensions();

        if (pcollidable1->GetShape() != nullptr && pcollidable1->GetShape()->IsRendered())
        {
            XMFLOAT3 p = pcollidable1->GetTransform()->GetWorldPosition();
            XMFLOAT2 d = pcollidable1->GetShape()->GetShapeDimensions();
            DebugDrawManager::GetInstance()->CreateWorldSpaceDebugRect(p, XMFLOAT3(d.x, d.y, 1), DebugDrawManager::DebugColour::PURPLE);
        }
    }


    for (int it1 = 0; it1 < m_children.size(); ++it1)
    {
        for (int it2 = 0; it2 < m_children.size(); ++it2)
        {
            if (it1 != it2)
            {
                if (m_children[it1]->ContainsType(GameObjectType::COLLIDABLE) == false || m_children[it2]->ContainsType(GameObjectType::COLLIDABLE) == false)
                {
                    continue;
                }

                pcollidable1 = dynamic_cast<CollidableGameObject*>(m_children[it1]);
                pcollidable2 = dynamic_cast<CollidableGameObject*>(m_children[it2]);

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

void Quadtree::UpdateScene(XMFLOAT2 updatePoint)
{
    QtreeCheckCollisions(updatePoint);
}

bool Quadtree::RemoveObject(GameObject* pgameObjectAddress)
{
    return LoacteObjectAndOverwrite(pgameObjectAddress, nullptr);
}

void Quadtree::UpdateQuadTreeStucture()
{

}

void Quadtree::GetUpdateList(XMFLOAT2 updatePoint, std::vector<GameObject*>& listToUpdate)
{
    if (m_direction == CompassFacing::Centre)
    {
        CheckForObjectUpdate(listToUpdate);
    }
    if (NW_ == nullptr)
    {
        return;
    }

    //float difference = std::abs(NW_->m_angle.x - updatePoint.x);
    //float difference2 = std::abs(NW_->m_angle.y - updatePoint.y);

    if (NW_ != nullptr)
    {
        NW_->CheckForObjectUpdate(listToUpdate);
        NW_->GetUpdateList(updatePoint, listToUpdate);
        NE_->CheckForObjectUpdate(listToUpdate);
        NE_->GetUpdateList(updatePoint, listToUpdate);
        SW_->CheckForObjectUpdate(listToUpdate);
        SW_->GetUpdateList(updatePoint, listToUpdate);
        SE_->CheckForObjectUpdate(listToUpdate);
        SE_->GetUpdateList(updatePoint, listToUpdate);
    }




    //if (difference < m_objectOffset && difference2 < m_objectOffset && updatePoint.x <= NW_->m_angle.x && updatePoint.y >= NW_->m_angle.y)
    //{
    //    NW_->CheckForObjectUpdate(listToUpdate);
    //    NW_->GetUpdateList(updatePoint, listToUpdate);
    //}
    // difference = std::abs(NE_->m_angle.x - updatePoint.x);
    //  difference2 = std::abs(NE_->m_angle.y - updatePoint.y);

    //if (difference < m_objectOffset && difference2 < m_objectOffset && updatePoint.x >= NE_->m_angle.y && updatePoint.y >= NE_->m_angle.y)
    //{
    //      NE_->CheckForObjectUpdate(listToUpdate);
    //      NE_->GetUpdateList(updatePoint, listToUpdate);
    //}

    // difference = std::abs(SW_->m_angle.x - updatePoint.x);
    //  difference2 = std::abs(SW_->m_angle.y - updatePoint.y);

    //if (difference < m_objectOffset && difference2 < m_objectOffset && updatePoint.x <= SW_->m_angle.x && updatePoint.y <= SW_->m_angle.y)
    //{
    //    SW_->CheckForObjectUpdate(listToUpdate);
    //    SW_->GetUpdateList(updatePoint, listToUpdate);
    //}

    // difference = std::abs(SE_->m_angle.x - updatePoint.x);
    //  difference2 = std::abs(SE_->m_angle.y - updatePoint.y);

    //if (difference < m_objectOffset && difference2 < m_objectOffset && updatePoint.x >= SE_->m_angle.x && updatePoint.y <= SE_->m_angle.y)
    //{
    //   SE_->CheckForObjectUpdate(listToUpdate);
    //   SE_->GetUpdateList(updatePoint, listToUpdate);
    //}
}

GameObject* Quadtree::SimpleQueryByIdentifier(std::string identifier)
{
    GameObject* gameObject = SearchQuadTree(identifier);
    if (gameObject == nullptr && NW_ != nullptr)
    {
        gameObject = NW_->SearchQuadTree(identifier);
        if (gameObject == nullptr)
        {
            return gameObject;
        }
        gameObject = NE_->SearchQuadTree(identifier);
        if (gameObject == nullptr)
        {
            return gameObject;
        }
        gameObject = SW_->SearchQuadTree(identifier);
        if (gameObject == nullptr)
        {
            return gameObject;
        }
        gameObject = SE_->SearchQuadTree(identifier);
    }
    return gameObject;
}

GameObject* Quadtree::QueryByIdentifier(std::string identifier, XMFLOAT2 point)
{
    GameObject* gameObject = SearchQuadTree(identifier);
    if (gameObject == nullptr && NW_ != nullptr)
    {
        if (point.x <= NW_->m_angle.x && point.y >= NW_->m_angle.y)
        {
            gameObject = NW_->SearchQuadTree(identifier);
            if (gameObject == nullptr)
            {
                return gameObject;
            }
        }

        if (point.x >= NE_->m_angle.y && point.y >= NE_->m_angle.y)
        {
            gameObject = NE_->SearchQuadTree(identifier);
            if (gameObject == nullptr)
            {
                return gameObject;
            }
        }

        if (point.x <= SW_->m_angle.x && point.y <= SW_->m_angle.y)
        {
            gameObject = SW_->SearchQuadTree(identifier);
            if (gameObject == nullptr)
            {
                return gameObject;
            }
        }

        if (point.x >= SE_->m_angle.x && point.y <= SE_->m_angle.y)
        {
            gameObject = SE_->SearchQuadTree(identifier);
            if (gameObject == nullptr)
            {
                return gameObject;
            }
        }
    }

    return gameObject;
}

GameObject* Quadtree::SearchQuadTree(std::string identifier)
{
    for (size_t i = 0; i < m_children.size(); i++)
    {   
        if (m_children[i]->GetIdentifier() == identifier)
        {
            return m_children[i];
        }
    }
    return nullptr;
}

void Quadtree::CheckForObjectUpdate(std::vector<GameObject*>& list)
{
    for (size_t i = 0; i < m_children.size(); i++)
    {
        list.push_back(m_children[i]);
    }
}

bool Quadtree::LoacteObjectAndOverwrite(GameObject* dest, GameObject* value)
{
    for (size_t i = 0; i < m_children.size(); i++)
    {
        if (m_children[i] == dest)
        {
            m_children[i] = value;
            return true;
        }
    }

    if (GetNW() == nullptr)
    {
        return false;
    }

    bool overwriteObjNW = GetNW()-> LoacteObjectAndOverwrite(dest, value);
    bool overwriteObjNE = GetNE()->LoacteObjectAndOverwrite(dest, value);
    bool overwriteObjSW = GetSW()->LoacteObjectAndOverwrite(dest, value);
    bool overwriteObjSE = GetSE()->LoacteObjectAndOverwrite(dest, value);


    if (overwriteObjNW || overwriteObjNE || overwriteObjSW || overwriteObjSE) {
        return true;
    }
    else
    {
        return false;
    }
}

