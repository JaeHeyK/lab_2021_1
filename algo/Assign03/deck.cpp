#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

int main() {
  ifstream in("deck.inp");
  ofstream out("deck.out");
  

  long N;
  long currentNumber;
  long input_count = 0;
  long sum = 0;
  long squareSum = 0;
  
  in >> N;

  while(in>>currentNumber) {
    sum += currentNumber;
    squareSum += currentNumber*currentNumber;
    input_count++;
  }

  long origSum = (N * (N+1))/2;

  if(N - input_count == 1) {
    out << origSum - sum << "\n";
  } else {
    long sumOfTwo = origSum - sum;
    long origSquareSum = (N * (N+1) * (2*N+1))/6;
    long sumOfTwoSquare = origSquareSum - squareSum;

    long squareSubOfTwo = 2*sumOfTwoSquare - (sumOfTwo*sumOfTwo);
    long subOfTwo = (long) sqrt((double)squareSubOfTwo);

    out << (sumOfTwo - subOfTwo)/2 << "\n"; 
    out << (sumOfTwo + subOfTwo)/2 << "\n"; 
  }

  in.close();
  out.close();
  
  return 0;
}