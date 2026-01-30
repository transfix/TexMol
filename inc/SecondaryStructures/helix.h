#ifndef __HELIX_H__
#define __HELIX_H__

#include <SecondaryStructures/datastruct_ss.h>

//using namespace SecondaryStructures;

typedef pair< vector<SecondaryStructures::Point>, SecondaryStructures::Vector> Cylinder;

class CVertex
{
	public:
		CVertex()
		{
		}
		CVertex(const SecondaryStructures::Point& p)
		{
			pos = p;
		}
		SecondaryStructures::Point pos;
		int id;
		bool visited;
		vector<int> inc_vid_list;
};

class CEdge
{
	public:
		CEdge()
		{
		}
		CEdge(const int& v1, const int& v2)
		{
			ep[0] = v1;
			ep[1] = v2;
		}
		int ep[2];
};

class Curve
{
	public:
		Curve()
		{
		}
		vector<CVertex> vert_list;
		vector<CEdge> edge_list;
};

#endif
