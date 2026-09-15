#include "word_guess_constants.hpp"
#include "word_guess_functions.hpp"

#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <random>
#include <sstream>

void DictionaryToMap(const std::string& name) {
    std::ifstream file(name);

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

void ChooseRandomWord() {
    // 1. Obtain a random seed from the hardware
    std::random_device rd;
    
    // 2. Initialize the standard mersenne_twister_engine with the seed
    std::mt19937 gen(rd());
    
    // 3. Define the range [min, max] - inclusive
    int min = 0;
    int max = kDictionaryMap.size() - 1;
    std::uniform_int_distribution<int> distrib(min, max);
    
    // 4. Generate the random number
    int random_num = distrib(gen);

    answer = kDictionaryMap[random_num];
    answer_size = answer.size();
    reveal_answer = std::string(answer_size, '_');
}

void GetAllowedWords() {
    for (int i = 0; i < kDictionaryMap.size(); i++) {
        if (kDictionaryMap[i].size() == answer_size) {
            allowed_words.insert(i);
        }
    }
}

bool contains(const std::string& word, const char& letter) {
    return word.find(letter) != std::string::npos;
}

int getIntegerInput(const std::string& prompt) {
    std::string input;
    int number;

    while (true) {
        std::cout << prompt << std::endl;
        std::getline(std::cin, input); // Read the full line of input

        std::istringstream iss(input);
        char extra;

        // 1. Try to stream into the integer
        // 2. Try to stream any leftover non-whitespace characters into 'extra'
        if ((iss >> number) && !(iss >> extra)) {
            if (number == 0 || number == 1) {
                return number; // Success! Only an integer was found.
            }
        }

        std::cout << "Invalid input. Please enter 0 or 1.\n";
    }
}

bool MakeAGuess(const char& letter) {
    bool found_letter = false;
    for (int i = 0; i < answer_size; i++) {
        if (answer[i] == letter) {
            reveal_answer[i] = letter;
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


std::vector<std::set<int>> CheckAllowedWordsAtPosition(const char& letter) {
    int answer_size = answer.size();
    std::vector<std::set<int>> position_sets(answer.size());

    for (auto index : allowed_words) { //const auto& index : allowed_words
        const std::string& word = kDictionaryMap.at(index);
        std::cout << word  << " ";
        for (int i = 0; i < word.size() && i < answer_size; i++) {
            if (word.at(i) == letter) {
                position_sets[i].insert(index);
                //std::cout << "yes ";
            } //else {
            //     std::cout << "no ";
            // }
        }
        std::cout << std::endl;
    }

    // for (auto thing : position_sets[4]) {
    //     std::cout << kDictionaryMap.at(thing) << std::endl;
    // }


    return position_sets;
}

void RevealLetterInLeastPosition(const char& letter) {
    //check every position in answer and get all sets
    std::vector<std::set<int>> position_sets = CheckAllowedWordsAtPosition(letter);

    int min = position_sets[0].size();
    for (int i = 0; i < position_sets.size(); i++) {
        int size = position_sets[i].size();
        std::cout << "size of " << i << ":" << size << std::endl;
        if (size < min) {
            min = size;
        }
    }
    if (min == 0) {
        min = 1;
    }

    std::vector<int> min_positions;
    for (int i = 0; i < position_sets.size(); i++) {
        int size = position_sets[i].size();
        std::cout << "size of " << i << ":" << size << std::endl;
        if (size == min) {
            min_positions.push_back(i);
        }
    }

    for (int i = 0; i < min_positions.size(); i++) {
        std::cout << "pos: " << min_positions[i] << " ";
    }
    std::cout << std::endl;

    int min_pos = min_positions[0];
    if (min_positions.size() > 1) {
        // 1. Obtain a random seed from the hardware
        std::random_device rd;
        
        // 2. Initialize the standard mersenne_twister_engine with the seed
        std::mt19937 gen(rd());
        
        // 3. Define the range [min, max] - inclusive
        int min = 0;
        int max = min_positions.size() - 1;
        std::uniform_int_distribution<int> distrib(min, max);
        
        // 4. Generate the random number
        min_pos = min_positions[distrib(gen)];
        std::cout << "Random number between " << min << " and " << max << ": " << min_pos << "\n";
    }

    std::cout << "min_pos: " << min_pos << std::endl;
    allowed_words = position_sets[min_pos];

    //print possible words
    std::vector<int> same_letter(answer_size);
    same_letter[min_pos] = allowed_words.size();
    for (int index : allowed_words) {
        std::string word = kDictionaryMap[index];
        std::cout << "check " << word << std::endl;
        for (int i = 0; i < answer_size; i++) {
            if (i != min_pos && word[i] == letter) {
                same_letter[i] += 1;
            }
        }

    }

    int words_left = allowed_words.size();
    for (int i = 0; i < same_letter.size(); i++) {
        int letter_count = same_letter[i];
        std::cout << same_letter[i] << " ";
        if (letter_count == words_left) {
            reveal_answer[i] = letter;
            letters_revealed++;
        }

    }
    std::cout << std::endl;
    





    // reveal_answer[min_pos] = letter;
}

void RevealLetterInMostPosition(const char& letter) {
    //check every position in answer and get all sets
    // std::vector<std::set<int>> position_sets = CheckAllowedWordsAtPosition(letter);

    // int max = position_sets[0].size();
    // for (int i = 0; i < position_sets.size(); i++) {
    //     int size = position_sets[i].size();
    //     std::cout << "size of " << i << ":" << size << std::endl;
    //     if (size > max) {
    //         max = size;
    //     }
    // }

    // std::vector<int> max_positions;
    // for (int i = 0; i < position_sets.size(); i++) {
    //     int size = position_sets[i].size();
    //     std::cout << "size of " << i << ":" << size << std::endl;
    //     if (size == max) {
    //         max_positions.push_back(i);
    //     }
    // }

    // for (int i = 0; i < max_positions.size(); i++) {
    //     std::cout << "pos: " << max_positions[i] << " ";
    // }
    // std::cout << std::endl;

    // int max_pos = max_positions[0];
    // if (max_positions.size() > 1) {
    //     // 1. Obtain a random seed from the hardware
    //     std::random_device rd;
        
    //     // 2. Initialize the standard mersenne_twister_engine with the seed
    //     std::mt19937 gen(rd());
        
    //     // 3. Define the range [min, max] - inclusive
    //     int min = 0;
    //     int max = max_positions.size() - 1;
    //     std::uniform_int_distribution<int> distrib(min, max);
        
    //     // 4. Generate the random number
    //     max_pos = max_positions[distrib(gen)];
    //     std::cout << "Random number between " << min << " and " << max << ": " << max_pos << "\n";
    // }

    // std::cout << "max_pos: " << max_pos << std::endl;
    // allowed_words = position_sets[max_pos];

    // //print possible words
    // std::vector<int> same_letter(answer_size);
    // same_letter[max_pos] = allowed_words.size();
    // for (int index : allowed_words) {
    //     std::string word = kDictionaryMap[index];
    //     std::cout << "check " << word << std::endl;
    //     for (int i = 0; i < answer_size; i++) {
    //         if (i != max_pos && word[i] == letter) {
    //             same_letter[i] += 1;
    //         }
    //     }

    // }





    int words_left = allowed_words.size();
    const size_t temp = answer_size;
    std::map<std::vector<int>, std::set<int>> repeat_letters;

    for (int index : allowed_words) {
        std::string word = kDictionaryMap[index];
        std::cout << "check2 " << word << std::endl;
        
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
            std::cout << "Key exists! size: " << value.size() << std::endl;
        } else {
            std::set<int> value;
            value.insert(index);
            repeat_letters.insert({curr, value});
        }

    }

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

    int random_num = 0;
    if (possible_sets.size() > 1) {
        // 1. Obtain a random seed from the hardware
        std::random_device rd;
        
        // 2. Initialize the standard mersenne_twister_engine with the seed
        std::mt19937 gen(rd());
        
        // 3. Define the range [min, max] - inclusive
        int min = 0;
        int max = possible_sets.size() - 1;
        std::uniform_int_distribution<int> distrib(min, max);
        
        // 4. Generate the random number
        int random_num = distrib(gen);
        std::cout << "Random number between " << min << " and " << max << ": size " << random_num << "\n";
    } else {
        std::cout << "Only one max pattern" << std::endl;
    }

    auto iter = possible_sets.begin();
    for (int i = 0; i < random_num; i++) {
        iter++;
    }

    allowed_words = iter->second;
    
    std::vector<int> chosen_pattern = iter->first;
    for (int i = 0; i < answer_size; i++) {
        if (chosen_pattern[i] == 1) {
            reveal_answer[i] = letter;
            letters_revealed++;
        }
    }
    


    // for (int i = 0; i < same_letter.size(); i++) {
    //     int letter_count = same_letter[i];
    //     std::cout << same_letter[i] << " ";
    //     if (letter_count == words_left) {
    //         reveal_answer[i] = letter;
    //         letters_revealed++;
    //     }

    // }
    // std::cout << std::endl;
    

    



    // reveal_answer[min_pos] = letter;
}