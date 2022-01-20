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
		for (int z = 0; z < mapheight; ++z)
		{
			nodes[z * mapwidth + x].pos.x = x;
			nodes[z * mapwidth + x].pos.z = z;
			nodes[z * mapwidth + x].m_obstacle = false;
			nodes[z * mapwidth + x].m_parent = nullptr;
			nodes[z * mapwidth + x].m_visited = false;
		}
	}

	//hardcoded, start will be passed in by object and objective will be passed in aswell
	m_nodeStart = &nodes[(mapheight/2) * mapwidth + 1];
	m_nodeEnd = &nodes[(mapheight / 2) * mapwidth + mapwidth - 2];


	//Connecting local nodes
	for (int x = 0; x < mapwidth; ++x)
	{
		for (int z = 0; z < mapheight; ++z)
		{
			
			if (z > 0)
				nodes[z * mapwidth + x].m_neighbours.push_back(&nodes[(z - 1) * mapwidth + (x)]);
			if (z < mapheight - 1)
				nodes[z * mapwidth + x].m_neighbours.push_back(&nodes[(z + 1) * mapwidth + (x)]);
			if (x > 0)
				nodes[z * mapwidth + x].m_neighbours.push_back(&nodes[(z) * mapwidth + (x - 1)]);
			if (x < mapwidth - 1)
				nodes[z * mapwidth + x].m_neighbours.push_back(&nodes[(z) * mapwidth + (x + 1)]);


			//Diagonal nodes if wanted, more of a question but could slow down the system as it double the number of paths for each node (exponentially more searching)
			/*
			if (y > 0 && x > 0)
				nodes[y * mapwidth + x].m_neighbours.push_back(&nodes[(y - 1) * mapwidth + (x - 1)]);
			if (y < mapwidth - 1 && x>0)
				nodes[y * mapwidth + x].m_neighbours.push_back(&nodes[(y + 1) * mapwidth + (x - 1)]);
			if (y > 0 && x < mapwidth - 1)
				nodes[y * mapwidth + x].m_neighbours.push_back(&nodes[(y - 1) * mapwidth + (x + 1)]);
			if (y < mapwidth - 1 && x < mapwidth - 1)
				nodes[y * mapwidth + x].m_neighbours.push_back(&nodes[(y + 1) * mapwidth + (x + 1)]);
				*/

		}
	}



}


/// <summary>
///
/// 
/// Solves the A* algorithm, returns a back to front vector of the path (the last node is the first in the vector)
/// Might not return "the best" but returns the best with pruning, making the search significantly more efficient for what would effectively be the same result
/// 
/// </summary>
/// <returns></returns>
vector<node*> Pathfinding::FindPath(XMFLOAT3 curPos, XMFLOAT3 tarPos)
{


	m_nodeStart = FindClosestNode(curPos);
	m_nodeEnd = FindClosestNode(tarPos);

	//Reset navigation grid
	for (int x = 0; x < mapwidth; ++x)
	{
		for (int z = 0; z < mapheight; ++z)
		{
			nodes[z * mapwidth + x].m_hCost = INFINITY;
			nodes[z * mapwidth + x].m_gCost = INFINITY;
			nodes[z * mapwidth + x].m_parent = nullptr;
			nodes[z * mapwidth + x].m_visited = false;
		}
	}


	auto distance = [](node* a, node* b)
	{
		return sqrtf((a->pos.x - b->pos.x) * (a->pos.x - b->pos.x) + (a->pos.z - b->pos.z) * (a->pos.z - b->pos.z));
	};

	node* nodeCurrentTemp = m_nodeStart;
	m_nodeStart->m_gCost = 0.0f;
	m_nodeStart->m_hCost = distance(m_nodeStart, m_nodeEnd);

	vector<node*> nodesToTest; //could use list instead. vector sort is significantly more efficient but list allows pop_front predefined. Vectors need a custom function (at the top). Speed needs to be tested.
	nodesToTest.push_back(m_nodeStart);


	//This permutation will not find the absolute shortest, just one of the shortest
	//Remove m_nodeEnd from the while loop to stop this. Consider another function
	
	//For enemy AI it will be a good way to use this current version as the AI doesnt just b-line the fastest route.

	while (!nodesToTest.empty() && nodeCurrentTemp != m_nodeEnd)
	{
		sort(nodesToTest.begin(), nodesToTest.end(), [](const node* a, const node* b)
			{
				return a->m_hCost < b->m_hCost;
			});

		while (!nodesToTest.empty() && nodesToTest.front()->m_visited)
		{
			pop_front(nodesToTest);
		}

		if (nodesToTest.empty())
			break;

		nodeCurrentTemp = nodesToTest.front();
		nodeCurrentTemp->m_visited = true;

		for (auto neighbour : nodeCurrentTemp->m_neighbours)
		{
			//If the node isnt an obstacle and hasnt been visited already, we want to visit it now
			if (!neighbour->m_visited && neighbour->m_obstacle == 0)
			{
				nodesToTest.push_back(neighbour);
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
		for (int z = 0; z < mapheight; z++)
		{
			currDist = MathHelper::Distance(nodes[z * mapwidth + x].pos, pos);
			//currDist = sqrtf((pos->x - nodes[z * mapwidth + x].pos.x) * (pos->x - nodes[z * mapwidth + x].pos.x) + (pos->z - nodes[z * mapwidth + x].pos.z) * (pos->z - nodes[z * mapwidth + x].pos.z));

			if (currDist < closestDist)
			{
				closestDist = currDist;
				l_closestNode = &nodes[z * mapwidth + x];
			}
		}
	}

	return l_closestNode;
	

}