#include <iostream>
#include <fstream>
#include <cmath>
#include <limits>
#include <vector>
#include <algorithm>

using namespace std;

class Point3D {
public:
  double x;
  double y;
  double z;
  Point3D(double _x = 0.0, double _y = 0.0, double _z = 0.0) : x(_x), y(_y), z(_z) {}
};

static Point3D stations[2][2];

double getSqaureDistance(Point3D p1, Point3D p2) {
  return (pow(p1.x-p2.x,2)+pow(p1.y-p2.y,2)+pow(p1.z-p2.z,2));
}

Point3D getMidPoint(Point3D p1, Point3D p2) {
  return Point3D((p1.x+p2.x)/2, (p1.y+p2.y)/2,(p1.z+p2.z)/2);
}

bool compareDistance(pair<int,double> a, pair<int,double> b) {
  return (a.second > b.second);
}

double closestPath() {

  static int count = 0; 
  vector<pair<int,double>> distances;
  
  for(int point1=0; point1<2; point1++) {
    for(int point2=0; point2<2; point2++) {
      double distance = getSqaureDistance(stations[0][point1], stations[1][point2]);
      distances.push_back(make_pair(2*point1+point2, distance));
    }
  }
  sort(distances.begin(), distances.end(), compareDistance);
  for (auto it:distances) {
    //cout << it.first << ": "<< (it).second <<" > ";
  }
  //cout << "\n";

    int farPointIndex1 = (3 - distances.back().first)/2;
    int farPointIndex2 = (3 - distances.back().first)%2;
    //cout << farPointIndex1 << ", " << farPointIndex2 << "\n";
    stations[0][farPointIndex1] = getMidPoint(stations[0][0], stations[0][1]);
    stations[1][farPointIndex2] = getMidPoint(stations[1][0], stations[1][1]);
    
    for(int station=0; station<2; station++) {
      for(int point=0; point<2; point++) {
        //cout << "(";
        //cout << stations[station][point].x << ", ";
        //cout << stations[station][point].y<< ", ";
        //cout << stations[station][point].z<< "),";
      }
    }
    //cout << "\n";
    
    if((sqrt(getSqaureDistance(stations[0][0], stations[0][1]))) < numeric_limits<double>::epsilon() ) {
      return (sqrt(distances.back().second));
    } else {
      return closestPath();
    }
}

double roundWithEpsilon(double original) {
  double floored = floor(original);
  if ((original - floored) < numeric_limits<double>::epsilon()) {
    return floored;
  } else {
    return floored+1;
  }
}

int main() {
  ifstream in("station.inp");
  ofstream out("station.out");
  

  for(int station=0; station<2; station++) {
    for(int point=0; point<2; point++) {
      in >> stations[station][point].x;
      in >> stations[station][point].y;
      in >> stations[station][point].z;
    }
  }

  double realPathDistance = closestPath();
  double roundedPathDistance = roundWithEpsilon(realPathDistance);

  out << roundedPathDistance << "\n";

  in.close();
  out.close();
  
  
  return 0;
}