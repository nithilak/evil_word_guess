#include "word_guess_constants.hpp"
#include "word_guess_functions.hpp"

#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <random>
#include <sstream>
// #include <stdexcept>

void DictionaryToMap(const std::string& filename) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open the file!" << std::endl;
        return;
    }

    std::string word;
    
    while (std::getline(file, word)) {
        kDictionaryMap.push_back(word);
    }
}

void PrintMap(const std::map<int, std::string>& dictionary_map) {
    for (const auto& pair : dictionary_map) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }
}

void PrintSet(const std::set<char>& guesses) {
    for (const char& c : guesses) {
        std::cout << c << " ";
    }
    std::cout << std::endl;
}
void PrintSet(const std::set<int>& guesses) {
    for (const int& c : guesses) {
        std::cout << c << " ";
    }
    std::cout << std::endl;
}

int ChooseRandomNum(int min, int max) {
    // 1. Obtain a random seed from the hardware
    std::random_device rd;
    
    // 2. Initialize the standard mersenne_twister_engine with the seed
    std::mt19937 gen(rd());
    
    // 3. Define the range [min, max] - inclusive
    // int min = 0;
    // int max = kDictionaryMap.size() - 1;
    std::uniform_int_distribution<int> distrib(min, max);
    
    // 4. Generate the random number
    return distrib(gen);
}

void GetAllowedWords() {
    for (int i = 0; i < kDictionaryMap.size(); i++) {
        if (kDictionaryMap[i].size() == answer_size) {
            allowed_words.insert(i);
        }
    }
}

void ChooseRandomWord(int num) {
    // answer_size = num;
    // if (answer_size == 0) {
    //     answer_size = ChooseRandomNum(kMinWordLength, kMaxWordLength);
    // }

    // //update allowed words to be of entered word size
    // std::set<int> new_allowed_words;
    // for (int index : allowed_words) {
    //     std::cout << "testing: " << index << std::endl;
    //     if (kDictionaryMap[index].size() == answer_size) {
    //         std::cout << index << std::endl;
    //         new_allowed_words.insert(index);
    //     }
    // }
    // allowed_words = new_allowed_words;


    GetAllowedWords();
    answer = ReturnRandomAllowedWord();

    // reveal_answer = std::string(answer_size, '_');
    // for (int i = 0; i < answer_size - 1; i++) {
    //     reveal_answer.push_back('_');
    //     reveal_answer.push_back(' ');
    // }
    // reveal_answer.push_back('_');
}

std::string ReturnRandomAllowedWord() {
    // if (allowed_words.empty()) {
    //     throw std::runtime_error("allowed_words is");
    // }
    auto it = std::next(allowed_words.begin(),  ChooseRandomNum(0, allowed_words.size() - 1));
    return kDictionaryMap[*it];
}


bool contains(const std::string& word, const char& letter) {
    return word.find(letter) != std::string::npos;
}

int getIntegerInputMode() {
    std::string input;
    int number;

    while (true) {
        std::cout << "Choose Mode: Normal 0, Evil 1, Easy 2, Easy(include ver.) 3" << std::endl;
        std::getline(std::cin, input); // Read the full line of input

        std::istringstream iss(input);
        char extra;

        // 1. Try to stream into the integer
        // 2. Try to stream any leftover non-whitespace characters into 'extra'
        if ((iss >> number) && !(iss >> extra)) {
            if (number == 0 || number == 1 || number == 2 || number == 3) {
                return number; // Success! Only an integer was found.
            }
        }

        std::cout << "Invalid input. Please enter 0 or 1 or 2.\n";
    }
}

int getIntegerInputWordSize() {
    std::string input;
    int number;
    int number2;

    while (true) {
        std::cout << "Choose Word Size " << kMinWordLength << "-" << kMaxWordLength << "(For Random Enter 0): " << std::endl;
        std::getline(std::cin, input); // Read the full line of input

        std::istringstream iss(input);
        char extra;

        // 1. Try to stream into the integer
        // 2. Try to stream any leftover non-whitespace characters into 'extra'
        if ((iss >> number)) {
            if (!(iss >> extra)) {
                if (number == 0 || ((kMinWordLength <= number) && (number <= kMaxWordLength))) {
                    return number;
                }
            } else if ((iss >> number2) && !(iss >> extra)) {
                int output = 10*number + number2; // Success! Only an integer was found.
                if (output == 0 || ((kMinWordLength <= output) && (output <= kMaxWordLength))) {
                    return output;
                }
            }
        }

        std::cout << "Invalid input.\n";
    }
}

