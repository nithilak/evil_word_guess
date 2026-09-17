#ifndef WORD_GUESS_FUNCTIONS_HPP
#define WORD_GUESS_FUNCTIONS_HPP

#include <map>
#include <set>
#include <string>
#include <vector>

void DictionaryToMap(const std::string& name);
void ChooseRandomWord();
void GetAllowedWords(); //all numbers from 0 to 178690
void PrintMap(const std::map<int, std::string>& dictionary_map);
void PrintSet(const std::set<int>& guesses);
void PrintSet(const std::set<char>& guesses);

int ChooseRandomNum(int min, int max);

//based on the current answer_size
void ChooseRandomWord(int num);

std::string ReturnRandomAllowedWord();

int getIntegerInputMode();
int getIntegerInputWordSize();

char getGuess(std::set<char>& guesses);

//returns true if a letter is found from the chosen word
bool MakeAGuess(const char& letter);

//cuts down the allowed words and if forced to, reveals a letter
bool MakeAnEvilGuess(const char& letter);
bool MakeAnEasyGuess(const char& letter);

bool contains(const std::string& word, const char& letter);

std::vector<std::set<int>> CheckAllowedWordsAtPosition(const char& letter);

std::map<std::vector<int>, std::set<int>> GatherPatternList(const char& letter);

void RevealLetterInLeastPosition(const char& letter);
void RevealLetterInMostPosition(const char& letter);

#endif // WORD_GUESS_FUNCTIONS_HPP