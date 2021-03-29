#include <fstream>
#include <iostream>

using namespace std;

int main() {
  ifstream in("jug.inp");
  ofstream out("jug.out");

  cout << "hello\n";

  in.close();
  out.close();
  return 0;
}