char getGuess(std::set<char>& guesses) {
    char guess;
    std::string input;
    while (true) {
        std::getline(std::cin, input); // Read the full line of input
        // std::cout << input << std::endl;
        if (input.length() == 1) {
            guess = input.at(0);
            // std::cout << guess << std::endl;
            if ( (guess < 65 || guess > 90)) {
                std::cout << "Please enter an uppercase letter." << std::endl;
                continue;
            }
            if (guesses.contains(guess)) {
                std::cout << "Already guessed." << std::endl;
                continue;
            }
            guesses.insert(guess);
            return guess;
        } else {
            std::cout << "Please enter an uppercase letter." << std::endl;
        }
    }
}

bool MakeAGuess(const char& letter) {
    bool found_letter = false;
    for (int i = 0; i < answer_size; i++) {
        if (answer[i] == letter) {
            reveal_answer[i*2] = letter;
            found_letter = true;
            letters_revealed++;
        }
    }
    return found_letter;
}

//cuts down the allowed words and if forced to, reveals a letter
bool MakeAnEvilGuess(const char& letter) {
    bool found_letter = false;
    std::set<int> contains_letter;
    std::set<int> not_contains_letter;

    for (const auto& index : allowed_words) {
        const std::string& word = kDictionaryMap.at(index);
        // std::cout << "Checking word: " << word << std::endl;
        // std::cout << "Does it contain '" << letter << "'? " << (contains(word, letter) ? "Yes" : "No") << std::endl;
        if (contains(word, letter)) {
            contains_letter.insert(index);
        } else {
            not_contains_letter.insert(index);

        }
    }

    if (not_contains_letter.size() >= contains_letter.size()) {
        allowed_words = not_contains_letter;
    } else {
        found_letter = true;
        allowed_words = contains_letter;
        RevealLetterInMostPosition(letter);
    }

    return found_letter;
}

//opposite of evil guess
bool MakeAnEasyGuess(const char& letter) {
    bool found_letter = false;
    std::set<int> contains_letter;
    std::set<int> not_contains_letter;

    for (const auto& index : allowed_words) {
        const std::string& word = kDictionaryMap.at(index);
        // std::cout << "Checking word: " << word << std::endl;
        // std::cout << "Does it contain '" << letter << "'? " << (contains(word, letter) ? "Yes" : "No") << std::endl;
        if (contains(word, letter)) {
            contains_letter.insert(index);
        } else {
            not_contains_letter.insert(index);

        }
    }

    // PrintSet(contains_letter);
    // std::cout << "next" << std::endl;
    // PrintSet(not_contains_letter);

    if (contains_letter.empty()) {
        allowed_words = not_contains_letter;
    } else if ((not_contains_letter.size() >= contains_letter.size())) {
        found_letter = true;
        allowed_words = contains_letter;
        RevealLetterInLeastPosition(letter);
    } else {
        allowed_words = not_contains_letter;
    }

    return found_letter;
}

bool MakeAnEasyIncludeGuess(const char& letter) {
    bool found_letter = false;
    std::set<int> contains_letter;
    std::set<int> not_contains_letter;

    for (const auto& index : allowed_words) {
        const std::string& word = kDictionaryMap.at(index);
        // std::cout << "Checking word: " << word << std::endl;
        // std::cout << "Does it contain '" << letter << "'? " << (contains(word, letter) ? "Yes" : "No") << std::endl;
        if (contains(word, letter)) {
            contains_letter.insert(index);
        } else {
            not_contains_letter.insert(index);

        }
    }

    // PrintSet(contains_letter);
    // std::cout << "next" << std::endl;
    // PrintSet(not_contains_letter);

    if (!contains_letter.empty()) {
        found_letter = true;
        allowed_words = contains_letter;
        RevealLetterInMostPosition(letter);
    } else {
        allowed_words = not_contains_letter;
    }

    return found_letter;
}


