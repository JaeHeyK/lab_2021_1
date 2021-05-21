#include <bits/stdc++.h>

using namespace std;

int main() {
  ifstream in("ct.inp");
  ofstream out("ct.out");

  int N;
  in >> N;
  
  int i;
  char ans[N][N];
  for(int i=0; i<N; i++) {
    for(int j=0; j<N; j++) {
      ans[i][j] = '-';
    }
  }
  
  int colB[N], rowB[N], lDiagB[2*N-1], rDiagB[2*N-1];
  for(i=0; i<N; i++) { in >> colB[i]; }
  for(i=0; i<N; i++) { in >> rowB[i]; }
  for(i=0; i<2*N-1; i++) { in >> lDiagB[i]; }
  for(i=0; i<2*N-1; i++) { in >> rDiagB[i]; }
  
  int blockNum = 0;
  for(int b : colB) { blockNum += b; }

  unsigned int visited[N] = {0,};
  stack<vector<int>> blockGrid;
  vector<int> init(N,0);
  for(int i=0;i<rowB[0];i++) {
    init[i] = 1;
  }
  blockGrid.push(init);

  while(true) {
    vector<int> blockRow = blockGrid.top(); blockGrid.pop();
    int row = blockGrid.size();
    
    bool isValidRow = false;

    do {
      if(visited[row] == 0) {
        for(int col=0; col<N; col++) {
          colB[col] -= blockRow[col];
          lDiagB[N-1+(row-col)] -= blockRow[col];
          rDiagB[row+col] -= blockRow[col];
        }
        bool isAllCellValid = true;
        for(int col=0; col<N; col++) {
          bool isValidCell = (colB[col]>=0)&&(lDiagB[N-1+(row-col)]>=0)&&(rDiagB[row+col]>=0);
          isAllCellValid &= isValidCell;
        }
        isValidRow = isAllCellValid;
      }
      if(!isValidRow) {
        for(int col=0; col<N; col++) {
          colB[col] += blockRow[col];
          lDiagB[N-1+(row-col)] += blockRow[col];
          rDiagB[row+col] += blockRow[col];
        }
        if(visited[row] == 1) {
          visited[row] = 0;
        }
        if(!prev_permutation(blockRow.begin(), blockRow.end())) {
          break;
        }
      }

      
    } while (!isValidRow);

    if(isValidRow) {
      if(blockGrid.size() == N) {
        break;
      }
      blockGrid.push(blockRow);
      visited[row] = 1;
      vector<int> nextRow(N, 0);
      if(row+1 == N) break;
      for(int i=0;i<rowB[row+1];i++) {
        nextRow[i] = 1;
      }
      blockGrid.push(nextRow);
    } else {
      if(visited[row]==1) {
        blockGrid.push(blockRow);
      }
      visited[row] = 0;
    }
  }

  while(!blockGrid.empty()) {
    vector<int> curRow = blockGrid.top();
    blockGrid.pop();
    int r = blockGrid.size();

    for(int c = 0; c<N; c++) {
      ans[r][c] = curRow[c];
    }
  }

  for(int i=0; i<N; i++) {
    for(int j=0; j<N; j++) {
      char block = ans[i][j]==0 ? '-' : 'B';
      out << block << " ";
    }
    out << "\n";
  }
  
  in.close();
  out.close();
  return 0;
}