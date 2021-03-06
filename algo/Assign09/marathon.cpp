#include <bits/stdc++.h>
using namespace std;

int graph[26][26]={0,};
int vst[26]={0,};
string ans;

void dfs(int nd, int dst, string tr) {
  for(int c=25;c>=0;c--) {
    int w = graph[nd][c];
    if(w!=0 && vst[c]==0){
      if(dst+w<42){
        vst[c]=1;
        dfs(c, dst+w, tr+(char)(c+'a'));
        vst[c]=0;
      } else if(dst+w==42 && c==0){
        if(ans.size()<=tr.size())
          ans = tr;
      }
    }
  }
}

int main() {
  ifstream in("marathon.inp");
  ofstream out("marathon.out");

  int N,M;
  in>>N>>M;

  char s,e;
  int w;
  for(int i=0;i<M;i++) {
    in>>s>>e>>w;
    graph[e-'a'][s-'a']=graph[s-'a'][e-'a']=w;
  }

  dfs(0,0,"a");

  if(ans.size()!=0){
    out<<ans.size()<<"\n";
    for(char ch:ans)
      out<<ch<<" ";
  } else {
    out<<"None";
  }
  return 0;
}