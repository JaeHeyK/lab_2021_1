#include <bits/stdc++.h>

using namespace std;

int graph[26][26]={0,};
int vst[26]={0,};
int longest = 0;
string ans;

void dfs(int node, int dst, string tr) {
  for(int adj=25;adj>=0;adj--) {
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
        dfs(adj, dst+w, tr+(char)(adj+'a'));
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
  }

  dfs(0,0,"a");

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