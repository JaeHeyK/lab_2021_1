#include <bits/stdc++.h>

using namespace std;

const int MAX_SIZE = 30;
const int directionMap[4][2] = {{0,1},{-1,0},{0,-1},{1,0}};
int N, M;
unsigned char graph[MAX_SIZE][MAX_SIZE];
bool isCutVertex[MAX_SIZE][MAX_SIZE] ;
int dfsNum[MAX_SIZE][MAX_SIZE];
bool isGoToDest[MAX_SIZE][MAX_SIZE];
int visitCount = 0;
int startRow = 0;
int startCol = 0;

bool isReachableCell(int row, int col);
int getEarliestAncientInAdj(int curRow, int curCol);


int main() {
  ifstream in("lock.inp");
  ofstream out("lock.out");

  in >> N >> M;

  for(int row=0; row<N; ++row) {
    string rowStr;
    in >> rowStr;
    for(int col=0; col<M; ++col) {
      if(rowStr[col] == '#') {
        graph[row][col] = 0;
      } else {
        graph[row][col] = 1;
        if(rowStr[col] == 'S') {
          startRow = row;
          startCol = col;
        } else if ( rowStr[col] == 'T') {
          isGoToDest[row][col] = true;
        }
      }
      dfsNum[row][col] = -1;
    }
  }

  getEarliestAncientInAdj(startRow, startCol);

  isCutVertex[startRow][startCol] = false;

  vector<pair<int,int>> answer;
  
  for(int row=0; row<N; ++row) {
    for(int col=0; col<M; ++col) {
      if(isCutVertex[row][col] && isGoToDest[row][col]) {
        answer.push_back({col+1, N-row});
      }
    }
  }

  sort(answer.begin(), answer.end());

  if(answer.empty()) {
    out << 0 << "\n";
  } else {
    out << answer.size() << "\n";
    for(auto coord: answer) {
      out << coord.first << " " << coord.second << "\n";
    }
  }

  in.close();
  out.close();
  
  return 0;
}

bool isReachableCell(int row, int col) {
  if(( row>=0 && row<N ) && ( col>=0 && col<M )) {
    if(graph[row][col] > 0) {
      return true;
    } else {
      return false;
    }
  } else {
    return false;
  }
}

int getEarliestAncientInAdj(int curRow, int curCol) {
  dfsNum[curRow][curCol] = visitCount++;
  int result = dfsNum[curRow][curCol];
  
  for(int dir=0; dir<4; ++dir) {
    int nextRow = curRow + directionMap[dir][0];
    int nextCol = curCol + directionMap[dir][1];

    if(isReachableCell(nextRow, nextCol)) {
      if(dfsNum[nextRow][nextCol] == -1) {
        int adjcentAncient = getEarliestAncientInAdj(nextRow, nextCol);
        
        if((adjcentAncient >= dfsNum[curRow][curCol]) && isGoToDest[nextRow][nextCol]) {
          isCutVertex[curRow][curCol] = true;
        }
        isGoToDest[curRow][curCol] |= isGoToDest[nextRow][nextCol];
        result = min(result, adjcentAncient);
      } else {
        result = min(result, dfsNum[nextRow][nextCol]);
      }
    }
  }
  return result;
}
