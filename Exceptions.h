#ifndef EXCEPTIONS
#define EXCEPTIONS
#include <iostream>
#include <cstring>

class Exception : public std::exception
{
protected:
	std::string error;

public:
	Exception(std::string error) : error(error) {}
	const char* what() const noexcept override
	{
		return error.c_str();
	}
};
//errors regarding the calc operation needs to be presented to the user
class CalcException : public Exception
{
protected:
	std::string name;

public:
	CalcException(std::string name) : Exception("Error: " + name) {}
};
//errors regargind the work with files 
class FileException : public Exception
{
protected:
	std::string name;

public:
	FileException(std::string name) : Exception("Error: '" + name) {}
};


//from here there are exceptions regarding - Calc

class UndefinedVariable : public CalcException
{
protected:
	std::string name;
public:
	UndefinedVariable(std::string name) : CalcException("Undefined Variable " + name) {}
};

class InvalidGraphDefinition : public CalcException
{
protected:
	std::string name;
public:
	InvalidGraphDefinition(std::string name) : CalcException("Invalid Graph Definition '" + name + "'") {}
};

class InvalidBracket : public CalcException
{
protected:
	std::string name;
public:
	InvalidBracket(std::string name) : CalcException("Invalid Bracket ( or ) " + name) {}
};

class InvalidVariableName : public CalcException
{
protected:
	std::string name;
public:
	InvalidVariableName(std::string name) : CalcException("Invalid Variable Name '" + name + "'") {}
};

class InvalidEdgeDestOrSrc : public CalcException
{
protected:
	std::string name;
public:
	InvalidEdgeDestOrSrc(std::string name) : CalcException("Edge Source Vertex Or Destenation Vertex Does Not Exist In Graph " + name) {}
};

class InvalidEdgeName : public CalcException
{
protected:
	std::string name;
public:
	InvalidEdgeName(std::string name) : CalcException("Invalid Edges definition " + name) {}
};

class InvalidVertexName : public CalcException
{
protected:
	std::string name;
public:
	InvalidVertexName(std::string name) : CalcException("Invalid Vertex Name Or a Problem With Spearting ',' '" + name + "'") {}
};

class VertexRepit : public CalcException
{
protected:
	std::string name;
public:
	VertexRepit(std::string name) : CalcException("Vertex Cannot Repit In Graph Definition :'" + name + "'") {}
};

class EdgeRepit : public CalcException
{
protected:
	std::string name;
public:
	EdgeRepit(std::string name) : CalcException("Edge Cannot Repit In Graph Definition '" + name + "'") {}
};

class SelefLoop : public CalcException
{
protected:
	std::string name;
public:
	SelefLoop(std::string name) : CalcException("Graph Cannot Have a Selef Loop '" + name + "'") {}
};

class UnrecognizedCommand : public CalcException
{
protected:
	std::string name;
public:
	UnrecognizedCommand(std::string name) : CalcException("Unrecognized Command '" + name + "'") {}
};

class InvalidCharachter : public CalcException
{
public:
	InvalidCharachter(std::string name) : CalcException("Invalid Charachter "+name) {}
};

class InvalidDeleteSyntax : public CalcException
{

public:
	InvalidDeleteSyntax() : CalcException("Invalid Delete Syntax, try 'delete(name_of_existing_graph)' ") {}
};

class InvalidPrintSyntax : public CalcException
{

public:
	InvalidPrintSyntax(std::string name) : CalcException("Invalid Print Syntax, try 'print(name_of_graph)' ") {}
};
class InvalidSaveSyntax : public CalcException
{

public:
	InvalidSaveSyntax(std::string name) : CalcException("Invalid save Syntax, try 'save(definition_of_graph, filename)' ") {}
};

class InvalidLoadSyntax : public CalcException
{

public:
	InvalidLoadSyntax(std::string name) : CalcException("Invalid load Syntax, try 'save(definition_of_graph, filename)'" + name) {}
};

class InvalidWhoSyntax : public CalcException
{

public:
	InvalidWhoSyntax() : CalcException("Invalid Who Syntax, try 'who' ' ") {}
};
class InvalidQuitSyntax : public CalcException
{

public:
	InvalidQuitSyntax() : CalcException("Invalid quit Syntax, try 'quit' ") {}
};

class InvalidResetSyntax : public CalcException
{

public:
	InvalidResetSyntax() : CalcException("Invalid reset Syntax, try 'reset' ") {}
};

class InvalidGraphDefinitionOperator : public CalcException
{

public:
	InvalidGraphDefinitionOperator() : CalcException("Invalid Graph Definition Missing An Operator Or A Graph ") {}
};
class InvalidFileForSave : public CalcException
{
protected:
	std::string name;
public:
	InvalidFileForSave(std::string name) : CalcException("Cannot Open File To Save Graph '" + name + "'") {}
};

class InvalidFileForRead : public CalcException
{
protected:
	std::string name;
public:
	InvalidFileForRead(std::string name) : CalcException("Cannot Open File To Load Graph From '" + name + "'") {}
};

//from here there are exceptions regarding- Files

class InvalidInputFile : public FileException
{
protected:
	std::string name;
public:
	InvalidInputFile(std::string name) : FileException("Invalid Input File '" + name + "'") {}
};

class InvalidOutputFile : public FileException
{
protected:
	std::string name;
public:
	InvalidOutputFile(std::string name) : FileException("Invalid Output File '" + name + "'") {}
};

class InvalidFileForLoad : public FileException
{
protected:
	std::string name;
public:
	InvalidFileForLoad(std::string name) : FileException("Invalid Graph Defined in Loaded File Graph Returned From file was : "+name) {}
};



#endif //EXCEPTIONS