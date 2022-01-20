#pragma once
#include <vector>


//We need access to the floor map & any extra information such as up/down componants to create the neighbours list
struct node
{
	bool m_obstacle = false;
	bool m_visited = false;
	int x;
	int y;

	float m_gCost; //total distance travelled to get to node
	float m_hCost; //heuristic cost (do we move towards the goal)
	vector<node*> m_neighbours;
	node* m_parent;

};





class Pathfinding
{
public:
	Pathfinding(XMFLOAT3* curPos);


	//test 16x16 grid 
	unsigned int mapwidth = 16;
	unsigned int mapheight = 16;
	node* nodes = nullptr;



	//These will need to be worked out based on the current positions of the destination, which may not be exactly on a node, so will need to lock the destination node?
	node* m_nodeStart = nullptr;
	node* m_nodeEnd = nullptr;


	void Solve(XMFLOAT3* tarPos);

	vector<node*> m_path;
	XMFLOAT3* m_curPos;



	//pass in current unit pos & destination pos
	//returns array of orders

public:
	void Update();
	vector<node*> GetPath() const { return m_path; }


	node* FindClosestNode(XMFLOAT3* pos);
	node* GetCurrentNode();


};

