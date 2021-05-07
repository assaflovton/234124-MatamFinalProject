#ifndef CALC
#define CALC
#include <string>
#include <ostream>
#include "Graph.h"
#include <iostream>
#include <fstream>
#include "Exceptions.h"
#include <map>
class Calc
{
public:
	Calc() = default;
	void addGraph(const std::string& name, const std::string& graphRepresentation);
	void addGraph(const std::string& name, const Graph& g);
	bool isGraphExist(const std::string& name);
	Graph getGraphByName(const std::string& name);
	void print(const std::string& name);
	void who();
	void reset();
	void deleteGraph(const std::string& name);
	std::string graphToString(const std::string& name);
	std::string loadGraph(const char* file_name);
	void saveGraph(const char* file_name, const Graph& g);
private:
	std::map<std::string, Graph> elements;
};
#endif