#include <iostream>
#include <fstream>
#include <string>

using namespace std;

bool isPalin(int first, int last, string& str) {
  if(first >= last) return true;

  if(str[first] == str[last]) {
    return isPalin(first+1, last-1, str);
  } else {
    return false;
  }
}

int getPalinType(int first, int last, string& str) {
  if(first >= last) return 1;

  if(str[first] == str[last]) {
    return getPalinType(first+1, last-1, str);
  } else {
    return (isPalin(first, last-1, str) || isPalin(first+1, last, str) ? 2 : 3);
  }
}

int main() {
  ifstream in("4.inp");
  ofstream out("4.out");
  
  int N;
  string input_str;
  
  in >> N;

  while(N--) {
    in >> input_str;
    int result = 0;
    int len = input_str.length();
    result = getPalinType(0, input_str.length()-1, input_str);
    out << result << "\n";
  }

  in.close();
  out.close();

  return 0;
}