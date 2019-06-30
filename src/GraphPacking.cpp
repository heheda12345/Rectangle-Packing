#include <cstdlib>
#include <iostream>
#include <typeinfo>
#include <cassert>

#include <boost/config.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/bellman_ford_shortest_paths.hpp>
#include <boost/graph/topological_sort.hpp>

#include "SPPackingCommand.h"
#include "Layout.h"

namespace RECTPACKING {
using namespace std;
using namespace boost;
static void FloydWarshallShortestPath(int **d, int num_nodes)
{
    for (int k=0; k<num_nodes; k++)
        for (int i=0; i<num_nodes; i++)
            for (int j=0; j<num_nodes; j++)
                if (d[i][k] != INT_MAX && d[k][j] != INT_MAX && d[i][k] + d[k][j] < d[i][j])
                {
                    d[i][j] = d[i][k] + d[k][j];
                }
}

typedef std::pair<int, int> Edge;

inline int getLength(Layout& layout, int id, bool hor)
{
    vector<Rect> &rects = layout.getRects();
    return hor?rects[id].width:rects[id].height;
}

static void getEdges(const vector<pair<int, int> > &cons, Layout& layout, bool hor, vector<Edge> &edges)
{
	vector<Rect> &rects = layout.getRects();
	const int rectNum = rects.size();
	const int sourceId = rectNum;

	if (hor)
	{
		for (int i = 0; i < rectNum; ++ i)
		{
			//cout << "Add edge <" << sourceId << "," << i << "> " << -rects[i].width << endl;
			edges.push_back(Edge(sourceId, i));
			//weights.push_back(-rects[i].width);
		}
		for (int i = 0; i < (int)cons.size(); ++ i)
		{
			//cout << "Add edge <" << cons[i].first << "," << cons[i].second << "> " << -rects[cons[i].second].width << endl;
			edges.push_back(Edge(cons[i].first, cons[i].second));
			//weights.push_back(-rects[cons[i].second].width);
		}
	}
	else
	{
		for (int i = 0; i < rectNum; ++ i)
		{
			//cout << "Add edge <" << sourceId << "," << i << "> " << -rects[i].height << endl;
			edges.push_back(Edge(sourceId, i));
			//weights.push_back(-rects[i].height);
		}
		for (int i = 0; i < (int)cons.size(); ++ i)
		{
			//cout << "Add edge <" << cons[i].first << "," << cons[i].second << "> " << -rects[cons[i].second].height << endl;
			edges.push_back(Edge(cons[i].first, cons[i].second));
			//weights.push_back(-rects[cons[i].second].height);
		}
	}
}

// implemented by topological sort and dynamic programming

static void OneDirectionPacking(vector<pair<int, int> > &cons, Layout &layout, bool hor)
{
	vector<Rect> &rects = layout.getRects();
	const int rectNum = rects.size();
	const int num_nodes = rectNum + 1;
	//const int sourceId = rectNum;
	//number of rects + virtual source
	vector<Edge> edges;

	getEdges(cons, layout, hor, edges);
	//cout << "Finished building the directed graph (edge list) with " << num_nodes << " vertices and " << edges.size() << " edges" << endl;

	typedef adjacency_list <vecS, vecS, directedS> Graph;
    typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
	typedef std::vector<Vertex> VertexContainer;

	Graph G(edges.begin(), edges.end(), num_nodes);
    VertexContainer c;
	topological_sort(G, std::back_inserter(c));

	typename graph_traits<Graph>::out_edge_iterator out_i,out_end;
    typename graph_traits<Graph>::edge_descriptor e;
    typename property_map<Graph, vertex_index_t>::type
        index = get(vertex_index, G);

    std::vector<int> d;
    d.assign(num_nodes,0);

	for (VertexContainer::reverse_iterator ii=c.rbegin(); ii!=c.rend(); ++ii)
    {
        Vertex v=*ii;
        int x=index[v];
        for (tie(out_i,out_end) = out_edges(v,G); out_i != out_end; ++out_i)
        {
            int y=index[target(*out_i,G)];
            d[y]=max(d[y],d[x]+getLength(layout,y,hor));
        }

    }

	for (int i = 0; i < rectNum; i++)
	{
		int dis = d[i];
		//std::cout<< " Rect " << i << ": " << dis <<endl;
		if (hor)
		{
			rects[i].lb.x = dis - rects[i].width;
		}
		else
		{
			rects[i].lb.y = dis - rects[i].height;
		}
	}
}


// implemented by Bellman Ford shortest path
/*
static void OneDirectionPacking(vector<pair<int, int> > &cons, Layout &layout, bool hor)
{
	vector<Rect> &rects = layout.getRects();
	const int rectNum = rects.size();
	const int num_nodes = rectNum + 1;
	const int sourceId = rectNum;
	//number of rects + virtual source
	vector<Edge> edges;
	vector<int> weights;

	getEdges(cons, layout, hor, edges, weights);
	//cout << "Finished building the directed graph (edge list) with " << num_nodes << " vertices and " << edges.size() << " edges" << endl;

	graph_t g(edges.begin(), edges.end(), weights.begin(), num_nodes);
	// property_map<graph_t, edge_weight_t>::type weightmap = get(edge_weight, g);

	std::vector<vertex_descriptor> p(num_vertices(g));
	std::vector<int> d(num_vertices(g));
	vertex_descriptor s = vertex(sourceId, g);

	//bell_shortest_paths(g, s, predecessor_map(&p[0]).distance_map(&d[0]));
	bellman_ford_shortest_paths(g, s, predecessor_map(&p[0]).distance_map(&d[0]));
	for (int i = 0; i < rectNum; i++)
	{
		int dis = -d[i];
		//std::cout<< " Rect " << i << ": " << dis <<endl;
		if (hor)
		{
			rects[i].lb.x = dis - rects[i].width;
		}
		else
		{
			rects[i].lb.y = dis - rects[i].height;
		}
	}
}
*/

void LongestGraphPacking(vector<pair<int, int> > &horCons, vector<pair<int, int> > &verCons, Layout &layout)
{
	//cout << "In LongestGraphPacking " << endl;
	bool hor = true;
	//cout << "========== horizontal ==========" << endl;
	OneDirectionPacking(horCons, layout, hor);
	hor = false;
	// cout << "========== vertical ==========" << endl;
	OneDirectionPacking(verCons, layout, hor);
}

}
