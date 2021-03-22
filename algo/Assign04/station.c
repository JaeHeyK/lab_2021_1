#include <stdio.h>
#include <math.h>
#include <float.h>
#define ABS(x) ((x<0)?(-x):(x))

int search_shortest(double x1, double y1, double z1, double x2, double y2, double z2);
double shortest = 2000000000.0;
double x,y,z;

struct point
{
    double x,y,z;
};

double cx1, cy1, cz1, cx2, cy2, cz2; //가장 최소


int main()
{
    FILE *in = fopen("station.inp","r");
    FILE *out = fopen("station.out","w");
    struct point p[4];
    

    
    for(int i =0;i<4;i++)//값을 p0,1,2,3에 넣음 p4가 없으면 값이 이상함..
    {
        fscanf(in,"%lf",&p[i].x);
        fscanf(in,"%lf",&p[i].y);
        fscanf(in,"%lf",&p[i].z);
    }
    x=p[2].x;
    y=p[2].y;
    z=p[2].z;
    int prev = 2000000000;
    int stationIndex = 0;
    while (1)
    {   
        search_shortest(p[stationIndex].x,p[stationIndex].y,p[stationIndex].z,p[stationIndex+1].x,p[stationIndex+1].y,p[stationIndex+1].z);
        int result_1;
        printf("shortest: %lf\n", shortest);
        double test = shortest - floor((int)shortest);
        if(ABS(test)<=0.00000001) {
            result_1 = (int)shortest;
        } else {
            result_1 = ceil(shortest);
        }

        if(prev<=result_1) {
            fprintf(out,"%d",prev);
            break;
        } 
        shortest = 2000000000.0;
        prev = result_1;

        stationIndex = (stationIndex+2) %4;        
    }
    return 0;
}

int search_shortest(double x1, double y1, double z1, double x2, double y2, double z2)
{
    double mx = (x2+x1)/2;
    double my = (y2+y1)/2;
    double mz = (z2+z1)/2;

    double l = sqrt(pow(x-x1,2.0)+pow(y-y1,2.0)+pow(z-z1,2.0));
    double r = sqrt(pow(x-x2,2.0)+pow(y-y2,2.0)+pow(z-z2,2.0));
    double m = sqrt(pow(x-mx,2.0)+pow(y-my,2.0)+pow(z-mz,2.0));
    double abs=m-shortest;

    if(ABS(abs)<=DBL_EPSILON) {
        x=mx;
        y=my;
        z=mz;
        return 0;
    }
    if(m<shortest) shortest = m;
    if(r<l) search_shortest(mx,my,mz,x2,y2,z2);
    else if(r>l) search_shortest(x1,y1,z1,mx,my,mz);
    return 0;
}