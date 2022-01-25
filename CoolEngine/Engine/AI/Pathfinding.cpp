#include "Pathfinding.h"
#include <algorithm>
#include "Engine/TileMap/TileMap/TileMap.h"

//Custom pop_front vector function because vector doesnt have one. Remove if swapping to lists. 
template<typename T>
void pop_front(vector<T>& vec)
{
	assert(!vec.empty());
	vec.erase(vec.begin());
}

Pathfinding::Pathfinding()
{
	////setting up a tile grid for testing. This will be come from the premade tile grid
	//nodes = new node[m_mapWidth * m_mapHeight];
	//for (int x = 0; x < m_mapWidth; ++x)
	//{
	//	for (int y = 0; y < m_mapHeight; ++y)
	//	{
	//		nodes[y * m_mapWidth + x].pos.x = x * 16;
	//		nodes[y * m_mapWidth + x].pos.y = y * 16;
	//		nodes[y * m_mapWidth + x].m_obstacle = false;
	//		nodes[y * m_mapWidth + x].m_parent = nullptr;
	//		nodes[y * m_mapWidth + x].m_visited = false;
	//	}
	//}

	////hardcoded, start will be passed in by object and objective will be passed in aswell
	////m_nodeStart = &nodes[(m_mapHeight/2) * m_mapWidth + 1];
	////m_nodeEnd = &nodes[(m_mapHeight / 2) * m_mapWidth + m_mapWidth - 2];


	////Connecting local nodes
	//for (int x = 0; x < m_mapWidth; ++x)
	//{
	//	for (int y = 0; y < m_mapHeight; ++y)
	//	{
	//		
	//		if (y > 0)
	//			nodes[y * m_mapWidth + x].m_neighbours.push_back(&nodes[(y - 1) * m_mapWidth + (x)]);
	//		if (y < m_mapHeight - 1)
	//			nodes[y * m_mapWidth + x].m_neighbours.push_back(&nodes[(y + 1) * m_mapWidth + (x)]);
	//		if (x > 0)
	//			nodes[y * m_mapWidth + x].m_neighbours.push_back(&nodes[(y) * m_mapWidth + (x - 1)]);
	//		if (x < m_mapWidth - 1)
	//			nodes[y * m_mapWidth + x].m_neighbours.push_back(&nodes[(y) * m_mapWidth + (x + 1)]);


	//		//Diagonal nodes if wanted, more of a question but could slow down the system as it double the number of paths for each node (exponentially more searching)
	//		//To enable diagonal nodes change DIAGONAL_ENABLED to true
	//		if (DIAGONAL_ENABLED)
	//		{
	//			if (y > 0 && x > 0)
	//				nodes[y * m_mapWidth + x].m_neighbours.push_back(&nodes[(y - 1) * m_mapWidth + (x - 1)]);
	//			if (y < m_mapWidth - 1 && x>0)
	//				nodes[y * m_mapWidth + x].m_neighbours.push_back(&nodes[(y + 1) * m_mapWidth + (x - 1)]);
	//			if (y > 0 && x < m_mapWidth - 1)
	//				nodes[y * m_mapWidth + x].m_neighbours.push_back(&nodes[(y - 1) * m_mapWidth + (x + 1)]);
	//			if (y < m_mapWidth - 1 && x < m_mapWidth - 1)
	//				nodes[y * m_mapWidth + x].m_neighbours.push_back(&nodes[(y + 1) * m_mapWidth + (x + 1)]);



	//		}


	//	}
	//}



}



void Pathfinding::SetupPath(XMFLOAT3 curPos, XMFLOAT3 tarPos)
{
	m_nodeStart = FindClosestNode(curPos);
	m_nodeEnd = FindClosestNode(tarPos);

	//Reset navigation grid
	for (int x = 0; x < m_mapWidth; ++x)
	{
		for (int y = 0; y < m_mapHeight; ++y)
		{
			nodes[y * m_mapWidth + x].m_hCost = INFINITY;
			nodes[y * m_mapWidth + x].m_gCost = INFINITY;
			nodes[y * m_mapWidth + x].m_parent = nullptr;
			nodes[y * m_mapWidth + x].m_visited = false;
		}
	}
}


