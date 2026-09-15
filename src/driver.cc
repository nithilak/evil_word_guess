#include "word_guess_constants.hpp"
#include "word_guess_functions.hpp"
#include <iostream>

int main() {
  DictionaryToMap(kDictionaryFile);
  std::cout << "Loaded " << kDictionaryMap.size() << " dictionary entries." << std::endl;
  ChooseRandomWord();
  GetAllowedWords();
  std::cout << "Loaded " << allowed_words.size() << " allowed words." << std::endl;
  


  int misses = 0;
  char guess;
  std::set<char> guesses;
  std::cout << "Make a guess: ";
  while (misses < kMaxMisses) {
    std::cin >> guess;
    if (guesses.contains(guess)) {
      std::cout << "Already guessed." << std::endl;
      continue;
    }
    guesses.insert(guess);

    misses += !MakeAnEvilGuess(guess);
    int words_left = allowed_words.size();
    std::cout << "Have " << allowed_words.size() << " allowed words." << std::endl;
    if (words_left < 40) {
      for (int index : allowed_words) {
        std::cout << kDictionaryMap[index] << std::endl;
      }
    }
    std::cout << reveal_answer << std::endl;
    if (letters_revealed == answer_size) {
      std::cout << "You win!" << std::endl;
      break;
    }
  }


  return 0;
}