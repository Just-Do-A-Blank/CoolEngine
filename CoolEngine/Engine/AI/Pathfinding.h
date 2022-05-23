#pragma once
#include <vector>
#include <queue>

#define DIAGONAL_ENABLED true

//We need access to the floor map & any extra information such as up/down componants to create the neighbours list
struct node
{
	bool m_obstacle = false;
	bool m_visited = false;

	XMFLOAT3 m_pos = { 0,0,0 };

	float m_gCost; //total distance travelled to get to node
	float m_hCost; //heuristic cost (do we move towards the goal)
	vector<node*> m_pNeighbours;
	node* m_pParent = nullptr;

};

class TileMap;

//Helper Function & class for comparing the priority_queue nodes so the closest node is always at the front of the queue
class NodeCompare
{
public:
	bool operator() (node* a, node* b)
	{
		return a->m_hCost > b->m_hCost;
	}
};


class Pathfinding
{
public:
	bool FindPath(XMFLOAT3 curPos,XMFLOAT3 tarPos, vector<node*>& path);
	bool FindPerfectPath(XMFLOAT3 curPos, XMFLOAT3 tarPos, vector<node*>& path);


	static Pathfinding* GetInstance();
	node* FindClosestNode(XMFLOAT3 pos);

	void Initialize(TileMap* map);

private:


	int m_mapWidth = 0;
	int m_mapHeight = 0;
	vector<vector<node*>> m_pNodes;


	Pathfinding();
	void SetupPath(XMFLOAT3 curPos,XMFLOAT3 tarPos);
	void CalculatePath();
	bool GeneratePath(XMFLOAT3 tarPos, vector<node*>& path);

	priority_queue<node*,vector<node*>,NodeCompare> m_nodesToTest;



	node* m_pNodeStart = nullptr;
	node* m_pNodeEnd = nullptr;
};