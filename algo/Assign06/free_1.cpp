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
} Request;

typedef struct _Schedule {
  int p = 0;  // Total pay
  int d = 0;  // Total workday
} Schedule;

typedef vector<Request>::iterator ReqIt;
typedef vector<Request>::reverse_iterator ReqRit;

bool reqCompare(const Request&, const Request&);
bool schdCompare(int, int, int, int);
Schedule getBestSchdBefore(int);


vector<Request> requests;
vector<Schedule> bestSchedules;

int main() {
  ifstream in("free.inp");
  ofstream out("free.out");

  int N;  // Total number of request
  in >> N;

  Schedule emptySchedule;
  bestSchedules.push_back(emptySchedule);

  for (int i=0; i<N; i++) {
    Request newReq;
    in >> newReq.b >> newReq.e >> newReq.c;
    requests.push_back(newReq);

    Schedule newSchedule;
    bestSchedules.push_back(newSchedule);
  }

  sort(requests.begin(), requests.end(), reqCompare);


  for (int currReq=N-1; currReq>=0; currReq--) {
    static int prevReq = N-2;
    
    while ( (prevReq >= 0) && (requests[currReq].b <= requests[prevReq].e) ) {
      prevReq--;
    }

    requests[currReq].pr = prevReq+1;
  }

  for(auto it : requests) {
    out << it.b << " ";
    out << it.e << " ";
    out << it.c << " ";
    out << it.pr << "\n";
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
  if(n==0) {
    return bestSchedules[0];
  } else if (bestSchedules[n].d != 0) {
    return bestSchedules[n];
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
    bestSchedules[n].d = inclD;
    bestSchedules[n].p = inclP;
  } else {
    bestSchedules[n].d = exclD;
    bestSchedules[n].p = exclP;
  }

  return bestSchedules[n];
}