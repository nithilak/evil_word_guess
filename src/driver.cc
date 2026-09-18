#include "word_guess_constants.hpp"
#include "word_guess_functions.hpp"
#include <iostream>
#include <fstream>
#include <queue>

int main() {
  DictionaryToMap(kDictionaryFile);
  std::cout << "Loaded " << kDictionaryMap.size() << " dictionary entries." << std::endl;

  int mode = getIntegerInputMode();
  std::cout << std::endl;
  if (mode != 4) {
    answer_size = getIntegerInputWordSize();
    if (answer_size == 0) {
      answer_size = ChooseRandomNum(kMinWordLength, kMaxWordLength);
    }

    GetAllowedWords();
    std::cout << "Loaded " << allowed_words.size() << " allowed words." << std::endl;

    if (mode == 0) {
      ChooseRandomAllowedWord();
    }
  } else {
    answer = getCustomWord();
    answer_size = answer.size();
    mode = 0;
    std::cout << std::string('\n', 12) << std::endl;
  }

  for (int i = 0; i < answer_size - 1; i++) {
    reveal_answer.push_back('_');
    reveal_answer.push_back(' ');
  }
  reveal_answer.push_back('_');

  std::cout << reveal_answer << std::endl;
  std::cout << "Guesses remaining: " << kMaxMisses << std::endl;

  int misses = 0;
  char guess;
  std::set<char> guesses;
  bool process_queue = false;
  int queue_int = 0;
  std::string guess_queue; 
  int queue_size = 0;
  std::cout << "Make a guess: ";
  while (misses < kMaxMisses) {
    //std::cin >> guess;

    if (!process_queue) {
      std::getline(std::cin, guess_queue); // Read the full line of input

      if (guess_queue.empty()) {
        std::cout << "Please enter an uppercase letter." << std::endl;
        continue;
      }

      guess = guess_queue.at(0);
      if (guess_queue.size() > 1) {
        process_queue = true;
        queue_int = 1;
        queue_size = guess_queue.size();
      }

    } else {
      guess = guess_queue.at(queue_int);
      if (++queue_int >= queue_size) { //could be ==
        process_queue = false;
      }
    }

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
      } else if (mode == 3) {
        misses += !MakeAnEasyIncludeGuess(guess);
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