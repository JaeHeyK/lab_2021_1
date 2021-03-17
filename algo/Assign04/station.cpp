#include <iostream>
#include <fstream>
#include <cmath>
#include <limits>
#include <vector>
#include <algorithm>

using namespace std;

const double epsilon = 0.00000001;

class Point3D {
public:
  double x;
  double y;
  double z;
  Point3D(double _x = 0.0, double _y = 0.0, double _z = 0.0) : x(_x), y(_y), z(_z) {}
  const bool isEqual(const Point3D& other) {
    bool isXEqual = (abs(x - other.x) < epsilon);
    bool isYEqual = (abs(y - other.y) < epsilon);
    bool isZEqual = (abs(z - other.z) < epsilon);
    return (isXEqual && isYEqual && isZEqual);
  }
};

static Point3D stations[2][2];

const double getSqaureDistance(Point3D& p1, Point3D& p2) {
  return ((p1.x-p2.x)*(p1.x-p2.x)+(p1.y-p2.y)*(p1.y-p2.y)+(p1.z-p2.z)*(p1.z-p2.z));
}

Point3D getMidPoint(Point3D& p1, Point3D& p2) {
  return Point3D((p1.x+p2.x)/2.0, (p1.y+p2.y)/2.0,(p1.z+p2.z)/2.0);
}

Point3D closestPoint(Point3D& startPoint, Point3D& stationPoint1, Point3D& stationPoint2) {
  double sPointDistanceSqaure1 = getSqaureDistance(startPoint, stationPoint1);
  double sPointDistanceSqaure2 = getSqaureDistance(startPoint, stationPoint2);
  double sLengthSqaure = getSqaureDistance(stationPoint1, stationPoint2);

  if(stationPoint1.isEqual(stationPoint2)) {
    return stationPoint1;
  }
   if(sPointDistanceSqaure1 >= sPointDistanceSqaure2 + sLengthSqaure) {
     
    return stationPoint2;
  } else if(sPointDistanceSqaure2 >= sPointDistanceSqaure1 + sLengthSqaure) {
    return stationPoint1;
  } else {
    Point3D midPoint = getMidPoint(stationPoint1, stationPoint2);
    if(sPointDistanceSqaure1 > sPointDistanceSqaure2) {
      return closestPoint(startPoint, midPoint, stationPoint2);
    } else if(sPointDistanceSqaure2 > sPointDistanceSqaure1) {
      return closestPoint(startPoint, stationPoint1, midPoint);
    } else {
      return midPoint;
    }
  }
}

double closestDistance(Point3D& prevStartPoint, Point3D& currentStartPoint, int stationNumber) {
  if(prevStartPoint.isEqual(currentStartPoint)) {
    return 0.0;
  }
  stationNumber = (stationNumber+1) % 2;
  Point3D nextStartPoint = closestPoint(currentStartPoint, stations[stationNumber][0], stations[stationNumber][1]);
  if(nextStartPoint.isEqual(prevStartPoint)) {
    return sqrt(getSqaureDistance(prevStartPoint, currentStartPoint));
  } else {
    return closestDistance(currentStartPoint, nextStartPoint, stationNumber);
  }
}

double roundWithEpsilon(double original) {
  double floored = floor(original);
  if ((original - floored) < epsilon) {
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

  double originalDistance = closestDistance(stations[0][0], stations[0][1], 0);
  double roundedDistance = roundWithEpsilon(originalDistance);
  out << roundedDistance << "\n";

  in.close();
  out.close();
  
  
  return 0;
}