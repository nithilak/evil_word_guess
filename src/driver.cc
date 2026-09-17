#include "word_guess_constants.hpp"
#include "word_guess_functions.hpp"
#include <iostream>
#include <fstream>

int main() {
  DictionaryToMap(kDictionaryFile);
  std::cout << "Loaded " << kDictionaryMap.size() << " dictionary entries." << std::endl;

  int mode = getIntegerInputMode();
  std::cout << std::endl;
  
  answer_size = getIntegerInputWordSize();;
  if (answer_size == 0) {
      answer_size = ChooseRandomNum(kMinWordLength, kMaxWordLength);
  }
  reveal_answer = std::string(answer_size, '_');
  if (mode == 0) {
    ChooseRandomWord(answer_size);
  }
  

  GetAllowedWords();
  std::cout << "Loaded " << allowed_words.size() << " allowed words." << std::endl;

  std::cout << reveal_answer << std::endl;
  std::cout << "Guesses remaining: " << kMaxMisses << std::endl;

  int misses = 0;
  char guess;
  std::set<char> guesses;
  std::cout << "Make a guess: ";
  while (misses < kMaxMisses) {
    std::cin >> guess;
    if ( (guess < 65 || guess > 90)) {
      std::cout << "Please enter an uppercase letter." << std::endl;
      continue;
    }
    if (guesses.contains(guess)) {
      std::cout << "Already guessed." << std::endl;
      continue;
    }

    guesses.insert(guess);

    if (mode == 0) {
      misses += !MakeAGuess(guess);
    } else if ((allowed_words.size() == 1)) {
      misses += !MakeAGuess(guess);
      std::cout << "Have " << allowed_words.size() << " allowed words." << std::endl; // 1 allowed word
    } else {
      if (mode == 1) { //mode == 1
        misses += !MakeAnEvilGuess(guess);
      } else if (mode == 2) {
        misses += !MakeAnEasyGuess(guess);
      }
      //int words_left = allowed_words.size();
      std::cout << "Have " << allowed_words.size() << " allowed words." << std::endl;
      // if (words_left < 40) {
      //   for (int index : allowed_words) {
      //     std::cout << kDictionaryMap[index] << std::endl;
      //   }
      // }
      if (allowed_words.size() == 1) {
        auto iter = allowed_words.begin();
        answer = kDictionaryMap[*iter];
        mode = 0;
      }
    }

    std::cout << reveal_answer << std::endl;
    if (letters_revealed == answer_size) {
      std::cout << "You win!" << std::endl;
      return 0;
    }
    
    //see already guessed
    std::cout << "Guesses: ";
    PrintSet(guesses);

    std::cout << "Guesses remaining: " << kMaxMisses - misses << std::endl;

  } 
  std::cout << "You lose!" << std::endl;
  std::cout << "The word was: ";
  if (mode == 0) {
    std::cout << answer;
  } else if (mode == 1) {
    std::cout << ReturnRandomAllowedWord();
  }
  std::cout << std::endl;


  return 0;
}