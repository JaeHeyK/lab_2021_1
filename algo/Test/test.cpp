#include <bits/stdc++.h>

using namespace std;

vector<vector<int>> adj(13);
vector<int> discovered(13, -1);
vector<bool> isCutVertex(13, false);
int counter = 0;

int findCutVertex(int, bool);

int main() {
	adj[0].push_back(1);
	adj[1].push_back(0);
	adj[1].push_back(2);
	adj[1].push_back(3);
	adj[2].push_back(1);
	adj[2].push_back(3);
	adj[2].push_back(4);
	adj[3].push_back(1);
	adj[3].push_back(2);
	adj[3].push_back(4);
	adj[4].push_back(2);
	adj[4].push_back(3);
	adj[4].push_back(5);
	adj[5].push_back(4);
	adj[5].push_back(6);
	adj[5].push_back(8);
	adj[5].push_back(9);
	adj[6].push_back(5);
	adj[6].push_back(7);
	adj[6].push_back(8);
	adj[7].push_back(6);
	adj[7].push_back(8);
	adj[8].push_back(5);
	adj[8].push_back(6);
	adj[8].push_back(7);
	adj[8].push_back(9);
	adj[9].push_back(5);
	adj[9].push_back(8);
	adj[9].push_back(10);
	adj[10].push_back(9);
	adj[10].push_back(11);
	adj[10].push_back(12);
	adj[11].push_back(10);
	adj[11].push_back(12);
	adj[12].push_back(10);
	adj[12].push_back(11);

	findCutVertex(0, true);

	for(int i=0; i<isCutVertex.size(); ++i) {
		if(isCutVertex[i])
			cout<<i<<"\n";
	}
	
	return 0;
}

int findCutVertex(int here, bool isRoot) {
	discovered[here] = counter++;
	int ret = discovered[here];

	int children = 0;
	for(int i=0; i<adj[here].size(); ++i) {
		int there = adj[here][i];
		if(discovered[there] == -1) {
			++children;
			int subtree = findCutVertex(there, false);
			if(!isRoot && subtree >= discovered[here]) {
				isCutVertex[here] = true;
			}
			ret = min(ret, subtree);
		} else {
			ret = min(ret, discovered[there]);
		}
		if(isRoot) isCutVertex[here] = (children >= 2);
		cout << "here: " << here << ", there: " << there << ", ret: " << ret << ", cutVertex?: " << isCutVertex[here] << "\n";
 	}
	return ret;
}