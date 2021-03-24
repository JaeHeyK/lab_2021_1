#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

typedef struct _Request {
  int b;  // Beginning day
  int e;  // Ending day
  int c;  // Cost
  int pr; // Last workable request among previous requests; Order of requests will be determined by sorting
  int num;  //  Request number after sorting
} Request;

typedef struct _Schedule {
  int p;  // Total pay
  int d;  // Total workday
} Schedule;

typedef vector<Request>::iterator ReqIt;
typedef vector<Request>::reverse_iterator ReqRit;

bool reqCompare(const Request&, const Request&);

vector<Request> requests;

int main() {
  ifstream in("free.inp");
  ofstream out("free.out");

  int N;  // Total number of request
  in >> N;

  for (int i=0; i<N; i++) {
    Request newReq;
    in >> newReq.b >> newReq.e >> newReq.c;
    requests.push_back(newReq);
  }

  sort(requests.begin(), requests.end(), reqCompare);

  for (int req=0; req<N; req++) {
    requests[req].num = req+1;
  }

  for (int currReq=N-1; currReq>=0; currReq--) {
    static int prevReq = N-2;
    
    while ( (prevReq >= 0) && (requests[currReq].b < requests[prevReq].e) ) {
      prevReq--;
    }

    requests[currReq].pr = prevReq+1;
  }

  

  for(auto it : requests) {
    out << it.b << " ";
    out << it.e << " ";
    out << it.c << " ";
    out << it.pr << " ";
    out << it.num << "\n";
  }

  in.close();
  out.close();

  return 0;
}

bool reqCompare(const Request& r1, const Request& r2) {
  // Sort by: 1)faster ending day, 2)faster beginning day.
  if (r1.e == r2.e) {
    return r1.b < r2.b;
  } else {
    return r1.e < r2.e;
  }
}