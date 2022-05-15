#include "Pathfinding.h"
#include <algorithm>
#include "Engine/TileMap/TileMap/TileMap.h"


Pathfinding::Pathfinding()
{
}

void Pathfinding::SetupPath(XMFLOAT3 curPos, XMFLOAT3 tarPos)
{
	m_pNodeStart = FindClosestNode(curPos);
	m_pNodeEnd = FindClosestNode(tarPos);
	
	//Reset navigation grid
	for (int x = 0; x < m_mapWidth; ++x)
	{
		for (int y = 0; y < m_mapHeight; ++y)
		{
			if (!m_pNodes[x][y])
			{
				continue;
			}

			m_pNodes[x][y]->m_gCost = INFINITY;
			m_pNodes[x][y]->m_gCost = INFINITY;
			m_pNodes[x][y]->m_pParent = nullptr;
			m_pNodes[x][y]->m_visited = false;
		}
	}

	m_pNodeStart->m_gCost = 0.0f;
	m_pNodeStart->m_hCost = MathHelper::Distance(m_pNodeStart->m_pos, m_pNodeEnd->m_pos);
	m_nodesToTest.push(m_pNodeStart);
}


/// <summary>
/// 
///	Might not return "the best" but for almost all use cases will be good enough. Much more efficient
/// 
/// </summary>
/// <param name="curPos"></param>
/// <param name="tarPos"></param>
/// <returns></returns>
bool Pathfinding::FindPath(XMFLOAT3 curPos, XMFLOAT3 tarPos, vector<node*> &path)
{
	SetupPath(curPos, tarPos);

	while (!m_nodesToTest.empty() && m_nodesToTest.top() != m_pNodeEnd)
	{
		while (!m_nodesToTest.empty() && m_nodesToTest.top()->m_visited)
		{
			m_nodesToTest.pop();
		}

		if (m_nodesToTest.empty())
		{
			LOG("No Path found");
			return false;
		}

		CalculatePath();
	}

	return GeneratePath(tarPos,path);

}

/// <summary>
/// 
/// Finds the perfect path by checking the whole connection tree before creating a path, could cause slowness if lots of m_pNodes & connections
/// 
/// </summary>
/// <param name="curPos"></param>
/// <param name="tarPos"></param>
/// <returns></returns>
bool Pathfinding::FindPerfectPath(XMFLOAT3 curPos, XMFLOAT3 tarPos, vector<node*>& path)
{
	SetupPath(curPos, tarPos);

	while (!m_nodesToTest.empty())
	{
		while (!m_nodesToTest.empty() && m_nodesToTest.top()->m_visited)
		{
			m_nodesToTest.pop();
		}

		if (m_nodesToTest.empty())
		{
			LOG("No Path found");
			return false;
		}

		CalculatePath();
	}

	return GeneratePath(tarPos,path);
}

/// <summary>
/// 
/// Calculates the path from start to end using A*
/// 
/// </summary>
void Pathfinding::CalculatePath()
{
	node* nodeCurrentTemp = m_nodesToTest.top();
	nodeCurrentTemp->m_visited = true;

	for (auto neighbour : nodeCurrentTemp->m_pNeighbours)
	{
		float gTest = nodeCurrentTemp->m_gCost + MathHelper::Distance(nodeCurrentTemp->m_pos, neighbour->m_pos);
		//Testing if the node cost is lower for this than what is currently on the neighbour
		if (gTest < neighbour->m_gCost)
		{
			neighbour->m_pParent = nodeCurrentTemp;
			neighbour->m_gCost = gTest;

			//updating the heuristic so we can prioritise the best 

			neighbour->m_hCost = neighbour->m_gCost + MathHelper::Distance(neighbour->m_pos, m_pNodeEnd->m_pos);// distance(neighbour, m_pNodeEnd);

		}

		//If the node isnt an obstacle and hasnt been visited already, we want to visit it now
		if (!neighbour->m_visited && !neighbour->m_obstacle)
		{
			m_nodesToTest.push(neighbour);
		}
	}
}

/// <summary>
/// 
/// Generates the path for a return to the requestie
/// 
/// </summary>
/// <param name="tarPos"></param>
/// <returns></returns>
bool Pathfinding::GeneratePath(XMFLOAT3 tarPos, vector<node*>& path)
{
	path.clear();

	//Follow the parent of the end node to the start node
	node* nodeCurrentTemp;

	if (m_pNodeEnd != nullptr)
	{
		nodeCurrentTemp = m_pNodeEnd;
		while (nodeCurrentTemp->m_pParent != nullptr)
		{
			path.push_back(nodeCurrentTemp);
			nodeCurrentTemp = nodeCurrentTemp->m_pParent;
		}
	}

	return true;
}


