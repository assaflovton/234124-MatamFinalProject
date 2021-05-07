#ifndef EXECUTE
#define EXECUTE
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <stack>
// redirecting cout's output thrrough its stream buffer
#include <iostream>     // std::streambuf, std::cout
#include <fstream>      // std::ofstream
#include "Graph.h"
#include "ParseUtility.h"
#include "Calc.h"
#include "Exceptions.h"
enum status_of_calc { RUN, QUIT };
//validates the Graph name and throw exception if invalid
void handleGraphName(const std::string& name);
//void executeBinaryOperatorWithLiteralsOnBothSides(Calc& calc, const std::vector<std::string>& tokens);
std::vector<std::string> replaceAllUnaryOperatorWithEvaluatedLiteral(Calc& calc, std::vector<std::string> tokens);
//return a final graph after we got a definition with no order
Graph evaluteAfterRemovingBrackets(Calc& calc, std::vector<std::string>& tokens_no_name_no_equ);
//gets a an expression input and return the graph
Graph evaluteBeforeRemovingBrackets(Calc& calc, const std::string& g_str);
int executeCommand(Calc& calc, const std::string& command);
std::vector<std::string> replaceAllLoad(Calc& calc, std::vector<std::string> tokens);

#endif 