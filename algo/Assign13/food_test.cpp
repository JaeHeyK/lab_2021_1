#include <bits/stdc++.h>

using namespace std;

int N;
double* prot, fat, carb, vit, cost;

int main(){
  ifstream in("food.inp");
  ofstream out("food.out");

  in >> N;
  double prot[N+1];
  double fat[N+1];
  double carb[N+1];
  double vit[N+1];
  double cost[N+1];

  in >> prot[0] >> fat[0] >> carb[0] >> vit[0];
  cost[0] = 0;

  for(int i=1; i<N+1; i++) {
    in >> prot[i] >> fat[i] >> carb[i] >> vit[i] >> cost[i];
  }

  out << "0";

  in.close();
  out.close();

  return 0;
}