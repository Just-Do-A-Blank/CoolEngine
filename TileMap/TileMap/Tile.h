#pragma once
#include <vector>

class Tile
{
public:
	Tile();
	Tile(int a);

	int GetID() 
	{ 
		try
		{
			if (ID != NULL)
			{
				return ID;
			}
		}
		catch(...)
		{
			return -25;
		}
	}
	void SetID(int id) { ID = id; }

	void SetEdges(bool N, bool E, bool S, bool W);

	void SetEdgeN(bool N);
	void SetEdgeS(bool S);
	void SetEdgeW(bool W);
	void SetEdgeE(bool E);

protected:

private:
	bool					m_edgeN, m_edgeS, m_edgeW, m_edgeE;
	int ID = -25;
};

