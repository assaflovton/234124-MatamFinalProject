#include "Calc.h"
using std::string;
using std::map;
//return true if the grhap exists in map
bool Calc::isGraphExist(const std::string& name)
{
	auto itr = elements.find(name);
	return itr == elements.end();
}
//get a name of graph to delete if not exists throws an error
void Calc::deleteGraph(const std::string& name)
{
	auto itr = elements.find(name);
	if (itr != elements.end())
		this->elements.erase(name);
	else
	{
		throw UndefinedVariable(name);
	}
}
//adds a graph to the map key is the name and value is the graph itself
void Calc::addGraph(const std::string& name, const std::string& graphRepresentation)
{
	try
	{
		elements[name] = Graph(graphRepresentation);
	}
	catch (const std::runtime_error & e)
	{
		std::cerr << e.what();
	}
}
//takes an existing grahp and translate it to a string that the defualt con' of graph can change back
std::string Calc::graphToString(const std::string& name)
{
	auto itr = elements.find(name);
	if (itr != elements.end())
	{
		std::string graphWithoutName = itr->second.graphToString();
		return graphWithoutName;
	}
	else
	{
		throw UndefinedVariable(name);
	}


}//return a graph by a given name if the graph does not exsits throw exception
//return a grhap by a given name, if the graph  does not exists throw exceptione
Graph Calc::getGraphByName(const std::string& name)
{
	auto itr = this->elements.find(name);
	if (itr == this->elements.end())
		throw UndefinedVariable(name);
	return itr->second;


}
//adds a graph to the map key is the name and value is the graph itself
void Calc::addGraph(const std::string& name, const Graph& g)
{

	try
	{

		auto it = elements.find(name);
		if (it != elements.end()) //the graph already exsits in the map
		{
			Graph g_old = it->second;
			elements.erase(it);
			elements.emplace(name, g);
		}
		else
		{
			elements.emplace(name, g);
		}
	}
	catch (const std::exception & e)
	{
		std::cerr << e.what();
	}

}
//prints a given graph by its name
void Calc::print(const std::string& name)
{
	auto itr = elements.find(name);
	if (itr != elements.end())
		itr->second.print();
	else
	{
		throw UndefinedVariable(name);
	}
}
//prints the names of all the variables saved in the calc
void Calc::who() //-----------need to pass os stream
{
	for (auto const& g : this->elements)
	{
		std::cout << g.first << std::endl;
	}
}
//reset the calc, erasing the map
void Calc::reset()
{
	this->elements.clear();
}
//save a grhap as a binary file throw exception if there is a problem with the file
void Calc::saveGraph(const char* file_name, const Graph& g)
{
	std::ofstream outfile(file_name, std::ios_base::binary);//outfile.write((const char*)&num, sizeof(int));
	if (!outfile.is_open())
	{
		throw InvalidFileForSave(file_name);
	}
	unsigned int number_of_vertices = (unsigned int)g.getVertices().size();
	unsigned int number_of_edges = (unsigned int)g.getEdges().size();
	outfile.write((const char*)&number_of_vertices, sizeof(unsigned int));
	outfile.write((const char*)&number_of_edges, sizeof(unsigned int));
	for (const auto& ver : g.getVertices())
	{
		unsigned int size_of_ver_name = (unsigned int)ver.size();
		outfile.write((const char*)&size_of_ver_name, sizeof(unsigned int));//size of vertex name
		for (auto const& c : ver)
		{
			outfile.write((const char*)&c, sizeof(char));//wtite the ver name char by char
		}
	}
	for (const auto& edg : g.getEdges())
	{
		unsigned int size_of_src_edg_name = (unsigned int)edg.first.size();
		outfile.write((const char*)&size_of_src_edg_name, sizeof(unsigned int));//size of edge src name
		for (auto const& c : edg.first)
		{
			outfile.write((const char*)&c, sizeof(char));//wtite the src edge name char by char
		}
		unsigned int size_of_dest_edg_name = (int)edg.second.size();
		outfile.write((const char*)&size_of_dest_edg_name, sizeof(unsigned int));//size of edge dest name
		for (auto const& c : edg.second)
		{
			outfile.write((const char*)&c, sizeof(unsigned char));//wtite the src edge name char by char
		}
	}


}
//return a string from a file throw exception if there is a problem with the file
string Calc::loadGraph(const char* file_name)
{
	string graph = "{";
	std::ifstream infile(file_name, std::ios_base::binary);//outfile.write((const char*)&num, sizeof(int));
	if (!infile.is_open())
	{
		throw InvalidFileForRead(file_name);
	}
	unsigned number_of_ver, number_of_edg, number_of_chars_name;
	char c;
	infile.read((char*)&number_of_ver, sizeof(unsigned int));
	infile.read((char*)&number_of_edg, sizeof(unsigned int));
	for (unsigned i = 0; i < number_of_ver; i++)
	{
		string ver;
		infile.read((char*)&number_of_chars_name, sizeof(unsigned int));
		for (unsigned i = 0; i < number_of_chars_name; i++)
		{

			infile.read((char*)&c, sizeof(char));
			ver += c;

		}
		graph += ver;
		graph += ",";
	}
	if (number_of_ver > 1)//removes the last comma 
	{
		graph.pop_back();
	}
	graph += "|";
	for (unsigned int i = 0; i < number_of_edg; i++)
	{
		string edg;
		edg += "<";
		infile.read((char*)&number_of_chars_name, sizeof(unsigned int));
		for (unsigned int i = 0; i < number_of_chars_name; i++)//read src ver
		{
			infile.read((char*)&c, sizeof(char));
			edg += c;
		}
		edg += ",";
		infile.read((char*)&number_of_chars_name, sizeof(unsigned int));
		for (unsigned int i = 0; i < number_of_chars_name; i++)//read dest ver
		{
			infile.read((char*)&c, sizeof(char));
			edg += c;
		}
		edg += ">";
		graph += edg;
		graph += ",";
	}
	if (number_of_ver > 1)//removes the last comma 
	{
		graph.pop_back();
	}
	return graph += "}";
}