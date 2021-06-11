#include <bits/stdc++.h>

using namespace std;

FILE* inpF;
vector<int> goalIndex;
vector<string> dnaData;

vector<int> radixSortOrder(vector<int>, vector<int>, int);

int main() {
  inpF = fopen("read.inp","r");
	ofstream out("read.out");
  
  char oneData[1002];
  int test=0;
	while(fgets(oneData, sizeof(oneData),inpF) != NULL) {
    oneData[strlen(oneData)-1] = '\0';
		string strOneData(oneData);
    int i=0;
    while (strOneData[i]!='\0') {
      char ch;
      switch (strOneData[i]) {
        case 'a': ch = '1'; break;
        case 'c': ch = '2'; break;
        case 'g': ch = '3'; break;
        case 't': ch = '4'; break;
        default: break;
      } 
      strOneData[i] = ch;
      i++;
    }
    strOneData.resize(1000,'0');
    dnaData.push_back(strOneData);
	}

  int dataSize = dnaData.size();
  for(int i=0; i<4; i++) {
    goalIndex.push_back((int) floor((double)dataSize * (double)(i+1)/5.0));
  }

  vector<int> dnaIndex(dataSize);
  iota(dnaIndex.begin(), dnaIndex.end(), 0);

  vector<int> ansIndex;
  ansIndex = radixSortOrder(dnaIndex, goalIndex, 0);

  vector<string> ans;

  for(int i : ansIndex) {
    string ansRead;
    int radix=0;
    while (dnaData[i][radix] != '0') {
      char ch;

      switch (dnaData[i][radix]) {
        case '1': ch = 'a'; break;
        case '2': ch = 'c'; break;
        case '3': ch = 'g'; break;
        case '4': ch = 't'; break;
        default: break;
      }

      ansRead.push_back(ch);
      radix++;
    }
    ans.push_back(ansRead);
  }

  for(string str : ans) {
    out << str << "\n";
  }

	fclose(inpF);
	out.close();
	return 0;
}

vector<int> radixSortOrder(vector<int> dataIndex, vector<int> targetIndex, int radixLevel) {
  vector<int> result;
  if(targetIndex.size() == 0) {
    return result;
  }
   if(dataIndex.size() == 1) {
   return (result = dataIndex);
   }

  vector<vector<int>> buckets;
  buckets.resize(21);
  for(int index : dataIndex) {
    int radixIndex = (int)(dnaData[index][radixLevel*2]-'0'-1)*5+(int)(dnaData[index][radixLevel*2+1]-'0')+1;
    radixIndex = radixIndex < 0 ? 0 : radixIndex;
    buckets[radixIndex].push_back(index);
  }

  if((buckets[0].size() == dataIndex.size())) {
    for(int i=0; i<targetIndex.size(); i++) {
      result.push_back(dataIndex[0]);
    }
    return result;
  }
  
  int bound = 0;
  int accDataSize = 0;
  for(vector<int> bucket : buckets) {
    if(bucket.empty()) continue;
    
    vector<int> newTargetIndex;
    vector<int> resultTargetIndex;

    while((bound < targetIndex.size())&&(bucket.size() > (targetIndex[bound]-accDataSize))) {
      newTargetIndex.push_back(targetIndex[bound]-accDataSize);
      bound++;
    }
    accDataSize += bucket.size();

    resultTargetIndex = radixSortOrder(bucket, newTargetIndex, radixLevel+1);
    result.insert(result.end(), resultTargetIndex.begin(), resultTargetIndex.end());
  }

  return result;
}