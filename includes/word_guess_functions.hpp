#ifndef WORD_GUESS_FUNCTIONS_HPP
#define WORD_GUESS_FUNCTIONS_HPP

#include <map>
#include <set>
#include <string>
#include <vector>

const std::map<int, std::string> DictionaryToMap(const std::string& name);
std::set<int> GetAllowedWords(const std::map<int, std::string>& dictionary_map); //all numbers from 0 to 178690
void PrintMap(const std::map<int, std::string>& dictionary_map);

//cuts down the allowed words and if forced to, reveals a letter
bool MakeAnEvilGuess(const char& letter);

bool contains(const std::string& word, const char& letter);

std::vector<std::set<int>> CheckAllowedWordsAtPosition(const char& letter);

void RevealLetterInLeastPosition(const char& letter);
void RevealLetterInMostPosition(const char& letter);

#endif // WORD_GUESS_FUNCTIONS_HPP