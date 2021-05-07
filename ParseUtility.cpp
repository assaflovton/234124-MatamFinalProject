#include "ParseUtility.h"
using std::vector;
using std::string;

/*
This calss is responsible to taking the string and parsing it into tokens, moreover to the syntax check of the
names, as if the tokens are valid or not.
*/

//represets the saved words in the calc
const std::vector<std::string> RERSERVED_KEYWORDS = { "print","reset","who","quit","delete","load","save" };
const std::vector<char> ALLOWED_CHARS = { '[' , ']' ,  ';' , '!' , '+', '-' , '*' , '^', '{' , '}', '(' , ')'
, ',' , '|' , '<' , '>', ' ' };
//get a string  and tokenized it to 'openChar .... closeChar', 'var_name(can be a vertex name, edge or graph)'
//, and the reset symbols are all a one  charachter token also get rid of spaces
std::vector<std::string> tokenize(const std::string& command, char open_char, char close_char)
{
	std::vector<std::string> result;
	int tokenStartIndex = 0;
	while (tokenStartIndex < (int)command.size())
	{
		if (isValidCharName(command[tokenStartIndex]))
		{
			int tokenEndIndex = tokenStartIndex + 1;
			while (tokenEndIndex < (int)command.size() && isValidCharName(command[tokenEndIndex]))
				//progress until token is not alphanumaric (digit or char) or [ ] ; which are  valid vertex name character
				++tokenEndIndex;

			result.emplace_back(command.substr(tokenStartIndex, (int)tokenEndIndex - tokenStartIndex));//insert the token we found to result
			tokenStartIndex = tokenEndIndex;//progress to find the next token
		}
		else if (command[tokenStartIndex] == open_char)//the token start with distinctive open char
		{
			int tokenEndIndex = tokenStartIndex + 1;
			while (tokenEndIndex < (int)command.size() && command[(int)tokenEndIndex - 1] != close_char)//progress until the end of the string or until you fine the distinctive end char
				++tokenEndIndex;

			result.emplace_back(command.substr(tokenStartIndex, (int)tokenEndIndex - tokenStartIndex));//insert the token from openChar to closeChar
			tokenStartIndex = tokenEndIndex;//continue to look for the next token
		}
		else
		{
			if (command[tokenStartIndex] != ' ')//the token is a one char 
				result.emplace_back(1, command[tokenStartIndex]);

			++tokenStartIndex;//continue to look for the next token
		}
	}

	return result;
}
//return true if the string is a reserved word
bool isAReservedWord(const string& str)
{
	for (const string& word : RERSERVED_KEYWORDS)
	{
		if (str == word)
		{
			return true;
		}
	}
	return false;
}
//starts with a letter and consists only letters and numbers
bool isAGraphName(const string& str)
{
	if (isAReservedWord(str)) //str is a reserved word
	{
		return false;
	}
	if (!isalpha(str[0])) //str does not start with a letter
	{
		return false;
	}
	for (char c : str)//str is not a valid var name
	{
		if (!isalnum(c))//if c is not a valid character
		{
			return false;
		}
	}
	return true;
}
//has only alphanumerical and [ ] ; characters and is balanced with [] , ; apperes only inside[] can have reserved words
bool isValidVertexName(const string& str)
{
	int count_open = 0, count_close = 0;
	for (char c : str)
	{

		if (c != '[' && c != ']' && !isalnum(c) && c != ';')//if c is not a valid character
		{
			return false;
		}
		if (c == '[')
		{
			count_open++;
		}
		if (c == ']')
		{
			count_close++;
		}
		if (count_close > count_open)
		{
			return false;
		}
		if (c == ';' && (count_close == count_open))
		{
			return false;
		}

	}
	return count_open == count_close;
}
//gets a string a splits it into tokens, 'load(....)', 'openChar .... closeChar', 'word or var_name'-
//(can be a  saved word ,vertex , edge or graph name), the reset are one token character.
//get rid of spacecs and the reset is a one char token besides the 'load(....)' where the spaces are important for the filename
std::vector<std::string> tokenizeLoad(const std::string& command, char openChar, char closeChar)
{
	std::vector<std::string> result;

	int tokenStartIndex = 0;
	while (tokenStartIndex < (int)command.size())
	{
		if (command.size() >= 6)
		{
			if (command.substr(tokenStartIndex, 4) == "load") {
				int tokenEndIndex = tokenStartIndex + 1;
				while (tokenEndIndex < (int)command.size() && command[tokenEndIndex] != ')')
				{
					++tokenEndIndex;
				}
				result.emplace_back(command.substr(tokenStartIndex, (int)tokenEndIndex - tokenStartIndex + 1));//insert the token we found to result
				tokenStartIndex = tokenEndIndex;//progress to find the next token
				tokenStartIndex++;
				if (tokenEndIndex == (int)command.size() - 1)//this was the last character
				{
					break;
				}
			}
		}
		if (isValidCharName(command[tokenStartIndex]))
		{
			int tokenEndIndex = tokenStartIndex + 1;
			while (tokenEndIndex < (int)command.size() && isValidCharName(command[tokenEndIndex]))
				//progress until token is not alphanumaric (digit or char) or [ ] ; which are  valid vertex name character
				++tokenEndIndex;

			result.emplace_back(command.substr(tokenStartIndex, (int)tokenEndIndex - tokenStartIndex));//insert the token we found to result
			tokenStartIndex = tokenEndIndex;//progress to find the next token
		}
		else if (command[tokenStartIndex] == openChar)//the token start with distinctive open char
		{
			int tokenEndIndex = tokenStartIndex + 1;
			while (tokenEndIndex < (int)command.size() && command[(int)tokenEndIndex - 1] != closeChar)//progress until the end of the string or until you fine the distinctive end char
				++tokenEndIndex;

			result.emplace_back(command.substr(tokenStartIndex, (int)tokenEndIndex - tokenStartIndex));//insert the token from openChar to closeChar
			tokenStartIndex = tokenEndIndex;//continue to look for the next token
		}
		else
		{
			if (command[tokenStartIndex] != ' ')//the token is a one char 
				result.emplace_back(1, command[tokenStartIndex]);

			++tokenStartIndex;//continue to look for the next token
		}
	}
	return result;
}
//gets a string a earse all of the spaces on the begining and end
std::string eraseSpacesFromSides(const std::string& str)
{
	int i = 0;
	while (str[i] == ' ')
	{
		i++;
	}
	int j = str.size() - 1;
	while (str[j] == ' ')
	{
		j--;
	}
	//str.erase(remove_if(str.begin(), str.begin()+i, isspace), str.end());
	return str.substr(i, j - i + 1);
}
//the valid vertex name charachters
bool isValidCharName(const char ch)
{
	return (std::isalnum(ch) || ch == ';' || ch == ']' || ch == '[');
}
//check if all the charchters are a literal graph charachters
//return { if the result is true 
char isValidCharsInTokensForGraphEvaluation(const std::vector<std::string>& tokens)
{
	for (const auto& token : tokens)//run for all tokens
	{

		for (const auto& char_in_token : token)//for each char in token
		{

			if (!isValidCharName(char_in_token))
			{
				bool was_equal = false;
				for (char allowed_c : ALLOWED_CHARS)
				{
					if (char_in_token == allowed_c)
					{
						was_equal = true;
						break;
					}
				}
				if (was_equal == false)
				{
					return char_in_token;
				}
			}

		}
	}
	return '{';//allowed char will never return it signs for true
}