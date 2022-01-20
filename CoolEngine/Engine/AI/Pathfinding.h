#pragma once
#include <vector>

#define DIAGONAL_ENABLED true

//We need access to the floor map & any extra information such as up/down componants to create the neighbours list
struct node
{
	bool m_obstacle = false;
	bool m_visited = false;

	XMFLOAT3 pos;

	float m_gCost; //total distance travelled to get to node
	float m_hCost; //heuristic cost (do we move towards the goal)
	vector<node*> m_neighbours;
	node* m_parent;

};





class Pathfinding
{
public:
	vector<node*> FindPath(XMFLOAT3 curPos,XMFLOAT3 tarPos);
	vector<node*> FindPerfectPath(XMFLOAT3 curPos, XMFLOAT3 tarPos);


	static Pathfinding* Instance();
	node* FindClosestNode(XMFLOAT3 pos);
	

private:

	//test 16x16 grid 
	unsigned int mapwidth = 16;
	unsigned int mapheight = 16;
	node* nodes = nullptr;

	Pathfinding();
	void SetupPath(XMFLOAT3 curPos,XMFLOAT3 tarPos);


	vector<node*> m_nodesToTest;

	node* m_nodeStart = nullptr;
	node* m_nodeEnd = nullptr;




};



//Code for working out if you're at the destination node and to go to the next one in a very simple sense 

/*
if (m_path.size() != 0)
{
	//if the closest node to us is the target node change to next node in the sequence, if this creates weird interactions then change.
	if (m_path.back() == FindClosestNode(&m_curPos))
	{
		m_path.pop_back();
	}

}
*/