#include "word_guess_constants.hpp"
#include "word_guess_functions.hpp"
#include <iostream>

int main() {
  DictionaryToMap(kDictionaryFile);
  std::cout << "Loaded " << kDictionaryMap.size() << " dictionary entries." << std::endl;
  ChooseRandomWord();
  GetAllowedWords();
  std::cout << "Loaded " << allowed_words.size() << " allowed words." << std::endl;

  int mode = getIntegerInput("Choose Mode: Normal 0 Evil 1");
  

  std::cout << reveal_answer << std::endl;

  int misses = 0;
  char guess;
  std::set<char> guesses;
  std::cout << "Make a guess: ";
  while (misses < kMaxMisses) {
    std::cin >> guess;
    if (guess < 65 || guess > 90) {
      std::cout << "Please enter an uppercase letter." << std::endl;
      continue;
    }
    if (guesses.contains(guess)) {
      std::cout << "Already guessed." << std::endl;
      continue;
    }
    guesses.insert(guess);

    if (mode) { //mode == 1
      misses += !MakeAnEvilGuess(guess);
      int words_left = allowed_words.size();
      std::cout << "Have " << allowed_words.size() << " allowed words." << std::endl;
      // if (words_left < 40) {
      //   for (int index : allowed_words) {
      //     std::cout << kDictionaryMap[index] << std::endl;
      //   }
      // }
    } else {
      misses += !MakeAGuess(guess);
    }

    std::cout << reveal_answer << std::endl;
    if (letters_revealed == answer_size) {
      std::cout << "You win!" << std::endl;
      break;
    }
    
    //see already guessed
    std::cout << "Guesses: ";
    PrintSet(guesses);


  }


  return 0;
}