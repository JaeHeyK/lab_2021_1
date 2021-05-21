#include <bits/stdc++.h>

using namespace std;

int N, M;
int graph[26][26]={0,};
vector<int> nodeIndex;
int aAdj = 0;
int aAdjVisited = 0;
int inputChar[26]={0,};
int vst[26]={0,};
int longest = 0;
string ans;

void dfs(int node, int dst, string tr) {
  int repeat;
  if(aAdjVisited == aAdj) {
    repeat = 1;
  } else {
    repeat = N;
  }
  for(int adj=N-1;adj>=0;adj--) {
    int w = graph[node][nodeIndex[adj]];
    if(vst[nodeIndex[adj]]==0 && w!=0){
      if(nodeIndex[adj]==0){
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
        vst[nodeIndex[adj]]=1;
        if(graph[nodeIndex[adj]][0]!=0) {
          aAdjVisited++;
          dfs(nodeIndex[adj], dst+w, tr+(char)(nodeIndex[adj]+'a'));
          aAdjVisited--;
        } else {
          dfs(nodeIndex[adj], dst+w, tr+(char)(nodeIndex[adj]+'a'));
        }
        vst[nodeIndex[adj]]=0;
      }
    }
  }
}

int main() {
  ifstream in("iron.inp");
  ofstream out("iron.out");
  
  in>>N>>M;

  char s,e;
  int w;
  nodeIndex.reserve(N);
  for(int i=0;i<M;i++) {
    in>>s>>e>>w;
    graph[e-'a'][s-'a']=graph[s-'a'][e-'a']=w;
    inputChar[s-'a'] = 1;
    inputChar[e-'a'] = 1;
  }

  for(int i=0; i<26; i++) {
    if(inputChar[i]==1) {
      nodeIndex.push_back(i);
    }
  }

  for(int i=0; i<26; i++) {
    if(graph[0][i] != 0) {
      aAdj++;
    }
  }
  
  bool isCycleExist;

  do {
    isCycleExist = true;
    int totalCount = 0;
    for(int s=0;s<N;s++) {
      int count=0;
      int last = 0;
      for(int e=0;e<N;e++) {
        if (graph[nodeIndex[s]][nodeIndex[e]]!=0) {
          count++;
          totalCount++;
          last = e;
        }
      }
      if(count==1) {
        graph[nodeIndex[s]][nodeIndex[last]] = 0;
        graph[nodeIndex[last]][nodeIndex[s]] = 0;
        isCycleExist = false;
      }
    }
    if(totalCount==0) {
      isCycleExist = false;
      break;
    }
  } while(!isCycleExist);

  if(!isCycleExist) {
    out<<"None";
  } else {
    dfs(0,0,"a");

    if(ans.size()>1){
      out<<longest<<"\n";
      for(char ch:ans)
        out<<ch<<" ";
    } else {
      out<<"None";
    }
  }

  in.close();
  out.close();

  return 0;
}