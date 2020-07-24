#pragma once
#include <cmath>
class vertex{
public:
    double x, y, z, theta;
    vertex(double X, double Y, double Z){
        x=X;y=Y;z=Z;theta = atan2((50-y), (50-x));
    }

    bool operator<(const vertex & other){
        return theta < other.theta;
    }
};

double distance(vertex a, vertex b){
    return sqrt((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y)+(a.z-b.z)*(a.z-b.z));
}
double theta(vertex a, vertex b){// sor points by theta????!!!
    return atan2((a.y-b.y), (a.x-b.x));
}
double theta(vertex a){// sor points by theta????!!!
    return atan2((a.y), (a.x));
}
