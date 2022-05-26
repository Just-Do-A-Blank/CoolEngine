#include "Engine/Includes/Quad Tree/include/quadtree.h"



Quadtree::Quadtree(XMFLOAT2 pos , int childrenSize, CompassFacing direction, int distantOffset)
{
    m_angle = pos;
    m_children.reserve(childrenSize);
    m_maxNodeSize = childrenSize;
    m_direction = direction;
    m_objectOffset = distantOffset;

    XMFLOAT3 pos3 = XMFLOAT3(pos.x, pos.y, 0);
    XMFLOAT3 rot = XMFLOAT3(0, 0, 0);
    XMFLOAT3 scal = XMFLOAT3(distantOffset, distantOffset, 1);

    m_Anchor = new GameObject();
    m_Anchor->GetTransform()->SetWorldPosition(pos3);
    m_Anchor->GetTransform()->SetWorldRotation(rot);
    m_Anchor->GetTransform()->SetWorldScale(scal);


    m_Collider = new Box(m_Anchor);
}

Quadtree::Quadtree(XMFLOAT2 pos, int childrenSize, int distantOffset)
{
    m_angle = pos;
    m_children.reserve(childrenSize);
    m_maxNodeSize = childrenSize;
    m_direction = CompassFacing::Centre;
    m_objectOffset = distantOffset;

    m_NW = new Quadtree(XMFLOAT2(-1.f + pos.x, pos.y), childrenSize, CompassFacing::NorthWest, distantOffset);
    m_NE = new Quadtree(XMFLOAT2(1.f + pos.x, pos.y), childrenSize, CompassFacing::NorthEast, distantOffset);
    m_SW = new Quadtree(XMFLOAT2(-1.f + pos.x, pos.y), childrenSize, CompassFacing::SouthWest, distantOffset);
    m_SE = new Quadtree(XMFLOAT2(1.f + pos.x, pos.y), childrenSize, CompassFacing::SouthEast, distantOffset);
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
    if(m_NW == nullptr) {
        Subdivide(this);
    }

    XMFLOAT3 pos = obj->GetTransform()->GetWorldPosition();

    bool insert_NW = false;
    bool insert_NE = false;
    bool insert_SE = false;
    bool insert_SW = false;

    if (pos.x <= m_NW->m_angle.x && pos.y >= m_NW->m_angle.y)
    {
        insert_NW = m_NW->InsertElement(obj);
        return insert_NW;
    }
    if (pos.x >= m_NE->m_angle.y && pos.y >= m_NE->m_angle.y)
    {
        insert_NE = m_NE->InsertElement(obj);
        return insert_NE;
    }
    if (pos.x <= m_SW->m_angle.x && pos.y <= m_SW->m_angle.y)
    {
        insert_SE = m_SW->InsertElement(obj);
        return insert_SE;
    }
    if (pos.x >= m_SE->m_angle.x && pos.y <= m_SE->m_angle.y)
    {
        insert_SW = m_SE->InsertElement(obj);
        return insert_SW;
    }


    std::cout << "In Insert element(), this should never happen." << std::endl;
    //assert(false);
    return false;
}


void Quadtree::Subdivide(Quadtree * root) {

    if (m_NW != nullptr)
    {
        delete m_NW;
    }
    if (m_NE != nullptr)
    {
        delete m_NE;
    }
    if (m_SW != nullptr)
    {
        delete m_SW;
    }
    if (m_SE != nullptr)
    {
        delete m_SE;
    }

    m_NW = new Quadtree(XMFLOAT2(m_angle.x + -1.f, m_angle.y + 1.f), m_maxNodeSize, CompassFacing::NorthWest, m_objectOffset);
    m_NE = new Quadtree(XMFLOAT2(m_angle.x + 1.f, m_angle.y + 1.f), m_maxNodeSize, CompassFacing::NorthEast, m_objectOffset);
    m_SW = new Quadtree(XMFLOAT2(m_angle.x + -1.f, m_angle.y + -1.f), m_maxNodeSize, CompassFacing::SouthWest, m_objectOffset);
    m_SE = new Quadtree(XMFLOAT2(m_angle.x + 1.f, m_angle.y + -1.f), m_maxNodeSize, CompassFacing::SouthEast, m_objectOffset);
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

bool Quadtree::RemoveObject(GameObject* pgameObjectAddress)
{
    return LoacteObjectAndOverwrite(pgameObjectAddress, nullptr);
}

void Quadtree::UpdateQuadTreeStucture()
{

}

void Quadtree::GetUpdateList(PlayerGameObject* player, std::vector<GameObject*>& listToUpdate)
{
    Transform* t = player->GetTransform();
    XMFLOAT2 updatePoint = XMFLOAT2(t->GetWorldPosition().x, t->GetWorldPosition().y);
    if (m_direction == CompassFacing::Centre)
    {
        CheckForObjectUpdate(listToUpdate);
    }
    if (m_NW == nullptr)
    {
        return;
    }
    
    PlayerGameObject* gObj = dynamic_cast<PlayerGameObject*>(SimpleQueryByIdentifier("Player"));

    m_NW->Collides(player, listToUpdate);
    m_NE->Collides(player, listToUpdate);
    m_SW->Collides(player, listToUpdate);
    m_SE->Collides(player, listToUpdate);

}

GameObject* Quadtree::SimpleQueryByIdentifier(std::string identifier)
{
    GameObject* gameObject = SearchQuadTree(identifier);
    if (gameObject == nullptr && m_NW != nullptr)
    {
        gameObject = m_NW->SearchQuadTree(identifier);
        if (gameObject == nullptr)
        {
            return gameObject;
        }
        gameObject = m_NE->SearchQuadTree(identifier);
        if (gameObject == nullptr)
        {
            return gameObject;
        }
        gameObject = m_SW->SearchQuadTree(identifier);
        if (gameObject == nullptr)
        {
            return gameObject;
        }
        gameObject = m_SE->SearchQuadTree(identifier);
    }
    return gameObject;
}

GameObject* Quadtree::QueryByIdentifier(std::string identifier, XMFLOAT2 point)
{
    GameObject* gameObject = SearchQuadTree(identifier);
    if (gameObject == nullptr && m_NW != nullptr)
    {
        if (point.x <= m_NW->m_angle.x && point.y >= m_NW->m_angle.y)
        {
            gameObject = m_NW->SearchQuadTree(identifier);
            if (gameObject == nullptr)
            {
                return gameObject;
            }
        }

        if (point.x >= m_NE->m_angle.y && point.y >= m_NE->m_angle.y)
        {
            gameObject = m_NE->SearchQuadTree(identifier);
            if (gameObject == nullptr)
            {
                return gameObject;
            }
        }

        if (point.x <= m_SW->m_angle.x && point.y <= m_SW->m_angle.y)
        {
            gameObject = m_SW->SearchQuadTree(identifier);
            if (gameObject == nullptr)
            {
                return gameObject;
            }
        }

        if (point.x >= m_SE->m_angle.x && point.y <= m_SE->m_angle.y)
        {
            gameObject = m_SE->SearchQuadTree(identifier);
            if (gameObject == nullptr)
            {
                return gameObject;
            }
        }
    }

    return gameObject;
}

bool Quadtree::Collides(PlayerGameObject* pG, std::vector<GameObject*>& list)
{
    if (m_NW != nullptr)
    {
        if (m_Collider->Collide(pG->GetShape()))
        {
            CheckForObjectUpdate(list);
            GetUpdateList(pG, list);
            return true;
        }
    }
    return false;
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

