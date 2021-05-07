#include "Graph.h"

using std::set;
using std::string;
using std::vector;
//creates a graph out of a string by dividing to tokens which are '{' 'ver_name' ',' '|' '<' 'ver_name' '>' '}'
//first while extarcts the verteices and the second one extracts the vertices, throw exception if sysntax is invalid
Graph::Graph(const std::string& str)
{
	if (str.empty() || str.front() != '{' || str.back() != '}')//the graph definition is  
		throw InvalidGraphDefinition(str);
	std::vector<std::string> tokens = tokenize(str.substr(1, str.size() - 2), '<', '>');
	// remove the {} and tokenize to words, tokes starting with < and ending with >, other as single char tokens 
	if (tokens.empty() || ((tokens.size() == 1 && tokens.front() == "|")))//handle {} and {|} which are empty graph
		return;

	int tokenIndex = 0;
	while (tokenIndex < (int)tokens.size())//iterate on the tokens we got
	{
		const std::string& token = tokens[tokenIndex];//token is the current token we will handle

		if (tokenIndex % 2 == 0)// tokens are :vertex, comma, vertex, comma ... so on the odd we have vertices
		{
			if (!isValidVertexName(token))
				throw InvalidVertexName("Try <src,dest> Seperated With ,");//VertexRepit
			if (!vertices.emplace(token).second)//the emplace return a pair the second is if the token is already existd
				throw VertexRepit(token);
		}
		else//handle the last char of the vertices input check if it ende
		{
			if (token == "|")//move to read edges input, end this loop
			{
				++tokenIndex;
				break;
			}
			else if (token != ",")//after the last vertex there was a comma
				throw InvalidGraphDefinition("");
		}

		++tokenIndex;//move to the next token in the arr
	}

	while (tokenIndex < (int)tokens.size())
	{
		const std::string& token = tokens[tokenIndex];

		if (tokenIndex % 2 == 0)// tokens are <vertex,vertec> ,comma, <vertex,vertec>, comma...
		{
			if (token.front() != '<')
				throw InvalidEdgeName(" Problem With '>' ");
			if (token.back() != '>')
				throw InvalidEdgeName(" Problem With '>' ");
			std::vector<std::string> edgeTokens = tokenize(token.substr(1, token.size() - 2));
			if (edgeTokens.size() != 3 || edgeTokens[1] != ",")//there is no comma in the middle or two vertex names
				throw InvalidEdgeName(" Missing ',' Or Vertecies Defining The Edge ");
			if (edgeTokens[0] == edgeTokens[2])
				throw SelefLoop(" <" + edgeTokens[0] +","+ edgeTokens[2]+"> ");
			if (vertices.find(edgeTokens[0]) == vertices.end())
				throw InvalidEdgeName(" Edge Source Vertex Does Not Exists: "+edgeTokens[0]);
			if (vertices.find(edgeTokens[2]) == vertices.end())
				throw InvalidEdgeName(" Edge Destenation Vertex Does Not Exists: " + edgeTokens[2]);
			if (!edges.emplace(edgeTokens[0], edgeTokens[2]).second)
				throw EdgeRepit("<" + edgeTokens[0] + "," + edgeTokens[2]+">");
		}
		else
		{
			if (token != ",")
				throw InvalidGraphDefinition(" There Is a Problem With ','");
		}

		++tokenIndex;
	}

	if (tokens.back() == ",")//validate the last token is not a comma , since we used %2 no need to check on both ends
		throw InvalidGraphDefinition("Probably a , Problem In Your Graph Definition");
}
//takes a valid graph and convert it into a string that the con' can reverse back to a valid Graph
std::string Graph::graphToString()
{
	string res;
	res += "{";
	for (const auto& ver : this->vertices)
	{
		res += ver;

		res += ",";
	}
	if (vertices.size() >= 1)
	{
		res.pop_back();
	}
	res += "|";

	for (const auto& edg : this->edges)
	{
		res += ("<" + edg.first + "," + edg.second + ">");
		res += ",";
	}
	if (edges.size() >= 1)
	{
		res.pop_back();
	}
	res += "}";
	return res;
}
//prints the graph notice that set is already sorted
void Graph::print() const
{
	for (string ver : this->vertices)
	{
		std::cout << ver << std::endl;
	}
	std::cout << "$" << std::endl;
	for (std::pair<string, string> edg : this->edges)
	{
		std::cout << edg.first + " " + edg.second << std::endl;
	}
}
//return the sum of the grahps 
Graph operator+(const Graph& g1, const Graph& g2)
{
	Graph result = g1;
	result.vertices.insert(g2.vertices.begin(), g2.vertices.end());
	result.edges.insert(g2.edges.begin(), g2.edges.end());
	return result;
}
//return the  of the grahps
Graph operator^(const Graph& g1, const Graph& g2)
{
	Graph result;
	for (const auto& ver1 : g1.vertices)
	{
		if (g2.vertices.find(ver1) != g2.vertices.end())
		{
			result.vertices.insert(ver1);
		}
	}
	for (const auto& edg1 : g1.edges)
	{
		if (g2.edges.find(edg1) != g2.edges.end())
		{
			result.edges.insert(edg1);
		}
	}

	return result;
}
//return the difference of the grahps
Graph operator-(const Graph& g1, const Graph& g2)
{
	Graph result;
	for (const auto& ver1 : g1.vertices)
	{
		if (g2.vertices.find(ver1) == g2.vertices.end())
		{
			result.vertices.insert(ver1);
		}
	}
	for (const auto& edge : g1.edges)
	{
		if ((result.vertices.find(edge.first) != result.vertices.end()) && (result.vertices.find(edge.second) != result.vertices.end()))
		{
			result.edges.insert(edge);
		}
	}
	return result;
}
//return the product of the graphs
Graph operator*(const Graph& g1, const Graph& g2)
{
	Graph result;
	for (const auto& ver_g1 : g1.vertices)
	{
		for (const auto& ver_g2 : g2.vertices)
		{
			result.vertices.emplace('[' + ver_g1 + ';' + ver_g2 + ']');
		}
	}
	for (const auto& edg_g1 : g1.edges)
	{
		for (const auto& edg_g2 : g2.edges)
		{
			//avoid self loops, we won't have paralle edges because we itrate on each ver one time as a source

			result.edges.emplace('[' + edg_g1.first + ';' + edg_g2.first + ']', '[' + edg_g1.second + ';' + edg_g2.second + ']');

		}
	}

	return result;
}
//an unary operator
Graph Graph::operator!()
{
	Graph result;
	result.vertices.insert(this->vertices.begin(), this->vertices.end());
	for (const auto& ver_src : result.vertices)
	{
		for (const auto& ver_dest : result.vertices)
		{
			if (ver_src != ver_dest) //avoid self loops
			{
				std::pair<string, string> edge_to_add(ver_src, ver_dest);
				if (this->edges.find(edge_to_add) == this->edges.end())
				{
					result.edges.insert(edge_to_add);
				}
			}

		}
	}
	return result;
}
const std::set<std::string>& Graph::getVertices() const  
{
	return this->vertices;
}
const std::set<std::pair<std::string, std::string>>& Graph::getEdges() const 
{
	return this->edges;
}
 std::set<std::string>& Graph::setVertices() 
{
	return this->vertices;
}
 std::set<std::pair<std::string, std::string>>& Graph::setEdges() 
{
	return this->edges;
}
//functions for python warp will return a nullptr if an error occured writes result to the graph_out and return it for chaning
Graph* create()
{
	Graph* g = new Graph();
	return g;
}
//delete the given pointer value
void destroy(Graph* g)
{
	delete g;
}
Graph* graphUnion(Graph* graph_in1, Graph* graph_in2, Graph* graph_out)
{
	if (isNull(graph_in1, graph_in2))
	{
		std::cout << "Error: Cannot Evaluate Union Got nullptr" << std::endl;
		return nullptr;
	}
	*graph_out = *(graph_in1) + *(graph_in2);
	return graph_out;
}
//return  true if one of the the grhaps are nullptr
bool isNull(Graph* graph_in1, Graph* graph_in2) 
{
	return graph_in1 == nullptr || graph_in2 == nullptr;
}
//return true if the pointer is null
bool isNull(Graph* graph)
{
	return graph == nullptr;
}
//same operation as the regualr operators
Graph* graphIntersection(Graph* graph_in1, Graph* graph_in2, Graph* graph_out)
{
	if (isNull(graph_in1, graph_in2))
	{
		std::cout << "Error: Cannot Evaluate Intersection Got nullptr" << std::endl;
		return nullptr;
	}
	*graph_out = *(graph_in1)^*(graph_in2);
	return graph_out;
}
Graph* graphDifference(Graph* graph_in1, Graph* graph_in2, Graph* graph_out)
{
	if (isNull(graph_in1, graph_in2))
	{
		std::cout << "Error: Cannot Evaluate Difference Got nullptr" << std::endl;
		return nullptr;
	}
	*graph_out = *(graph_in1)-*(graph_in2);
	return graph_out;
}
Graph* graphProduct(Graph* graph_in1, Graph* graph_in2, Graph* graph_out)
{
	if (isNull(graph_in1, graph_in2))
	{
		std::cout << "Error: Cannot Evaluate Product Got nullptr" << std::endl;
		return nullptr;
	}
	*graph_out = (*(graph_in1))*(*(graph_in2));
	return graph_out;
}
Graph* graphComplement(Graph* graph_in, Graph* graph_out)
{
	if (graph_in==nullptr)
	{
		std::cout << "Error: Cannot Evaluate Complement Got nullptr" << std::endl;
		return nullptr;
	}
	*graph_out = !(*(graph_in));
	return graph_out;
}
//adds a vertex to the given graph
Graph* addVertex(Graph* graph_in, char* ver_name)
{
	if (isNull(graph_in)) 
	{
		std::cout << "Error: Cannot Add Vertex Got nullptr" << std::endl;
		return nullptr;
	}
	auto it = (*graph_in).getVertices().find(ver_name);
	if (it != (*graph_in).getVertices().end()) {//vertex  already exsits 
		std::cout << "Error: Vertex Cannot Repit in Graph: " << ver_name << std::endl;
		return nullptr;
	}
	else {
		if (isValidVertexName(ver_name))
		{
			(*graph_in).setVertices().emplace(ver_name);
		}
		else 
		{
			std::cout << "Error: Vertex Name is invalid: " << ver_name << std::endl;
			return nullptr;
		}
	}
	return graph_in;
}
//adds an edge to a given graph
Graph* addEdge(Graph* graph_in, char* src_edge, char* dest_edge) 
{
	if (isNull(graph_in))
	{
		std::cout << "Error: Cannot Add  Edge Got nullptr" << std::endl;
		return nullptr;
	}
	for (const auto& edg : (*graph_in).getEdges()) //check if edge already exists
	{
		if (edg.first == src_edge && edg.second == dest_edge) 
		{
			std::cout << "Error: Edge Cannot Repit in Graph: " << "source: " << edg.first << " destenation: " << edg.second << std::endl;
			return nullptr;

		}
	}
	auto it = (*graph_in).getVertices().find((src_edge));//check the src is a vertex in graph
	if (it == (*graph_in).getVertices().end()) 
	{
		std::cout << "Error: Edge src vertex does not exsist in graph: " << src_edge <<std::endl;
		return nullptr;
	}
	it = (*graph_in).getVertices().find((dest_edge));//check the dest is a vertex in graph
	if (it == (*graph_in).getVertices().end())
	{
		std::cout << "Error: Edge dest vertex does not exsist in graph: " << dest_edge << std::endl;
		return nullptr;

	}
	if (!isValidVertexName(src_edge)) //check if the src ver name is valid
	{
		std::cout << "Error: Edge src Vertex name is invalid: " << src_edge << std::endl;
		return nullptr;
	}
	if (!isValidVertexName(dest_edge))//check if the dest ver name is valid
	{
		std::cout << "Error: Edge src Vertex name is invalid: " << dest_edge << std::endl;
		return nullptr;
	}
	if (src_edge == dest_edge) //check if the the edge is a self loop
	{
		std::cout << "Error: Edge cannot be a self loop: " << src_edge<< "," <<dest_edge << std::endl;
		return nullptr;
	}
	(*graph_in).setEdges().emplace(src_edge,dest_edge);
	return graph_in;
}
//print the grhap
void disp(Graph* graph_in)
{
	if (isNull(graph_in))
	{
		std::cout << "Error: Cannot Print Got nullptr"<<std::endl;
	}else
    (*graph_in).print();
}