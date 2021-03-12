#include <iostream>
#include <fstream>
#include <string>

using namespace std;

bool isPalin(string::iterator first, string::iterator last) {
  if(distance(first, last) <= 0) return true;

  if(*first == *last) {
    return isPalin(next(first), prev(last));
  } else {
    return false;
  }
}

int getPalinType(string::iterator first, string::iterator last) {
  
  if(distance(first, last) <= 0) return 1;

  if(*first == *last) {
    return getPalinType(next(first), prev(last));
  } else {
    return (isPalin(first, prev(last)) || isPalin(next(first), last) ? 2 : 3);
  }
}

int main() {
  ifstream in("1.inp");
  ofstream out("1.out");
  
  int N;
  string input_str;
  
  in >> N;

  while(N--) {
    in >> input_str;
    int result = 0;
    string::iterator it_front = input_str.begin();
    string::iterator it_back = prev(input_str.end());
    result = getPalinType(it_front, it_back);
    out << result << "\n";
  }

  in.close();
  out.close();

  return 0;
}