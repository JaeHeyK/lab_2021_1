#include <iostream>
#include <fstream>
#include <algorithm>
#include <queue>
#include <string>
#include <iomanip>

using namespace std;

int m, s, g; // score of each match, mismatch, gap
string wholeMusic;
typedef struct _Score {
  int s;
  int row;
  int col;
} Score;

struct cmp {
  bool operator() (Score& a, Score& b) {
    if(a.s > b.s) {
      if(a.row > b.row) {
        return a.col <= b.col;
      } else {
        return a.row <= b.row;
      }
    } else {
      return a.s <= b.s;
    }
  }
};

int main() {
  ifstream in("music.inp");
  ofstream out("music.out");

  in >> m >> s >> g >> wholeMusic;
  
  int musicLen = wholeMusic.length();

  int wholeAlignMax = 0;
  int maxAlignRow, maxAlignCol;
  string frontAlign = "";
  string backAlign = "";

  string maxFront = "";
  string maxBack = "";

  int **LAT; // local alignment table
  int **maxLAT;
  
  for(int musicIdx = musicLen-1; musicIdx >= 1; musicIdx--) {
    int frontLen = musicIdx;
    int backLen = musicLen - musicIdx;

    string front = wholeMusic.substr(0, frontLen);
    string back = wholeMusic.substr(musicIdx, backLen);
    
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
      }
    }

    int alignMax = 0;
    int maxRow = 0;
    int maxCol = 0;
    // for(int row = frontLen; row >= 0; row--) {
    //   for(int col = backLen; col >= 0; col--) {
    //     if (LAT[row][col] >= alignMax) {
    //       alignMax = LAT[row][col];
    //       maxRow = row;
    //       maxCol = col;
    //     }
    //   }
    // }
    priority_queue<Score, vector<Score>, cmp> Scores;

    for(int row = frontLen; row >= 0; row--) {
      for(int col = backLen; col >= 0; col--) {
        Score score;
        score.s = LAT[row][col];
        score.row = row;
        score.col = col;
        Scores.push(score);
      }
    }

    alignMax = Scores.top().s;
    maxRow = Scores.top().row;
    maxCol = Scores.top().col;

    if( alignMax >= wholeAlignMax ) {
      int maxFrontLen = maxFront.length()+1;
      if(maxFront != "") {
        for(int i = 0; i < maxFrontLen; i++) {
          delete[] maxLAT[i];
        }
        delete[] maxLAT;
      }
      wholeAlignMax = alignMax;
      maxFront = front;
      maxBack = back;
      maxAlignRow = maxRow;
      maxAlignCol = maxCol;
      maxLAT = LAT;
    } else {
      for(int i = 0; i < frontLen+1; i++) {
        delete[] LAT[i];
      }
      delete[] LAT;
    }
  }

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

  out << wholeAlignMax << "\n";

  int maxFrontLen = maxFront.length()+1;
  for(int i = 0; i < maxFrontLen; i++) {
    delete[] maxLAT[i];
  }
  delete[] maxLAT;

  return 0;
}