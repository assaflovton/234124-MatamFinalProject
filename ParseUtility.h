#ifndef PARSE_UTILITY
#define PARSE_UTILITY
#include <algorithm>
#include <cctype>
#include <string>
#include <vector>

std::vector<std::string> tokenize(const std::string& command, char openChar = '\0', char closeChar = '\0');
bool isAReservedWord(const std::string& str);
bool isAGraphName(const std::string& str);
bool isValidVertexName(const std::string& str);
bool isValidCharName(const char ch);
extern const std::vector<std::string> RERSERVED_KEYWORDS;
std::vector<std::string> tokenizeLoad(const std::string& command, char openChar, char closeChar);
std::vector<std::string> tokenizeLoad(const std::string& command, char openChar, char closeChar);
std::string eraseSpacesFromSides(const std::string& str);
char isValidCharsInTokensForGraphEvaluation(const std::vector<std::string>& tokens);

#endif