std::vector<std::set<int>> CheckAllowedWordsAtPosition(const char& letter) {
    int answer_size = answer.size();
    std::vector<std::set<int>> position_sets(answer.size());

    for (auto index : allowed_words) { //const auto& index : allowed_words
        const std::string& word = kDictionaryMap.at(index);
        // std::cout << word  << " ";
        for (int i = 0; i < word.size() && i < answer_size; i++) {
            if (word.at(i) == letter) {
                position_sets[i].insert(index);
                //std::cout << "yes ";
            } //else {
            //     std::cout << "no ";
            // }
        }
        // std::cout << std::endl;
    }

    // for (auto thing : position_sets[4]) {
    //     std::cout << kDictionaryMap.at(thing) << std::endl;
    // }


    return position_sets;
}

std::map<std::vector<int>, std::set<int>> GatherPatternList(const char& letter) {
    std::map<std::vector<int>, std::set<int>> repeat_letters;
    for (int index : allowed_words) {
        std::string word = kDictionaryMap[index];
        // std::cout << "check2 " << word << std::endl;
        
        std::vector<int> curr(answer_size);
        for (int i = 0; i < answer_size; i++) {
            if (word.at(i) == letter) {
                curr[i] = 1;
            }
        }

        auto it = repeat_letters.find(curr);
        if (it != repeat_letters.end()) {
            std::set<int>& value = it->second;
            value.insert(index);
            // std::cout << "Key exists! size: " << value.size() << std::endl;
        } else {
            std::set<int> value;
            value.insert(index);
            repeat_letters.insert({curr, value});
        }

    }
    return repeat_letters;
}

void RevealLetterInLeastPosition(const char& letter) {    
    std::map<std::vector<int>, std::set<int>> repeat_letters = GatherPatternList(letter);

    int min_count = kDictionaryMap.size();
    for (const auto& [pattern, keys] : repeat_letters) {
        if (keys.size() < min_count) {
            min_count = keys.size();
        }
    }

    std::map<std::vector<int>, std::set<int>> possible_sets;
    for (const auto& [pattern, keys] : repeat_letters) {
        if (keys.size() == min_count) {
            possible_sets.insert({pattern, keys});
        }
    }


    auto iter = possible_sets.begin();
    // for (int i = 0; i < ChooseRandomNum(0, possible_sets.size() - 1); i++) {
    //     iter++;
    // }
    iter = std::next(iter, ChooseRandomNum(0, possible_sets.size() - 1));

    allowed_words = iter->second;
    
    std::vector<int> chosen_pattern = iter->first;
    for (int i = 0; i < answer_size; i++) {
        if (chosen_pattern[i] == 1) {
            reveal_answer[i*2] = letter;
            letters_revealed++;
        }
    }


}

void RevealLetterInMostPosition(const char& letter) {
    std::map<std::vector<int>, std::set<int>> repeat_letters = GatherPatternList(letter);

    int max_count = 0;
    for (const auto& [pattern, keys] : repeat_letters) {
        if (keys.size() > max_count) {
            max_count = keys.size();
        }
    }

    std::map<std::vector<int>, std::set<int>> possible_sets;
    for (const auto& [pattern, keys] : repeat_letters) {
        if (keys.size() == max_count) {
            possible_sets.insert({pattern, keys});
        }
    }


    auto iter = possible_sets.begin();
    // for (int i = 0; i < ChooseRandomNum(0, possible_sets.size() - 1); i++) {
    //     iter++;
    // }
    iter = std::next(iter, ChooseRandomNum(0, possible_sets.size() - 1));

    allowed_words = iter->second;
    
    std::vector<int> chosen_pattern = iter->first;
    for (int i = 0; i < answer_size; i++) {
        if (chosen_pattern[i] == 1) {
            reveal_answer[i*2] = letter;
            letters_revealed++;
        }
    }

}