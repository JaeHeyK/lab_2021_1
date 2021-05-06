#include <iostream>
#include <fstream>
#include <algorithm>
#include <queue>
#include <string>

using namespace std;

class LAT { // Local Alignment Table class
private:
  int** table;
  int row;
  int col;

  string src;
  string frontSrc;
  string backSrc;

  string frontAlign = "";
  string backAlign = "";

  int alignEndRow;
  int alignEndCol;

public:
  int index;
  int maxScore = -1;
  
public:
  LAT(string _src, int _index) : src(_src), index(_index) {
    row = index+1;
    col = src.length() - row;

    frontSrc = src.substr(0, row);
    backSrc = src.substr(index+1, col);


    table = new int*[row+1];
    for(int i = 0; i < row+1; i++) {
      table[i] = new int[col+1];
    }
  }

  void calculateTable(int m, int s, int g) {
    for(int r = 0; r < row+1; r++) {
      table[r][0] = 0;
    }

    for(int c = 0; c < col+1; c++) {
      table[0][c] = 0;
    }

    for(int r = 1; r < row+1; r++) {
      for(int c = 1; c < col+1; c++) {
        int vertScore = table[r-1][c] + g;
        int horiScore = table[r][c-1] + g;
        int diagScore = table[r-1][c-1];
        diagScore += (frontSrc[r-1] == backSrc[c-1]) ? m : s;

        priority_queue<int> alignScores;

        alignScores.push(vertScore);
        alignScores.push(horiScore);
        alignScores.push(diagScore);
        alignScores.push(0);

        table[r][c] = alignScores.top();
      }
    }
  }

  int getMaxScore(int m, int s, int g) {
    if(maxScore != -1) return maxScore;

    calculateTable(m, s, g);

    for(int r = row; r >= 0; r--) {
      for(int c = col; c >= 0; c--) {
        if (table[r][c] >= maxScore) {
          maxScore = table[r][c];
          alignEndRow = r;
          alignEndCol = c;
        }
      }
    }

    return maxScore;
  }

  pair<string,string> getAlignPair(int m, int s, int g) {
    if((frontAlign != "") && (backAlign != "")) {
      return make_pair(frontAlign, backAlign);
    }

    int currScore = maxScore;
    int currRow = alignEndRow;
    int currCol = alignEndCol;

    while(currScore != 0) {
      if (currScore - table[currRow-1][currCol] == g) {
        frontAlign.append(string(1, frontSrc[currRow-1]));
        currRow--;
      } else if (currScore - table[currRow][currCol-1] == g) {
        backAlign.append(string(1, backSrc[currCol-1]));
        currCol--;
      } else if (currScore - table[currRow-1][currCol-1] == m || currScore - table[currRow-1][currCol-1] == s) {
        frontAlign.append(string(1, frontSrc[currRow-1]));
        backAlign.append(string(1, backSrc[currCol-1]));
        currRow--;
        currCol--;
      }
      currScore = table[currRow][currCol];
    }

    std::reverse(frontAlign.begin(), frontAlign.end());
    std::reverse(backAlign.begin(), backAlign.end());


    if (frontAlign.compare(backAlign) < 0) {
      return make_pair(frontAlign, backAlign);
    } else {
      return make_pair(backAlign, frontAlign);
    }
  }

  ~LAT() {
    for(int i = 0; i < row; i++) {
      delete[] table[i];
    }
    delete[] table;
  }
};

int m, s, g; // score of each match, mismatch, gap
string wholeMusic;

int maxTripple (int a, int b, int c) {
  if(a >= b) {
    if(a >= c) {
      return -1;
    } else {
      return 1;
    }
  } else {
    if (b >= c) {
      return 0;
    } else {
      return 1;
    }
  }
}

int main() {
  ifstream in("music.inp");
  ofstream out("music.out");

  in >> m >> s >> g >> wholeMusic;
  
  string frontAlign = "";
  string backAlign = "";

  string maxFront = "";
  string maxBack = "";

  int rangeHead = 0;
  int rangeTail = wholeMusic.length()-2;
  int rangeMid = (rangeTail-rangeHead)/2;

  LAT* middleLAT;
  LAT* leftLAT;
  LAT* rightLAT;

  
  middleLAT = new LAT(wholeMusic, rangeMid);

  while (rangeHead < rangeMid && rangeMid < rangeTail) {
    leftLAT = new LAT(wholeMusic, rangeHead);
    rightLAT = new LAT(wholeMusic, rangeTail);

    int leftScore = leftLAT->getMaxScore(m, s, g);
    int midScore = middleLAT->getMaxScore(m, s, g);
    int rightScore = rightLAT->getMaxScore(m, s, g);

    int maxScorePos = maxTripple(leftScore, midScore, rightScore);

    switch (maxScorePos) {
    case -1:
      rangeTail = rangeMid-1;
      delete middleLAT;
      delete rightLAT;
      middleLAT = leftLAT;
      rangeMid = (rangeTail + rangeHead)/2;
      break;

    case 0:
      rangeHead = leftLAT->index+1;
      rangeTail = rightLAT->index-1;
      delete leftLAT;
      delete rightLAT;
      break;

    case 1:
      rangeHead = rangeMid+1;
      delete leftLAT;
      delete middleLAT;
      middleLAT = rightLAT;
      rangeMid = (rangeTail + rangeHead)/2;
      break;

    default:
      break;
    }
  }

  pair<string,string> align = middleLAT->getAlignPair(m, s, g);

  out << align.first << "\n";
  out << align.second << "\n";
  out << middleLAT->maxScore << "\n";

  return 0;
}