Pathfinding* Pathfinding::GetInstance()
{
	static Pathfinding instance;
	return &instance;
}

/// <summary>
/// 
/// Returns the closest node to the XMFLOAT3 pos passed in on init of this class
/// 
/// </summary>
/// <returns></returns>
node* Pathfinding::FindClosestNode(XMFLOAT3 pos)
{
	node* closestNode = nullptr;
	float closestDist = INFINITY;
	float currDist;
	
	for (int x = 0; x < m_mapWidth; x++)
	{
		for (int y = 0; y < m_mapHeight; y++)
		{
			if (!m_pNodes[x][y] || m_pNodes[x][y]->m_obstacle)
			{
				continue;
			}

			currDist = MathHelper::Distance(pos, m_pNodes[x][y]->m_pos);

			if (currDist < closestDist)
			{
				closestDist = currDist;
				closestNode = m_pNodes[x][y];
			}
		}
	}
	
	return closestNode;

}

/// <summary>
/// 
/// If the tilemap changes init needs to be called again to reconnect the tile tree together
/// 
/// 
/// </summary>
/// <param name="map"></param>
void Pathfinding::Initialize(TileMap* map)
{
	m_mapWidth = map->GetWidth();
	m_mapHeight = map->GetHeight();
	vector<node*> tempVec;
	node* tempNode;

	Tile* ptile;
	
	//Setting up tile grid
	for (int x = 0; x < m_mapWidth; ++x)
	{
		tempVec.clear();
		for (int y = 0; y < m_mapHeight; ++y)
		{
			map->GetTileFromMapPos(x, y, ptile);
			tempNode = nullptr;


			if (ptile)
			{
				tempNode = new node();
				tempNode->m_pos = ptile->GetTransform()->GetPosition();
				tempNode->m_obstacle = ptile->GetIsPassable();
			}

			tempVec.push_back(tempNode);
		}
		m_pNodes.push_back(tempVec);
	}
	
	//Connecting local m_pNodes
	for (int x = 0; x < m_mapWidth; ++x)
	{
		for (int y = 0; y < m_mapHeight; ++y)
		{
			if (!m_pNodes[x][y])
			{
				continue;
			}

			if (x > 0 && m_pNodes[x - 1][y])
			{
				m_pNodes[x][y]->m_pNeighbours.push_back(m_pNodes[x - 1][y]);
			}
			if (x < m_mapWidth - 1 && m_pNodes[x + 1][y])
			{
				m_pNodes[x][y]->m_pNeighbours.push_back(m_pNodes[x + 1][y]);
			}
			if (y > 0 && m_pNodes[x][y - 1])
			{
				m_pNodes[x][y]->m_pNeighbours.push_back(m_pNodes[x][y - 1]);
			}
			if (y < m_mapHeight - 1 && m_pNodes[x][y + 1])
			{
				m_pNodes[x][y]->m_pNeighbours.push_back(m_pNodes[x][y + 1]);
			}

			//Diagonal m_pNodes if wanted, more of a question but could slow down the system as it double the number of paths for each node (exponentially more searching)
			//To enable diagonal m_pNodes change DIAGONAL_ENABLED to true
			if (DIAGONAL_ENABLED)
			{
				if (x > 0 && y > 0 && m_pNodes[x - 1][y - 1])
				{
					m_pNodes[x][y]->m_pNeighbours.push_back(m_pNodes[x - 1][y - 1]);
				}
				if (x < m_mapWidth - 1 && y > 0 && m_pNodes[x + 1][y - 1])
				{
					m_pNodes[x][y]->m_pNeighbours.push_back(m_pNodes[x + 1][y - 1]);
				}
				if (x > 0 && y < m_mapHeight - 1 && m_pNodes[x - 1][y + 1])
				{
					m_pNodes[x][y]->m_pNeighbours.push_back(m_pNodes[x - 1][y + 1]);
				}
				if (x < m_mapWidth - 1 && y < m_mapHeight - 1 && m_pNodes[x + 1][y + 1])
				{
					m_pNodes[x][y]->m_pNeighbours.push_back(m_pNodes[x + 1][y + 1]);
				}



			}


		}
	}
	


}
