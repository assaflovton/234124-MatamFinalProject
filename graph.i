%module graph
%{
#include "Graph.h"
%}
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
