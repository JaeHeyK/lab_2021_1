#include <iostream>
#include <fstream>
#include <algorithm>
#include <queue>
#include <string>
#include <iomanip>

using namespace std;

int m, s, g; // score of each match, mismatch, gap
string wholeMusic;
string wholeMusic2;

int main() {
  ifstream in("music.inp");
  ofstream out("music.out");

  in >> m >> s >> g;
  

  int wholeAlignMax = 0;
  int maxAlignRow, maxAlignCol;

  int **LAT; // local alignment table
  int **maxLAT;
  
  for(int i=0; i < 3; i++) {
  string frontAlign = "";
  string backAlign = "";

  string maxFront = "";
  string maxBack = "";

  in >>wholeMusic >> wholeMusic2;
  int musicLen = wholeMusic.length();
  int musicLen2 = wholeMusic2.length();

  int frontLen = musicLen;
  int backLen = musicLen2;

  string front = wholeMusic;
  string back = wholeMusic2;
  
  LAT = new int*[frontLen+1];
  for(int i = 0; i < frontLen+1; i++) {
    LAT[i] = new int[backLen+1];
  }

  for(int row = 0; row < frontLen+1; row++) {
    LAT[row][0] = 0;
  }

  for(int col = 0; col < backLen+1; col++) {
    LAT[0][col] = 0;
  }


  for(int row = 1; row < frontLen+1; row++) {
    for(int col = 1; col < backLen+1; col++) {
      int vertScore = LAT[row-1][col] + g;
      int horiScore = LAT[row][col-1] + g;
      int diagScore = LAT[row-1][col-1];
      diagScore += (front[row-1] == back[col-1]) ? m : s;

      priority_queue<int> alignScores;

      alignScores.push(vertScore);
      alignScores.push(horiScore);
      alignScores.push(diagScore);
      alignScores.push(0);

      LAT[row][col] = alignScores.top();
      cout << LAT[row][col] << setw(3);
    }
    cout << "\n";
  }
  cout << "\n";

  int alignMax = 0;
  int maxRow = 0;
  int maxCol = 0;
  for(int row = frontLen; row >= 0; row--) {
    for(int col = backLen; col >= 0; col--) {
      if (LAT[row][col] >= alignMax) {
        alignMax = LAT[row][col];
        maxRow = row;
        maxCol = col;
      }
    }
  }

  wholeAlignMax = alignMax;
  maxFront = front;
  maxBack = back;
  maxAlignRow = maxRow;
  maxAlignCol = maxCol;
  maxLAT = LAT;


  int currScore = maxLAT[maxAlignRow][maxAlignCol];
  while(currScore != 0) {
    if (currScore - maxLAT[maxAlignRow-1][maxAlignCol] == g) {
      frontAlign.append(string(1, maxFront[maxAlignRow-1]));
      maxAlignRow--;
    } else if (currScore - maxLAT[maxAlignRow][maxAlignCol-1] == g) {
      backAlign.append(string(1, maxBack[maxAlignCol-1]));
      maxAlignCol--;
    } else if (currScore - maxLAT[maxAlignRow-1][maxAlignCol-1] == m || currScore - maxLAT[maxAlignRow-1][maxAlignCol-1] == s) {
      frontAlign.append(string(1, maxFront[maxAlignRow-1]));
      backAlign.append(string(1, maxBack[maxAlignCol-1]));
      maxAlignRow--;
      maxAlignCol--;
    }
    currScore = maxLAT[maxAlignRow][maxAlignCol];
  }

  std::reverse(frontAlign.begin(), frontAlign.end());
  std::reverse(backAlign.begin(), backAlign.end());
  if(frontAlign.compare(backAlign) < 0) {
    out << frontAlign << "\n";
    out << backAlign << "\n";
  } else {
    out << backAlign << "\n";
    out << frontAlign << "\n";
  }

  out << wholeAlignMax << "\n\n";

  int maxFrontLen = maxFront.length()+1;
  for(int i = 0; i < maxFrontLen; i++) {
    delete[] maxLAT[i];
  }
  delete[] maxLAT;
  }

  return 0;
}