#ifndef GRAPH
#define GRAPH
#include <set>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "Exceptions.h"
#include "ParseUtility.h"
class Graph {
public:
	Graph() = default;
	Graph(const std::string& str);
	~Graph() = default;
	Graph(const Graph& g) = default;
	Graph& operator=(const Graph& g) = default;
	Graph& operator=(Graph&& g) = default;
	Graph operator!();
	std::string graphToString();
	friend Graph operator+(const Graph& g1, const Graph& g2);
	friend Graph operator^(const Graph& g1, const Graph& g2);
	friend Graph operator-(const Graph& g1, const Graph& g2);
	friend Graph operator*(const Graph& g1, const Graph& g2);
	void print() const;
	const std::set<std::string>& getVertices() const;
	const std::set<std::pair<std::string, std::string>>& getEdges() const;
	std::set<std::string>& setVertices();
	std::set<std::pair<std::string, std::string>>& setEdges();
private:
	std::set<std::string> vertices;
	std::set<std::pair<std::string, std::string>> edges;
};
bool isNull(Graph* graph_in1, Graph* graph_in2);
bool isNull(Graph* graph);
Graph* create();
void destroy(Graph* g);
Graph* graphUnion(Graph* graph_in1, Graph* graph_in2, Graph* graph_out);
Graph* graphIntersection(Graph* graph_in1, Graph* graph_in2, Graph* graph_out);
Graph* graphDifference(Graph* graph_in1, Graph* graph_in2, Graph* graph_out);
Graph* graphProduct(Graph* graph_in1, Graph* graph_in2, Graph* graph_out);
Graph* graphComplement(Graph* graph_in, Graph* graph_out);
Graph* addVertex(Graph* graph_in, char* ver_name);
Graph* addEdge(Graph* graph_in, char* src_edge, char* dest_edge);
void disp(Graph* graph_in);
#endif //GRAPH