/// <summary>
/// 
///	Might not return "the best" but for almost all use cases will be good enough. Much more efficient
/// 
/// </summary>
/// <param name="curPos"></param>
/// <param name="tarPos"></param>
/// <returns></returns>
vector<node*> Pathfinding::FindPath(XMFLOAT3 curPos, XMFLOAT3 tarPos)
{
	SetupPath(curPos, tarPos);

	auto distance = [](node* a, node* b)
	{
		return sqrtf((a->pos.x - b->pos.x) * (a->pos.x - b->pos.x) + (a->pos.y - b->pos.y) * (a->pos.y - b->pos.y));
	};

	node* nodeCurrentTemp = m_nodeStart;
	m_nodeStart->m_gCost = 0.0f;
	m_nodeStart->m_hCost = distance(m_nodeStart, m_nodeEnd);

	m_nodesToTest.push_back(m_nodeStart);



	//This permutation will not find the absolute shortest, just one of the shortest
	//Remove m_nodeEnd from the while loop to stop this. Consider another function
	
	//For enemy AI it will be a good way to use this current version as the AI doesnt just b-line the fastest route.

	while (!m_nodesToTest.empty() && nodeCurrentTemp != m_nodeEnd)
	{
		sort(m_nodesToTest.begin(), m_nodesToTest.end(), [](const node* a, const node* b)
			{
				return a->m_hCost < b->m_hCost;
			});

		while (!m_nodesToTest.empty() && m_nodesToTest.front()->m_visited)
		{
			pop_front(m_nodesToTest);
		}

		if (m_nodesToTest.empty())
			break;

		nodeCurrentTemp = m_nodesToTest.front();
		nodeCurrentTemp->m_visited = true;

		for (auto neighbour : nodeCurrentTemp->m_neighbours)
		{
			//If the node isnt an obstacle and hasnt been visited already, we want to visit it now
			if (!neighbour->m_visited && neighbour->m_obstacle == 0)
			{
				m_nodesToTest.push_back(neighbour);
			}

			float gTest = nodeCurrentTemp->m_gCost + distance(nodeCurrentTemp, neighbour);



			//Testing if the node cost is lower for this than what is currently on the neighbour
			if (gTest < neighbour->m_gCost)
			{
				neighbour->m_parent = nodeCurrentTemp;
				neighbour->m_gCost = gTest;

				//updating the heuristic so we can prioritise the best 

				neighbour->m_hCost = neighbour->m_gCost + distance(neighbour, m_nodeEnd);




			}
		}
	}


	//Constructing the "path"
	//Follow the parent of the end node to the start node
	//Path is backwards in this setting, use std::list if wanting front to back but std list is bad so using vector and doing it backwards
	vector<node*>path;
	node* targetNode = new node();
	targetNode->pos = tarPos;
	//path.push_back(targetNode);



	if (m_nodeEnd != nullptr)
	{
		nodeCurrentTemp = m_nodeEnd;
		
		while (nodeCurrentTemp->m_parent != nullptr)
		{
			path.push_back(nodeCurrentTemp);
			nodeCurrentTemp = nodeCurrentTemp->m_parent;
		}


	}

	return path;

}


/// <summary>
/// 
/// Finds the perfect path by checking the whole connection tree before creating a path, could cause slowness if lots of nodes & connections
/// 
/// </summary>
/// <param name="curPos"></param>
/// <param name="tarPos"></param>
/// <returns></returns>
vector<node*> Pathfinding::FindPerfectPath(XMFLOAT3 curPos, XMFLOAT3 tarPos)
{
	SetupPath(curPos, tarPos);

	auto distance = [](node* a, node* b)
	{
		return sqrtf((a->pos.x - b->pos.x) * (a->pos.x - b->pos.x) + (a->pos.y - b->pos.y) * (a->pos.y - b->pos.y));
	};

	node* nodeCurrentTemp = m_nodeStart;
	m_nodeStart->m_gCost = 0.0f;
	m_nodeStart->m_hCost = distance(m_nodeStart, m_nodeEnd);

	m_nodesToTest.push_back(m_nodeStart);


	//This permutation will not find the absolute shortest, just one of the shortest
	//Remove m_nodeEnd from the while loop to stop this. Consider another function

	//For enemy AI it will be a good way to use this current version as the AI doesnt just b-line the fastest route.

	while (!m_nodesToTest.empty())
	{
		while (!m_nodesToTest.empty() && m_nodesToTest.front()->m_visited)
		{
			pop_front(m_nodesToTest);
		}

		if (m_nodesToTest.empty())
			break;

		nodeCurrentTemp = m_nodesToTest.front();
		nodeCurrentTemp->m_visited = true;

		for (auto neighbour : nodeCurrentTemp->m_neighbours)
		{
			//If the node isnt an obstacle and hasnt been visited already, we want to visit it now
			if (!neighbour->m_visited && neighbour->m_obstacle == 0)
			{
				m_nodesToTest.push_back(neighbour);
			}

			float gTest = nodeCurrentTemp->m_gCost + distance(nodeCurrentTemp, neighbour);



			//Testing if the node cost is lower for this than what is currently on the neighbour
			if (gTest < neighbour->m_gCost)
			{
				neighbour->m_parent = nodeCurrentTemp;
				neighbour->m_gCost = gTest;
			}
		}
	}


	//Constructing the "path"
	//Follow the parent of the end node to the start node
	//Path is backwards in this setting, use std::list if wanting front to back but std list is bad so using vector and doing it backwards
	vector<node*>path;
	node* targetNode = new node();
	targetNode->pos = tarPos;
	path.push_back(targetNode);



	if (m_nodeEnd != nullptr)
	{
		nodeCurrentTemp = m_nodeEnd;
		
		while (nodeCurrentTemp->m_parent != nullptr)
		{
			path.push_back(nodeCurrentTemp);
			nodeCurrentTemp = nodeCurrentTemp->m_parent;
		}


	}

	return path;


}



