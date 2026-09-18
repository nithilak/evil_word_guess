#ifndef WORD_GUESS_FUNCTIONS_HPP
#define WORD_GUESS_FUNCTIONS_HPP

#include <map>
#include <set>
#include <string>
#include <vector>

//initializes kDictionaryMap from filename
void DictionaryToMap(const std::string& filename);

//void ChooseRandomWord(); //scrapped

//makes allowed_words to be of answer size
void GetAllowedWords(); //all numbers from 0 to 178690

void PrintMap(const std::map<int, std::string>& dictionary_map);
void PrintSet(const std::set<int>& guesses);
void PrintSet(const std::set<char>& guesses);

int ChooseRandomNum(int min, int max);

//deprecated. based on the current answer_size, runs GetAllowedWords() and then runs ReturnRandomAllowedWord()
void ChooseRandomWord(int num);

//returns a random word from id in allowed_words
std::string ReturnRandomAllowedWord();
//makes the answer a random word from id in allowed_words
void ChooseRandomAllowedWord();

//prompts user to enter mode
int getIntegerInputMode();
//prompts user to enter word size, 0 being a valid answer reserved for choice of random word size
int getIntegerInputWordSize();

//unused function meant to prompt the user to make a valid guess
char getGuess(std::set<char>& guesses);

//prompts user for custom word and then sets the word as answer and updates answer_size
std::string getCustomWord();

//returns true if a letter is found from the chosen word
bool MakeAGuess(const char& letter);

//cuts down the allowed words to max of contains or not contains leter, and true reveals most common pattern of letter
bool MakeAnEvilGuess(const char& letter);
//opposite of evil guess
bool MakeAnEasyGuess(const char& letter);
//leans towards including the guessed letter and reveals least common pattern of letter
bool MakeAnEasyIncludeGuess(const char& letter);

bool contains(const std::string& word, const char& letter);

//deprecated function that returns a vector with all word indexes that contain a letter at each position
std::vector<std::set<int>> CheckAllowedWordsAtPosition(const char& letter);

//returns list of patterns like "(0, 1, 0)" and the set of word ids that follow each pattern
std::map<std::vector<int>, std::set<int>> GatherPatternList(const char& letter);

//reveals the min/max most common pattern of letter in words in allowed_words
//allowed_words should be filtered to contain the letter at least once by this point
//but it can still work with "(0, 0, 0)" as a pattern and not reveal, might cause bug, not tested
void RevealLetterInLeastPosition(const char& letter);
void RevealLetterInMostPosition(const char& letter);

#endif // WORD_GUESS_FUNCTIONS_HPP