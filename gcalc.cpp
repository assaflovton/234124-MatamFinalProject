#include <string>
// redirecting cout's output thrrough its stream buffer
#include <iostream>     // std::streambuf, std::cout
#include <fstream>      // std::ofstream
#include "Graph.h"
#include "ParseUtility.h"
#include "Calc.h"
#include "Exceptions.h"
#include "Execute.h"
using std::iostream;
using std::string;
using std::cin;
using std::cout;
/*
Explantion:
when trying to decipher the input we have two options:
1.input that has a saved word that defines an opperation such as save, print ,delete...
2.input that describes assignment g=....
execute command  function first take care of all cases of option 1 and than the reset must be from option 2.
First we tokenizr the whole string, notice that each case of option 1 has a uniqe structure that we can take
advantage of. for example for print ({...}): there must be 'print' '(' 'something with value' ')'
so we will tokenize the string and validate that the first is 'print' second is '(' and last is ')'
than we will take the whole third token and pass it to the evaluation function. If we are on option 2
we will validate that the first token is a valid graph name,  the second  token is '=' and the the reset we will
pass to the evaluation process.
The proccess of evaluating the 2 option is first tokeninzing the input into a vector of tokens
than we start to replace all the load with a literal representing the graph
than change the order of the tokens according to the brackets () that determens the order.
i did it using stack, push the indexes of open barack and when you get a closing barack you evaluate the
expression from the open barack index to the closeing one. when doing so we need to check if we had an !
because we would like to apply the operator on the whole exprssion. after we evaluated the expression we inseret
it to the vector and remove the tokesn we have just evaluated and than pop the stack and continue.
after that we have an expression with operators variable name and literals. we go throw the vector and
evalute all the ! operator expression since it has a higher order, again each time replacing the tokens inside
the vector. than we run on the vector one last time and evaluate g_res operator 'literal or variable.
*/
int main(int argc, char* argv[])
{
	Calc calc;
	int status = RUN;
	if (argc == 3) //batch mode
	{
		try {
			std::ifstream in_file(argv[1]);
			std::ofstream out_file(argv[2]);
			if (in_file.is_open()) //where we write to
			{
				if (out_file.is_open()) //where we get input from
				{
					std::cin.rdbuf(in_file.rdbuf());
					std::cout.rdbuf(out_file.rdbuf());

					std::string line;

					while (getline(in_file, line))
					{
						try {
							status = executeCommand(calc, line);
						}
						catch (CalcException & e)//errors for the user
						{
							out_file << e.what() << std::endl;
						}
						catch (std::exception & e)//fatal errors goes to cerr
						{
							std::cerr << e.what() << std::endl;
						}
						if (status == QUIT) //handle quit->finish the program
						{
							in_file.close();
							out_file.close();
							return 0;
						}
					}
					out_file.close();
				}
				else
				{
					throw InvalidInputFile(argv[1]);//problem with using the out file
				}
				in_file.close();
			}
			else
			{
				throw InvalidInputFile(argv[2]);//problem with using the in file
			}

		}
		catch (FileException & e)//looking for files erorrs that must be presented on screen, for the user
		{
			std::cout << e.what() << std::endl;
		}
		catch (std::exception & e)//looking for fatal erorrs such as bad alloc 
		{
			std::cerr << e.what() << std::endl;
		}

	}
	else if (argc == 1)//auto mode
	{
		try {
			while (status == RUN)
			{
				std::cout << "Gcalc> " << std::flush;
				std::string command;
				if (!std::getline(std::cin, command)) //got to end of file (eof)
				{
					return 0;
				}
				try {
					status = executeCommand(calc, command);
				}
				catch (CalcException & e)
				{
					std::cout << e.what() << std::endl;
				}
				if (status == QUIT) //finish
				{
					return 0;
				}
			}
		}
		catch (std::exception & e)//looking for fatal erorrs such as bad alloc
		{
			std::cerr << e.what() << std::endl;
		}
	}
	else
	{
		std::cout << "Error: Number Of File args Provided is Invalid" << std::endl;
		return 0;
	}
	return 0;
}