Pathfinding* Pathfinding::Instance()
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
	node* l_closestNode = nullptr;
	float closestDist = INFINITY;
	float currDist;

	for (int x = 0; x < m_mapWidth; x++)
	{
		for (int y = 0; y < m_mapHeight; y++)
		{
			currDist = sqrtf((pos.x - nodes[y * m_mapWidth + x].pos.x) * (pos.x - nodes[y * m_mapWidth + x].pos.x) + (pos.y - nodes[y * m_mapWidth + x].pos.y) * (pos.y - nodes[y * m_mapWidth + x].pos.y));

			if (currDist < closestDist)
			{
				closestDist = currDist;
				l_closestNode = &nodes[y * m_mapWidth + x];
			}
		}
	}

	return l_closestNode;

}


/// <summary>
/// 
/// If the tilemap changes init needs to be called again to reconnect the tile tree together
/// 
/// 
/// </summary>
/// <param name="map"></param>
void Pathfinding::Initialize(TileMap map)
{



	m_mapWidth = map.GetWidth();
	m_mapHeight = map.GetHeight();

	//setting up a tile grid for testing. This will be come from the premade tile grid
	nodes = new node[m_mapWidth * m_mapHeight];
	for (int x = 0; x < m_mapWidth; ++x)
	{
		for (int y = 0; y < m_mapHeight; ++y)
		{
			nodes[y * m_mapWidth + x].pos = map.GetTileFromMapPos(x,y)->GetTransform()->GetPosition();
			map.GetTileFromMapPos(x, y);
			//If check for obstacle tile here


			nodes[y * m_mapWidth + x].m_obstacle = false; 
			nodes[y * m_mapWidth + x].m_parent = nullptr;
			nodes[y * m_mapWidth + x].m_visited = false;
		}
	}

	//hardcoded, start will be passed in by object and objective will be passed in aswell
	//m_nodeStart = &nodes[(m_mapHeight/2) * m_mapWidth + 1];
	//m_nodeEnd = &nodes[(m_mapHeight / 2) * m_mapWidth + m_mapWidth - 2];


	//Connecting local nodes
	for (int x = 0; x < m_mapWidth; ++x)
	{
		for (int y = 0; y < m_mapHeight; ++y)
		{

			if (y > 0)
				nodes[y * m_mapWidth + x].m_neighbours.push_back(&nodes[(y - 1) * m_mapWidth + (x)]);
			if (y < m_mapHeight - 1)
				nodes[y * m_mapWidth + x].m_neighbours.push_back(&nodes[(y + 1) * m_mapWidth + (x)]);
			if (x > 0)
				nodes[y * m_mapWidth + x].m_neighbours.push_back(&nodes[(y)*m_mapWidth + (x - 1)]);
			if (x < m_mapWidth - 1)
				nodes[y * m_mapWidth + x].m_neighbours.push_back(&nodes[(y)*m_mapWidth + (x + 1)]);


			//Diagonal nodes if wanted, more of a question but could slow down the system as it double the number of paths for each node (exponentially more searching)
			//To enable diagonal nodes change DIAGONAL_ENABLED to true
			if (DIAGONAL_ENABLED)
			{
				if (y > 0 && x > 0)
					nodes[y * m_mapWidth + x].m_neighbours.push_back(&nodes[(y - 1) * m_mapWidth + (x - 1)]);
				if (y < m_mapWidth - 1 && x>0)
					nodes[y * m_mapWidth + x].m_neighbours.push_back(&nodes[(y + 1) * m_mapWidth + (x - 1)]);
				if (y > 0 && x < m_mapWidth - 1)
					nodes[y * m_mapWidth + x].m_neighbours.push_back(&nodes[(y - 1) * m_mapWidth + (x + 1)]);
				if (y < m_mapWidth - 1 && x < m_mapWidth - 1)
					nodes[y * m_mapWidth + x].m_neighbours.push_back(&nodes[(y + 1) * m_mapWidth + (x + 1)]);



			}


		}
	}


}
