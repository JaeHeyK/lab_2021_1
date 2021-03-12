#include <iostream>
#include <fstream>
#include <string>
#include <list>

using namespace std;

int main() {
  ifstream in("words.inp");
  ofstream out("words.out");
  //My Code
  
  int N;

  list<string> words; 
  list<string> past_words;
  string first_input_word;
  string second_input_word;
  
  in >> N;
  for(int pair_count = 1; pair_count <= N/2; ++pair_count) {
    in >> first_input_word;
    in >> second_input_word;
    if(first_input_word == second_input_word) {
      words.push_back(first_input_word);
    } else {
      past_words.push_back(first_input_word);
    }
    past_words.push_back(second_input_word);
  }

  if( N%2 == 1) { //When N is odd number - so there is one last words after above loop
    in >> first_input_word;
    
    list<string>::iterator it;
    for(it = words.begin(); it != words.end(); ++it) {
      if(*it == first_input_word) {
        break;
      }
    } 
    if(it == words.end() && !words.empty()) {
      past_words.push_back(first_input_word);
    } else {
      words.push_back(first_input_word);
    }
  } 

  list<string>::iterator first_word = words.begin();
  list<string>::iterator second_word = next(first_word);

  while(words.size() > 1) {
    while(first_word != words.end() && next(first_word) != words.end()) {
      second_word = next(first_word);
      if(*first_word == *second_word) {
        past_words.push_back((*second_word));
        first_word = words.erase(second_word);
      } else {
        past_words.push_back((*first_word));
        past_words.push_back((*second_word));
        first_word = words.erase(first_word, next(second_word));
      }
    }

    if(first_word != words.end() && words.size() > 1) {
      list<string>::iterator it;
      for(it = second_word; it != words.end(); ++it) {
        if(*it == *first_word) {
          break;
        }
      } 
      if(it == words.end()) {
        past_words.push_back(*first_word);
        words.erase(first_word);
      }
    }

    first_word = words.begin();
  }

  if(words.size() == 0) {
    out << "NONE" << "\n";
  } else {
    list<string>::iterator it;
    int final_word_count = 0;
    for(it = past_words.begin(); it != past_words.end(); ++it) {
      if(*it == *first_word) {
        final_word_count++;
      }
    }
    if(final_word_count < (N/2)) {
      out << "NONE" << "\n";
    } else {
      out << *first_word << "\n";
    }
  }

  in.close();
  out.close();

  return 0;
}