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

  Point3D(const Point3D& p) {
    x = p.x;
    y = p.y;
    z = p.z;
  }

  bool isEqual (const Point3D& other) const {
    bool isXEqual = (abs(x - other.x) < numeric_limits<double>::epsilon());
    return (isXEqual);
  }
};

static Point3D stations[2][2];

const double getSqaureDistance(const Point3D& p1, const Point3D& p2) {
  return ((p1.x-p2.x)*(p1.x-p2.x)+(p1.y-p2.y)*(p1.y-p2.y)+(p1.z-p2.z)*(p1.z-p2.z));
}

Point3D getMidPoint(const Point3D& p1, const Point3D& p2) {
  Point3D midPoint;
  midPoint.x = (p1.x+p2.x)/2;
  midPoint.y = (p1.y+p2.y)/2;
  midPoint.z = (p1.z+p2.z)/2;
  return midPoint;
}

Point3D closestPoint(Point3D& startPoint, Point3D& stationPoint1, Point3D& stationPoint2) {
  if(stationPoint1.isEqual(stationPoint2)) {
    return stationPoint1;
  }
  
  if(getSqaureDistance(startPoint, stationPoint1) > getSqaureDistance(startPoint, stationPoint2)) {
    stationPoint1 = getMidPoint(stationPoint1, stationPoint2);
  } else if( getSqaureDistance(startPoint, stationPoint2) > getSqaureDistance(startPoint, stationPoint1)) {
    stationPoint2 = getMidPoint(stationPoint1, stationPoint2);
  } else {
    return getMidPoint(stationPoint1, stationPoint2);
  }
  return closestPoint(startPoint, stationPoint1, stationPoint2);
  
}

double closestDistance(Point3D& prevStartPoint, Point3D& currentStartPoint, int stationNumber) {
  Point3D tmpCurrentPoint = currentStartPoint;
  Point3D tmpStationPoint1 = stations[stationNumber][0];
  Point3D tmpStationPoint2 = stations[stationNumber][1];

  Point3D nextStartPoint = closestPoint(tmpCurrentPoint, tmpStationPoint1, tmpStationPoint2);

  if(nextStartPoint.isEqual(prevStartPoint)) {
    return sqrt(getSqaureDistance(prevStartPoint, currentStartPoint));
  } 

  stationNumber = (stationNumber+1) % 2;

  return closestDistance(currentStartPoint, nextStartPoint, stationNumber);
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

  double originalDistance = closestDistance(stations[0][0], stations[0][1], 1);
  double roundedDistance = roundWithEpsilon(originalDistance);
  out << roundedDistance << "\n";

  in.close();
  out.close();
  
  
  return 0;
}