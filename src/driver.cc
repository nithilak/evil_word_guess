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
  
  answer_size = getIntegerInputWordSize();;
  if (answer_size == 0) {
    answer_size = ChooseRandomNum(kMinWordLength, kMaxWordLength);
  }

  for (int i = 0; i < answer_size - 1; i++) {
    reveal_answer.push_back('_');
    reveal_answer.push_back(' ');
  }
  reveal_answer.push_back('_');
  
  GetAllowedWords();
  std::cout << "Loaded " << allowed_words.size() << " allowed words." << std::endl;

  if (mode == 0) {
    answer = ReturnRandomAllowedWord();
  }

  std::cout << reveal_answer << std::endl;
  std::cout << "Guesses remaining: " << kMaxMisses << std::endl;

  int misses = 0;
  char guess;
  std::set<char> guesses;
  std::queue<char> guess_queue; 
  std::cout << "Make a guess: ";
  while (misses < kMaxMisses) {
    //std::cin >> guess;

    if (guess_queue.empty()) {
      std::string input;
      std::getline(std::cin, input); // Read the full line of input

      if (input.empty()) {
        std::cout << "Please enter an uppercase letter." << std::endl;
        continue;
      }

      if (input.size() == 1) {
        guess = input.at(0);
      } else {
        // Push each character into the queue
        for (char ch : input) {
            guess_queue.push(ch);
        }
        //continue;
        guess = guess_queue.front();
        guess_queue.pop();
      }

    } else {
      guess = guess_queue.front();
      guess_queue.pop();
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