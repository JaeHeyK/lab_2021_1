#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

typedef struct _Request {
  int b;  // Beginning day
  int e;  // Ending day
  int c;  // Cost
  int num;  //  Request number after sorting
  int pr; // Last workable request among previous requests; Order of requests will be determined by sorting
} Request;

typedef struct _Schedule {
  int p;  // Total pay
  int d = 0;  // Total workday
} Schedule;

typedef vector<Request>::iterator ReqIt;
typedef vector<Request>::reverse_iterator ReqRit;

bool reqCompare(const Request&, const Request&);
bool schdCompare(const Schedule&, const Schedule&);
Schedule getBestSchdBefore(int);


vector<Request> requests;
vector<Schedule> bestSchedules;

int main() {
  ifstream in("free.inp");
  ofstream out("free.out");

  int N;  // Total number of request
  in >> N;

  for (int i=0; i<N; i++) {
    Request newReq;
    in >> newReq.b >> newReq.e >> newReq.c;
    requests.push_back(newReq);

    Schedule newSchedule;
    bestSchedules.push_back(newSchedule);
  }

  sort(requests.begin(), requests.end(), reqCompare);

  for (int req=0; req<N; req++) {
    requests[req].num = req+1;
  }

  for (int currReq=N-1; currReq>=0; currReq--) {
    static int prevReq = N-2;
    
    while ( (prevReq >= 0) && (requests[currReq].b <= requests[prevReq].e) ) {
      prevReq--;
    }

    requests[currReq].pr = prevReq+1;
  }

  Schedule bestSchedule = getBestSchdBefore(N);

  out << bestSchedule.p << " " << bestSchedule.d << "\n";

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

bool schdCompare(int d1, int p1, int d2, int p2) {
  if (p1 == p2) {
    return (d1 < d2);
  } else {
    return (p1 > p2);
  }
}

Schedule getBestSchdBefore(int n) {
  if ((n>0) && (bestSchedules[n-1].d != 0)) {
    return bestSchedules[n-1];
  } else if(n==0) {
    Schedule empty;
    empty.d = 0;
    empty.p = 0;
    return empty;
  }

  int inclD, inclP, exclD, exclP;
  exclD = getBestSchdBefore(n-1).d;
  exclP = getBestSchdBefore(n-1).p;
  inclD = getBestSchdBefore(requests[n-1].pr).d + (requests[n-1].e - requests[n-1].b + 1);
  inclP = getBestSchdBefore(requests[n-1].pr).p + requests[n-1].c;

  if(requests[n-1].pr > 0) {
    inclP -= 10;
  }

  if (schdCompare(inclD, inclP, exclD, exclP)) {
    bestSchedules[n-1].d = inclD;
    bestSchedules[n-1].p = inclP;
  } else {
    bestSchedules[n-1].d = exclD;
    bestSchedules[n-1].p = exclP;
  }

  return bestSchedules[n-1];
}