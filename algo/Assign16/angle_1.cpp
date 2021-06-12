#include <bits/stdc++.h>

using namespace std;

class Point {
public:
  int x;
  int y;
  int num;

  Point(int _x, int _y, int _num) : x(_x), y(_y), num(_num) {}

  bool operator< (const Point& rhs) {
    int sArea = (y * rhs.x) - (x * rhs.y);
    if (sArea != 0) {
      return (sArea<0);
    } else {
      return (((x*x)+(y*y)) < ((rhs.x*rhs.x)+(rhs.y*rhs.y)));
    }
  }
};

int main() {
  ifstream in("angle.inp");
  ofstream out("angle.out");

  int N;
  in >> N;
  
  vector<vector<Point>> points(4);
  for(int i=0; i<N; ++i) {
    int x, y;
    in >> x >> y;

    Point newPoint = {x,y,i+1};
    if(y>=0) {
      if(x>0) {
        points[0].push_back(newPoint); // 1사분면
      } else {
        points[1].push_back(newPoint); // 2사분면
      }
    } else {
      if(x<=0) {
        points[2].push_back(newPoint); // 3사분면
      } else {
        points[3].push_back(newPoint); // 4사분면
      }
    }
  }

  for(auto& quarter: points) {
    sort(quarter.begin(), quarter.end());
  }

  for(auto quarter : points) {
    for(auto point : quarter) {
      out << point.num <<"\n";
    }
  }

  in.close();
  out.close();
  
  return 0;
}