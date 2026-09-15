#ifndef WORD_GUESS_CONSTANTS_HPP
#define WORD_GUESS_CONSTANTS_HPP

#include <vector>
#include <set>
#include <string>

constexpr int kMaxMisses = 70; //7
constexpr int kRandomSeed = 5500;
constexpr int kMinWordLength = 2;
constexpr int kMaxWordLength = 15;
const std::string kDictionaryFile = "includes/dictionary.txt";

inline std::vector<std::string> kDictionaryMap;
inline std::set<int> allowed_words;

inline std::string answer = "APPLE"; //make random
inline std::string reveal_answer;
inline size_t answer_size;
inline int letters_revealed = 0;



#endif // WORD_GUESS_CONSTANTS_HPP