#include "Pathfinding.h"
#include <algorithm>


//Custom pop_front vector function because vector doesnt have one. Remove if swapping to lists. 
template<typename T>
void pop_front(vector<T>& vec)
{
	assert(!vec.empty());
	vec.erase(vec.begin());
}

Pathfinding::Pathfinding()
{
	//setting up a tile grid for testing. This will be come from the premade tile grid
	nodes = new node[mapwidth * mapheight];
	for (int x = 0; x < mapwidth; ++x)
	{
		for (int y = 0; y < mapheight; ++y)
		{
			nodes[y * mapwidth + x].pos.x = x * 32;
			nodes[y * mapwidth + x].pos.y = y * 32;
			nodes[y * mapwidth + x].m_obstacle = false;
			nodes[y * mapwidth + x].m_parent = nullptr;
			nodes[y * mapwidth + x].m_visited = false;
		}
	}

	//hardcoded, start will be passed in by object and objective will be passed in aswell
	//m_nodeStart = &nodes[(mapheight/2) * mapwidth + 1];
	//m_nodeEnd = &nodes[(mapheight / 2) * mapwidth + mapwidth - 2];


	//Connecting local nodes
	for (int x = 0; x < mapwidth; ++x)
	{
		for (int y = 0; y < mapheight; ++y)
		{
			
			if (y > 0)
				nodes[y * mapwidth + x].m_neighbours.push_back(&nodes[(y - 1) * mapwidth + (x)]);
			if (y < mapheight - 1)
				nodes[y * mapwidth + x].m_neighbours.push_back(&nodes[(y + 1) * mapwidth + (x)]);
			if (x > 0)
				nodes[y * mapwidth + x].m_neighbours.push_back(&nodes[(y) * mapwidth + (x - 1)]);
			if (x < mapwidth - 1)
				nodes[y * mapwidth + x].m_neighbours.push_back(&nodes[(y) * mapwidth + (x + 1)]);


			//Diagonal nodes if wanted, more of a question but could slow down the system as it double the number of paths for each node (exponentially more searching)
			//To enable diagonal nodes change DIAGONAL_ENABLED to true
			if (DIAGONAL_ENABLED)
			{
				if (y > 0 && x > 0)
					nodes[y * mapwidth + x].m_neighbours.push_back(&nodes[(y - 1) * mapwidth + (x - 1)]);
				if (y < mapwidth - 1 && x>0)
					nodes[y * mapwidth + x].m_neighbours.push_back(&nodes[(y + 1) * mapwidth + (x - 1)]);
				if (y > 0 && x < mapwidth - 1)
					nodes[y * mapwidth + x].m_neighbours.push_back(&nodes[(y - 1) * mapwidth + (x + 1)]);
				if (y < mapwidth - 1 && x < mapwidth - 1)
					nodes[y * mapwidth + x].m_neighbours.push_back(&nodes[(y + 1) * mapwidth + (x + 1)]);



			}


		}
	}



}



void Pathfinding::SetupPath(XMFLOAT3 curPos, XMFLOAT3 tarPos)
{
	m_nodeStart = FindClosestNode(curPos);
	m_nodeEnd = FindClosestNode(tarPos);

	//Reset navigation grid
	for (int x = 0; x < mapwidth; ++x)
	{
		for (int y = 0; y < mapheight; ++y)
		{
			nodes[y * mapwidth + x].m_hCost = INFINITY;
			nodes[y * mapwidth + x].m_gCost = INFINITY;
			nodes[y * mapwidth + x].m_parent = nullptr;
			nodes[y * mapwidth + x].m_visited = false;
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

	for (int x = 0; x < mapwidth; x++)
	{
		for (int y = 0; y < mapheight; y++)
		{
			currDist = sqrtf((pos.x - nodes[y * mapwidth + x].pos.x) * (pos.x - nodes[y * mapwidth + x].pos.x) + (pos.y - nodes[y * mapwidth + x].pos.y) * (pos.y - nodes[y * mapwidth + x].pos.y));

			if (currDist < closestDist)
			{
				closestDist = currDist;
				l_closestNode = &nodes[y * mapwidth + x];
			}
		}
	}

	return l_closestNode;

}