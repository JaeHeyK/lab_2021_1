#include <iostream>
#include <fstream>
#include <string>
#include <list>

using namespace std;

int main() {
  ifstream in("3.inp");
  ofstream out("3_1.out");
  //My Code
  
  int N;

  list<string> words; 
  list<string> past_words;
  string input_word;
  
  in >> N;
  for(int word_count = 0; word_count < N; ++word_count) {
    in >> input_word;
    words.push_back(input_word);
  }

  list<string>::iterator word_first = words.begin();
  list<string>::iterator word_second = next(word_first);

  while(words.size() > 1) {
    int iter_count = words.size()/2;
    int is_size_odd = words.size()%2;

    while(iter_count > 0) {
      if((*word_first).compare(*word_second) == 0) {
        words.push_back((*word_first));
      } else {
        past_words.push_back((*word_first));
      }
      past_words.push_back((*word_second));
      advance(word_first, 2);
      advance(word_second, 2);

      words.pop_front();
      words.pop_front();
      --iter_count;
    }

    if((words.size() > 1) && is_size_odd == 1) {
      list<string>::iterator it;
      for(it = word_second; it != words.end(); ++it) {
        if((*it).compare(*word_first) == 0) {
          break;
        }
      } 
      if(it == words.end()) {
        past_words.push_back(*word_first);
        words.pop_front();
      }
    }
  }

  if(words.size() == 0) {
    out << "NONE" << "\n";
  } else {
    list<string>::iterator it;
    int final_word_count = 0;
    for(it = past_words.begin(); it != past_words.end(); ++it) {
      if((*it).compare(*word_first) == 0) {
        final_word_count++;
      }
    }
    if(final_word_count >= (N/2)) {
      out << *word_first << "\n";
    } else {
      out << "NONE" << "\n";
    }
  }

  in.close();
  out.close();

  return 0;
}