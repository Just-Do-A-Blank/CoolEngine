#pragma once
#include <vector>


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
	
	static Pathfinding* Instance();
	node* FindClosestNode(XMFLOAT3 pos);
	//pass in current unit pos & destination pos
	//returns array of orders

private:

	//test 16x16 grid 
	unsigned int mapwidth = 16;
	unsigned int mapheight = 16;
	node* nodes = nullptr;

	Pathfinding();

	//These will need to be worked out based on the current positions of the destination, which may not be exactly on a node, so will need to lock the destination node?
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