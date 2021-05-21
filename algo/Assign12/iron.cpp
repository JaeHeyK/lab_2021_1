#include <bits/stdc++.h>

using namespace std;

int graph[26][26]={0,};
int vst[26]={0,};
int nodes[26]={0,};
int maxEdge[26] = {0,};
int aAdj = 0;
int aAdjVisited = 0;
int longest = 0;
string ans;

void dfs(int node, int dst, int fwd, string tr) {
  if(dst+fwd < longest) {
    //cout << "bound! " << dst+fwd << " < " << longest <<"\n";
    return;
  }
  int repeat;
  if(aAdjVisited == aAdj) {
    repeat = 1;
  } else {
    repeat = 26;
  }
  for(int adj=repeat-1;adj>=0;adj--) {
    int w = graph[node][adj];
    if(vst[adj]==0 && w!=0){
      if(adj==0){
        if (dst+w>longest) {
          ans = tr;
          longest = dst+w;
        } else if (dst+w==longest){
          if(ans.size()<=tr.size()) {
            ans = tr;
            longest = dst+w;
          }
        } 
      } else {
        vst[adj]=1;
        if(graph[adj][0]!=0) {
          aAdjVisited++;
          dfs(adj, dst+w, fwd-maxEdge[node], tr+(char)(adj+'a'));
          aAdjVisited--;
        } else {
          dfs(adj, dst+w, fwd-maxEdge[node], tr+(char)(adj+'a'));
        }
        vst[adj]=0;
      }
    }
  }
}

int main() {
  ifstream in("iron.inp");
  ofstream out("iron.out");
  
  int N,M;
  in>>N>>M;

  char s,e;
  int w;
  for(int i=0;i<M;i++) {
    in>>s>>e>>w;
    graph[e-'a'][s-'a']=graph[s-'a'][e-'a']=w;
    nodes[s-'a']=nodes[e-'a']=1;
  }

  for(int i=0;i<26;i++) {
    if(graph[0][i] != 0) {
      aAdj++;
    }
  }

  int totalMax=0;
  for(int i=0;i<26;i++) {
    int max=0;
    if(nodes[i] == 1) {
      for(int j=0;j<26;j++) {
        max = max < graph[i][j] ? graph[i][j] : max;
        maxEdge[i] = max;
      }
      //cout << (char)(i+'a') << ": " << maxEdge <<"\n";
    }
    totalMax += max;
  }
  //cout << "totalMAXXX : " << totalMax <<"\n";

  dfs(0,0,totalMax, "a");

  if(ans.size()>1){
    out<<longest<<"\n";
    for(char ch:ans)
      out<<ch<<" ";
  } else {
    out<<"None";
  }

  in.close();
  out.close();

  return 0;
}