#include <bits/stdc++.h>

using namespace std;
using coord = pair<int,int>;

int main() {
  ifstream in("ct.inp");
  ofstream out("ct.out");

  int N;
  in >> N;
  
  int i;
  char ans[N][N];
  unsigned int visited[N] = {0,};
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
  cout << "blockNum: " << blockNum <<"\n";

  vector<vector<int>> blockIndex;
  for(int row=0; row<N; row++) {
    vector<int> newRow(N,0);
    blockIndex.push_back(newRow);
    for(int col=0; col<rowB[row]; col++) {
      blockIndex[row][col] = 1;
    }
  }

  while(blockIndex.size() < N) {
    vector<int> colIndex = blockIndex.top(); blockIndex.pop();
    
    int row = blockIndex.size();

    bool isValidRow = false;

    while(!isValidRow) {
      for(int col=0; col<N; col++) {
        colB[col]--;
        lDiagB[N-1+(row-col)]--;
        rDiagB[row+col]--;
      }
      bool isCellConnected = false;
      for(int col=0; col<N; col++) {
        bool isValidCell = (colB[col]>=0)&&(lDiagB[N-1+(row-col)]>=0)&&(rDiagB[row+col]>=0);
        isValidRow = isValidCell ? true : false;
      }
    }

    if(isValidRow) {
      vector<int> nextRow(N, 0);
      for(int i=0;i<rowB[row];i++) {
        nextRow[i] = 1;
      }
      blockIndex.push(nextRow);
    } else {
      if(prev_permutation(colIndex.begin(), colIndex.end())) {
        blockIndex.push(colIndex);
      } else {
        vector<int> prevColIndex = blockIndex.top();
        
      }
    }


    while(rowB[row] > 0) {
      
      int nextCol = colIndex.empty() ? 0 : colIndex.top()+1;

      if(visited[row]){

      }
      int col = colIndex.top(); colIndex.pop();
      if(col == N) {continue;}
      bool isFine = (rowB[row]>0)&&(colB[col]>0)&&(lDiagB[N-1+(row-col)]>0)&&(rDiagB[row+col]>0);
      while(isFine) {

      }
      if(isFine) {
        rowB[row]--;
        colB[col]--;
        lDiagB[N-1+(row-col)]--;
        rDiagB[row+col]--;
        colIndex.push(col);
      }
    }

    if(blockIndex.size() == N) {
      break;
    }

    if(rowB[row] == 0)  {
      blockIndex.push(colIndex);
      stack<int> nextColIndex;
      blockIndex.push(nextColIndex);
    } else {
      visited[row] += 1<<(colIndex.top());
    }

  }

  while(!blockIndex.empty()) {
    coord curr = blockIndex.top(); blockIndex.pop();
    ans[curr.first][curr.second] = 'B';
    cout << "(" << curr.first << ", " << curr.second << ")\n";
  }
  cout << "\n";

  for(int i=0; i<N; i++) {
    for(int j=0; j<N; j++) {
      cout << ans[i][j] << " ";
    }
    cout << "\n";
  }
  
  in.close();
  out.close();
  return 0;
}