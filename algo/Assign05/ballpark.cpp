#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

int tripleMax(int a, int b, int c) {
  if(a < b) {
    return (b < c ? c : b);
  } else {
    return (a < c ? c : a);
  }
}

int main() {
  ifstream in("ballpark.inp");
  ofstream out("ballpark.out");

  int height, width;

  in >> height >> width;
 
  int field[height][width];

  // string rowString;

  // for(int row = height - 1; row >= 0; row--) {
  //   in >> rowString;

  //   for(int col = 0; col < width; col++) {
  //     field[row][col] = (int)(rowString[col] - 48)-1;
  //   }
  // }
  char num;
  for(int row = height - 1; row >= 0; row--) {
    for(int col = 0; col < width; col++) {
      in >> num;
      field[row][col] = (int)(num - 48)-1;
    }
  }

  int minusMaxRadius = 0;

  for(int pivotY = height-2; pivotY >= 0; pivotY--) {
    for(int pivotX = width-2; pivotX >=0; pivotX--) {
      if (field[pivotY][pivotX] <= -1) {
        int currentMax = tripleMax(field[pivotY+1][pivotX], field[pivotY][pivotX+1], field[pivotY+1][pivotX+1]) - 1;

        field[pivotY][pivotX] = currentMax;
        
        minusMaxRadius = (minusMaxRadius < currentMax ? minusMaxRadius : currentMax);
        }
    }
  }

  vector<pair<int,int>> maxAreaPivots;
  int maxAreaCount = 0;

  for(int pivotX = 0; pivotX < width; pivotX++) {
    for(int pivotY = 0; pivotY < height; pivotY++) {
      if(field[pivotY][pivotX] == minusMaxRadius) {
        maxAreaPivots.push_back(make_pair(pivotX, pivotY));
        maxAreaCount++;
      }
    }
  }

  out << -minusMaxRadius << " " << maxAreaCount << "\n";
  for(auto pivot : maxAreaPivots) {
    out << pivot.first+1 << " " << pivot.second+1 << "\n";
  }
  
  in.close();
  out.close();
  
  return 0;
}

