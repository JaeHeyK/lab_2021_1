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
  int visited[N][N] = {0,};
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

  stack<stack<int>> blockIndex;
  stack<int> init;
  blockIndex.push(init);

  while(!blockIndex.empty()) {
    stack<int> colIndex = blockIndex.top(); blockIndex.pop();
    int row = blockIndex.size();

    while(rowB[row] > 0) {
      
      int col = colIndex.top(); colIndex.pop();
      colIndex.push(col+1);
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

    if(colIndex.size() == rowB[row])  {
      stack<int> nextRow;
      nextRow.push(0);
      blockIndex.push(nextRow);
    }

  }

  while(!blockIndex.empty()) {
    // coord curr = blockIndex.top(); blockIndex.pop();
    // ans[curr.first][curr.second] = 'B';
    // cout << "(" << curr.first << ", " << curr.second << ")\n";
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