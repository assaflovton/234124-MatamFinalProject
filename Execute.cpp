#include "Execute.h"
using std::iostream;
using std::vector;
using std::string;
using std::stack;
using std::cin;
using std::cout;


//gets a string to execute call the apropraite function to interpret the expression
int executeCommand(Calc& calc, const std::string& command)
{
	std::vector<std::string> tokens = tokenize(command, '{', '}');
	//we will start with handlying all the reserved word such print, save...
	if (tokens.empty())
	{
		return RUN;
	}
	if (tokens[0] == "save")//save({},filename) {}-can be anything with value
	{
		if (tokens.size() < 6 || tokens[1] != "(" || tokens.back() != ")")//save(G,filename) -> ( {} ,  sdf/ ds  fg  /s d.txt ) 
		{
			throw InvalidSaveSyntax(command);
		}
		int index_of_last_comma = -1;
		for (int i = 0; i < (int)tokens.size(); i++)
		{
			if (tokens[i][0] == ',')
			{
				index_of_last_comma = i;
			}
		}
		if (index_of_last_comma < 3) //need to be at least 3
		{
			throw InvalidSaveSyntax("Missing ','");
		}
		else
		{
			string g_def_input = command.substr(command.find_first_of('(') + 1, command.find_last_of(',') - 1 - (command.find_first_of('(')));//copy Graph def without ( and ,
			string file_name_input = command.substr(command.find_last_of(',') + 1);//copy filename without ,
			file_name_input.erase(file_name_input.size() - 1);//remove the )
			file_name_input = eraseSpacesFromSides(file_name_input);
			Graph g;
			g = evaluteBeforeRemovingBrackets(calc, g_def_input);
			calc.saveGraph(file_name_input.c_str(), g);

		}
		return RUN;
	}
	else if (tokens[0] == "print")//print({})
	{
		if (tokens.size() < 4 || tokens[1] != "(" || tokens.back() != ")")
		{
			throw InvalidPrintSyntax(command);
		}
		std::string g_str = command.substr(command.find_first_of("(") + 1, command.find_last_of(")") - command.find_first_of("(") - 1);
		Graph g = evaluteBeforeRemovingBrackets(calc, g_str);
		g.print();
		return RUN;
	}
	else if (tokens[0] == "who")//who
	{
		if (tokens.size() > 1)
		{
			throw InvalidWhoSyntax();
		}
		calc.who();
		return RUN;
	}
	else if (tokens[0] == "delete") //delete(G)
	{
		if (tokens.size() < 4 || tokens[1] != "(" || tokens.back() != ")")
		{
			throw InvalidDeleteSyntax();
		}
		calc.deleteGraph(tokens[2]);//will trow an error if not exsiting
		return RUN;
	}
	else if (tokens[0] == "quit")//quit
	{
		if (tokens.size() > 1)
		{
			throw InvalidQuitSyntax();
		}
		return QUIT;
	}
	else if (tokens[0] == "reset")//reset
	{
		if (tokens.size() > 1)
		{
			throw InvalidResetSyntax();
		}

		calc.reset();
		return RUN;
	}
	//from here all cases are G=...
	std::vector<std::string> tokens_n = tokenizeLoad(command, '{', '}');
	handleGraphName(tokens_n[0]);//will thorw an exception if the Graph name is invalid
	if (tokens_n.size() > 2 && tokens_n[1] == "=")//will thorw an exception if missing =
	{
		Graph g;
		g = evaluteBeforeRemovingBrackets(calc, command.substr(command.find_first_of('=') + 1));
		if (!isAGraphName(tokens_n[0]))
		{
			throw InvalidVariableName(tokens[0]);
		}
		calc.addGraph(tokens_n[0], g);
		return RUN;
	}
	throw UnrecognizedCommand(command);//if we havn't executed anything there was a syntax problem
}
//goes through the string and replace all the load(filename) with a literal grap 
std::vector<std::string> replaceAllLoad(Calc& calc, std::vector<std::string> tokens)
{
	for (int i = 0; i < (int)tokens.size(); i++)
	{
		if (tokens[i].substr(0, 4) == "load" && tokens[i].back() == ')') {//load(...)

			string load = tokens[i];
			int first_index_no_spaces, last_index_no_spaces;
			for (first_index_no_spaces = tokens[i].find_first_of('(') + 1; first_index_no_spaces < (int)load.size(); first_index_no_spaces++)//skip load(
			{
				if (load[first_index_no_spaces] != ' ')
				{
					break;
				}
			}
			for (last_index_no_spaces = load.size() - 2; last_index_no_spaces >= 0; last_index_no_spaces--)//skip last )
			{
				if (load[last_index_no_spaces] != ' ')
				{
					break;
				}
			}
			string filename = load.substr(first_index_no_spaces, last_index_no_spaces - first_index_no_spaces + 1);//takes only the name of the file
			const char* file = filename.c_str();
			string loaded_g_str = calc.loadGraph(file);
			try {

				Graph g(loaded_g_str);
			}
			catch (CalcException & e)//if the graph saved is not a correct graph 
			{
				throw InvalidFileForLoad(e.what());
			}
			tokens[i] = loaded_g_str;//replace the token with a string discribing the graph loaded from the file
		}
	}
	return tokens;
}
//gets a an expression input and return the grap
Graph evaluteBeforeRemovingBrackets(Calc& calc, const std::string& g_str)
{
	std::vector<std::string> t_edit = tokenizeLoad(g_str, '{', '}');
	t_edit = replaceAllLoad(calc, t_edit);
	//from here we cannot have any character diffrent than allowed chars
	char c = isValidCharsInTokensForGraphEvaluation(t_edit);
	string res;
	res.push_back(c);
	if (c != '{')
	{
		throw InvalidCharachter("Try To Remove: " + res);
	}
	stack<int> stack;
	Graph g;
	//remove () and organize the vec in order
	for (int i = 0; i < (int)t_edit.size(); i++)//
	{
		if (t_edit[i] == "(")
		{
			stack.push(i);
			continue;
		}
		else if (t_edit[i] == ")" && stack.empty())
		{
			throw InvalidBracket("");
		}
		else if (t_edit[i] == ")" && !stack.empty())//we have a sequence to evaluate
		{
			vector<string> tokens_to_evaluate;

			if ((stack.top() - 1 > 0) && t_edit[(int)stack.top() - 1] == "!")//!()
			{
				tokens_to_evaluate.emplace_back("!");//add ! at begin

				for (int j = stack.top() + 1; j < i; j++) //takes all besides "(", ")" and 
				{
					if (t_edit[j] != "(" && t_edit[j] != ")")
					{
						tokens_to_evaluate.emplace_back(t_edit[j]);
					}
				}
				if (tokens_to_evaluate.size() == 0)
				{
					throw InvalidBracket("");
				}
				else if (tokens_to_evaluate.size() >= 1) {
					g = evaluteAfterRemovingBrackets(calc, tokens_to_evaluate);
					string g_str = g.graphToString();
					t_edit[(int)stack.top() - 1] = g_str;//add evaluated grhap string where ! was
					t_edit.erase(t_edit.begin() + stack.top(), t_edit.begin() + i + 1);//delete the tokesn we already evaluated
				}
				else
				{
					t_edit.erase(t_edit.begin() + stack.top());//delete (
					t_edit.erase(t_edit.begin() + stack.top() + 1);//delete )
				}
				i = stack.top() - 1;
			}
			else {//()
				for (int j = stack.top() + 1; j < i; j++) //takes all besides "(", ")" 
				{
					if (t_edit[j] != "(" && t_edit[j] != ")")
					{
						tokens_to_evaluate.emplace_back(t_edit[j]);
					}
				}
				if (tokens_to_evaluate.size() == 0)
				{
					throw InvalidBracket("");
				}
				else if (tokens_to_evaluate.size() != 0) {
					g = evaluteAfterRemovingBrackets(calc, tokens_to_evaluate);
					string g_str = g.graphToString();
					t_edit[stack.top()] = g_str;//add evaluated grhap string where ( was
					t_edit.erase(t_edit.begin() + stack.top() + 1, t_edit.begin() + i + 1);//delete the tokesn we already evaluated from ...)
				}
				else {
					t_edit.erase(t_edit.begin() + stack.top());//delete (
					t_edit.erase(t_edit.begin() + stack.top());//delete )
				}
				i = stack.top();
			}
			stack.pop();
		}
	}
	return evaluteAfterRemovingBrackets(calc, t_edit);

}
//return a final graph after we got a definition with no meaning to order of the operators
Graph evaluteAfterRemovingBrackets(Calc& calc, std::vector<std::string>& tokens_no_name_no_equ)
{
	Graph g;
	std::vector<std::string> tokens = replaceAllUnaryOperatorWithEvaluatedLiteral(calc, tokens_no_name_no_equ);
	int indexFirst = 0, indexSecond = 1, indexThird = 2;
	if (tokens[indexFirst][0] == '{')//assign g to be the first graph appering
	{
		g = Graph(tokens[indexFirst]);//will throw if not a correct defintion of graph
	}
	else
	{
		g = calc.getGraphByName(tokens[indexFirst]);//will throw if not exists
	}
	int tokens_size = (int)tokens.size();
	if (tokens_size == 2) //it is impossible to have an argument size of two since we replace all the unary operators 
	{
		throw InvalidGraphDefinitionOperator();
	}
	while (indexThird < (int)tokens.size()) //all operator have the same oreder, and looks like  g 'operator' 'G' or 'literal'
	{
		if (tokens[indexThird][0] == '{')
		{
			if (tokens[indexSecond] == "+")
			{
				g = g + Graph(tokens[indexThird]);

			}
			else if (tokens[indexSecond] == "-")
			{
				g = g - Graph(tokens[indexThird]);

			}
			else if (tokens[indexSecond] == "^")
			{
				g = g ^ Graph(tokens[indexThird]);

			}
			else if (tokens[indexSecond] == "*")
			{
				g = g * Graph(tokens[indexThird]);

			}
		}
		else
		{
			if (tokens[indexSecond] == "+")
			{
				g = g + calc.getGraphByName(tokens[indexThird]);

			}
			else if (tokens[indexSecond] == "-")
			{
				g = g - calc.getGraphByName(tokens[indexThird]);

			}
			else if (tokens[indexSecond] == "^")
			{
				g = g ^ calc.getGraphByName(tokens[indexThird]);

			}
			else if (tokens[indexSecond] == "*")
			{
				g = g * calc.getGraphByName(tokens[indexThird]);

			}
		}

		indexSecond += 2;
		indexThird += 2;
	}
	return g;
}
//takes a string discribing a graph and replace every expression with ! 
//in order to get a string with no meaning to the order of the operators
std::vector<std::string> replaceAllUnaryOperatorWithEvaluatedLiteral(Calc& calc, std::vector<std::string> tokens)
{
	for (int i = (int)tokens.size() - 1; i >= 1; i--)
	{
		std::string g_str;
		if (tokens[(int)i - 1] == "!")
		{
			Graph g;
			if (tokens[i][0] == '{')//there is a literal graph since we might have !!G -> !(!G) -> !(literal G)-> (literal !G)
			{
				g = Graph(tokens[i]);
				g = !g;
				g_str = g.graphToString();//will replace the literal graph g with !graph parsed to string
			}
			else if (isAGraphName(tokens[i]))//will replace the graph G with the !G parsed to string
			{
				g = calc.getGraphByName(tokens[i]);
				g = !g;
				g_str = g.graphToString();

			}
			tokens[i] = g_str;
			tokens.erase(tokens.begin() + i - 1);//erase ! sign
		}
	}
	return tokens;
}
//validates the Graph name and throw exception if invalid
void handleGraphName(const std::string& name)
{
	if (!isAGraphName(name))//validate if the name of the graph is correct
		throw InvalidVariableName(name);

}



