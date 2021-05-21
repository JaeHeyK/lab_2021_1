#include <bits/stdc++.h>

using namespace std;

int N, M;

int main() {
  ifstream in("iron.inp");
  ofstream out("iron.out");
  
  in>>N>>M;

  char s,e;
  int w;
  for(int i=0;i<M;i++) {
    in>>s>>e>>w;
  }

  
  out << "None";
  in.close();
  out.close();

  return 0;
}