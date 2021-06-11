#include <bits/stdc++.h>

using namespace std;

FILE* inpF;
vector<int> goalIndex;
vector<string> dnaData;

vector<string> radixSortOrder(vector<string>, vector<int>, int);

int main() {
  inpF = fopen("read.inp","r");
	ofstream out("read.out");
  
  char oneData[1002];
  int test=0;
	while(fgets(oneData, sizeof(oneData),inpF) != NULL) {
    oneData[strlen(oneData)-2] = '\0';
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

  vector<string> ans;
  ans = radixSortOrder(dnaData, goalIndex, 0);
  vector<string> actualAns;


  for(string ansRead : ans) {
    string actualRead(ansRead);
    int actualLength = actualRead.find_first_of('0');
    actualRead.resize(actualLength);

    replace(actualRead.begin(), actualRead.end(),'1','a');
    replace(actualRead.begin(), actualRead.end(),'2','c');
    replace(actualRead.begin(), actualRead.end(),'3','g');
    replace(actualRead.begin(), actualRead.end(),'4','t');
    
    actualAns.push_back(actualRead);
  }

  for(string str : actualAns) {
    out << str << "\n";
  }

	fclose(inpF);
	out.close();
	return 0;
}

vector<string> radixSortOrder(vector<string> unorderedData, vector<int> targetIndex, int radixLevel) {
  vector<string> result;
  if(targetIndex.size() == 0) {
    return result;
  }
   if(unorderedData.size() == 1) {
   return (result = unorderedData);
   }

  vector<vector<string>> buckets;
  buckets.resize(21);
  for(string str : unorderedData) {
    int radixIndex = (int)(str[radixLevel*2]-'0'-1)*5+(int)(str[radixLevel*2+1]-'0')+1;
    radixIndex = radixIndex < 0 ? 0 : radixIndex;
    buckets[radixIndex].push_back(str);
  }

  if((buckets[0].size() == unorderedData.size())) {
    for(int i=0; i<targetIndex.size(); i++) {
      result.push_back(unorderedData[0]);
    }
    return result;
  }
  
  int bound = 0;
  int accDataSize = 0;
  for(vector<string> bucket : buckets) {
    if(bucket.empty()) continue;
    
    vector<int> newTargetIndex;
    vector<string> resultString;

    while((bucket.size() > (targetIndex[bound]-accDataSize))) {
      newTargetIndex.push_back(targetIndex[bound]-accDataSize);
      bound++;
    }
    accDataSize += bucket.size();

    resultString = radixSortOrder(bucket, newTargetIndex, radixLevel+1);
    result.insert(result.end(), resultString.begin(), resultString.end());
  }

  return result;
}