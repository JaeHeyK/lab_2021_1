#include <bits/stdc++.h>

using namespace std;

FILE* inpF;
vector<int> goalIndex;
vector<char*> dnaData;

vector<int> radixSortOrder(vector<int>, vector<int>, int);

int main() {
	inpF = fopen("read.inp", "r");
	ofstream out("read.out");
	fseek(inpF, 0, SEEK_END);
	long inpSize = ftell(inpF);
	fseek(inpF, 0, SEEK_SET);

	char *str = new char[inpSize+1];
	
	fread(str,1, inpSize, inpF);


	int i=0;
	int j=0;
  char* oneData = new char[1001];
  memset(oneData,'0',1001*sizeof(char));

  bool lineEnd = false;
	while(str[i]!='\0'){
		switch (str[i]) {
		case 'a': oneData[j] = '1'; break;
		case 'c': oneData[j] = '2'; break;
		case 'g': oneData[j] = '3'; break;
		case 't': oneData[j] = '4'; break;
		default: lineEnd = !lineEnd; break;
		}

    if(lineEnd) {
      oneData[1000] = '\0';
      dnaData.push_back(oneData);
      j = -2;

      oneData = new char[1001];
      memset(oneData,'0',1001*sizeof(char));
    }

    i++; j++;
	}

  delete[] oneData;
  delete[] str;

  int dataSize = dnaData.size();
  for(int i=0; i<4; i++) {
    goalIndex.push_back((int) floor((double)dataSize * (double)(i+1)/5.0));
    //cout << "Goal index " << i << ": " << goalIndex[i] <<"\n";
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
  //if(radixLevel == 15) return result;
  if(targetIndex.size() == 0) {
    return result;
  }
   if(dataIndex.size() == 1) {
  //  cout << "\tfound!: " << dataIndex[0] << "\n";
   return (result = dataIndex);
   }

  // cout << "\nLevel: " << radixLevel<< ", data size: " << dataIndex.size() <<"\n";
  vector<vector<int>> buckets;
  buckets.resize(5);
  for(int index : dataIndex) {
    buckets[dnaData[index][radixLevel]-'0'].push_back(index);
  }

  if(buckets[0].size() == dataIndex.size()) {
    result.push_back(dataIndex[0]);
    return result;
  }
  
  int bucketNum=0;
  int bound = 0;
  int accDataSize = 0;
  for(vector<int> bucket : buckets) {
    vector<int> newTargetIndex;
    vector<int> resultTargetIndex;

    while((bound<targetIndex.size()) && (bucket.size() > (targetIndex[bound]-accDataSize))) {
      newTargetIndex.push_back(targetIndex[bound]-accDataSize);
      bound++;
    }
    accDataSize += bucket.size();
    // cout << "\n\tbucket " << bucketNum++ << " , newTargets " << newTargetIndex.size() << " , acc " << accDataSize <<"\n";
    resultTargetIndex = radixSortOrder(bucket, newTargetIndex, radixLevel+1);
    result.insert(result.end(), resultTargetIndex.begin(), resultTargetIndex.end());
  }

  return result;
}