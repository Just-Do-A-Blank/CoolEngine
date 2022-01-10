#pragma once
#include <vector>

class Tile
{
public:
	Tile(int a);

	void setID(int id) { ID = id; }
	int getID() { return ID; }

protected:

private:
	std::vector<bool>		m_edges;
	int ID;
};

